#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// TODO: Make a proper parser for these shaders... This is *very* hacky
const char* VERTEX_SHADER_SOURCE = 
	"#version 330 core\n"\
	"layout(location = 0) in vec3 aPos;\n"\
	"void main()\n"\
	"{\n"\
	"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"\
	"}\0";

const char* FRAGMENT_SHADER_SOURCE = "#version 330 core\n"\
    "out vec4 FragColor;\n"\
    "void main()\n"\
    "{\n"\
    "    FragColor = vec4(0.52f, 0.52f, 0.52f, 1.0f);\n"\
    "}\0";


int main()
{
    glfwInit();

    // Tell glfw the version of OpenGL we're using
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Vertices for our triangle, ranging from -1 to 1
    // These make an equilateral triangle!
    const GLfloat rootThreeOverThree = sqrtf(3) / 3.f;

    const GLfloat vertices[] =
    {
        -0.5f, -0.5f * rootThreeOverThree, 0.f,
         0.5f, -0.5f * rootThreeOverThree, 0.f,
         0.0f,  0.5f * rootThreeOverThree * 2.f, 0.f
    };


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

    // Create the shaders and get a reference value for them
    const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Tell OpenGL where to get the source code from...
	glShaderSource(vertexShader, 1, &VERTEX_SHADER_SOURCE, nullptr);
	// Finally, compile the shader
    glCompileShader(vertexShader);

    // Same for the Fragment Shader...
    const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &FRAGMENT_SHADER_SOURCE, nullptr);
    glCompileShader(fragmentShader);

    // To use the shaders, we need to create a shader program
    const GLuint shaderProgram = glCreateProgram();

    // Attach the shaders to the shader program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);

    // Because we've linked them, we can delete the references
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // In order to use the vertices defined above, we need to create buffers to store them
    // IMPORTANT - VAO needs to be defined before the VBO
    GLuint vertexArrayObject, vertexBufferObject;

	// We need to create vertex array object to allow OpenGL to use our vertex buffer
    glGenVertexArrays(1, &vertexArrayObject);
    glGenBuffers(1, &vertexBufferObject);

    // Then, we need to bind the buffers
    glBindVertexArray(vertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

    // Assign the data to the VBO
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );

    // Assign data to the VAO
    glVertexAttribPointer(
        0,                   // Index of the VAO
        3,                   // The amount of items of data per vertex
        GL_FLOAT,            // The data format vertex data is stored in
        GL_FALSE,            // We aren't working with integers
        3 * sizeof(float),   // The stride of the data
        (void*)nullptr       
    );

    // 0 since that's the index defined above
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    // OpenGL likes normalised-decimal form of RGB. 1.f is opaque, 0.f is transparent
    glClearColor(0.27f, 0.15f, 0.34f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT);

    // Swap the front and back buffers
    glfwSwapBuffers(mainWindow);

    while(!glfwWindowShouldClose(mainWindow))
    {
        // Clear the screen before the render
        glClearColor(0.27f, 0.15f, 0.34f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Use the shader program
        glUseProgram(shaderProgram);
        glBindVertexArray(vertexArrayObject);

        // Draw the triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap the buffers to display the triangle on screen
        glfwSwapBuffers(mainWindow);

        glfwPollEvents();
    }

    // Delete all of the objects created so far
    glDeleteVertexArrays(1, &vertexArrayObject);
    glDeleteBuffers(1, &vertexBufferObject);
    glDeleteProgram(shaderProgram);


    // Delete and stop GLFW when the program finishes
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
    return 0;
}