#include "Basic/Player.h"
#include "ShipObjects/ShipObjects.h"

#include <iostream>

using namespace std;

Player::Player()
{
	Mouse::subscribeToButtonPressed(
		function<void(Button)>([this](Button button) -> void
		{
			if (button == Button::Left)
			{
				try
				{
					ship->fire(0);
				}
				catch (const exception& e)
				{
					cout << "exception when firing" << endl;
					cout << e.what() << endl;
				}
			}
		}));

	Keyboard::subscribeToKeyPressed(
		function<void(Key)>([this](Key key) -> void
		{
			if (key == Key::Left_Alt)
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
		}));
}

void Player::newShip(shared_ptr<Ship> newShip)
{
	ship = newShip;
}

void Player::updateControl()
{
	switch(controls)
	{
		case controlScheme::PointToRotate:
		{
			ship->pointAtAngle(ship->getKinetics().angle);

			Vector shipPos = ship->getKinetics().pos;
			auto cursorPos = Camera::getInstance().mouseLocation();
			auto relativePos = cursorPos - shipPos;
			ship->pointAtAngle(atan2(relativePos.y, relativePos.x));
		}
		break;
		case controlScheme::ButtonRotate:
		{
			if (Keyboard::isDown(Key::D))
			{
				ship->rotateRight();
			}
			else if (Keyboard::isDown(Key::A))
			{
				ship->rotateLeft();
			}
		}
		break;
	}

	if (Keyboard::isDown(Key::W) || Mouse::isDown(Button::Right))
	{
		ship->accelForward();
	}

	if (Keyboard::isDown(Key::S))
	{
		ship->inertialBrake();
	}
	// ship->updateControl();
}

void Player::updateLogic()
{
	cam.setPosition(this, ship->getKinetics().pos);

	// ship->updateLogic();
}

void Player::updateMovement()
{
	// ship->updateMovement();
}

// void Player::updateEngine_Collision()
// {
// 	// ship->updateEngine_Collision(ship);
// }

void Player::draw()
{
	// ship->draw();
}
