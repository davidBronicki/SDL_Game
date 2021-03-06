#include "Items/ItemObjects.h"
#include "math.h"
#include <memory>

using namespace std;

Item::Item(unsigned int initialCount,
	const ImageTexture& inTexture)
:
	count(initialCount),
	Pure_Draw(inTexture)
{}

void Item::draw()
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

#include <iostream>

shared_ptr<Projectile> Ammunition::use(
	shared_ptr<I_Composite> parent,
	const Kinematic& weaponState,
	int baseDamage)
{
	return make_shared<Projectile>(
		parent,
		projectileTexture,
		PhysicsObject(
			Kinematic(
				weaponState.pos,//init position
				60 * Vector(cos(weaponState.angle), sin(weaponState.angle))
					+ weaponState.vel,//velocity
				weaponState.angle,//angle
				0),//angular velocity
			1,//mass
			1,//mom of inertia
			0.1));//radius
}