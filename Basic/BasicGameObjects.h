#pragma once

#include <memory>
#include <vector>

#include "SDL_Handling/SDL_Handling.h"
#include "Basic/GameUtilities.h"

#define cam Camera::getInstance()

class GameObject
{
protected:

public:
	virtual void updateGame_ControlLogic(){}
	virtual void updateGame_GeneralLogic(){}

	virtual void updateEngine_Move(){}
	// virtual void updateEngine_Collision(
	// 	std::shared_ptr<GameObject> self){}

	virtual void draw() const{}
};

class Projectile;

class Entity : public GameObject
{
protected:
	// friend Game;//for collision detection
	friend float getCollisionTime(const Entity&, const Entity&);
	friend float getExitTime(const Entity&, const Entity&);

	ImageTexture worldTexture;
	PhysicsObject state;
	HitBox currentHitBox;
	HitBox previousHitBox;

	// std::vector<std::shared_ptr<Entity>> children;
	// //child states are set relative to this
	// Vector posToParent;
	// //if it has a parent, these are used for
	// //relative position setting
	// float angleToParent;
	// //if there is no parent, these play no role

	Entity(const ImageTexture& inWorldTexture,
		const PhysicsObject& inState);
public:
	// bool noAngleFlag = false;
	inline PhysicsObject& getPhysics()
	{
		return state;
	}
	inline Kinematic& getKinetics()
	{
		return state.state;
	}

	virtual void collide(std::shared_ptr<Projectile> p);

	void updateEngine_Move() override;

	void draw() const override;
};

class CompositeEntity : public Entity
{
protected:
	std::vector<std::shared_ptr<Entity>> children;
	std::vector<std::shared_ptr<CompositeEntity>> compositeChildren;
	std::vector<Vector> childrenLocations;
	std::vector<float> childrenAngles;

	std::vector<std::shared_ptr<Projectile>> insideShots;
public:
	CompositeEntity(const ImageTexture& inWorldTexture,
		const PhysicsObject& inState);

	void collide(std::shared_ptr<Projectile> p) override;

	void updateEngine_Move() override;

	virtual void updateEngine_Collision();

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

inline float getCollisionTime(
	const Entity& collisionItem1, const Entity& collisionItem2)
{
	return getCollisionTime(
		collisionItem1.previousHitBox, collisionItem1.currentHitBox,
		collisionItem2.previousHitBox, collisionItem2.currentHitBox);
}

inline float getExitTime(
	const Entity& collisionItem1, const Entity& collisionItem2)
{
	return getExitTime(
		collisionItem1.previousHitBox, collisionItem1.currentHitBox,
		collisionItem2.previousHitBox, collisionItem2.currentHitBox);
}
