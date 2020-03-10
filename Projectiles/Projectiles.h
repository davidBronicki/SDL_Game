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

	virtual void updateGame_ControlLogic() override;
	virtual void updateGame_GeneralLogic() override;
	// void updateEngine_Move() override;

	// virtual bool updateGame_Removal() = 0;

	// void draw() const override;
};
