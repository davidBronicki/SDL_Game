#pragma once
#include "ShipObjects/ShipObjects.h"

class AI:
	public I_CoreUpdate
{
protected:
	std::weak_ptr<Ship> controlObject;
public:
	AI(std::weak_ptr<Ship> inControlObject);
	// void updateMovement() override{}
	// void updateControl() override;
	// void updateLogic() override;
};

class PlayerController:
	public AI
{
	enum controlScheme
	{
		PointToRotate,
		ButtonRotate
	} controls = controlScheme::PointToRotate;
public:
	PlayerController(std::weak_ptr<Ship> inControlObject);
	void toggleControlScheme();
	void updateMovement() override{}
	void updateControl() override;
	void updateLogic() override{}
};
