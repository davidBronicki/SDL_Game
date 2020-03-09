#pragma once

#include "MyGameObjects.h"

class Item : public GameObject
{
protected:
	unsigned int count;
	const ImageTexture& itemTexture;
public:
	Item(unsigned int initialCount,
		const ImageTexture& inTexture);

	void updateGame_ControlLogic() override;
	void updateGame_GeneralLogic() override;
	void updateEngine_Move() override;

	void draw() const override;
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

	// void updateGame_ControlLogic() override;
	// void updateGame_GeneralLogic() override;
	// void updateEngine_Move() override;

	// void draw() const override;

	void fire(const GamePosition& weaponPosition,
		float angleOfFire, int baseDamage);
};