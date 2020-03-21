#include "Basic/Player.h"

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
	// Keyboard::subscribeToKeyPressed(
	// 	function<void(Key)>([this](Key key) -> void
	// 	{
	// 		if (key == Key::Left_Alt)
	// 		{
	// 			if (controls == controlScheme::PointToRotate)
	// 			{
	// 				controls = controlScheme::ButtonRotate;
	// 			}
	// 			else
	// 			{
	// 				controls = controlScheme::PointToRotate;
	// 			}
	// 		}
	// 	}));
}

void Player::newShip(shared_ptr<Ship> newShip)
{
	ship = newShip;
}

void Player::updateGame_ControlLogic()
{
	switch(controls)
	{
		case controlScheme::PointToRotate:
		{
			ship->pointAtAngle(ship->getKinetics().angle);

			GamePosition shipPos = ship->getKinetics().pos;
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
	ship->updateGame_ControlLogic();
}

void Player::updateGame_GeneralLogic()
{
	cam.setPosition(this, ship->getKinetics().pos);

	ship->updateGame_GeneralLogic();
}

void Player::updateEngine_Move()
{
	ship->updateEngine_Move();
}

void Player::updateEngine_Collision(
	shared_ptr<GameObject> self)
{
	ship->updateEngine_Collision(ship);
}

void Player::draw() const
{
	ship->draw();
}
