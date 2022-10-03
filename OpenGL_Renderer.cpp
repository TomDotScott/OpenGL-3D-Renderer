#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main()
{
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

    // OpenGL likes normalised-decimal form of RGB. 1.f is opaque, 0.f is transparent
    glClearColor(0.27f, 0.15f, 0.34f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT);

    // Swap the front and back buffers
    glfwSwapBuffers(mainWindow);

    while(!glfwWindowShouldClose(mainWindow))
    {
        glfwPollEvents();
    }

    // Delete and stop GLFW when the program finishes
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
    return 0;
}