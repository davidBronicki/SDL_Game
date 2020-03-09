#include "GameUtilities.h"

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
	torque(0)
{}

void PhysicsObject::update()
{
	state.acc = force / mass;
	state.angularAcc = torque / momentInertia;
	state.update();
	force = Vector(0, 0);
	torque = 0;
}

// const GamePosition& PhysicsObject::getPos() const
// {
// 	return state.pos;
// }

/////---------------LifePointCounter-------------\\\\\
