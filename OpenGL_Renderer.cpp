#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Model.h"
#include "GameObject.h"
#include "Physics.h"
#include "Sphere.h"

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

	// Shader shaderProgram("shaders\\point_light.vert", "shaders\\point_light.frag");


	Shader lightShader("shaders\\light.vert", "shaders\\light.frag");

	const glm::vec4 lightColour = glm::vec4(1.f);

	// Enable the depth buffer for proper culling
	glEnable(GL_DEPTH_TEST);

	Camera camera(
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		{ 291.79f, 10.53f, -0.23f },
		{ -1.f, 0.f, 0.f },
		true
	);

	// Model sword("assets\\models\\sword\\scene.gltf");
	Shader defaultShader("shaders\\directional_light.vert", "shaders\\directional_light.frag");

	Sphere movingSphere(1, glm::vec3(200.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));

	Sphere stationarySphere(1, glm::vec3(200.f, 10.f, 0.f), glm::vec3(0.f));

	const std::vector<GameObject*> gameObjects = {
		&movingSphere,
		&stationarySphere
	};

	std::chrono::high_resolution_clock::time_point timeAtBeginning = std::chrono::high_resolution_clock::now();

	while (!glfwWindowShouldClose(mainWindow))
	{
		std::chrono::high_resolution_clock::time_point timeThisFrame = std::chrono::high_resolution_clock::now();
		const float deltaTime = std::chrono::duration<float, std::milli>(timeThisFrame - timeAtBeginning).count() / 1000.f;

		if (glfwGetKey(mainWindow, GLFW_KEY_ESCAPE))
		{
			// Escape was pressed, kill the game
			glfwSetWindowShouldClose(mainWindow, GLFW_TRUE);
		}


		// Clear the screen before the render
		glClearColor(0.004f, 0.196f, 0.125f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (camera.IsControllable())
		{
			camera.HandleInput(mainWindow);

			// camera.OutputPositionOrientation();
		}

		camera.UpdateMatrix();


		// Update all the GOs
		for (GameObject* gameObject : gameObjects)
		{
			gameObject->Update(deltaTime);
		}

		// TODO: Come up with a more efficient way of checking collisions than checking everything against everything else...
		Physics::SphereToStationarySphereCCD(stationarySphere, movingSphere, deltaTime);

		// Finally draw the result to the screen, after resolving collisions
		for (const auto* gameObject : gameObjects)
		{
			gameObject->Render(camera);
		}

		// Swap the buffers to display the triangle on screen
		glfwSwapBuffers(mainWindow);

		glfwPollEvents();

		// Start the clock for the next frame...
		timeAtBeginning = timeThisFrame;
	}


	// shaderProgram.Delete();

	for (const auto* gameObject : gameObjects)
	{
		gameObject->CleanUp();
	}

	lightShader.Delete();

	// Delete and stop GLFW when the program finishes
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
	return 0;
}