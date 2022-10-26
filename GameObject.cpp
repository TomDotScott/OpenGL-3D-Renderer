#include "GameObject.h"

GameObject::GameObject(const std::string& modelFilename, const Shader& shader, const float scale, const glm::vec3& position) :
	m_position(position),
	// TODO: Make this a parameter, for now it's just the default rotation quaternion
	m_rotation(1.f, 0.f, 0.f, 0.f),
	m_scale(scale),
	m_model(modelFilename),
	m_shader(shader)
{

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
