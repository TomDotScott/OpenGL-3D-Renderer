#pragma once
#include <glm/glm.hpp>

#include "GameObject.h"
#include "Plane.h"

class Sphere : public GameObject
{
public:
	Sphere(float radius, const glm::vec3& position, const glm::vec3& velocity);

	void Update(float deltaTime) override;

	void CheckCollision(const GameObject* other);
	void CheckCollision(const Sphere& other);
	void CheckCollision(const Plane& other);

private:
	glm::vec3 m_velocity;
	bool m_isMoving;

	void SphereToStationarySphere(const Sphere& other);
	void SphereToMovingSphere(const Sphere& other);
};
