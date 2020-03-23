#pragma once

#include "Basic/Interfaces.h"

class ParticleField:
	public I_CoreUpdate,
	public I_Draw
{
	std::shared_ptr<I_WorldKinetic> center;
	std::vector<PhysicsObject> states;
	std::vector<float> zVals;
public:
	ParticleField(size_t count, float maxSpeed,
		float maxRotation, float minZ, float maxZ,
		float minSize, float maxSize);

	void setCenter(
		std::shared_ptr<I_WorldKinetic> inCenter);

	void updateMovement() override;

	void draw() override;
};
