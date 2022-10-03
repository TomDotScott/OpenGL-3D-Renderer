#include <iostream>
#include <GLFW/glfw3.h>

int main()
{
    glfwInit();

    // Tell glfw the version of OpenGL we're using
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* mainWindow = glfwCreateWindow(
        800, 
        600, 
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

    while(!glfwWindowShouldClose(mainWindow))
    {
        glfwPollEvents();
    }

    // Delete and stop GLFW when the program finishes
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
    return 0;
}