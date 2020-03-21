#include "Projectiles/Projectiles.h"
#include "math.h"

using namespace std;

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

void Projectile::realignPartialTick(float timeOfImpact, float kineticReduction)
{
	float reverseTime = 1 - timeOfImpact;
	state.update(-reverseTime);//brings projectile back to moment of impact
	float kineticEnergy = 0.5f * state.mass * state.state.vel.magnitudeSqr();
	if (kineticReduction >= kineticEnergy) game.removeFromUpdates(this);
	else
	{
		float reductionRatio = 1 - kineticReduction / kineticEnergy;
		state.state.vel *= sqrt(reductionRatio);
		state.update(reverseTime);
	}
}

void Projectile::updateGame_GeneralLogic()
{
	--lifetime;
	if (lifetime <= 0)
		game.removeFromUpdates(this);
}

void Projectile::updateEngine_Collision(
	shared_ptr<GameObject> self)
{
	Entity* collisionObject(NULL);
	float timeOfImpact = game.projectileHitDetection(
		*this, collisionObject);
	if (collisionObject != NULL)
	{
		collisionObject->collide(
			static_pointer_cast<Projectile>(self));
	}
}
