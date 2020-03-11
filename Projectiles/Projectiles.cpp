#include "Projectiles/Projectiles.h"


Projectile::Projectile(const ImageTexture& inWorldTexture,
	const PhysicsObject& inState,
	float inDamage,
	size_t inLifetime)
:
	Entity(inWorldTexture, inState),
	damage(inDamage),
	lifetime(inLifetime)
{}

float Projectile::getDamage()
{
	return damage;
}

// void Projectile::updateGame_ControlLogic()
// {
// }

void Projectile::updateGame_GeneralLogic()
{
	--lifetime;
	if (lifetime <= 0)
		game.removeFromUpdates(this);
}

void Projectile::updateEngine_Collision()
{
	Entity* collisionObject(game.projectileHitDetection(state));
	if (collisionObject != NULL)
	{
		collisionObject->collide(*this);
	}
}

// void draw() const override
// {

// }