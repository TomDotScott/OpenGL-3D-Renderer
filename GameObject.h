#pragma once
#include "Model.h"

struct Shader;
class Camera;

class GameObject
{
public:
	GameObject(const std::string& modelFilename, const Shader& shader, const glm::vec3& position = glm::vec3(0.f), const glm::vec3& velocity = glm::vec3(0.f));

	virtual void Update(float dt);
	void Render(const Camera& camera) const;

	void CleanUp() const;

	virtual ~GameObject() = default;

private:
	glm::vec3 m_position;
	glm::vec3 m_velocity;

	Model m_model;
	Shader m_shader;
};
