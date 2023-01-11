#pragma once
#include "Sphere.h"

class Physics
{
public:
	static void SphereToStationarySphereCCD(const Sphere& stationarySphere, Sphere& movingSphere, float deltaTime);

};
