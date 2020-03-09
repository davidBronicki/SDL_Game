#include "ItemObjects.h"
#include "Projectiles.h"
#include "math.h"
#include <memory>

Item::Item(unsigned int initialCount,
	const ImageTexture& inTexture)
:
	count(initialCount),
	itemTexture(inTexture)
{}

void Item::updateGame_ControlLogic()
{

}
void Item::updateGame_GeneralLogic()
{

}
void Item::updateEngine_Move()
{

}

void Item::draw() const
{

}

Ammunition::Ammunition(unsigned int initialCount,
	const ImageTexture& inItemTexture,
	const ImageTexture& inProjectileTexture,
	float inDamageModifier)
:
	Item(initialCount, inItemTexture),
	projectileTexture(inProjectileTexture),
	damageModifier(inDamageModifier)
{}

void Ammunition::fire(const GamePosition& weaponPosition,
	float angleOfFire, int baseDamage)
{
	game.addToUpdateList(
		std::make_shared<Projectile>(projectileTexture,
			PhysicsObject(
				Kinematic(
					weaponPosition,//init position
					10 * Vector(cos(angleOfFire), sin(angleOfFire)),//velocity
					angleOfFire,//angle
					0),//angular velocity
				1,//mass
				1,//mom of inertia
				1),//radius
			baseDamage * damageModifier,//damage
			100));//lifetime
}