#include "ShipObjects/ShipObjects.h"
#include "Items/ItemObjects.h"

#include "math.h"

#include <iostream>

using namespace std;

/////----------------ShipComponent-----------------\\\\\

ShipComponent::ShipComponent(const ImageTexture& inTexture,
	int maxHP, const PhysicsObject& inState)
:
	Entity(inTexture, inState),
	hp{(float)maxHP, maxHP}
{}

void ShipComponent::updateGame_ControlLogic()
{
	
}

void ShipComponent::updateGame_GeneralLogic()
{
	
}

/////----------------ShipChassis----------------\\\\\

ShipChassis::ShipChassis(
	const ImageTexture& inTexture,
	int maxHP, const PhysicsObject& inState,
	const std::vector<size_t>& inComponentSlots)
:
	ShipComponent(inTexture,
		maxHP, inState),
	componentSlots(inComponentSlots)
{}

size_t ShipChassis::engineCount()
{
	return componentSlots[(size_t)(ComponentType::Engine)];
}
size_t ShipChassis::weaponCount()
{
	return componentSlots[(size_t)(ComponentType::Weapon)];
}
size_t ShipChassis::shieldCount()
{
	return componentSlots[(size_t)(ComponentType::Shield)];
}

/////---------------------Hull----------------------\\\\\

Hull::Hull(const ImageTexture& inTexture,
	int maxHP, const PhysicsObject& inState)
:
	ShipComponent(inTexture,
		maxHP, inState)
{}

/////------------------Engine-------------------\\\\\

Engine::Engine(const ImageTexture& inTexture,
	int maxHP, const PhysicsObject& inState,
	float inThrust)
:
	ShipComponent(inTexture,
		maxHP, inState),
	thrust(inThrust)
{}

/////------------------Weapon-------------------\\\\\

Weapon::Weapon(const ImageTexture& inTexture,
	int maxHP, const PhysicsObject& inState,
	int inBaseDamage,
	shared_ptr<Ammunition> inAmmo)
:
	ShipComponent(inTexture,
		maxHP, inState),
	baseDamage(inBaseDamage),
	ammo(inAmmo)
{}

void Weapon::pointTo(float angle)
{
	state.state.angle = angle;
}

void Weapon::fire()
{
	ammo->fire(state.state, baseDamage);
}

/////------------------Shield-------------------\\\\\

Shield::Shield(const ImageTexture& inTexture,
	int maxHP, const PhysicsObject& inState,
	int maxSP, float inShieldDamageReduction)
:
	ShipComponent(inTexture,
		maxHP, inState),
	sp{(float)maxSP, maxSP},
	shieldDamageReduction(inShieldDamageReduction)
{}

/////--------------------Ship-------------------\\\\\

Ship::Ship(const GamePosition& inPos)
:
	Entity(NULL_TEXTURE,
		PhysicsObject(Kinematic(inPos), 0, 0, 1)),
	inertialDampenerEngaged(true)
{}

void Ship::setChassis(shared_ptr<ShipChassis> newChassis)
{
	chassis = newChassis;
	children.push_back(chassis);
	state.mass = chassis->state.mass;
	state.momentInertia = chassis->state.momentInertia;
	state.radius = chassis->state.radius;
	engineBank.resize(chassis->engineCount());
	weaponBank.resize(chassis->weaponCount());
	shieldBank.resize(chassis->shieldCount());
}

void Ship::setHull(shared_ptr<Hull> newHull)
{
	hull = newHull;
}

void Ship::setShield(shared_ptr<Shield> newShield, size_t slot)
{
	if (slot <= shieldBank.size())
	{
		shieldBank[slot] = newShield;
		children.push_back(newShield);
	}
	else
	{
		throw length_error("Shield slot out of range.");
	}
}

void Ship::setWeapon(shared_ptr<Weapon> newWeapon, size_t slot)
{
	if (slot <= weaponBank.size())
	{
		weaponBank[slot] = newWeapon;
		children.push_back(newWeapon);
	}
	else
	{
		throw length_error("Weapon slot out of range.");
	}
}

void Ship::setEngine(shared_ptr<Engine> newEngine, size_t slot)
{
	if (slot <= engineBank.size())
	{
		engineBank[slot] = newEngine;
		children.push_back(newEngine);
	}
	else
	{
		throw length_error("Engine slot out of range.");
	}
}

void Ship::accelForward()
{
	state.force += 20 * Vector(cos(state.state.angle), sin(state.state.angle));
}

void Ship::accelBackward()
{
	state.force -= 20 * Vector(cos(state.state.angle), sin(state.state.angle));
}

void Ship::accelRight()
{
	state.force += Vector(5, 0);
}

void Ship::accelLeft()
{
	state.force += Vector(-5, 0);
}

void Ship::inertialBrake()
{
	state.force -= state.state.vel * 2.0;
}

void Ship::rotateLeft()
{
	state.torque += 3;//counterclockwise motion is positive
}

void Ship::rotateRight()
{
	state.torque -= 3;
}
void Ship::rotate(float magnitude)
{
	state.torque += min(max(20 * magnitude, -20.0f), 20.0f);
}

void Ship::pointAtAngle(float angle)
{
	float relativeAngleToCurser =
		angle - getPhysics().state.angle;
	if (relativeAngleToCurser < M_PI) relativeAngleToCurser += 2 * M_PI;
	if (relativeAngleToCurser > M_PI) relativeAngleToCurser -= 2 * M_PI;
	rotate(relativeAngleToCurser);
	for (auto&& weapon : weaponBank)
	{
		weapon->pointTo(angle);
	}
}

void Ship::fire(size_t weaponSlot)
{
	if (weaponSlot <= weaponBank.size())
	{
		if (weaponBank[weaponSlot].get() == NULL) throw SDL_MiscException("weaponBank slot NULL");
		weaponBank[weaponSlot]->fire();
	}
	else
	{
		throw length_error("Weapon slot out of range.");
	}
}



void Ship::updateGame_ControlLogic()
{}

void Ship::updateGame_GeneralLogic(){}

void Ship::updateEngine_Move()
{
	Entity::updateEngine_Move();
	if (inertialDampenerEngaged)
	{
		state.force -= state.state.vel * 0.2;
		state.torque -= state.state.angularVel * 5.0;
	}
}

void Ship::draw() const
{
	chassis->draw();
	weaponBank[0]->draw();
}