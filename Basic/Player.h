#pragma once

#include "Basic/Interfaces.h"

class PlayerController;
class Ship;

class Player:
	public I_CoreUpdate,
	public I_Draw
{
	std::shared_ptr<Ship> ship;
	std::shared_ptr<PlayerController> controller;

public:
	Player();

	void newShip(std::shared_ptr<Ship> newShip);

	void updateControl() override;
	void updateLogic() override;

	void updateMovement() override;

	void draw() override;
};