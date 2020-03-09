#pragma once

#include <memory>
#include <vector>

#include "SDL_Handling.h"

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
	// bool remove = false;//mark this for removal from the game (unload)
public:
	virtual void updateGame_ControlLogic() = 0;
	virtual void updateGame_GeneralLogic() = 0;
	virtual void updateEngine_Move() = 0;

	// virtual bool updateGame_Removal() = 0;

	virtual void draw() const = 0;
};

class Entity : public GameObject
{
protected:
	ImageTexture worldTexture;
	PhysicsObject state;
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

	// void updateGame_ControlLogic() override;
	// void updateGame_GeneralLogic() override;
	void updateEngine_Move() override;

	// virtual bool updateGame_Removal() = 0;

	void draw() const override;
};

class Camera
{
	GamePosition pos;
	float zoom;
	std::vector<const GameObject*> controlStack;
	// const GameObject* controller;
	Camera();
	// Camera(const Camera& camera);
	// Camera& operator=(const Camera& camera);
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
	// Particle(GamePosition inPos, Vector inVel, float inZ, float inSize);
	ParticleField(size_t count, float maxSpeed, float maxRotation,
		float minZ, float maxZ, float minSize, float maxSize);
	void updateGame_ControlLogic() override;
	void updateGame_GeneralLogic() override;

	void updateEngine_Move() override;

	void draw() const override;
};

class Game : public GameObject
{
	// ImageTexture background;
	std::shared_ptr<Player> player;
	std::shared_ptr<ParticleField> field1;
	std::vector<std::shared_ptr<GameObject>> updateList;
	Game();

	void updateGame_ControlLogic();
	void updateGame_GeneralLogic();
	// bool updateGame_Removal();
	void updateEngine_Move();
	void draw() const;
public:
	inline static Game& getInstance()
	{
		static Game g;
		g.draw();
		return g;
	}

	void run();
	void addToUpdateList(std::shared_ptr<GameObject> item);
};
