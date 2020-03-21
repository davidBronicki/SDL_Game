#pragma once

#include "Basic/BasicGameObjects.h"

class ParticleField : public GameObject
{
	std::vector<PhysicsObject> states;
	std::vector<float> zVals;
public:
	ParticleField(size_t count, float maxSpeed, float maxRotation,
		float minZ, float maxZ, float minSize, float maxSize);

	void updateEngine_Move() override;

	void draw() const override;
};
