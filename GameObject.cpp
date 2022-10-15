#include "GameObject.h"

GameObject::GameObject(const std::string& modelFilename, const Shader& shader, const glm::vec3& position, const glm::vec3& velocity) :
	m_position(position),
	m_velocity(velocity),
	m_model(modelFilename),
	m_shader(shader)
{

}

void GameObject::Update(const float dt)
{
	m_position += m_velocity * dt;

	
	printf("GAME_OBJECT_POSITION: (x:%f, y:%f, z:%f)\n", m_position.x, m_position.y, m_position.z);
}

void GameObject::Render(const Camera& camera) const
{
	// TODO: THIS IS HACKY, I SHOULD MAKE A LIGHT OBJECT TO CONTROL THIS INFO...
	constexpr glm::vec4 lightColour = glm::vec4(1.f);

	m_shader.Activate();

	glUniform4f(glGetUniformLocation(m_shader.m_ID, "lightColour"), lightColour.x, lightColour.y, lightColour.z, lightColour.w);

	glUniform3f(glGetUniformLocation(m_shader.m_ID, "camPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

	camera.SendMatrixToShader(m_shader, "camMatrix");
	m_model.Render(camera, m_shader, m_position);
}

void GameObject::CleanUp() const
{
	m_shader.Delete();
}
