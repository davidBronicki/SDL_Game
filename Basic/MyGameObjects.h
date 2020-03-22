#pragma once

#include "SDL_Handling/SDL_Handling.h"
#include "Basic/GameUtilities.h"
// // #include "Basic/BasicGameObjects.h"
// #include "Basic/AdditionalObjects.h"
// #include "Basic/Player.h"
// #include "Basic/Game.h"

#include "Basic/Interfaces.h"

// class Entity:
// 	public I_CoreUpdate,
// 	public Pure_Draw
// {};

class I_Hittable;

class Projectile://assumed kinetic for now
	public I_Child,
	public Pure_WorldPhysics,
	public Pure_Draw
{
	friend I_Hittable;
public:
	Projectile(
		std::weak_ptr<I_Composite> inParent,
		ImageTexture const& inTexture,
		PhysicsObject const& initState);

	virtual void collide(I_Hittable* hit);

	void updateMovement() override;
	void updateControl() override{}
	void updateLogic() override{}

	void draw() override;
};

class I_Hittable:
	public I_Child
{
public:
	I_Hittable(std::weak_ptr<I_Composite> parent)
		:I_Child(parent){}
	virtual void hit(Projectile& hit) = 0;
};

class PlaySpace:
	public I_ChildSpace
{
protected:
	std::vector<std::shared_ptr<I_Hittable>> childEntities;
	std::vector<std::shared_ptr<Projectile>> projectiles;
	std::shared_ptr<I_WorldKinetic> centerPoint;
public:
	void setCenter(
		std::shared_ptr<I_WorldKinetic> newCenter);

	void addEntity(std::shared_ptr<I_Hittable> entity);
	void addProjectile(
		std::shared_ptr<Projectile> projectile);

	void updateMovement() override;
	void updateControl() override;
	void updateLogic() override;

	void updateCollisions() override;

	void draw() override;
};

#define cam Camera::getInstance()

class Camera
{
	GamePosition pos;
	float zoom;
	std::vector<const I_CoreUpdate*> controlStack;
	Camera();
public:
	inline static Camera& getInstance()
	{
		static Camera camera;
		return camera;
	}
	void take(const I_CoreUpdate* newController);
	bool setPosition(const I_CoreUpdate* requester, const GamePosition& newPos);
	bool setZoom(const I_CoreUpdate* requester, float newZoom);
	void release(const I_CoreUpdate* controller);

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

class Player;

#define game Game::getInstance()

class Game:
	public I_ParentSpace
{
	std::weak_ptr<Game> self;

	std::shared_ptr<Player> player;
	std::shared_ptr<PlaySpace> playSpace;

	Game();
public:
	inline static Game& getInstance()
	{
		static Game* g = new Game();
		return *g;
	}
	static void makeInstance();
	void run();

	void updateControl() override;
	void updateLogic() override;

	void draw() override;
};


/*
play system
	game
	solar system
	planet
	play system

ship controller?
	player
	ai
*/

/*
game:
	handles player
	is the galaxy?
	contains solar systems (composite)

solar system:
	part of a galaxy (child)
	has planet systems etc (composite)
	may contain play system

planet:
	maybe components?
	no engines
	not effected by weaponry (no physics)

play system:
	part of some larger area (child)
	contains player ship and other ships (composite)

player:
	controls a ship
	does not draw

ai:
	controls a ship
	does not draw

ship:
	has several components
	can take control of projectiles to handle
		collisions with components
	can be controlled

space station:
	can have components
	does not have engines
	can be effected by weaponry (needs physics)
*/

/*
GameObject
	*Player
	ParticleField
	Item
		Ammunition
	Entity
		Projectile
		ShipComponent
			ShipChassis
			Hull
			Engine
			Weapon
			Shield
		CompositeEntity
			Ship
			Game
Camera
*/
