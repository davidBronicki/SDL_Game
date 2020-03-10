#pragma once

#include <memory>
#include <vector>

#include "SDL_Handling/SDL_Handling.h"

#include "GameUtilities.h"

#define cam Camera::getInstance()
#define game Game::getInstance()

/*
GameObject
	Item
		Ammunition
	Entity
		Projectile
		Ship
		ShipComponent
			ShipChassis
			Hull
			Engine
			Weapon
			Shield
	Game
	*Player
Camera
*/

class GameObject
{
protected:

public:
	virtual void updateGame_ControlLogic() = 0;
	virtual void updateGame_GeneralLogic() = 0;
	virtual void updateEngine_Move() = 0;

	virtual void draw() const = 0;
};

class Game;

class Entity : public GameObject
{
protected:
	friend Game;

	ImageTexture worldTexture;
	PhysicsObject state;

	std::vector<std::shared_ptr<Entity>> children;//child states are set relative to this
	Vector posToParent;//if it has a parent, these are used for relative position setting
	float angleToParent;//if there is no parent, these play no role

	Entity(const ImageTexture& inWorldTexture,
		const PhysicsObject& inState);
public:
	inline const PhysicsObject& getPhysics() const
	{
		return state;
	}
	inline const Kinematic& getKinetics() const
	{
		return state.state;
	}

	void updateEngine_Move() override;

	void draw() const override;
};

class Camera
{
	GamePosition pos;
	float zoom;
	std::vector<const GameObject*> controlStack;
	Camera();
public:
	inline static Camera& getInstance()
	{
		static Camera camera;
		return camera;
	}
	void take(const GameObject* newController);
	bool setPosition(const GameObject* requester, const GamePosition& newPos);
	bool setZoom(const GameObject* requester, float newZoom);
	void release(const GameObject* controller);

	DrawParameters stateToParameters(const PhysicsObject& state) const;
	DrawParameters stateToParameters(const PhysicsObject& state, float z) const;
	Vector drawSpace() const;//get width and height drawn on screen (at player z level)
	Vector drawSpace(float z) const;
	GamePosition pixelLocation(int x, int y) const;//get the in world location of a point on screen
	GamePosition mouseLocation() const;//get in world position of mouse (at player z level)
	inline GamePosition getPos() const//get camera postion
	{
		return pos;
	}
};

class Ship;

class Player : public GameObject
{
	std::shared_ptr<Ship> ship;
public:
	Player();

	void newShip(std::shared_ptr<Ship> newShip);

	void updateGame_ControlLogic() override;
	void updateGame_GeneralLogic() override;

	void updateEngine_Move() override;

	void draw() const override;
};

class ParticleField : public GameObject
{
	std::vector<PhysicsObject> states;
	std::vector<float> zVals;
public:
	ParticleField(size_t count, float maxSpeed, float maxRotation,
		float minZ, float maxZ, float minSize, float maxSize);
	void updateGame_ControlLogic() override;
	void updateGame_GeneralLogic() override;

	void updateEngine_Move() override;

	void draw() const override;
};

class Game : public GameObject
{
	std::shared_ptr<Player> player;
	std::shared_ptr<ParticleField> field1;

	// std::vector<std::shared_ptr<GameObject>> backgroundUpdateList;
	// std::vector<std::shared_ptr<GameObject>> nonHittableUpdateList;
	std::vector<std::shared_ptr<Entity>> hittableUpdateList;
	// std::vector<std::shared_ptr<Projectile>> projectileUpdateList;
	// std::vector<std::shared_ptr<GameObject>> foregroundUpdateList;

	// std::vector<std::shared_ptr<GameObject>> updateList;
	std::vector<const GameObject*> removalList;

	Game();

	void updateGame_ControlLogic();
	void updateGame_GeneralLogic();
	void updateGame_RemovalLogic();
	void updateEngine_Move();
	void draw() const;
public:
	inline static Game& getInstance()
	{
		static Game g;
		return g;
	}

	void run();
	void addHitUpdate(std::shared_ptr<Entity> item);
	// void addProjectileUpdate(std::shared_ptr<Projectile> item);
	void removeFromUpdates(const GameObject* item);

	void projectileHitDetection(PhysicsObject& projectileState);
};
