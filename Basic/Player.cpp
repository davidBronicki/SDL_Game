#include "Basic/Player.h"
#include "ShipObjects/ShipObjects.h"

#include <iostream>

using namespace std;

Player::Player()
{
	Keyboard::subscribeToKeyPressed(
		function<void(Key)>([this](Key key) -> void
		{
			if (key == Key::Left_Alt)
			{
				controller->toggleControlScheme();
			}
		}));
}

void Player::newShip(shared_ptr<Ship> newShip)
{
	controller = make_shared<PlayerController>(newShip);
	newShip->setShipController(controller);
	ship = newShip;
}

void Player::updateControl()
{
}

void Player::updateLogic()
{
	cam.setPosition(this, ship->getKinetics().pos);
}

void Player::updateMovement()
{

}

void Player::draw()
{
	
}
