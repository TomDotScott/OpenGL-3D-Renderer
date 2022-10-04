#include "Camera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.inl>

Camera::Camera(const int width, const int height, const glm::vec3& position) :
	m_width(width),
	m_height(height),
	m_fov(40.f),
	m_nearPlane(0.1f),
	m_farPlane(1000.f),
	m_currentSpeed(0.f),
	m_fastSpeed(0.1f),
	m_normalSpeed(0.01f),
	m_sensitivity(100.f),
	m_position(position),
	m_orientation(0.f, 0.f, -1.f),
	m_upDirection(0.f, 1.f, 0.f)
{
	m_currentSpeed = m_normalSpeed;
}

void Camera::HandleInput(GLFWwindow* window)
{
	// WASD to move the camera forwards and left and right
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		m_position += m_currentSpeed * m_orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		m_position += m_currentSpeed * -normalize(cross(m_orientation, m_upDirection));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		m_position -= m_currentSpeed * m_orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		m_position += m_currentSpeed * normalize(cross(m_orientation, m_upDirection));
	}

	// SPACE to move the camera up
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		m_position += m_currentSpeed * m_upDirection;
	}

	// CONTROL to move down
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		m_position += m_currentSpeed * -m_upDirection;
	}

	// SHIFT to speed up the camera movement
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		m_currentSpeed = m_fastSpeed;
	}
	else if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		m_currentSpeed = m_normalSpeed;
	}
}

void Camera::SendMatrixToShader(const Shader& shader, const std::string& uniform) const
{
	const glm::mat4 viewMatrix = glm::lookAt(m_position, m_position + m_orientation, m_upDirection);

	const glm::mat4 projMatrix = glm::perspective(
		glm::radians(m_fov),
		static_cast<float>(m_width) / static_cast<float>(m_height),
		m_nearPlane,
		m_farPlane
	);

	const char* c = uniform.c_str();

	glUniformMatrix4fv(
		glGetUniformLocation(shader.m_ID, c),
		1,
		GL_FALSE,
		glm::value_ptr(projMatrix * viewMatrix)
	);
}
