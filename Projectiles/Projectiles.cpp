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

void Projectile::updateGame_ControlLogic()
{
}

void Projectile::updateGame_GeneralLogic()
{
	--lifetime;
	if (lifetime <= 0)
		game.removeFromUpdates(this);
}

void Projectile::updateEngine_Move()
{
	Entity::updateEngine_Move();
	game.projectileHitDetection(state);
}

// void draw() const override
// {

// }