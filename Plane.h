#pragma once
#include "GameObject.h"

// TODO: IMPLEMENT PLANE CLASS, LEAVING AS AUTOGENERATED FOR NOW....
class Plane : public GameObject
{
public:
	Plane(const std::string& modelFilename, const Shader& shader, float scale, const glm::vec3& position)
		: GameObject(modelFilename, shader, scale, position)
	{
	}

	void Update(float dt) override;
	~Plane() override;
};
