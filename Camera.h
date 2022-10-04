#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Shader.h"

class Camera
{
public:
	Camera(int width, int height, const glm::vec3& position);

	void HandleInput(GLFWwindow* window);

	void SendMatrixToShader(const Shader& shader, const std::string& uniform) const;

private:
	const int m_width;
	const int m_height;

	float m_fov;
	float m_nearPlane;
	float m_farPlane;

	float m_currentSpeed;
	float m_fastSpeed;
	float m_normalSpeed;
	float m_sensitivity;

	glm::vec3 m_position;
	glm::vec3 m_orientation;
	glm::vec3 m_upDirection;
};
