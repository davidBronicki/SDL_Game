// #include "Basic/Game.h"
// #include "Basic/Player.h"
// #include "ShipObjects/ShipObjects.h"
// #include "Basic/AdditionalObjects.h"

// using namespace std;

// /////---------------------Game---------------------\\\\\

// Game::Game()
// :
// 	player(make_shared<Player>()),
// 	CompositeEntity(ImageTexture("Textures/small_mand.bmp"),
// 	PhysicsObject(Kinematic(GamePosition(0, 0)), 1, 1, 1))
// {
// 	shared_ptr<Ship> playerShip(make_shared<Ship>(
// 		GamePosition(0,0)));
// 	playerShip->setChassis(make_shared<ShipChassis>(
// 		ImageTexture("Textures/small_mand.bmp"),
// 		100,//hp
// 		PhysicsObject(Kinematic(GamePosition(0, 0)), 1, 1, 1),
// 		vector<size_t>{
// 			1,//engine count
// 			1,//weapon count
// 			1//shield count
// 		}));
// 	playerShip->setWeapon(make_shared<Weapon>(
// 		ImageTexture("Textures/contour.png"),//weapon texture
// 		100,//hp
// 		PhysicsObject(Kinematic(GamePosition(0,0)), 1, 1, 0.2),//state
// 		10,//base damage
// 		make_shared<Ammunition>(
// 			100,//initial count
// 			ImageTexture("Textures/graph.png"),//item texture
// 			ImageTexture("Textures/graph.png"),//projectile texture
// 			1.0)), 0);//damage modifier and weapon slot
// 	player->newShip(playerShip);

// 	shared_ptr<Ship> aiShip(make_shared<Ship>(
// 		GamePosition(20,20)));
// 	aiShip->setChassis(make_shared<ShipChassis>(
// 		ImageTexture("Textures/small_mand.bmp"),
// 		100,//hp
// 		PhysicsObject(Kinematic(GamePosition(0, 0)), 1, 1, 1),
// 		vector<size_t>{
// 			1,//engine count
// 			1,//weapon count
// 			1//shield count
// 		}));
// 	aiShip->setWeapon(make_shared<Weapon>(
// 		ImageTexture("Textures/contour.png"),//weapon texture
// 		100,//hp
// 		PhysicsObject(Kinematic(GamePosition(0,0)), 1, 1, 0.2),//state
// 		100,//base damage
// 		make_shared<Ammunition>(
// 			100,//initial count
// 			ImageTexture("Textures/graph.png"),//item texture
// 			ImageTexture("Textures/graph.png"),//projectile texture
// 			1.0)), 0);//damage modifier and weapon slot
// 	children.push_back(aiShip);

// 	field1 = make_shared<ParticleField>(
// 		50,//count
// 		1.3f,//max speed
// 		1,//max rotation
// 		1e5,//min z
// 		5e5,//max z
// 		0.2,//min size
// 		0.5);//max size

// 	Keyboard::subscribeToKeyPressed(
// 		function<void(Key)>([](Key key) -> void
// 		{
// 			if (key == Key::Escape || key == Key::Q)
// 			{
// 				GameControlEnvironment::quit();
// 			}
// 		}));

// 	cam.take(this);
// 	cam.setZoom(this, 0.02);
// 	cam.release(this);
// 	cam.take(player.get());
// }

// void Game::run()
// {
// 	while (!GameControlEnvironment::userHasQuit())
// 	{
// 		GameControlEnvironment::sortQueue();

// 		updateGame_ControlLogic();
// 		updateEngine_Move();
// 		updateEngine_Collision();
// 		// updateEngine_Collision(
// 		// 	shared_ptr<GameObject>(NULL));
// 		updateGame_GeneralLogic();
// 		updateGame_RemovalLogic();
// 		draw();
// 	}
// }

// void Game::updateGame_ControlLogic()
// {
// 	player->updateGame_ControlLogic();
// 	for (auto&& item : children)
// 	{
// 		item->updateGame_ControlLogic();
// 	}
// }

// void Game::updateGame_GeneralLogic()
// {
// 	player->updateGame_GeneralLogic();
// 	for (auto&& item : children)
// 	{
// 		item->updateGame_GeneralLogic();
// 	}
// }

// void Game::updateGame_RemovalLogic()
// {
// 	size_t n = 0;
// 	for (auto&& updateItem : children)
// 	{
// 		bool isRemoved = false;
// 		for (auto&& removalItem : removalList)
// 		{
// 			if (removalItem == updateItem.get())
// 			{
// 				isRemoved = true;
// 				break;
// 			}
// 		}
// 		if (!isRemoved)
// 		{
// 			children[n] = updateItem;
// 			++n;
// 		}
// 	}
// 	children.resize(n);
// 	removalList = vector<const GameObject*>();
// }

// void Game::updateEngine_Move()
// {
// 	field1->updateEngine_Move();
// 	player->updateEngine_Move();
// 	for (auto&& item : children)
// 	{
// 		item->updateEngine_Move();
// 	}
// }

// // void Game::updateEngine_Collision()
// // {
// // 	field1->updateEngine_Collision(field1);
// // 	player->updateEngine_Collision(player);
// // 	for (auto&& item : hittableUpdateList)
// // 	{
// // 		item->updateEngine_Collision(item);
// // 	}
// // }

// void Game::draw() const
// {
// 	GameDrawEnvironment::clearRender();

// 	field1->draw();
// 	for (auto&& item : children)
// 	{
// 		item->draw();
// 	}
// 	player->draw();

// 	GameDrawEnvironment::render();
// }

// void Game::addHitUpdate(std::shared_ptr<Entity> item)
// {
// 	children.push_back(item);
// }

// void Game::removeFromUpdates(const GameObject* item)
// {
// 	removalList.push_back(item);
// }

// float Game::projectileHitDetection(
// 	Entity& projectile, Entity*& hitReturnEntity)
// {
// 	float timeOfImpact = 2;
// 	for (auto&& item : children)
// 	{
// 		float hitTime = getCollisionTime(item.get(), projectile);
// 		if (hitTime >= 0 && hitTime < timeOfImpact)
// 		{//we only want the earliest hit
// 			timeOfImpact = hitTime;
// 			hitReturnEntity = item.get();
// 		}
// 	}
// 	return timeOfImpact;//will be 2 for no hit
// }
