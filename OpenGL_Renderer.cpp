#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "VertexBufferObject.h"
#include "VertexArrayObject.h"
#include "ElementBufferObject.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main()
{
    // Vertices for our triangle, ranging from -1 to 1
   // These make a LOZ tri-force!
    const GLfloat rootThreeOverThree = sqrtf(3) / 3.f;

    const GLfloat vertices[] =
    {
    	// X        |                 Y               |  Z   |  Red  |  Green  |  Blue
        -0.5f       , -0.5f * rootThreeOverThree      , 0.f,    0.8f,   0.2f,     0.02f,   // Lower Left Corner
         0.5f       , -0.5f * rootThreeOverThree      , 0.f,    0.8f,   0.2f,     0.02f,   // Lower Right Corner
         0.0f       ,  0.5f * rootThreeOverThree * 2.f, 0.f,    1.0f,   0.6f,     0.32f,   // Upper Corner
        -0.5f / 2.f ,  0.5f * rootThreeOverThree / 2.f, 0.f,    0.9f,   0.45f,    0.07f,   // Inner Left
         0.5f / 2.f ,  0.5f * rootThreeOverThree / 2.f, 0.f,    0.9f,   0.45f,    0.07f,   // Inner Right
         0.0f       , -0.5f * rootThreeOverThree      , 0.f,    0.8f,   0.2f,     0.02f    // Inner Down
    };

    GLuint indices[] =
    {
        0, 3, 5, // Lower Left triangle
        3, 2, 4, // Lower Right triangle
        5, 4, 1, // Upper triangle
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
        6 * sizeof(float),
        nullptr
    );

    // Attributes for the colour data per vertex
    vao1.LinkAttrib(
        vbo1,
        1,
        3,
        GL_FLOAT,
        6 * sizeof(float),
        (void*)(3 * sizeof(float))
    );

    vao1.Unbind();
    vbo1.Unbind();
    ebo1.Unbind();

    const GLint scaleUniformID = glGetUniformLocation(shaderProgram.m_ID, "scale");

    while(!glfwWindowShouldClose(mainWindow))
    {
        // Clear the screen before the render
        glClearColor(0.004f, 0.196f, 0.125f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Use the shader program
        shaderProgram.Activate();

        glUniform1f(scaleUniformID, 0.5f);

        vao1.Bind();

        // Draw the triangle
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

        // Swap the buffers to display the triangle on screen
        glfwSwapBuffers(mainWindow);

        glfwPollEvents();
    }

    vao1.Delete();
    vbo1.Delete();
    ebo1.Delete();
    shaderProgram.Delete();

    // Delete and stop GLFW when the program finishes
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
    return 0;
}