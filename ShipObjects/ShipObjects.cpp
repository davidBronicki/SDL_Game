#include "ShipObjects/ShipObjects.h"
// #include "Items/ItemObjects.h"
// #include "Projectiles/Projectiles.h"

#include "math.h"

#include <iostream>

using namespace std;

/////----------------ShipComponent-----------------\\\\\

ShipComponent::ShipComponent(
	weak_ptr<I_Composite> parent,
	const ImageTexture& inTexture,
	int maxHP, const PhysicsObject& inState)
:
	I_Hittable(parent),
	Pure_Draw(inTexture),
	Pure_WorldPhysics(inState),
	hp{(float)maxHP, maxHP}
{}

void ShipComponent::hit(Projectile& hit)
{
	hp.health -= 10;
	// hp.health -= p->getDamage();
	// game.removeFromUpdates((Entity*)(&p));
}

void ShipComponent::draw()
{
	worldTexture.draw(cam.stateToParameters(state));
}

/////----------------ShipChassis----------------\\\\\

ShipChassis::ShipChassis(
	weak_ptr<I_Composite> parent,
	const ImageTexture& inTexture,
	int maxHP, const PhysicsObject& inState,
	const std::vector<size_t>& inComponentSlots)
:
	ShipComponent(parent, inTexture,
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

Hull::Hull(
	weak_ptr<I_Composite> parent,
	const ImageTexture& inTexture,
	int maxHP, const PhysicsObject& inState)
:
	ShipComponent(parent, inTexture,
		maxHP, inState)
{}

/////------------------Engine-------------------\\\\\

Engine::Engine(
	weak_ptr<I_Composite> parent,
	const ImageTexture& inTexture,
	int maxHP, const PhysicsObject& inState,
	float inThrust)
:
	ShipComponent(parent, inTexture,
		maxHP, inState),
	thrust(inThrust)
{}

/////------------------Weapon-------------------\\\\\

Weapon::Weapon(
	weak_ptr<I_Composite> parent,
	const ImageTexture& inTexture,
	int maxHP, const PhysicsObject& inState,
	int inBaseDamage,
	shared_ptr<Ammunition> inAmmo)
:
	ShipComponent(parent, inTexture,
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
	// ammo->use( state.state, baseDamage);
}

/////------------------Shield-------------------\\\\\

Shield::Shield(
	weak_ptr<I_Composite> parent,
	const ImageTexture& inTexture,
	int maxHP, const PhysicsObject& inState,
	int maxSP, float inShieldDamageReduction)
:
	ShipComponent(parent, inTexture,
		maxHP, inState),
	sp{(float)maxSP, maxSP},
	shieldDamageReduction(inShieldDamageReduction)
{}

/////--------------------Ship-------------------\\\\\

Vector Ship::velocityFromStationary() const
{
	return state.state.vel +
		dynamic_pointer_cast<PlaySpace>(parent.lock())
		->getStationaryVelocity();
}

Ship::Ship(
	weak_ptr<I_Composite> parent,
	const Vector& inPos)
:
	I_Hittable(parent),
	Pure_WorldPhysics(inPos, 1, 1, 1),
	inertialDampenerEngaged(true)
{}

void Ship::setChassis(shared_ptr<ShipChassis> newChassis)
{
	chassis = newChassis;
	components.push_back(chassis);
	state.mass = chassis->getState().mass;
	state.momentInertia = chassis->getState().momentInertia;
	state.radius = chassis->getState().radius;
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
		components.push_back(newShield);
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
		// newWeapon->noAngleFlag = true;
		components.push_back(newWeapon);
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
		components.push_back(newEngine);
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
	state.force -= velocityFromStationary() * 2.0;
}

void Ship::rotateLeft()
{
	state.torque += 15;//counterclockwise motion is positive
}

void Ship::rotateRight()
{
	state.torque -= 15;
}
void Ship::rotate(float magnitude)
{
	state.torque += min(max(20 * magnitude, -20.0f), 20.0f);
}

void Ship::pointAtAngle(float angle)
{
	float relativeAngleToCurser =
		angle - state.state.angle;
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

void Ship::hit(Projectile& hit)
{

	/*

***I am temporarily forgoing the following.
While the projectile is inside the ship, it will
not be able to collide with anything in the environment.
This can be changed if need be.

Potential bug: I am aiming to make this able
to collide with internal structures of the ship
and simultaniously be able to collide with other objects
just in case there are very nearby objects.

It is possible for there to be a very nearby composite
object (such as a ship) which will sieze control
of the projectile upon calling of collide. This
would take control away from this ship and therefor
not allow impact with internal structures of this ship.

	*/

	// game.removeFromUpdates((Entity*)(&hit));
	// insideShots.push_back(p);

	chassis->hit(hit);
	if (chassis->hp.health <= 0)
		dynamic_pointer_cast<PlaySpace>
			(parent.lock())->remove(this);
}

void Ship::updateMovement()
{
	if (inertialDampenerEngaged)
	{
		state.force -= state.state.vel * 0.2;
		state.torque -= state.state.angularVel * 5.0;
	}
	Pure_WorldPhysics::update(
		GameUpdateEnvironment::getDT());
	chassis->setState(state);

	// for (auto&& item : components)
	// {
	// 	item->updateMovement();
	// }

	// for ()

	// for (auto&& projectile : insideShots)
	// {
	// 	projectile->updateMovement();
	// 	Entity* hitObject(NULL);
	// 	// float earliestImpact = game.projectileHitDetection(
	// 	// 	*projectile, hitObject);
	// 	float earliestImpact = 2;
	// 	for (auto&& item : children)
	// 	{
	// 		float impactTime = projectileHitDetection(
	// 			*projectile, *item);
	// 		if (impactTime > 0 && impactTime < earliestImpact)
	// 		{
	// 			earliestImpact = impactTime;
	// 			hitObject = item.get();
	// 		}
	// 	}
	// }
}

void Ship::updateCollisions()
{

}

void Ship::updateRemoval()
{}

void Ship::draw()
{
	chassis->draw();
	weaponBank[0]->draw();
}
