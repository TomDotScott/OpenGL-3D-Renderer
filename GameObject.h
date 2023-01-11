#pragma once
#include "Model.h"

struct Shader;
class Camera;

class GameObject
{
public:
	GameObject(const std::string& modelFilename, const Shader& shader, float scale = 1.f, const glm::vec3& position = glm::vec3(0.f), const glm::vec3& velocity = glm::vec3(0.f));

	virtual void Update(float dt) = 0;

	const glm::vec3& GetPosition() const;
	float GetScale() const;
	const glm::quat& GetRotation() const;
	const glm::vec3& GetVelocity() const;

	void SetPosition(const glm::vec3& position);
	void SetVelocity(const glm::vec3& velocity);

	void Render(const Camera& camera) const;
	void CleanUp() const;

	virtual ~GameObject() = default;
protected:
	glm::vec3 m_position;
	glm::quat m_rotation;
	float m_scale;

	glm::vec3 m_velocity;
	bool m_isMoving;

private:
	Model m_model;
	Shader m_shader;
};
