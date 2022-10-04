#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include "Shader.h"
#include "VertexBufferObject.h"
#include "VertexArrayObject.h"
#include "ElementBufferObject.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main()
{
    // Vertex data for the quad
    // XYZ go from -1 to 1
    // RGB is from  0 to 1
    // UV  is from  0 to n where n defines how much the texture will repeat
    constexpr GLfloat vertices[] =
    {
    	// X |   Y  |  Z   |  Red  |  Green  |  Blue  |   U   |   V
        -0.5f, -0.5f, 0.0f,   1.0f,   0.0f,     0.0f,   0.0f,    0.0f,  // Lower Left Corner
        -0.5f,  0.5f, 0.0f,   0.0f,   1.0f,     0.0f,   0.0f,    1.0f,  // Lower Right Corner
         0.5f,  0.5f, 0.0f,   0.0f,   0.0f,     1.0f,   1.0f,    1.0f,  // Upper Corner
         0.5f, -0.5f, 0.0f,   1.0f,   1.0f,     1.0f,   1.0f,    0.0f   // Inner Left
    };

    constexpr GLuint indices[] =
    {
        0, 2, 1,
        0, 3, 2
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

    const GLint scaleUniformID = glGetUniformLocation(shaderProgram.m_ID, "scale");

    // Tell stb to flip the image because OpenGL loads from bottom left
    stbi_set_flip_vertically_on_load(true);

    // Set up drawing our texture. We need floats to store the width, height and amount of colour channels in the image
    int width, height, numColourChannels;
    unsigned char* imagePixels = stbi_load(
        "Red_Panda.png", 
        &width, 
        &height, 
        &numColourChannels, 
        0
    );

    // In order to use the texture, we need to create the reference and generate the GL Object
    GLuint textureID;
    glGenTextures(1, &textureID);

    // After, we need to activate the texture and bind it so that OpenGL can draw it
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Now we've bound the texture, we need to change the image filtering mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Then we can set up the repeating mode for the texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Finally, we generate the image to be displayed on screen
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,          // If the image is JPG use GL_RGB
        width,
        height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        imagePixels
    );

    // And then we'll generate the mipmaps for that image
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(imagePixels);

	// Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    const GLuint tex0UniformID = glGetUniformLocation(shaderProgram.m_ID, "tex0");
    shaderProgram.Activate();
    glUniform1i(tex0UniformID, 0);

    while(!glfwWindowShouldClose(mainWindow))
    {
        // Clear the screen before the render
        glClearColor(0.004f, 0.196f, 0.125f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Use the shader program
        shaderProgram.Activate();

        glUniform1f(scaleUniformID, 0.1f);

        // To use our texture, we need to bind it!
        glBindTexture(GL_TEXTURE_2D, textureID);


        vao1.Bind();

        // Draw the quad with the texture
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Swap the buffers to display the triangle on screen
        glfwSwapBuffers(mainWindow);

        glfwPollEvents();
    }

    vao1.Delete();
    vbo1.Delete();
    ebo1.Delete();
    glDeleteTextures(1, &textureID);
    shaderProgram.Delete();

    // Delete and stop GLFW when the program finishes
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
    return 0;
}