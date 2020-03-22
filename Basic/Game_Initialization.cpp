#include "Basic/MyGameObjects.h"
#include "Basic/Player.h"
#include "ShipObjects/ShipObjects.h"
#include "Items/ItemObjects.h"

using namespace std;

Game::Game()
:
	player(make_shared<Player>()),
	playSpace(make_shared<PlaySpace>())
{
	childSpaces.push_back(playSpace);

	shared_ptr<Ship> playerShip(make_shared<Ship>(
		self,
		GamePosition(0,0)));
	playerShip->setChassis(make_shared<ShipChassis>(
		playerShip,
		ImageTexture("Textures/small_mand.bmp"),
		100,//hp
		PhysicsObject(
			Kinematic(GamePosition(0, 0)), 1, 1, 1),
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
			Kinematic(GamePosition(0,0)), 1, 1, 0.2),//state
		10,//base damage
		make_shared<Ammunition>(
			100,//initial count
			ImageTexture("Textures/graph.png"),//item texture
			ImageTexture("Textures/graph.png"),//projectile texture
			1.0)), 0);//damage modifier and weapon slot
	player->newShip(playerShip);
	playSpace->addEntity(playerShip);
	playSpace->setCenter(playerShip);

	// shared_ptr<Ship> aiShip(make_shared<Ship>(
	// 	GamePosition(20,20)));
	// aiShip->setChassis(make_shared<ShipChassis>(
	// 	ImageTexture("Textures/small_mand.bmp"),
	// 	100,//hp
	// 	PhysicsObject(Kinematic(GamePosition(0, 0)), 1, 1, 1),
	// 	vector<size_t>{
	// 		1,//engine count
	// 		1,//weapon count
	// 		1//shield count
	// 	}));
	// aiShip->setWeapon(make_shared<Weapon>(
	// 	ImageTexture("Textures/contour.png"),//weapon texture
	// 	100,//hp
	// 	PhysicsObject(Kinematic(GamePosition(0,0)), 1, 1, 0.2),//state
	// 	100,//base damage
	// 	make_shared<Ammunition>(
	// 		100,//initial count
	// 		ImageTexture("Textures/graph.png"),//item texture
	// 		ImageTexture("Textures/graph.png"),//projectile texture
	// 		1.0)), 0);//damage modifier and weapon slot
	// children.push_back(aiShip);

	// field1 = make_shared<ParticleField>(
	// 	50,//count
	// 	1.3f,//max speed
	// 	1,//max rotation
	// 	1e5,//min z
	// 	5e5,//max z
	// 	0.2,//min size
	// 	0.5);//max size

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
}

void Game::makeInstance()
{
	static std::shared_ptr<Game> gp(&game);
	gp->self = gp;
}

void Game::run()
{
	while (!GameControlEnvironment::userHasQuit())
	{
		GameControlEnvironment::sortQueue();
		updateControl();
		updateMovement();
		updateCollisions();
		// updateEngine_Collision(
		// 	shared_ptr<GameObject>(NULL));
		updateLogic();
		// updateGame_RemovalLogic();
		draw();
	}
}