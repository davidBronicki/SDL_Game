#pragma once

#include "Basic/Interfaces.h"
#include "Basic/UI.h"

class PlayerController;
class Ship;

class Player:
	public I_CoreUpdate,
	public I_Draw
{
	std::shared_ptr<Ship> ship;
	std::shared_ptr<PlayerController> controller;
	std::shared_ptr<HUD> hud;

public:
	Player(std::shared_ptr<HUD> inHud);

	void newShip(std::shared_ptr<Ship> newShip);

	void updateControl() override;
	void updateLogic() override;

	void updateMovement() override;

	void draw() override;
};