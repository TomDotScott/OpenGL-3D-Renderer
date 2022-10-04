#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ElementBufferObject.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main()
{
    // Vertex data for the pyramid
    // XYZ go from -1 to 1
    // RGB is from  0 to 1
    // UV  is from  0 to n where n defines how much the texture will repeat
    constexpr GLfloat vertices[] =
    {
    	// X |   Y  |   Z   |  Red  |  Green  |  Blue  |  U   |    V
        -0.5f,  0.0f,  0.5f,   0.8f,   0.72f,    0.44f,  0.0f,    0.0f,
        -0.5f,  0.0f, -0.5f,   0.8f,   0.72f,    0.44f,  5.0f,    0.0f,
         0.5f,  0.0f, -0.5f,   0.8f,   0.73f,    0.44f,  0.0f,    0.0f,
         0.5f,  0.0f,  0.5f,   0.8f,   0.72f,    0.44f,  5.0f,    0.0f,
         0.0f,  0.8f,  0.0f,   0.9f,   0.86f,    0.76f,  2.5f,    5.0f
    };

    constexpr GLuint indices[] =
    {
        0, 2, 1,
        0, 2, 3,
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4
    };


    glfwInit();

    // Tell glfw the version of OpenGL we're using
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* mainWindow = glfwCreateWindow(
        WINDOW_WIDTH, 
        WINDOW_HEIGHT, 
        "OpenGL Game", 
        nullptr, 
        nullptr
    );

    // If the window can't be created, let's kill the game
    if(!mainWindow)
    {
	    std::cout << "Failed to create the GLFW Window!\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(mainWindow);

    std::cout << "Hello World!\n";

    gladLoadGL();

    // 0, 0 is bottom left of the viewport
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    Shader shaderProgram("default.vert", "default.frag");

    VertexArrayObject vao1;
    vao1.Bind();

    VertexBufferObject vbo1(vertices, sizeof(vertices));
    ElementBufferObject ebo1(indices, sizeof(indices));

    // Attributes for the vertex data
    vao1.LinkAttrib(
        vbo1, 
        0,
        3,
        GL_FLOAT,
        8 * sizeof(float),
        nullptr
    );

    // Attributes for the colour data per vertex
    vao1.LinkAttrib(
        vbo1,
        1,
        3,
        GL_FLOAT,
        8 * sizeof(float),
        (void*)(3 * sizeof(float))
    );

    // Attributes for the UV data per vertex
    vao1.LinkAttrib(
        vbo1,
        2,
        2,
        GL_FLOAT,
        8 * sizeof(float),
        (void*)(6 * sizeof(float))
    );

    vao1.Unbind();
    vbo1.Unbind();
    ebo1.Unbind();

    Texture brickTexture("Mario_Bricks.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    brickTexture.TexUniformUnit(shaderProgram, "tex0", 1);

    const GLuint tex0UniformID = glGetUniformLocation(shaderProgram.m_ID, "tex0");
    shaderProgram.Activate();
    glUniform1i(tex0UniformID, 0);


    float pyramidRotation = 0.f;
    double previousTime = glfwGetTime();

    glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(mainWindow))
    {
        // Clear the screen before the render
        glClearColor(0.004f, 0.196f, 0.125f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use the shader program
        shaderProgram.Activate();

        const double currentTime = glfwGetTime();
        // Every 60 frames-ish, rotate the pyramid
        if(currentTime - previousTime >= static_cast<double>(1.f / 60.f))
        {
            pyramidRotation += 0.05f;
            previousTime = currentTime;
        }

        glm::mat4 modelMatrix = glm::mat4(1.f);
        glm::mat4 viewMatrix = glm::mat4(1.f);
        glm::mat4 projectionMatrix = glm::mat4(1.f);

        // Move the camera to give perspective
        viewMatrix = translate(viewMatrix, glm::vec3(0.f, -0.5f, -3.f));

        // Set up the projection matrix and the distance planes of our camera's viewing frustum
        projectionMatrix = glm::perspective(
            glm::radians(40.f),
            static_cast<float>(WINDOW_WIDTH / WINDOW_HEIGHT),
            0.1f,
            100.f
        );

        // Rotate the model
        modelMatrix = rotate(modelMatrix, pyramidRotation, glm::vec3(0.f, 1.f, 0.f));

        const GLint modelMatLocation = glGetUniformLocation(shaderProgram.m_ID, "modelMatrix");
        glUniformMatrix4fv(modelMatLocation, 1, GL_FALSE, value_ptr(modelMatrix));

        const GLint viewMatLocation = glGetUniformLocation(shaderProgram.m_ID, "viewMatrix");
        glUniformMatrix4fv(viewMatLocation, 1, GL_FALSE, value_ptr(viewMatrix));

        const GLint projectionMatLocation = glGetUniformLocation(shaderProgram.m_ID, "projectionMatrix");
        glUniformMatrix4fv(projectionMatLocation, 1, GL_FALSE, value_ptr(projectionMatrix));

        // To use our texture, we need to bind it!
        glBindTexture(GL_TEXTURE_2D, brickTexture.m_ID);

        vao1.Bind();

        // Draw the quad with the texture
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

        // Swap the buffers to display the triangle on screen
        glfwSwapBuffers(mainWindow);

        glfwPollEvents();
    }

    vao1.Delete();
    vbo1.Delete();
    ebo1.Delete();
    brickTexture.Delete();
    shaderProgram.Delete();

    // Delete and stop GLFW when the program finishes
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
    return 0;
}