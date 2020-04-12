#pragma once

#include <random>

float inline randValue()
{
	return (float)std::rand() / RAND_MAX;
}

class GameUpdateEnvironment
{
	static float dt;
	GameUpdateEnvironment();
public:
	inline static float getDT()
	{
		return dt;
	}
};

struct Vector
{
	float x;
	float y;

	Vector();
	Vector(float inX, float inY);
	float magnitude() const;
	float magnitudeSqr() const;

	Vector operator-() const;
	Vector& operator+=(const Vector& other);
	Vector& operator-=(const Vector& other);
	Vector& operator*=(float other);
	Vector& operator/=(float other);
};

Vector operator+(Vector left, const Vector& right);
Vector operator-(Vector left, const Vector& right);
Vector operator*(Vector left, float right);
Vector operator*(float left, Vector right);
Vector operator/(Vector left, float right);

struct Kinematic
{
	Vector pos, vel, acc;
	float angle, angularVel, angularAcc;

	Kinematic();
	Kinematic(const Vector& inPos);
	Kinematic(const Vector& inPos,
		const Vector& inVel);
	Kinematic(const Vector& inPos,
		float inAngle);
	Kinematic(const Vector& inPos,
		const Vector& inVel,
		float inAngle, float inAngularVel);

	inline void update()
	{
		update(GameUpdateEnvironment::getDT());
	}
	void update(float dt);

	Kinematic& operator+=(const Kinematic& other);
	Kinematic& operator-=(const Kinematic& other);
};

Kinematic operator+(Kinematic left, const Kinematic& right);
Kinematic operator-(Kinematic left, const Kinematic& right);

struct LifePointCounter
{
	float health;
	int maxHealth;
};

struct HitBox
{
	float radius;
	Vector center;
};

struct PhysicsObject
{
	Kinematic state;
	float mass, momentInertia, radius;
	Vector force;
	float torque;

	PhysicsObject(const Kinematic& inState,
		float inMass, float inMomentInertia,
		float inRadius);

	inline void update()
	{
		update(GameUpdateEnvironment::getDT());
	}
	void update(float dt);

	inline const Vector& getPos() const
	{
		return state.pos;
	}
	PhysicsObject& operator+=(const PhysicsObject& other);
	PhysicsObject& operator-=(const PhysicsObject& other);
};

PhysicsObject operator+(
	PhysicsObject left, const PhysicsObject& right);
PhysicsObject operator-(
	PhysicsObject left, const PhysicsObject& right);

float getCollisionTime(
	const HitBox& Astart, const HitBox& Aend,
	const HitBox& Bstart, const HitBox& Bend);

float getExitTime(
	const HitBox& Astart, const HitBox& Aend,
	const HitBox& Bstart, const HitBox& Bend);
