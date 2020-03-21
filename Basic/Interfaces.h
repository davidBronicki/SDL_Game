#pragma once

#include <memory>
#include <vector>

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
	void updateMovement() virtual{}
	void updateControl() virtual{}
	void updateLogic() virtual{}
};

class I_Draw
{
public:
	void draw() virtual{};
};

class I_WorldPosition
{
public:
	GamePosition const& pos() const virtual = 0;
	void setPos(GamePosition const& newPos) virtual = 0;
};
class I_WorldKinetic : public I_WorldPosition
{
public:
	Vector const& vel() const virtual = 0;
	float const& angularVel() const virtual = 0;

	Kinetic const& getKinetics() const virtual = 0;
	void setKinetics(Kinetic const& newState) virtual = 0;

	void update(float dt) virtual = 0;
};
class I_WorldPhysics : public I_WorldKinetic
{
public:
	void applyForce(Vector const& force) virtual = 0;
	void applyTorque(float torque) virtual = 0;

	PhysicsObject getState() const virtual = 0;
	void setState(PhysicsObject const& newState) virtual = 0;
};

class I_Collidable
{
	friend float getCollisionTime(
		const I_Collidable&, const I_Collidable&);
	friend float getExitTime(
		const I_Collidable&, const I_Collidable&);

	HitBox currentHitBox;
	HitBox previousHitBox;
};

class I_Composite
{
protected:
	std::vector<std::shared_ptr<I_Collidable>> children;
public:
	void updateCollisions() virtual = 0;
};

class I_Controllable
{
public:
	void toggleControl(Control key) virtual = 0;
	void simpleControl(Control key, float controlValue) virtual = 0;
	void advancedControl(Control key,
		std::vector<float> const& controlValues) virtual = 0;
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

class Pure_WorldPosition : public I_WorldPosition
{
	GamePosition pos;
public:
	Pure_WorldPosition(){}
	Pure_WorldPosition(GamePosition const& init)
	: pos(init){}

	GamePosition const& pos() const override
	{
		return pos;
	}
	void setPos(GamePosition const& newPos) override
	{
		pos = newPos;
	}
};

class Pure_WorldKinetic : public I_WorldKinetic
{
	Kinetic state;
public:
	Pure_WorldKinetic(){}
	Pure_WorldKinetic(GamePosition const& initPos)
	: state(initPos){}
	Pure_WorldKinetic(Kinetic const& initState)
	: state(initState){}

	Vector const& vel() const override
	{
		return state.vel;
	}
	float const& angularVel() const override
	{
		return state.angularVel;
	}

	GamePosition const& pos() const override
	{
		return state.pos;
	}
	void setPos(GamePosition const& newPos) override
	{
		state.pos = newPos;
	}

	Kinetic const& getKinetics() const override
	{
		return state;
	}
	void setKinetics(Kinetic const& newState) override
	{
		state = newState;
	}

	void update(float dt) override
	{
		state.update(dt);
	}
};

class Pure_WorldPhysics : public I_WorldPhysics
{
	PhysicsObject state;
public:
	Pure_WorldPhysics(
		float inRadius, float inMass, float inMoment)
	: state(Kinetic(), radius(inRadius), mass(inMass),
		momentInertia(inMoment)){}
	Pure_WorldPhysics(GamePosition const& initPos,
		float inRadius, float inMass, float inMoment)
	: state(Kinetic(initPos), radius(inRadius),
		mass(inMass), momentInertia(inMoment)){}
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

	GamePosition const& pos() const override
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
	Kinetic const& getKinetics() const override
	{
		return state.state;
	}
	PhysicsObject const& getState() const override
	{
		return state;
	}

	void setPos(GamePosition const& newPos) override
	{
		state.state.pos = newPos;
	}
	void setKinetics(Kinetic const& newState) override
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
