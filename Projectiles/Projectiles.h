#pragma once

#include "Basic/MyGameObjects.h"

class Projectile : public Entity
{
protected:
	float damage;
	size_t lifetime;
public:
	Projectile(const ImageTexture& inWorldTexture,
		const PhysicsObject& inState,
		float inDamage,
		size_t inLifetime);

	float getDamage();

	// void updateGame_ControlLogic() override;
	void updateGame_GeneralLogic() override;
	// void updateEngine_Move() override;

	void updateEngine_Collision() override;

	// virtual bool updateGame_Removal() = 0;

	// void draw() const override;
};
