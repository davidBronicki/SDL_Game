#include "Basic/MyGameObjects.h"

#include "Basic/Player.h"

#include "Basic/AdditionalObjects.h"

using namespace std;

/////--------------------Camera-------------------\\\\\

Camera::Camera()
:
	pos(0, 0),
	zoom(0.01)
{}

void Camera::take(const I_CoreUpdate* newController)
{
	controlStack.push_back(newController);
}

bool Camera::setPosition(const I_CoreUpdate* requester, const Vector& newPos)
{
	if (controlStack.size() == 0) return false;
	if (requester == controlStack.back())
	{
		pos = newPos;
		return true;
	}
	else
	{
		return false;
	}
}

bool Camera::setZoom(const I_CoreUpdate* requester, float newZoom)
{
	if (controlStack.size() == 0) return false;
	if (requester == controlStack.back())
	{
		zoom = newZoom;
		return true;
	}
	else
	{
		return false;
	}
}

void Camera::release(const I_CoreUpdate* controller)
{
	for (int i = controlStack.size() - 1; i >= 0; --i)
	{
		if (controller == controlStack[i])
		{
			controlStack.erase(controlStack.begin() + i);
		}
	}
}
#include <iostream>
DrawParameters Camera::stateToParameters(const PhysicsObject& state) const
{
	Vector relativePosition(state.state.pos - pos);
	int size = state.radius * zoom * GameDrawEnvironment::getHeight();
	return DrawParameters::Rectangle(ScreenRectangle(
			relativePosition.x * zoom * GameDrawEnvironment::getHeight()
				+ GameDrawEnvironment::getWidth() / 2 + 0.5f - size,//x
			-(relativePosition.y) * zoom * GameDrawEnvironment::getHeight()
				+ GameDrawEnvironment::getHeight() / 2 + 0.5f - size,//y
			2 * size,//w
			2 * size//h
		),
		state.state.angle);
}

DrawParameters Camera::stateToParameters(const PhysicsObject& state, float z) const
{
	float camDist = 3440.0f / zoom;//dist from cam to player level
	float distToCam = camDist + z;//dist from cam to object
	float ratio = camDist / distToCam;

	Vector relativePosition(state.state.pos - pos);
	relativePosition *= ratio;
	int size = state.radius * zoom * GameDrawEnvironment::getHeight() * ratio;

	return DrawParameters::Rectangle(ScreenRectangle(
		relativePosition.x * zoom * GameDrawEnvironment::getHeight() + GameDrawEnvironment::getWidth() / 2 + 0.5f - size,//x
		-(relativePosition.y) * zoom * GameDrawEnvironment::getHeight() + GameDrawEnvironment::getHeight() / 2 + 0.5f - size,//y
		2 * size,//w
		2 * size//h
	),
		state.state.angle);
}

Vector Camera::drawSpace() const
{
	return Vector((float)(GameDrawEnvironment::getWidth()) / GameDrawEnvironment::getHeight() / zoom,
		1.0f / zoom);
}

Vector Camera::drawSpace(float z) const
{
	float camDist = 3440.0f / zoom;
	float distToCam = camDist + z;
	return drawSpace() * (distToCam / camDist);
}

Vector Camera::pixelLocation(int x, int y) const
{
	float worldX = pos.x + (x - GameDrawEnvironment::getWidth() / 2) / (zoom * GameDrawEnvironment::getHeight());
	float worldY = pos.y - (y - GameDrawEnvironment::getHeight() / 2) / (zoom * GameDrawEnvironment::getHeight());
	return Vector(worldX, worldY);
}


Vector Camera::mouseLocation() const
{
	return pixelLocation(Mouse::x(), Mouse::y());
}

