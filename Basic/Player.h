#pragma once

class Ship;

class Player : public GameObject
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

	void updateGame_ControlLogic() override;
	void updateGame_GeneralLogic() override;

	void updateEngine_Move() override;
	// void updateEngine_Collision(
	// 	std::shared_ptr<GameObject> self) override;

	void draw() const override;
};