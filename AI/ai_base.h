#pragma once
#include "ShipObjects/ShipObjects.h"
#include "Basic/UI.h"

class AI:
	public I_CoreUpdate
{
protected:
	std::weak_ptr<Ship> controlObject;
public:
	AI(std::weak_ptr<Ship> inControlObject);
};

class NoControl:
	public AI
{
public:
	NoControl(std::weak_ptr<Ship> inControlObject)
	: AI(inControlObject){}
	void updateMovement() override{}
	void updateControl() override{}
	void updateLogic() override{}
};

class BasicAI:
	public AI
{
	std::shared_ptr<std::vector<SensorDatum>> workingData;
	SensorDatum* targetLock;
public:
	BasicAI(std::weak_ptr<Ship> inControlObject);
	void updateMovement() override{}
	void updateControl() override;
	void updateLogic() override;
};

class PlayerController:
	public AI
{
	enum controlScheme
	{
		PointToRotate,
		ButtonRotate
	} controls = controlScheme::PointToRotate;
	std::shared_ptr<HUD> hud;
public:
	PlayerController(std::weak_ptr<Ship> inControlObject,
		std::shared_ptr<HUD> inHud);
	void toggleControlScheme();
	void updateMovement() override{}
	void updateControl() override;
	void updateLogic() override{}
};
