#pragma once

#include "Basic/BasicGameObjects.h"

class Projectile : public Entity
{
protected:
	float damage;
	size_t lifetime;
	std::shared_ptr<CompositeEntity> parent;
public:
	Projectile(const ImageTexture& inWorldTexture,
		const PhysicsObject& inState,
		float inDamage,
		size_t inLifetime);
	
	void take(const GameObject* newController);

	float getDamage();

	virtual void realignPartialTick(
		float timeOfImpact, float kineticReduction);
	//used to resolve post-collision position
	//and secondary collisions.
	//very important for ship pass-through trajectories

	void updateGame_GeneralLogic() override;

	void updateEngine_Collision(
		std::shared_ptr<GameObject> self) override;
};
