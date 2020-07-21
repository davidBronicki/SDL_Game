#include "Basic/MyGameObjects.h"
#include "Basic/Player.h"
#include "ShipObjects/ShipObjects.h"
#include "Items/ItemObjects.h"
#include "AI/ai_base.h"
#include "Settings/controlMap.h"

#include <iostream>

using namespace std;

Game::Game()
:
	hud(make_shared<HUD>())
{
	player = make_shared<Player>(hud);
	cout << "Game Instantiated" << endl;
}

void Game::makeInstance()
{
	static std::shared_ptr<Game> gp(&game);
	gp->self = gp;
}

void Game::initialize()
{
	cout << "Game Initialization has Started" << endl;
	ControlMap::init();

	playSpace = make_shared<PlaySpace>();
	cout << "PlaySpace Instantiated" << endl;

	shared_ptr<Ship> playerShip(make_shared<Ship>(
		playSpace,
		Vector(0,0)));
	playerShip->setChassis(make_shared<ShipChassis>(
		playerShip,
		ImageTexture("Textures/small_mand.bmp"),
		100,//hp
		PhysicsObject(
			Kinematic(Vector(0, 0)), 1, 1, 1),
		vector<size_t>{
			1,//engine count
			1,//weapon count
			1//shield count
		}));
	playerShip->setWeapon(make_shared<Weapon>(
		playerShip,
		ImageTexture("Textures/contour.png"),//weapon texture
		100,//hp
		PhysicsObject(
			Kinematic(Vector(0,0)), 1, 1, 0.2),//state
		10,//base damage
		make_shared<Ammunition>(
			100,//initial count
			ImageTexture("Textures/graph.png"),//item texture
			ImageTexture("Textures/graph.png"),//projectile texture
			1.0)), 0);//damage modifier and weapon slot
	player->newShip(playerShip);
	playSpace->addEntity(playerShip);
	playSpace->setCenter(playerShip);
	cout << "Player Initialized" << endl;

	shared_ptr<Ship> aiShip(make_shared<Ship>(
		playSpace,
		Vector(20,20)));
	aiShip->setChassis(make_shared<ShipChassis>(
		aiShip,
		ImageTexture("Textures/small_mand.bmp"),
		100,//hp
		PhysicsObject(Kinematic(Vector(0, 0)), 1, 1, 1),
		vector<size_t>{
			1,//engine count
			1,//weapon count
			1//shield count
		}));
	aiShip->setWeapon(make_shared<Weapon>(
		aiShip,
		ImageTexture("Textures/contour.png"),//weapon texture
		100,//hp
		PhysicsObject(Kinematic(Vector(0,0)), 1, 1, 0.2),//state
		100,//base damage
		make_shared<Ammunition>(
			100,//initial count
			ImageTexture("Textures/graph.png"),//item texture
			ImageTexture("Textures/graph.png"),//projectile texture
			1.0)), 0);//damage modifier and weapon slot
	playSpace->addEntity(aiShip);
	aiShip->setShipController(
		make_shared<BasicAI>(aiShip));
	cout << "AI Initialized" << endl;

	cout << "Playspace Initialized" << endl;

	Keyboard::subscribeToKeyPressed(
		function<void(Key)>([](Key key) -> void
		{
			if (key == Key::Escape || key == Key::Q)
			{
				GameControlEnvironment::quit();
			}
		}));

	cam.take(this);
	cam.setZoom(this, 0.02);
	cam.release(this);
	cam.take((I_CoreUpdate*)(player.get()));
	cout << "Camera Initialized" << endl;

	cout << "Initialization Now Exiting" << endl;
}

void Game::run()
{
	cout << "Entering Game Loop" << endl;
	while (!GameControlEnvironment::userHasQuit())
	{
		GameControlEnvironment::sortQueue();
		updateControl();
		updateMovement();
		updateCollisions();
		updateRemoval();
		updateLogic();
		draw();
	}
}
