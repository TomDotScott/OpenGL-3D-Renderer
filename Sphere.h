#pragma once
#include <glm/glm.hpp>

#include "GameObject.h"

class Sphere final : public GameObject
{
public:
	Sphere(float radius, const glm::vec3& position, const glm::vec3& velocity);

	void Update(float deltaTime) override;

	float GetRadius() const;
};
