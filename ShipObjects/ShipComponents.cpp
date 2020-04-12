#include "ShipObjects/ShipObjects.h"
#include "Items/ItemObjects.h"

using namespace std;

/////----------------ShipComponent-----------------\\\\\

ShipComponent::ShipComponent(
	weak_ptr<Ship> parent,
	const ImageTexture& inTexture,
	int maxHP, const PhysicsObject& inState)
:
	parentShip(parent),
	Pure_Draw(inTexture),
	Pure_WorldPhysics(inState),
	hp{(float)maxHP, maxHP}
{}

shared_ptr<I_Composite> ShipComponent::getParent() const
{
	return dynamic_pointer_cast<I_Composite>(
		parentShip.lock());
}

void ShipComponent::hit(Projectile& hit)
{
	hp.health -= 10;
}

void ShipComponent::updateMovement()
{
	Pure_WorldPhysics::update(
		GameUpdateEnvironment::getDT());
	updateHitBoxes({state.radius,state.state.pos});
}

void ShipComponent::draw()
{
	worldTexture.draw(cam.stateToParameters(
		state
		+ dynamic_pointer_cast<Ship>(parentShip.lock())
		->getState()));
}

/////----------------ShipChassis----------------\\\\\

ShipChassis::ShipChassis(
	weak_ptr<Ship> parent,
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
	weak_ptr<Ship> parent,
	const ImageTexture& inTexture,
	int maxHP, const PhysicsObject& inState)
:
	ShipComponent(parent, inTexture,
		maxHP, inState)
{}

/////------------------Engine-------------------\\\\\

Engine::Engine(
	weak_ptr<Ship> parent,
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
	weak_ptr<Ship> parent,
	const ImageTexture& inTexture,
	int maxHP, const PhysicsObject& inState,
	int inBaseDamage,
	shared_ptr<Ammunition> inAmmo)
:
	ShipComponent(parent, inTexture,
		maxHP, inState),
	baseDamage(inBaseDamage),
	ammo(inAmmo),
	cooldown(60),
	currentCooldown(0)
{}

void Weapon::pointTo(float angle)
{
	state.state.angle = angle;
}

void Weapon::fire()
{
	if (currentCooldown <= 0)
	{
		dynamic_pointer_cast<PlaySpace>(
			parentShip.lock()->getParent())->addProjectile(
				(ammo->use(parentShip.lock()->getParent(),
					state.state, baseDamage)));
		currentCooldown = cooldown;
	}
}

void Weapon::updateLogic()
{
	--currentCooldown;
}

/////------------------Shield-------------------\\\\\

Shield::Shield(
	weak_ptr<Ship> parent,
	const ImageTexture& inTexture,
	int maxHP, const PhysicsObject& inState,
	int maxSP, float inShieldDamageReduction)
:
	ShipComponent(parent, inTexture,
		maxHP, inState),
	sp{(float)maxSP, maxSP},
	shieldDamageReduction(inShieldDamageReduction)
{}
