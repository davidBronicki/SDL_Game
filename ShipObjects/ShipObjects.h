#pragma once

#include "Basic/MyGameObjects.h"

class Ship;

class ShipComponent:
	public I_FullWorldObject,
	public Pure_WorldPhysics,
	public Pure_Draw
{
protected:
	std::weak_ptr<Ship> parentShip;
	LifePointCounter hp;

	friend Ship;
public:
	ShipComponent(std::weak_ptr<Ship> parent,
		const ImageTexture& inTexture,
		int maxHP, const PhysicsObject& inState);

	std::shared_ptr<I_Composite>
		getParent() const override;

	void updateMovement() override;

	void hit(Projectile& hit) override;

	void draw() override;
};

enum class ComponentType
{
	Engine,
	Weapon,
	Shield,
	COMPONENT_TYPE_COUNT
};

class ShipChassis : public ShipComponent
{
	std::vector<size_t> componentSlots;//[(int)(ComponentType::COMPONENT_TYPE_COUNT)];
public:
	ShipChassis(std::weak_ptr<Ship> parent,
		const ImageTexture& inTexture,
		int maxHP, const PhysicsObject& inState,
		const std::vector<size_t>& inComponentSlots);

	size_t engineCount();
	size_t weaponCount();
	size_t shieldCount();
};

class Hull : public ShipComponent
{
public:
	Hull(std::weak_ptr<Ship> parent,
		const ImageTexture& inTexture,
		int maxHP, const PhysicsObject& inState);
};

class Engine : public ShipComponent
{
protected:
	float thrust;
public:
	Engine(std::weak_ptr<Ship> parent,
		const ImageTexture& inTexture,
		int maxHP, const PhysicsObject& inState,
		float inThrust);
};

class Ammunition;

class Weapon : public ShipComponent
{
protected:
	int baseDamage;
	int cooldown;
	int currentCooldown;
	std::shared_ptr<Ammunition> ammo;
public:
	Weapon(std::weak_ptr<Ship> parent,
		const ImageTexture& inTexture,
		int maxHP, const PhysicsObject& inState,
		int inBaseDamage,
		std::shared_ptr<Ammunition> inAmmo);
	void pointTo(float angle);
	void fire();

	void updateLogic() override;
};

class Shield : public ShipComponent
{
protected:
	float shieldDamageReduction;
	LifePointCounter sp;
public:
	Shield(std::weak_ptr<Ship> parent,
		const ImageTexture& inTexture,
		int maxHP, const PhysicsObject& inState,
		int maxSP, float inShieldDamageReduction);
};

struct SensorDatum
{
	Vector pos, vel;
	LifePointCounter ScannableHealth, scannableShields;
};

class AI;

class Ship:
	public I_FullWorldObject,
	public Pure_WorldPhysics,
	public I_Composite
{
	bool inertialDampenerEngaged;

	std::weak_ptr<PlaySpace> playSpace;
	std::shared_ptr<AI> controller;

	std::vector<std::shared_ptr<ShipComponent>> components;

	std::shared_ptr<ShipChassis> chassis;
	std::shared_ptr<Hull> hull;
	std::vector<std::shared_ptr<Engine>> engineBank;
	std::vector<std::shared_ptr<Weapon>> weaponBank;
	std::vector<std::shared_ptr<Shield>> shieldBank;//put this stuff on chassis?

	std::vector<std::shared_ptr<Projectile>> insideShots;

	Vector trueVelocity() const;

public:
	Ship(
	std::weak_ptr<PlaySpace> inPlaySpace,
	const Vector& inPos);

	/////changing ship components\\\\\

	void setChassis(std::shared_ptr<ShipChassis> newChassis);
	void setHull(std::shared_ptr<Hull> newHull);
	void setShield(std::shared_ptr<Shield> newShield, size_t slot);
	void setWeapon(std::shared_ptr<Weapon> newWeapon, size_t slot);
	void setEngine(std::shared_ptr<Engine> newEngine, size_t slot);

	/////ship control logic\\\\\

	void setShipController(std::shared_ptr<AI> newController);

	//engine control
	void accelForward();
	void accelBackward();
	void accelRight();
	void accelLeft();
	void inertialBrake();
	void rotateLeft();
	void rotateRight();
	void rotate(float magnitude);

	//general rotation control
	void pointAtAngle(float angle);

	//weapon control
	void fire(size_t weaponSlot);

	//sensors
	std::shared_ptr<std::vector<SensorDatum>>
		pullSensors();

	/////parrent class functions\\\\\

	std::shared_ptr<I_Composite>
		getParent() const override;

	void hit(Projectile& hit) override;

	void updateMovement() override;
	void updateControl() override;
	void updateLogic() override;

	void updateCollisions() override;
	void updateRemoval() override;
	void remove(I_Child* removeItem) override;

	void draw() override;
};
