#pragma once

#include "Basic/MyGameObjects.h"

class Item:
	public I_CoreUpdate,
	public Pure_Draw
{
protected:
	unsigned int count;
public:
	Item(unsigned int initialCount,
		const ImageTexture& inTexture);

	// void updateGame_ControlLogic() override;
	// void updateGame_GeneralLogic() override;
	// void updateEngine_Move() override;

	void draw() override;
};

class Ammunition : public Item
{
protected:
	ImageTexture projectileTexture;
	float damageModifier;
public:
	Ammunition(unsigned int initialCount,
		const ImageTexture& inItemTexture,
		const ImageTexture& inProjectileTexture,
		float inDamageModifier);

	std::shared_ptr<Projectile> use(
		std::shared_ptr<I_Composite> parent,
		const Kinematic& weaponState, int baseDamage);
};