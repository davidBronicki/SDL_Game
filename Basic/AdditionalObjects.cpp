#include "Basic/AdditionalObjects.h"

#include "Basic/MyGameObjects.h"

/////--------------------ParticleField-------------------\\\\\

ParticleField::ParticleField(
	size_t count, float maxSpeed, float maxRotation,
	float minZ, float maxZ, float minSize, float maxSize)
:
	zVals(count)
{
	for (int i = 0; i < count; ++i)
	{
		zVals[i] = randValue() * (maxZ - minZ) + minZ;
		float size = randValue() * (maxSize - minSize) + minSize;
		float theta = randValue() * 2 * M_PI;
		float temp = randValue();
		float speed = temp * temp * maxSpeed;
		Vector velocity(speed * cos(theta), speed * sin(theta));
		float rotation = randValue() * 2 * maxRotation - maxRotation;

		Vector drawSpace(cam.drawSpace(zVals[i]));
		Vector minPoint(cam.getPos() - drawSpace / 2);
		Vector pos(
			randValue() * drawSpace.x + minPoint.x,
			randValue() * drawSpace.y + minPoint.y);
		states.push_back(PhysicsObject(
			Kinematic(pos, velocity, 0,  rotation),
			1.0f, 1.0f, size));
	}
}

void ParticleField::setCenter(
	std::shared_ptr<I_WorldKinetic> inCenter)
{
	center = inCenter;
}

void ParticleField::updateMovement()
{
	for (int i = 0; i < states.size(); ++i)
	{
		states[i].state.pos = Vector(
			states[i].state.pos.x - center->pos().x,
			states[i].state.pos.y - center->pos().y);
		states[i].state.vel =
			states[i].state.vel - center->vel();
		states[i].update();

		Vector drawSpace(cam.drawSpace(zVals[i]));
		Vector minPoint(cam.getPos() - drawSpace / 2);
		Vector relativePosition(states[i].getPos() - minPoint);

		if (relativePosition.x < 0.1 * drawSpace.x)
		{
			states[i].state.pos += Vector(1.2 * drawSpace.x, 0);
		}
		else if (relativePosition.x > 1.2 * drawSpace.x)
		{
			states[i].state.pos += Vector(-1.2 * drawSpace.x, 0);
		}

		if (relativePosition.y < 0.1 * drawSpace.y)
		{
			states[i].state.pos += Vector(0, 1.2 * drawSpace.y);
		}
		else if (relativePosition.y > 1.2 * drawSpace.y)
		{
			states[i].state.pos += Vector(0, -1.2 * drawSpace.y);
		}
	}
}

// void ParticleField::updateEngine_Collision(){}

void ParticleField::draw()
{
	for (int i = 0; i < states.size(); ++i)
	{
		GameDrawEnvironment::renderRectFilled(
			cam.stateToParameters(states[i], zVals[i]));
	}
}
