#include "Basic/GameUtilities.h"

#include "math.h"

using namespace std;

/////-----------------Vector------------------\\\\\

Vector::Vector()
	:x(0), y(0){}

Vector::Vector(float inX, float inY)
	:x(inX), y(inY){}

// Vector::Vector(const GamePosition& input)
// 	:x(input.x), y(input.y){}

float Vector::magnitude() const
{
	return sqrt(magnitudeSqr());
}

float Vector::magnitudeSqr() const
{
	return x * x + y * y;
}

Vector Vector::operator-() const
{
	return Vector(-x, -y);
}

Vector& Vector::operator+=(const Vector& other)
{
	x += other.x;
	y += other.y;
	return *this;
}

Vector& Vector::operator-=(const Vector& other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}

Vector& Vector::operator*=(float other)
{
	x *= other;
	y *= other;
	return *this;
}

Vector& Vector::operator/=(float other)
{
	x /= other;
	y /= other;
	return *this;
}

Vector operator+(Vector left, const Vector& right)
{
	return left += right;
}

Vector operator-(Vector left, const Vector& right)
{
	return left -= right;
}

Vector operator*(Vector left, float right)
{
	return left *= right;
}

Vector operator*(float left, Vector right)
{
	return right *= left;
}

Vector operator/(Vector left, float right)
{
	return left /= right;
}

/////-------------GamePosition------------------\\\\\

GamePosition::GamePosition(float inX, float inY)
	:x(inX), y(inY){}

GamePosition::GamePosition(const Vector& input)
	:x(input.x), y(input.y){}

float GamePosition::distance(const GamePosition& otherPoint) const
{
	return sqrt(distanceSqr(otherPoint));
}

float GamePosition::distanceSqr(const GamePosition& otherPoint) const
{
	float dx = x - otherPoint.x;
	float dy = y - otherPoint.y;
	return dx * dx + dy * dy;
}

GamePosition& GamePosition::operator+=(const Vector& displacement)
{
	x += displacement.x;
	y += displacement.y;
	return *this;
}

GamePosition& GamePosition::operator-=(const Vector& displacement)
{
	x -= displacement.x;
	y -= displacement.y;
	return *this;
}

GamePosition operator+(GamePosition left, const Vector& right)
{
	return left += right;
}

GamePosition operator-(GamePosition left, const Vector& right)
{
	return left -= right;
}

GamePosition operator+(const Vector& left, GamePosition right)
{
	return right += left;
}

Vector operator-(const GamePosition& left, const GamePosition& right)
{
	return Vector(left.x - right.x, left.y - right.y);
}

/////-----------------Kinematic--------------------\\\\\

Kinematic::Kinematic(const GamePosition& inPos)
:
	pos(inPos),
	vel(),
	acc(),
	angle(0),
	angularVel(0),
	angularAcc(0)
{}

Kinematic::Kinematic(const GamePosition& inPos,
		const Vector& inVel)
:
	pos(inPos),
	vel(inVel),
	acc(),
	angle(0),
	angularVel(0),
	angularAcc(0)
{}
	
Kinematic::Kinematic(const GamePosition& inPos,
		float inAngle)
:
	pos(inPos),
	vel(),
	acc(),
	angle(inAngle),
	angularVel(0),
	angularAcc(0)
{}

Kinematic::Kinematic(const GamePosition& inPos,
		const Vector& inVel,
		float inAngle, float inAngularVel)
:
	pos(inPos),
	vel(inVel),
	acc(),
	angle(inAngle),
	angularVel(inAngularVel),
	angularAcc(0)
{}

void Kinematic::update()
{
	acc *= GameUpdateEnvironment::getDT();
	vel += acc;
	pos += GameUpdateEnvironment::getDT() * vel;
	acc = Vector();

	angularVel += angularAcc * GameUpdateEnvironment::getDT();
	angle += GameUpdateEnvironment::getDT() * angularVel;
	angularAcc = 0;
}

/////---------------PhysicsObject-------------------------\\\\\

PhysicsObject::PhysicsObject(const Kinematic& inState,
	float inMass, float inMomentInertia, float inRadius)
:
	state(inState),
	mass(inMass),
	momentInertia(inMomentInertia),
	radius(inRadius),
	force(),
	torque(0),
	before{inRadius, inState.pos, NULL},
	now{inRadius, inState.pos, NULL}
{}

void PhysicsObject::update()
{
	before.center = state.pos;
	state.acc = force / mass;
	state.angularAcc = torque / momentInertia;
	state.update();
	force = Vector(0, 0);
	torque = 0;
	now.center = state.pos;
}

/////---------------LifePointCounter-------------\\\\\

/////-----------------HitBox---------------------\\\\\

#include <iostream>

bool checkHit(
	const HitBox& Astart, HitBox& Aend,
	const HitBox& Bstart, HitBox& Bend)
{
	Vector separationStart = Bstart.center - Astart.center;
	Vector separationEnd = Bend.center - Aend.center;
	Vector change = separationEnd - separationStart;//This is the rate of change of the separation
	float timeOfCloseApproach =
		(-separationStart.x * change.x - separationStart.y * change.y)
			/ (change.x * change.x + change.y * change.y);

	float separationRadius = Astart.radius + Bstart.radius;//total distance for a hit

	return false;

	if (timeOfCloseApproach > 1)
	{//close approach happens after end. Need to check end condition to see what happens
		float distSquared = separationEnd.x * separationEnd.x + separationEnd.y * separationEnd.y;
		if (distSquared <= separationRadius * separationRadius)
		{//hit!
			Aend.hit = &Bend;
			Bend.hit = &Aend;
			return true;
		}
		else return false;
	}
	else if (timeOfCloseApproach > 0)
	{//close approach is between time steps. Need to interpolate to see how close they are.
		//these will generall be "very near hits" or "very near miss".
		//this is only necessary because the game involves very high velocity projectiles and ships.
		separationStart += change * timeOfCloseApproach;//changed to the separation at close approach
		float distSquared = separationStart.x * separationStart.x + separationStart.y * separationStart.y;
		if (distSquared <= separationRadius * separationRadius)
		{//hit!
			Aend.hit = &Bend;
			Bend.hit = &Aend;
			return true;
		}
		else return false;
	}
	else return false;//close approach is before start; would have been detected in previous time step.
}
