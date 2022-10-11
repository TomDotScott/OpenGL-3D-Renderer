#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Model.h"

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
	if (!mainWindow)
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

	Shader shaderProgram("shaders\\point_light.vert", "shaders\\point_light.frag");


	Shader lightShader("shaders\\light.vert", "shaders\\light.frag");

	const glm::vec4 lightColour = glm::vec4(1.f);

	shaderProgram.Activate();

	glUniform4f(glGetUniformLocation(shaderProgram.m_ID, "lightColour"), lightColour.x, lightColour.y, lightColour.z, lightColour.w);

	// Enable the depth buffer for proper culling
	glEnable(GL_DEPTH_TEST);

	Camera camera(
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		{ 0.f, 1.f, 3.f }
	);

	Model sword("assets\\models\\scroll\\scene.gltf");

	while (!glfwWindowShouldClose(mainWindow))
	{
		if (glfwGetKey(mainWindow, GLFW_KEY_ESCAPE))
		{
			// Escape was pressed, kill the game
			glfwSetWindowShouldClose(mainWindow, GLFW_TRUE);
		}


		// Clear the screen before the render
		glClearColor(0.004f, 0.196f, 0.125f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.HandleInput(mainWindow);

		camera.UpdateMatrix();

		shaderProgram.Activate();

		glUniform3f(glGetUniformLocation(shaderProgram.m_ID, "camPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		// Send the camera data to the default shader for the pyramid verts and texture
		camera.SendMatrixToShader(shaderProgram, "camMatrix");

		sword.Render(shaderProgram, camera);

		// Swap the buffers to display the triangle on screen
		glfwSwapBuffers(mainWindow);

		glfwPollEvents();
	}


	shaderProgram.Delete();
	lightShader.Delete();

	// Delete and stop GLFW when the program finishes
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
	return 0;
}