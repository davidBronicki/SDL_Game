#include "AI/ai_base.h"
#include <iostream>

#include "Settings/controlMap.h"

using namespace std;

AI::AI(weak_ptr<Ship> inControlObject)
:
	controlObject(inControlObject)
{}

/////-----------------BasicAI-----------------\\\\\

BasicAI::BasicAI(weak_ptr<Ship> inControlObject)
:
	AI(inControlObject)
{}

void BasicAI::updateControl()
{
	if (targetLock != NULL)
	{
		float angle = atan2(targetLock->pos.y,
			targetLock->pos.x);
		controlObject.lock()->pointAtAngle(angle);
		controlObject.lock()->fire(0);
	}
}

void BasicAI::updateLogic()
{
	workingData = controlObject.lock()->pullSensors();

	if (workingData->size() != 0)
	{
		SensorDatum* minDistanceObject(
			(*workingData).data());
		float minDistSqr = 1e30f;
		for (auto&& item : *workingData)
		{
			float distanceSqr = item.pos.magnitudeSqr();
			if (distanceSqr < minDistSqr)
			{
				minDistanceObject = &item;
				minDistSqr = distanceSqr;
			}
		}
		targetLock = minDistanceObject;
	}
	else
	{
		targetLock = NULL;
	}
}

/////-------------PlayerController-------------\\\\\

PlayerController::PlayerController(
	weak_ptr<Ship> inControlObject,
	std::shared_ptr<HUD> inHud)
:
	AI(inControlObject),
	hud(inHud),
	targetHudElement(make_shared<TargetElement>())
{
	hud->addToDraw(targetHudElement);
	Mouse::subscribeToButtonPressed(
		function<void(Button)>([this](Button button) -> void
		{
			if (button == Button::Left)
			{
				try
				{
					controlObject.lock()->fire(0);
				}
				catch (const exception& e)
				{
					cout << "exception when firing" << endl;
					cout << e.what() << endl;
				}
			}
		}));
}

void PlayerController::toggleControlScheme()
{
	if (controls == controlScheme::PointToRotate)
	{
		controls = controlScheme::ButtonRotate;
	}
	else
	{
		controls = controlScheme::PointToRotate;
	}
}

void PlayerController::updateControl()
{
	switch(controls)
	{
		case controlScheme::PointToRotate:
		{
			controlObject.lock()->pointAtAngle(controlObject.lock()->getKinetics().angle);

			Vector shipPos = controlObject.lock()->getKinetics().pos;
			auto cursorPos = Camera::getInstance().mouseLocation();
			auto relativePos = cursorPos - shipPos;
			controlObject.lock()->pointAtAngle(atan2(relativePos.y, relativePos.x));
		}
		break;
		case controlScheme::ButtonRotate:
		{
			if (Keyboard::isDown(ControlMap::getActionKey(Action::RotateRight)))
			{
				controlObject.lock()->rotateRight();
			}
			else if (Keyboard::isDown(ControlMap::getActionKey(Action::RotateLeft)))
			{
				controlObject.lock()->rotateLeft();
			}
		}
		break;
	}

	if (Keyboard::isDown(ControlMap::getActionKey(Action::AccelForward)) || Mouse::isDown(Button::Right))
	{
		controlObject.lock()->accelForward();
	}

	if (Keyboard::isDown(ControlMap::getActionKey(Action::Decel)))
	{
		controlObject.lock()->inertialBrake();
	}
}

void PlayerController::updateLogic()
{
	std::shared_ptr<std::vector<SensorDatum>> workingData(
		controlObject.lock()->pullSensors());

	SensorDatum* targetLock;

	if (workingData->size() != 0)
	{
		SensorDatum* minDistanceObject(
			(*workingData).data());
		float minDistSqr = 1e30f;
		for (auto&& item : *workingData)
		{
			float distanceSqr = item.pos.magnitudeSqr();
			if (distanceSqr < minDistSqr)
			{
				minDistanceObject = &item;
				minDistSqr = distanceSqr;
			}
		}
		targetLock = minDistanceObject;
	}
	else
	{
		targetLock = NULL;
	}

	targetHudElement->updatePosition(
		make_shared<PhysicsObject>(Kinematic(targetLock->pos), 1, 1, 1));
}
