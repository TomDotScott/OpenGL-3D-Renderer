#include "Physics.h"


void Physics::SphereToStationarySphereCCD(const Sphere& stationarySphere, Sphere& movingSphere, const float deltaTime)
{
	constexpr float timeStep = 1 / 60.f;
	float remainingTime = deltaTime;

	// Find the vector that points from this sphere to the other sphere

	while (remainingTime > 0)
	{
		// update the position of the moving sphere based on its velocity
		movingSphere.Update(timeStep);

		const glm::vec3 A = stationarySphere.GetPosition() - movingSphere.GetPosition();

		const float lenA = glm::length(A);

		const glm::vec3 V = movingSphere.GetVelocity();

		const float q = acos(glm::dot(A, V) / (glm::length(A) * glm::length(V)));

		const float d = sin(q) * lenA;

		const float sumRadii = stationarySphere.GetRadius() + movingSphere.GetRadius();


		if (lenA < sumRadii)
		{
			const float e = sqrt(sumRadii * sumRadii - d * d);

			const float magVc = abs(cos(q) * glm::length(A) - e);

			const glm::vec3 Vc = (magVc * V) / glm::length(V);

			const glm::vec3 pointOfCollision = movingSphere.GetPosition() + Vc;

			printf("\n\nPoint of collision = {\n\tx = %f\n\ty = %f\n\tz = %f\n}\n\n",
				pointOfCollision.x,
				pointOfCollision.y,
				pointOfCollision.z
			);


			movingSphere.SetPosition(pointOfCollision);

			movingSphere.SetVelocity(glm::reflect(movingSphere.GetVelocity(), glm::normalize(Vc)));

			remainingTime = 0.f;
		}
		else
		{
			// no collision, decrement the remaining time
			remainingTime -= timeStep;
		}
	}
}


