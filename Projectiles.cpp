#include "Projectiles.h"


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
	--lifetime;
}

void Projectile::updateGame_GeneralLogic()
{

}

// void updateEngine_Move() override
// {

// }

// void draw() const override
// {

// }