#pragma once
#include "Model.h"

struct Shader;
class Camera;

class GameObject
{
public:
	GameObject(const std::string& modelFilename, const Shader& shader, float scale = 1.f, const glm::vec3& position = glm::vec3(0.f));

	virtual void Update(float dt) = 0;
	virtual void CheckCollision(const GameObject* other) = 0;

	void Render(const Camera& camera) const;

	void CleanUp() const;

	virtual ~GameObject() = default;
protected:
	glm::vec3 m_position;
	glm::quat m_rotation;
	float m_scale;

private:
	Model m_model;
	Shader m_shader;
};
