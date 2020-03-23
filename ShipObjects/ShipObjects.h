#pragma once

#include "Basic/MyGameObjects.h"


class Ship;

class ShipComponent:
	public I_Hittable,
	public Pure_WorldPhysics,
	public Pure_Draw
{
protected:
	LifePointCounter hp;

	friend Ship;
public:
	ShipComponent(std::weak_ptr<I_Composite> parent,
		const ImageTexture& inTexture,
		int maxHP, const PhysicsObject& inState);

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
	ShipChassis(std::weak_ptr<I_Composite> parent,
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
	Hull(std::weak_ptr<I_Composite> parent,
		const ImageTexture& inTexture,
		int maxHP, const PhysicsObject& inState);
};

class Engine : public ShipComponent
{
protected:
	float thrust;
public:
	Engine(std::weak_ptr<I_Composite> parent,
		const ImageTexture& inTexture,
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
	Weapon(std::weak_ptr<I_Composite> parent,
		const ImageTexture& inTexture,
		int maxHP, const PhysicsObject& inState,
		int inBaseDamage,
		std::shared_ptr<Ammunition> inAmmo);
	void pointTo(float angle);
	void fire();
};

class Shield : public ShipComponent
{
protected:
	float shieldDamageReduction;
	LifePointCounter sp;
public:
	Shield(std::weak_ptr<I_Composite> parent,
		const ImageTexture& inTexture,
		int maxHP, const PhysicsObject& inState,
		int maxSP, float inShieldDamageReduction);
};

class Ship:
	public I_Hittable,
	public Pure_WorldPhysics,
	public I_Composite
{
	bool inertialDampenerEngaged;

	std::vector<std::shared_ptr<ShipComponent>> components;

	std::shared_ptr<ShipChassis> chassis;
	std::shared_ptr<Hull> hull;
	std::vector<std::shared_ptr<Engine>> engineBank;
	std::vector<std::shared_ptr<Weapon>> weaponBank;
	std::vector<std::shared_ptr<Shield>> shieldBank;//put this stuff on chassis?

	std::vector<std::shared_ptr<Projectile>> insideShots;

	Vector velocityFromStationary() const;

public:
	Ship(
	std::weak_ptr<I_Composite> parent,
	const Vector& inPos);

	/////changing ship components\\\\\

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

	//general rotation control
	void pointAtAngle(float angle);

	//weapon control
	void fire(size_t weaponSlot);

	/////parrent class functions\\\\\

	void hit(Projectile& hit) override;

	void updateMovement() override;

	void updateCollisions() override;
	void updateRemoval() override;

	void draw() override;
};
