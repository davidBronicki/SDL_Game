#pragma once

#include "SDL_Handling/SDL_Handling.h"
#include "Basic/GameUtilities.h"
// #include "Basic/BasicGameObjects.h"
#include "Basic/AdditionalObjects.h"
#include "Basic/Player.h"
#include "Basic/Game.h"

#include "Interfaces.h"

class WorldObject : public I_CoreUpdate, I_Draw
{

};



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
