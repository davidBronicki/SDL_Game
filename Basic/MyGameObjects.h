#pragma once

// #include "SDL_Handling/SDL_Handling.h"
// #include "Basic/GameUtilities.h"
// // #include "Basic/BasicGameObjects.h"
// #include "Basic/AdditionalObjects.h"
// #include "Basic/Player.h"
// #include "Basic/Game.h"

#include "Interfaces.h"

// class Entity:
// 	public I_CoreUpdate,
// 	public Pure_Draw
// {};

class Projectile:
	public I_Child,
	public Pure_PhysicsObject
{
}

class PlaySpace:
	public I_ChildSpace
{
	std::vector<std::shared_ptr<I_Child>> childEntities;
	std::shared_ptr<I_WorldPosition> centerPoint;
public:
	void updateMovement() override;
	void updateControl() override;
	void updateLogic() override;

	void updateCollisions() override;

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
