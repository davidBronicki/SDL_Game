#pragma once

#include "Basic/Interfaces.h"

class Ship;

class Player:
	public I_CoreUpdate,
	public I_Draw
{
	std::shared_ptr<Ship> ship;

	enum controlScheme
	{
		PointToRotate,
		ButtonRotate
	} controls = controlScheme::PointToRotate;

public:
	Player();

	void newShip(std::shared_ptr<Ship> newShip);

	void updateControl() override;
	void updateLogic() override;

	void updateMovement() override;

	void draw() override;
};