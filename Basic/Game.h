#pragma once

#include "BasicGameObjects.h"

#define game Game::getInstance()

class Player;
class ParticleField;

class Game : public CompositeEntity
{
	std::shared_ptr<Player> player;
	std::shared_ptr<ParticleField> field1;

	// std::vector<std::shared_ptr<GameObject>> backgroundUpdateList;
	// std::vector<std::shared_ptr<GameObject>> nonHittableUpdateList;
	// std::vector<std::shared_ptr<Entity>> hittableUpdateList;
	// std::vector<std::shared_ptr<Projectile>> projectileUpdateList;
	// std::vector<std::shared_ptr<GameObject>> foregroundUpdateList;

	// std::vector<std::shared_ptr<GameObject>> updateList;
	std::vector<const GameObject*> removalList;

	Game();

	void updateGame_ControlLogic() override;
	void updateGame_GeneralLogic() override;
	void updateGame_RemovalLogic();
	void updateEngine_Move() override;
	void updateEngine_Collision() override;
	void draw() const override;
public:
	inline static Game& getInstance()
	{
		static Game g;
		return g;
	}

	void run();
	void addHitUpdate(std::shared_ptr<Entity> item);
	void removeFromUpdates(const GameObject* item);

	float projectileHitDetection(
		Entity& projectile, Entity*& hitReturnEntity);
};
