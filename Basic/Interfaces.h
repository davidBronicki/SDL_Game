#pragma once

#include <memory>
#include <vector>

#include "SDL_Handling/SDL_Handling.h"
#include "Basic/GameUtilities.h"

enum class Control
{
	Forward,
	Backward,
	Right,
	Left,
	R_Right,
	R_Left,
	Point_Weapons,
	Point_Ship
};

/////-------------------Interfaces--------------------\\\\\

class I_CoreUpdate
{
public:
	virtual void updateMovement(){}
	virtual void updateControl(){}
	virtual void updateLogic(){}
};

class I_Draw
{
public:
	virtual void draw(){};
};

class I_WorldPosition
{
public:
	virtual Vector const& pos() const = 0;
	virtual void setPos(Vector const& newPos) = 0;
};
class I_WorldKinetic:
	public virtual I_WorldPosition
{
public:
	virtual Vector const& vel() const = 0;
	virtual void setVel(Vector const& newVel) = 0;
	virtual float const& angularVel() const = 0;

	virtual Kinematic const& getKinetics() const = 0;
	virtual void setKinetics(Kinematic const& newState) = 0;

	virtual void update(float dt) = 0;
};
class I_WorldPhysics : public virtual I_WorldKinetic
{
public:
	virtual void applyForce(Vector const& force) = 0;
	virtual void applyTorque(float torque) = 0;

	virtual PhysicsObject const& getState() const = 0;
	virtual void setState(PhysicsObject const& newState) = 0;
};

class I_Collidable
{
protected:

	friend float getCollisionTime(
		const I_Collidable&, const I_Collidable&);
	friend float getExitTime(
		const I_Collidable&, const I_Collidable&);

	HitBox currentHitBox;
	HitBox previousHitBox;
public:
	I_Collidable(){}
	I_Collidable(HitBox current):
		currentHitBox(current),
		previousHitBox(current)
	{}
};

class I_FullObject :
	public virtual I_CoreUpdate,
	public virtual I_Draw,
	public virtual I_Collidable,
	public virtual I_WorldKinetic
{};

class I_Child;

class I_Composite
{
public:
	virtual void updateCollisions() = 0;
	virtual void updateRemoval() = 0;
};

class I_Child : public virtual I_FullObject
{
protected:
	std::weak_ptr<I_Composite> parent;
public:
	I_Child(std::weak_ptr<I_Composite> inParent):
		parent(inParent){}
};

class I_Controllable
{
public:
	virtual void toggleControl(Control key) = 0;
	virtual void simpleControl(Control key, float controlValue) = 0;
	virtual void advancedControl(Control key,
		std::vector<float> const& controlValues) = 0;
};

class I_WorldSpace:
	public virtual I_CoreUpdate,
	public virtual I_Composite,
	public virtual I_Draw
{};

class I_ChildSpace;

class I_ParentSpace:
	public virtual I_WorldSpace
{
protected:
	std::vector<std::shared_ptr<I_ChildSpace>> childSpaces;
	std::vector<I_ChildSpace*> removalList;
public:
	void updateMovement() override;
	void updateControl() override;
	void updateLogic() override;

	void updateCollisions() override;
	void updateRemoval() override;
	void addSpace(std::shared_ptr<I_ChildSpace> space);
	void removeSpace(I_ChildSpace* space);

	void draw() override;
};

class I_ChildSpace:
	public virtual I_WorldSpace
{
	std::weak_ptr<I_WorldSpace> parentSpace;
public:
	I_ChildSpace(
		std::weak_ptr<I_WorldSpace> inParentSpace):
		parentSpace(inParentSpace){}
};

/////------------external interface functions-----------\\\\\

inline float getCollisionTime(
	const I_Collidable& collisionItem1, const I_Collidable& collisionItem2)
{
	return getCollisionTime(
		collisionItem1.previousHitBox, collisionItem1.currentHitBox,
		collisionItem2.previousHitBox, collisionItem2.currentHitBox);
}

inline float getExitTime(
	const I_Collidable& collisionItem1, const I_Collidable& collisionItem2)
{
	return getExitTime(
		collisionItem1.previousHitBox, collisionItem1.currentHitBox,
		collisionItem2.previousHitBox, collisionItem2.currentHitBox);
}

/////---------------basic implementations---------------\\\\\

class Pure_Draw:
	public virtual I_Draw
{
protected:
	ImageTexture worldTexture;
public:
	Pure_Draw(ImageTexture const& inTexture)
	: worldTexture(inTexture){}
};

class Pure_WorldPosition:
	public virtual I_WorldPosition
{
protected:
	Vector posistion;
public:
	Pure_WorldPosition(){}
	Pure_WorldPosition(Vector const& init)
	: posistion(init){}

	Vector const& pos() const override
	{
		return posistion;
	}
	void setPos(Vector const& newPos) override
	{
		posistion = newPos;
	}
};

class Pure_WorldKinetic:
	public virtual I_WorldKinetic
{
protected:
	Kinematic state;
public:
	Pure_WorldKinetic(){}
	Pure_WorldKinetic(Vector const& initPos)
	: state(initPos){}
	Pure_WorldKinetic(Kinematic const& initState)
	: state(initState){}

	Vector const& vel() const override
	{
		return state.vel;
	}
	void setVel(Vector const& newVel) override
	{
		state.vel = newVel;
	}

	float const& angularVel() const override
	{
		return state.angularVel;
	}

	Vector const& pos() const override
	{
		return state.pos;
	}
	void setPos(Vector const& newPos) override
	{
		state.pos = newPos;
	}

	Kinematic const& getKinetics() const override
	{
		return state;
	}
	void setKinetics(Kinematic const& newState) override
	{
		state = newState;
	}

	void update(float dt) override
	{
		state.update(dt);
	}
};

class Pure_WorldPhysics:
	public virtual I_WorldPhysics
{
protected:
	PhysicsObject state;
public:
	Pure_WorldPhysics(
		float inRadius, float inMass, float inMoment)
	: state(Kinematic(),
		inRadius, inMass, inMoment){}
	Pure_WorldPhysics(Vector const& initPos,
		float inRadius, float inMass, float inMoment)
	: state(Kinematic(initPos),
		inRadius, inMass, inMoment){}
	Pure_WorldPhysics(PhysicsObject const& initState)
	: state(initState){}

	void applyForce(Vector const& force) override
	{
		state.force += force;
	}
	void applyTorque(float torque) override
	{
		state.torque += torque;
	}

	Vector const& pos() const override
	{
		return state.state.pos;
	}
	Vector const& vel() const override
	{
		return state.state.vel;
	}
	float const& angularVel() const override
	{
		return state.state.angularVel;
	}
	Kinematic const& getKinetics() const override
	{
		return state.state;
	}
	PhysicsObject const& getState() const override
	{
		return state;
	}

	void setPos(Vector const& newPos) override
	{
		state.state.pos = newPos;
	}
	void setVel(Vector const& newVel) override
	{
		state.state.vel = newVel;
	}
	void setKinetics(Kinematic const& newState) override
	{
		state.state = newState;
	}
	void setState(PhysicsObject const& newState) override
	{
		state = newState;
	}

	void update(float dt) override
	{
		state.update(dt);
	}
};
