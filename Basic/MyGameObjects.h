#pragma once

#include "SDL_Handling/SDL_Handling.h"
#include "Basic/GameUtilities.h"

#include "Basic/Interfaces.h"

class Projectile;

class I_FullWorldObject:
	public virtual I_CoreUpdate,
	public virtual I_Draw,
	public virtual I_Collidable,
	public virtual I_WorldKinetic,
	public virtual I_Child
{
public:
	virtual void hit(Projectile& hit) = 0;
};

class Projectile://assumed kinetic for now
	public virtual I_CoreUpdate,
	public virtual I_Collidable,
	public virtual I_Child,
	public virtual Pure_WorldPhysics,
	public virtual Pure_Draw
{
protected:
	friend I_FullWorldObject;

	std::weak_ptr<I_Composite> parent;
public:
	Projectile(
		std::weak_ptr<I_Composite> inParent,
		ImageTexture const& inTexture,
		PhysicsObject const& initState);

	virtual void collide(I_FullWorldObject* hit);

	void updateMovement() override;
	void updateControl() override{}
	void updateLogic() override;

	std::shared_ptr<I_Composite>
		getParent() const override;

	void draw() override;
};

class ParticleField;

class PlaySpace:
	public I_FullGameLoop
{
protected:
	std::vector<std::shared_ptr<I_FullWorldObject>>
		childEntities;
	std::vector<std::shared_ptr<Projectile>>
		projectiles;

	std::vector<I_Child*> removalList;

	std::shared_ptr<I_WorldKinetic> centerPoint;
	Vector stationaryVelocity;//accrues velocity
	//since centerPoint will be modified to zero v

	std::shared_ptr<ParticleField> particleField;
public:
	PlaySpace();

	void setCenter(
		std::shared_ptr<I_WorldKinetic> newCenter);

	Vector const& getStationaryVelocity() const;

	void updateMovement() final;
	void updateControl() final;
	void updateLogic() final;

	void updateCollisions() final;
	void updateRemoval() final;
	void remove(I_Child* removeItem) override;
	void addEntity(std::shared_ptr<I_FullWorldObject> entity);
	void addProjectile(
		std::shared_ptr<Projectile> projectile);

	std::vector<std::shared_ptr<
		I_FullWorldObject>> const&
			getFieldStatus() const;

	void draw() final;
};

#define cam Camera::getInstance()

class Camera
{
	Vector pos;
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
	bool setPosition(const I_CoreUpdate* requester, const Vector& newPos);
	bool setZoom(const I_CoreUpdate* requester, float newZoom);
	void release(const I_CoreUpdate* controller);

	DrawParameters stateToParameters(const PhysicsObject& state) const;
	DrawParameters stateToParameters(const PhysicsObject& state, float z) const;
	Vector drawSpace() const;//get width and height drawn on screen (at player z level)
	Vector drawSpace(float z) const;
	Vector pixelLocation(int x, int y) const;//get the in world location of a point on screen
	Vector mouseLocation() const;//get in world position of mouse (at player z level)
	inline Vector getPos() const//get camera postion
	{
		return pos;
	}
};

class Player;

#define game Game::getInstance()

class Game:
	public I_FullGameLoop
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
	void initialize();
	void run();

	void updateMovement() final;
	void updateControl() final;
	void updateLogic() final;

	void updateCollisions() final;
	void updateRemoval() final;
	void remove(I_Child* removeItem) override;

	void draw() final;
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
