#pragma once

#include "MyGameObjects.h"


class Ship;

class ShipComponent : public Entity
{
protected:
	LifePointCounter hp;
	// float mass;
	// float momentOfInertia;
	// GamePosition pos;
	// Vector relativePosition;
	// float radius;
	// float angle;

	friend Ship;
	void setPosition(const GamePosition& shipPosition);
public:
	ShipComponent(const ImageTexture& inTexture,
		int maxHP, const PhysicsObject& inState);
		 // const Vector& inRelativePosition,
		// float inRadius);

	void updateGame_ControlLogic() override;
	void updateGame_GeneralLogic() override;
	void updateEngine_Move() override;

	// void hudDraw();
	// void inWorldHudDraw();

	// bool updateGame_Removal() override;

	void draw() const override;
};

// enum class ComponentType
// {
// 	Hull,
// 	Engine,
// 	Weapon,
// 	Shield,
// 	COMPONENT_TYPE_COUNT
// };

class ShipChassis : public ShipComponent
{
	std::vector<size_t> componentSlots;//[(int)(ComponentType::COMPONENT_TYPE_COUNT)];
public:
	ShipChassis(const ImageTexture& inTexture,
		int maxHP, const PhysicsObject& inState,
		const std::vector<size_t>& inComponentSlots);

	size_t engineCount();
	size_t weaponCount();
	size_t shieldCount();

	// std::shared_ptr<Component> addComponent(
	// 	std::shared_ptr<Component> newComponent,
	// 	ComponentType type, size_t slotNumber);//returns what was there

	// void draw() const override;
};

class Hull : public ShipComponent
{
public:
	Hull(const ImageTexture& inTexture,
		int maxHP, const PhysicsObject& inState);
};

class Engine : public ShipComponent
{
protected:
	float thrust;
public:
	Engine(const ImageTexture& inTexture,
		int maxHP, const PhysicsObject& inState,
		float inThrust);
};

class Ammunition;

class Weapon : public ShipComponent
{
protected:
	int baseDamage;
	std::shared_ptr<Ammunition> ammo;
public:
	Weapon(const ImageTexture& inTexture,
		int maxHP, const PhysicsObject& inState,
		int inBaseDamage,
		std::shared_ptr<Ammunition> inAmmo);
	void fire();
};

class Shield : public ShipComponent
{
protected:
	float shieldDamageReduction;
	LifePointCounter sp;
public:
	Shield(const ImageTexture& inTexture,
		int maxHP, const PhysicsObject& inState,
		int maxSP, float inShieldDamageReduction);
};

class Ship : public Entity
{
	bool inertialDampenerEngaged;

	std::shared_ptr<ShipChassis> chassis;
	std::shared_ptr<Hull> hull;
	std::vector<std::shared_ptr<Engine>> engineBank;
	std::vector<std::shared_ptr<Weapon>> weaponBank;
	std::vector<std::shared_ptr<Shield>> shieldBank;//put this stuff on chassis?
public:
	Ship(const GamePosition& inPos);

	// inline GamePosition getPos() const
	// {
	// 	return state.state.pos;
	// }

	//changing ship components
	void setChassis(std::shared_ptr<ShipChassis> newChassis);
	void setHull(std::shared_ptr<Hull> newHull);
	void setShield(std::shared_ptr<Shield> newShield, size_t slot);
	void setWeapon(std::shared_ptr<Weapon> newWeapon, size_t slot);
	void setEngine(std::shared_ptr<Engine> newEngine, size_t slot);

	/////ship control logic\\\\\

	//engine control
	void accelForward();
	void accelBackward();
	void accelRight();
	void accelLeft();
	void inertialBrake();
	void rotateLeft();
	void rotateRight();
	void rotate(float magnitude);

	//weapon control
	void fire(size_t weaponSlot);

	//GameObject functions
	void updateGame_ControlLogic() override;
	void updateGame_GeneralLogic() override;

	void updateEngine_Move() override;

	void draw() const override;
};
