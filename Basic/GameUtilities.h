#pragma once

#include <random>

float inline randValue()
{
	return (float)std::rand() / RAND_MAX;
}

// template<typename T, typename S>
// T* getP(shared_ptr<S> input)
// {
// 	return (T*)(input.get());
// }

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

struct GamePosition;

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

struct GamePosition
{
	float x;
	float y;

	GamePosition();
	GamePosition(float inX, float inY);
	GamePosition(const Vector& input);
	float distance(const GamePosition& otherPoint) const;
	float distanceSqr(const GamePosition& otherPoint) const;

	GamePosition& operator+=(const Vector& displacement);
	GamePosition& operator-=(const Vector& displacement);
	friend Vector operator-(const GamePosition& left, const GamePosition& right);
};

GamePosition operator+(GamePosition left, const Vector& right);
GamePosition operator-(GamePosition left, const Vector& right);
GamePosition operator+(const Vector& left, GamePosition right);
Vector operator-(const GamePosition& left, const GamePosition& right);

struct Kinematic
{
	GamePosition pos;
	Vector vel, acc;
	float angle, angularVel, angularAcc;

	Kinematic();
	Kinematic(const GamePosition& inPos);
	Kinematic(const GamePosition& inPos,
		const Vector& inVel);
	Kinematic(const GamePosition& inPos,
		float inAngle);
	Kinematic(const GamePosition& inPos,
		const Vector& inVel,
		float inAngle, float inAngularVel);

	inline void update()
	{
		update(GameUpdateEnvironment::getDT());
	}
	void update(float dt);
};

struct LifePointCounter
{
	float health;
	int maxHealth;
};

struct HitBox
{
	float radius;
	GamePosition center;
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

	inline const GamePosition& getPos() const
	{
		return state.pos;
	}
};

float getCollisionTime(
	const HitBox& Astart, const HitBox& Aend,
	const HitBox& Bstart, const HitBox& Bend);

float getExitTime(
	const HitBox& Astart, const HitBox& Aend,
	const HitBox& Bstart, const HitBox& Bend);
