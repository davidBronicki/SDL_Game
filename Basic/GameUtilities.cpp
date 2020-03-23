#include "Basic/GameUtilities.h"

#include "math.h"

using namespace std;

/////-------------GameUpdateEnvironment---------------\\\\\

float GameUpdateEnvironment::dt = 1.0/60;

/////-----------------Vector------------------\\\\\

Vector::Vector()
	:x(0), y(0){}

Vector::Vector(float inX, float inY)
	:x(inX), y(inY){}

// Vector::Vector(const Vector& input)
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

/////-----------------Kinematic--------------------\\\\\

Kinematic::Kinematic()
:
	pos(),
	vel(),
	acc(),
	angle(0),
	angularVel(0),
	angularAcc(0)
{}

Kinematic::Kinematic(const Vector& inPos)
:
	pos(inPos),
	vel(),
	acc(),
	angle(0),
	angularVel(0),
	angularAcc(0)
{}

Kinematic::Kinematic(const Vector& inPos,
		const Vector& inVel)
:
	pos(inPos),
	vel(inVel),
	acc(),
	angle(0),
	angularVel(0),
	angularAcc(0)
{}
	
Kinematic::Kinematic(const Vector& inPos,
		float inAngle)
:
	pos(inPos),
	vel(),
	acc(),
	angle(inAngle),
	angularVel(0),
	angularAcc(0)
{}

Kinematic::Kinematic(const Vector& inPos,
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

void Kinematic::update(float dt)
{
	acc *= dt;
	vel += acc;
	pos += dt * vel;
	acc = Vector();

	angularVel += angularAcc * dt;
	angle += dt * angularVel;
	angularAcc = 0;
}

Kinematic& Kinematic::operator+=(
	const Kinematic& other)
{
	pos += other.pos;
	vel += other.vel;
	acc += other.acc;
	angle += other.angle;
	angularVel += other.angularVel;
	angularAcc += other.angularAcc;
	return *this;
}

Kinematic& Kinematic::operator-=(
	const Kinematic& other)
{
	pos -= other.pos;
	vel -= other.vel;
	acc -= other.acc;
	angle -= other.angle;
	angularVel -= other.angularVel;
	angularAcc -= other.angularAcc;
	return *this;
}

Kinematic operator+(Kinematic left,
	const Kinematic& right)
{
	return left += right;
}

Kinematic operator-(Kinematic left,
	const Kinematic& right)
{
	return left -= right;
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
	torque(0)
{}

void PhysicsObject::update(float dt)
{
	state.acc = force / mass;
	state.angularAcc = torque / momentInertia;
	state.update(dt);
	force = Vector(0, 0);
	torque = 0;
}

/////---------------LifePointCounter-------------\\\\\

/////-----------------HitBox---------------------\\\\\

#include <iostream>

float getCollisionTime(
	const HitBox& Astart, HitBox const& Aend,
	const HitBox& Bstart, HitBox const& Bend)
{//returns relative time to impact (from start) for a hit, and -1 for a miss.

	Vector separationStart = Bstart.center - Astart.center;
	Vector change = Bend.center - Aend.center;//temperary variable; should be called "separationEnd"
	change -= separationStart;//"change" now represents the rate of change

	//the following are used for the collision condition and time of collision calulations
	float velocityMagnitudeSquared = change.magnitudeSqr();
	float posDotVelocity = separationStart.x * change.x
		+ separationStart.y * change.y;
	float totalRadius = Astart.radius + Bstart.radius;
	float distanceDifference = separationStart.magnitudeSqr()
		- totalRadius * totalRadius;

	float discriminant = posDotVelocity * posDotVelocity
		- velocityMagnitudeSquared * distanceDifference;

	if (discriminant < 0)//from quadratic formula, no collision in this case
	{
		return -1;
	}
	else
		return (-posDotVelocity + sqrt(discriminant))
			/ velocityMagnitudeSquared;
	// {
	// 	float timeOfCollision = (-posDotVelocity - sqrt(discriminant)) / velocityMagnitudeSquared;
	// 	if (timeOfCollision > 1 || timeOfCollision < 0) return -1;//collision didn't happen in this time step
	// 	else return timeOfCollision;
	// }
}


float getExitTime(
//get the moment two objects are no longer touching
	const HitBox& Astart, const HitBox& Aend,
	const HitBox& Bstart, const HitBox& Bend)
{

	Vector separationStart = Bstart.center - Astart.center;
	Vector change = Bend.center - Aend.center;//temperary variable; should be called "separationEnd"
	change -= separationStart;//"change" now represents the rate of change

	//the following are used for the collision condition and time of collision calulations
	float velocityMagnitudeSquared = change.magnitudeSqr();
	float posDotVelocity = separationStart.x * change.x
		+ separationStart.y * change.y;
	float totalRadius = Astart.radius + Bstart.radius;
	float distanceDifference = separationStart.magnitudeSqr()
		- totalRadius * totalRadius;

	float discriminant = posDotVelocity * posDotVelocity
		- velocityMagnitudeSquared * distanceDifference;
	if (discriminant < 0)//shouldn't happen, but just in case. (should throw an error lol)
		return -1;
	else
		return (-posDotVelocity + sqrt(discriminant))
			/ velocityMagnitudeSquared;
}
