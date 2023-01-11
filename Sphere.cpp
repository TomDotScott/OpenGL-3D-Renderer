#include "Sphere.h"

#include <glm/gtx/norm.hpp>


Sphere::Sphere(const float radius, const glm::vec3& position, const glm::vec3& velocity) :
	GameObject(
		"assets\\models\\pool_ball\\scene.gltf",
		{ "shaders\\directional_light.vert", "shaders\\directional_light.frag" },
		radius * 2,
		position,
		velocity
	)
{
}

void Sphere::Update(const float deltaTime)
{
	if (m_isMoving)
	{
		m_position += m_velocity * deltaTime;
	}
}

float Sphere::GetRadius() const
{
	return m_scale / 2.f;
}

//void Sphere::SphereToStationarySphere(const Sphere& other)
//{

//}
//
//void Sphere::SphereToMovingSphere(const Sphere& other)
//{
//	printf("Sphere to moving sphere!\n");
//}
