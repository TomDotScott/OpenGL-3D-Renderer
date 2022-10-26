#include "Sphere.h"

#include <glm/gtx/norm.hpp>


Sphere::Sphere(const float radius, const glm::vec3& position, const glm::vec3& velocity) :
	GameObject(
		"assets\\models\\pool_ball\\scene.gltf",
		{ "shaders\\directional_light.vert", "shaders\\directional_light.frag" },
		radius * 2,
		position
	),
	m_velocity(velocity)
{
	// Work out if the sphere is to be considered moving (if its overall speed is less than 1/2000th of a unit because yayyy, arbitrary numbers!) 
	m_isMoving = glm::length2(m_velocity) < 0.0005 * 0.0005 ? false : true;
}

void Sphere::Update(const float deltaTime)
{
	if (m_isMoving)
	{
		m_position += m_velocity * deltaTime;
	}
}

void Sphere::CheckCollision(const GameObject* other)
{
	if (const Sphere* sphere = dynamic_cast<const Sphere*>(other))
	{
		CheckCollision(*sphere);
	}
	else if (const Plane* plane = dynamic_cast<const Plane*>(other))
	{
		CheckCollision(*plane);
	}
}

void Sphere::CheckCollision(const Sphere& other)
{
	if (m_isMoving && other.m_isMoving)
	{
		SphereToMovingSphere(other);
	}
	else
	{
		SphereToStationarySphere(other);
	}
}

void Sphere::CheckCollision(const Plane& other)
{

}

void Sphere::SphereToStationarySphere(const Sphere& other)
{
	printf("Sphere to stationary sphere!\n");
}

void Sphere::SphereToMovingSphere(const Sphere& other)
{
	printf("Sphere to moving sphere!\n");
}
