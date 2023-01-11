#include "GameObject.h"

GameObject::GameObject(const std::string& modelFilename, const Shader& shader, const float scale, const glm::vec3& position, const glm::vec3& velocity) :
	m_position(position),
	// TODO: Make this a parameter, for now it's just the default rotation quaternion
	m_rotation(1.f, 0.f, 0.f, 0.f),
	m_scale(scale),
	m_velocity(velocity),
	m_model(modelFilename),
	m_shader(shader)
{
	// Work out if the sphere is to be considered moving (if its overall speed is less than 1/2000th of a unit because yayyy, arbitrary numbers!) 
	m_isMoving = glm::length(m_velocity) < (1.f / 2000.f) ? false : true;
}

const glm::vec3& GameObject::GetPosition() const
{
	return m_position;
}

float GameObject::GetScale() const
{
	return m_scale;
}

const glm::quat& GameObject::GetRotation() const
{
	return m_rotation;
}

const glm::vec3& GameObject::GetVelocity() const
{
	return m_velocity;
}

void GameObject::SetPosition(const glm::vec3& position)
{
	m_position = position;
}

void GameObject::SetVelocity(const glm::vec3& velocity)
{
	printf("\n\nCurrent velocity = {\n\tx = %f\n\ty = %f\n\tz = %f\n}\n\n",
		m_velocity.x,
		m_velocity.y,
		m_velocity.z
	);

	printf("\n\nIncoming velocity = {\n\tx = %f\n\ty = %f\n\tz = %f\n}\n\n",
		velocity.x,
		velocity.y,
		velocity.z
	);

	m_velocity = velocity;

	printf("\n\nNew velocity = {\n\tx = %f\n\ty = %f\n\tz = %f\n}\n\n",
		m_velocity.x,
		m_velocity.y,
		m_velocity.z
	);

	m_isMoving = glm::length(m_velocity) < (1.f / 2000.f) ? false : true;

	printf(m_isMoving ? "MOVING\n" : "NOT MOVING\n");
}

void GameObject::Render(const Camera& camera) const
{
	// TODO: THIS IS HACKY, I SHOULD MAKE A LIGHT OBJECT TO CONTROL THIS INFO...
	constexpr glm::vec4 lightColour = glm::vec4(1.f);

	m_shader.Activate();

	glUniform4f(glGetUniformLocation(m_shader.m_ID, "lightColour"), lightColour.x, lightColour.y, lightColour.z, lightColour.w);

	glUniform3f(glGetUniformLocation(m_shader.m_ID, "camPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

	camera.SendMatrixToShader(m_shader, "camMatrix");
	m_model.Render(camera, m_shader, m_position, m_rotation, glm::vec3(m_scale));
}

void GameObject::CleanUp() const
{
	m_shader.Delete();
}
