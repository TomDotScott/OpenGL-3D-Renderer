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
#include "Camera.h"

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
		// X |  Y  |   Z  |  R  |  G  |  B  |  U  |  V  |  nX |  nY |  nZ
		-1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f,
		 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 2.0f, 2.0f, 0.0f, 1.0f, 0.0f,
		 1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f
	};

	constexpr GLuint indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};


	// Vertex data for the Light cube
	// XYZ go from -1 to 1
	// RGB is from  0 to 1
	// UV  is from  0 to n where n defines how much the texture will repeat
	constexpr GLfloat lightVertices[] =
	{
		// X |   Y  |   Z 
		-0.1f, -0.1f,  0.1f,
		-0.1f, -0.1f, -0.1f,
		 0.1f, -0.1f, -0.1f,
		 0.1f, -0.1f,  0.1f,
		-0.1f,  0.1f,  0.1f,
		-0.1f,  0.1f, -0.1f,
		 0.1f,  0.1f, -0.1f,
		 0.1f,  0.1f,  0.1f
	};

	constexpr GLuint lightIndices[] =
	{
		0, 1, 2,
		0, 2, 3,
		0, 4, 7,
		0, 7, 3,
		3, 7, 6,
		3, 6, 2,
		2, 6, 5,
		2, 5, 1,
		1, 5, 4,
		1, 4, 0,
		4, 5, 6,
		4, 6, 7
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
		11 * sizeof(float),
		nullptr
	);

	// Attributes for the colour data per vertex
	vao1.LinkAttrib(
		vbo1,
		1,
		3,
		GL_FLOAT,
		11 * sizeof(float),
		(void*)(3 * sizeof(float))
	);

	// Attributes for the UV data per vertex
	vao1.LinkAttrib(
		vbo1,
		2,
		2,
		GL_FLOAT,
		11 * sizeof(float),
		(void*)(6 * sizeof(float))
	);

	// Attributes for the normal data per vertex
	vao1.LinkAttrib(
		vbo1,
		3,
		3,
		GL_FLOAT,
		11 * sizeof(float),
		(void*)(8 * sizeof(float))
	);

	vao1.Unbind();
	vbo1.Unbind();
	ebo1.Unbind();

	Shader lightShader("light.vert", "light.frag");

	VertexArrayObject lightVAO;
	lightVAO.Bind();

	VertexBufferObject lightVBO{ lightVertices, sizeof(lightVertices) };

	ElementBufferObject lightEBO{ lightIndices, sizeof(lightVertices) };

	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), nullptr);

	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();

	glm::vec4 lightColour = glm::vec4(1.f, 1.f, 1.f, 1.f);

	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);

	glm::mat4 lightModelMat = glm::mat4(1.f);
	lightModelMat = translate(lightModelMat, lightPos);

	glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::mat4 pyramidModelMat = glm::mat4(1.f);
	pyramidModelMat = translate(pyramidModelMat, pyramidPos);

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.m_ID, "model"), 1, GL_FALSE, value_ptr(lightModelMat));
	glUniform4f(glGetUniformLocation(lightShader.m_ID, "lightColour"), lightColour.x, lightColour.y, lightColour.z, lightColour.w);


	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.m_ID, "modelMatrix"), 1, GL_FALSE, value_ptr(pyramidModelMat));
	glUniform4f(glGetUniformLocation(shaderProgram.m_ID, "lightColour"), lightColour.x, lightColour.y, lightColour.z, lightColour.w);
	glUniform3f(glGetUniformLocation(shaderProgram.m_ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	Texture rock("Rock.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE);
	rock.SendToShader(shaderProgram, "albedo", 0);

	Texture rockSpec("Rock_Specular.png", GL_TEXTURE_2D, 1, GL_R, GL_UNSIGNED_BYTE);
	rockSpec.SendToShader(shaderProgram, "specularMap", 1);

	const GLuint tex0UniformID = glGetUniformLocation(shaderProgram.m_ID, "tex0");
	shaderProgram.Activate();
	glUniform1i(tex0UniformID, 0);

	// Enable the depth buffer for proper culling
	glEnable(GL_DEPTH_TEST);

	Camera camera(
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		{ 0.f, 0.f, 3.f }
	);

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

		// To use our texture, we need to bind it!
		rock.Bind();
		rockSpec.Bind();

		vao1.Bind();

		// Draw the quad with the texture
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

		lightShader.Activate();

		// Send the camera data to the lighting shader for proper lighting
		camera.SendMatrixToShader(lightShader, "camMatrix");

		// Draw the Light cube
		lightVAO.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);


		// Swap the buffers to display the triangle on screen
		glfwSwapBuffers(mainWindow);

		glfwPollEvents();
	}

	vao1.Delete();
	vbo1.Delete();
	ebo1.Delete();
	rock.Delete();
	rockSpec.Delete();
	shaderProgram.Delete();

	// Delete and stop GLFW when the program finishes
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
	return 0;
}