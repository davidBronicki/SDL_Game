#include "Basic/MyGameObjects.h"
#include "ShipObjects/ShipObjects.h"
#include "Items/ItemObjects.h"

#include <iostream>
#include <stdexcept>
#include "math.h"
#include <random>

using namespace std;

float randValue()
{
	return (float)rand() / RAND_MAX;
}

template<typename T, typename S>
T* getP(shared_ptr<S> input)
{
	return (T*)(input.get());
}

/////-------------GameUpdateEnvironment---------------\\\\\

float GameUpdateEnvironment::dt = 1.0/60;

/////-------------------GameObject-------------------\\\\\

/////--------------------Entity-------------------\\\\\

Entity::Entity(const ImageTexture& inWorldTexture,
	const PhysicsObject& inState)
:
	worldTexture(inWorldTexture),
	state(inState)
{
}

void Entity::collide(Projectile& p)
{
	// game.removeFromUpdates((GameObject*)(&p));
}

void Entity::updateEngine_Move()
{
	state.update();
	float cAngle = cos(state.state.angle);
	float sAngle = sin(state.state.angle);
	for (auto&& child : children)
	{
		Vector relativePosition(
			child->posToParent.x * cAngle - child->posToParent.y * sAngle,
			child->posToParent.x * sAngle + child->posToParent.y * cAngle);
		child->state.state.pos = state.state.pos + relativePosition;
		child->state.state.vel = state.state.vel + Vector(relativePosition.y, - relativePosition.x);
		if (!child->noAngleFlag)
		{
			child->state.state.angularVel = state.state.angularVel;
			child->state.state.angle = state.state.angle + angleToParent;
			if (child->state.state.angle > 2 * M_PI)
				child->state.state.angle -= 2 * M_PI;//ensure angle is between 0 and 2pi
		}
	}
}

void Entity::draw() const
{
	worldTexture.draw(cam.stateToParameters(state));
}

/////---------------------Camera---------------------\\\\\

Camera::Camera()
:
	pos(0, 0),
	zoom(0.01)
{}

void Camera::take(const GameObject* newController)
{
	controlStack.push_back(newController);
}

bool Camera::setPosition(const GameObject* requester, const GamePosition& newPos)
{
	if (controlStack.size() == 0) return false;
	if (requester == controlStack.back())
	{
		pos = newPos;
		return true;
	}
	else
	{
		return false;
	}
}

bool Camera::setZoom(const GameObject* requester, float newZoom)
{
	if (controlStack.size() == 0) return false;
	if (requester == controlStack.back())
	{
		zoom = newZoom;
		return true;
	}
	else
	{
		return false;
	}
}

void Camera::release(const GameObject* controller)
{
	for (int i = controlStack.size() - 1; i >= 0; --i)
	{
		if (controller == controlStack[i])
		{
			controlStack.erase(controlStack.begin() + i);
		}
	}
}

DrawParameters Camera::stateToParameters(const PhysicsObject& state) const
{
	//make relative position instead
	Vector relativePosition(state.state.pos - pos);
	int size = state.radius * zoom * GameDrawEnvironment::getHeight();
	return DrawParameters::Rectangle(ScreenRectangle(
			relativePosition.x * zoom * GameDrawEnvironment::getHeight() + GameDrawEnvironment::getWidth() / 2 + 0.5f - size,//x
			-(relativePosition.y) * zoom * GameDrawEnvironment::getHeight() + GameDrawEnvironment::getHeight() / 2 + 0.5f - size,//y
			2 * size,//w
			2 * size//h
		),
		state.state.angle);
}

DrawParameters Camera::stateToParameters(const PhysicsObject& state, float z) const
{
	float camDist = 3440.0f / zoom;//dist from cam to player level
	float distToCam = camDist + z;//dist from cam to object
	float ratio = camDist / distToCam;

	Vector relativePosition(state.state.pos - pos);
	relativePosition *= ratio;
	int size = state.radius * zoom * GameDrawEnvironment::getHeight() * ratio;

	return DrawParameters::Rectangle(ScreenRectangle(
		relativePosition.x * zoom * GameDrawEnvironment::getHeight() + GameDrawEnvironment::getWidth() / 2 + 0.5f - size,//x
		-(relativePosition.y) * zoom * GameDrawEnvironment::getHeight() + GameDrawEnvironment::getHeight() / 2 + 0.5f - size,//y
		2 * size,//w
		2 * size//h
	),
		state.state.angle);
}

Vector Camera::drawSpace() const
{
	return Vector((float)(GameDrawEnvironment::getWidth()) / GameDrawEnvironment::getHeight() / zoom,
		1.0f / zoom);
}

Vector Camera::drawSpace(float z) const
{
	float camDist = 3440.0f / zoom;
	float distToCam = camDist + z;
	return drawSpace() * (distToCam / camDist);
}

GamePosition Camera::pixelLocation(int x, int y) const
{
	float worldX = pos.x + (x - GameDrawEnvironment::getWidth() / 2) / (zoom * GameDrawEnvironment::getHeight());
	float worldY = pos.y - (y - GameDrawEnvironment::getHeight() / 2) / (zoom * GameDrawEnvironment::getHeight());
	return GamePosition(worldX, worldY);
}


GamePosition Camera::mouseLocation() const
{
	return pixelLocation(Mouse::x(), Mouse::y());
}

/////--------------------Player-------------------\\\\\

Player::Player()
{
	Mouse::subscribeToButtonPressed(
		function<void(Button)>([this](Button button) -> void
		{
			if (button == Button::Left)
			{
				try
				{
					ship->fire(0);
				}
				catch (const exception& e)
				{
					cout << "exception when firing" << endl;
					cout << e.what() << endl;
				}
			}
		}));
	// Keyboard::subscribeToKeyPressed(
	// 	function<void(Key)>([this](Key key) -> void
	// 	{
	// 		if (key == Key::Left_Alt)
	// 		{
	// 			if (controls == controlScheme::PointToRotate)
	// 			{
	// 				controls = controlScheme::ButtonRotate;
	// 			}
	// 			else
	// 			{
	// 				controls = controlScheme::PointToRotate;
	// 			}
	// 		}
	// 	}));
}

void Player::newShip(shared_ptr<Ship> newShip)
{
	ship = newShip;
}

void Player::updateGame_ControlLogic()
{
	switch(controls)
	{
		case controlScheme::PointToRotate:
		{
			ship->pointAtAngle(ship->getKinetics().angle);

			GamePosition shipPos = ship->getKinetics().pos;
			auto cursorPos = Camera::getInstance().mouseLocation();
			auto relativePos = cursorPos - shipPos;
			ship->pointAtAngle(atan2(relativePos.y, relativePos.x));
		}
		break;
		case controlScheme::ButtonRotate:
		{
			if (Keyboard::isDown(Key::D))
			{
				ship->rotateRight();
			}
			else if (Keyboard::isDown(Key::A))
			{
				ship->rotateLeft();
			}
		}
		break;
	}

	if (Keyboard::isDown(Key::W) || Mouse::isDown(Button::Right))
	{
		ship->accelForward();
	}

	if (Keyboard::isDown(Key::S))
	{
		ship->inertialBrake();
	}
	ship->updateGame_ControlLogic();
}

void Player::updateGame_GeneralLogic()
{
	cam.setPosition(this, ship->getKinetics().pos);

	ship->updateGame_GeneralLogic();
}

void Player::updateEngine_Move()
{
	ship->updateEngine_Move();
}

void Player::updateEngine_Collision()
{
	ship->updateEngine_Collision();
}

void Player::draw() const
{
	ship->draw();
}

/////--------------------ParticleField-------------------\\\\\

ParticleField::ParticleField(size_t count, float maxSpeed, float maxRotation,
	float minZ, float maxZ, float minSize, float maxSize)
:
	zVals(count)
{
	for (int i = 0; i < count; ++i)
	{
		zVals[i] = randValue() * (maxZ - minZ) + minZ;
		float size = randValue() * (maxSize - minSize) + minSize;
		float theta = randValue() * 2 * M_PI;
		float temp = randValue();
		float speed = temp * temp * maxSpeed;
		Vector velocity(speed * cos(theta), speed * sin(theta));
		float rotation = randValue() * 2 * maxRotation - maxRotation;

		Vector drawSpace(cam.drawSpace(zVals[i]));
		GamePosition minPoint(cam.getPos() - drawSpace / 2);
		GamePosition pos(
			randValue() * drawSpace.x + minPoint.x,
			randValue() * drawSpace.y + minPoint.y);
		states.push_back(PhysicsObject(
			Kinematic(pos, velocity, 0,  rotation), 1.0f, 1.0f, size));
	}
}

// void ParticleField::updateGame_ControlLogic(){}
// void ParticleField::updateGame_GeneralLogic(){}

void ParticleField::updateEngine_Move()
{
	for (int i = 0; i < states.size(); ++i)
	{
		states[i].update();

		Vector drawSpace(cam.drawSpace(zVals[i]));
		GamePosition minPoint(cam.getPos() - drawSpace / 2);
		Vector relativePosition(states[i].getPos() - minPoint);

		if (relativePosition.x < 0.1 * drawSpace.x)
		{
			states[i].state.pos += Vector(1.2 * drawSpace.x, 0);
		}
		else if (relativePosition.x > 1.2 * drawSpace.x)
		{
			states[i].state.pos += Vector(-1.2 * drawSpace.x, 0);
		}

		if (relativePosition.y < 0.1 * drawSpace.y)
		{
			states[i].state.pos += Vector(0, 1.2 * drawSpace.y);
		}
		else if (relativePosition.y > 1.2 * drawSpace.y)
		{
			states[i].state.pos += Vector(0, -1.2 * drawSpace.y);
		}
	}
}

// void ParticleField::updateEngine_Collision(){}

void ParticleField::draw() const
{
	for (int i = 0; i < states.size(); ++i)
	{
		GameDrawEnvironment::renderRectFilled(
			cam.stateToParameters(states[i], zVals[i]));
	}
}

/////---------------------Game---------------------\\\\\

Game::Game()
:
	player(make_shared<Player>())
{
	shared_ptr<Ship> playerShip(make_shared<Ship>(
		GamePosition(0,0)));
	playerShip->setChassis(make_shared<ShipChassis>(
		ImageTexture("Textures/small_mand.bmp"),
		100,//hp
		PhysicsObject(Kinematic(GamePosition(0, 0)), 1, 1, 1),
		vector<size_t>{
			1,//engine count
			1,//weapon count
			1//shield count
		}));
	playerShip->setWeapon(make_shared<Weapon>(
		ImageTexture("Textures/contour.png"),//weapon texture
		100,//hp
		PhysicsObject(Kinematic(GamePosition(0,0)), 1, 1, 0.2),//state
		10,//base damage
		make_shared<Ammunition>(
			100,//initial count
			ImageTexture("Textures/graph.png"),//item texture
			ImageTexture("Textures/graph.png"),//projectile texture
			1.0)), 0);//damage modifier and weapon slot
	player->newShip(playerShip);

	shared_ptr<Ship> aiShip(make_shared<Ship>(
		GamePosition(20,20)));
	aiShip->setChassis(make_shared<ShipChassis>(
		ImageTexture("Textures/small_mand.bmp"),
		100,//hp
		PhysicsObject(Kinematic(GamePosition(0, 0)), 1, 1, 1),
		vector<size_t>{
			1,//engine count
			1,//weapon count
			1//shield count
		}));
	aiShip->setWeapon(make_shared<Weapon>(
		ImageTexture("Textures/contour.png"),//weapon texture
		100,//hp
		PhysicsObject(Kinematic(GamePosition(0,0)), 1, 1, 0.2),//state
		100,//base damage
		make_shared<Ammunition>(
			100,//initial count
			ImageTexture("Textures/graph.png"),//item texture
			ImageTexture("Textures/graph.png"),//projectile texture
			1.0)), 0);//damage modifier and weapon slot
	hittableUpdateList.push_back(aiShip);

	field1 = make_shared<ParticleField>(
		50,//count
		1.3f,//max speed
		1,//max rotation
		1e5,//min z
		5e5,//max z
		0.2,//min size
		0.5);//max size

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
	cam.take(player.get());
}

void Game::run()
{
	while (!GameControlEnvironment::userHasQuit())
	{
		GameControlEnvironment::sortQueue();

		updateGame_ControlLogic();
		updateEngine_Move();
		updateEngine_Collision();
		updateGame_GeneralLogic();
		updateGame_RemovalLogic();
		draw();
	}
}

void Game::updateGame_ControlLogic()
{
	player->updateGame_ControlLogic();
	for (auto&& item : hittableUpdateList)
	{
		item->updateGame_ControlLogic();
	}
}

void Game::updateGame_GeneralLogic()
{
	player->updateGame_GeneralLogic();
	for (auto&& item : hittableUpdateList)
	{
		item->updateGame_GeneralLogic();
	}
}

void Game::updateGame_RemovalLogic()
{
	size_t n = 0;
	for (auto&& updateItem : hittableUpdateList)
	{
		bool isRemoved = false;
		for (auto&& removalItem : removalList)
		{
			if (removalItem == updateItem.get())
			{
				isRemoved = true;
				break;
			}
		}
		if (!isRemoved)
		{
			hittableUpdateList[n] = updateItem;
			++n;
		}
	}
	hittableUpdateList.resize(n);
	removalList = vector<const GameObject*>();
}

void Game::updateEngine_Move()
{
	field1->updateEngine_Move();
	player->updateEngine_Move();
	for (auto&& item : hittableUpdateList)
	{
		item->updateEngine_Move();
	}
}

void Game::updateEngine_Collision()
{
	field1->updateEngine_Collision();
	player->updateEngine_Collision();
	for (auto&& item : hittableUpdateList)
	{
		item->updateEngine_Collision();
	}
}

void Game::draw() const
{
	GameDrawEnvironment::clearRender();

	field1->draw();
	for (auto&& item : hittableUpdateList)
	{
		item->draw();
	}
	player->draw();

	GameDrawEnvironment::render();
}

void Game::addHitUpdate(std::shared_ptr<Entity> item)
{
	hittableUpdateList.push_back(item);
}

void Game::removeFromUpdates(const GameObject* item)
{
	removalList.push_back(item);
}

Entity* Game::projectileHitDetection(PhysicsObject& projectileState)
{
	shared_ptr<Entity> hitObject;
	float timeOfImpact = 2;
	for (auto&& item : hittableUpdateList)
	{
		float hitTime = checkHit(item->state, projectileState);
		if (hitTime >= 0 && hitTime < timeOfImpact)
		{
			timeOfImpact = hitTime;
			hitObject = item;
		}
	}
	if (timeOfImpact == 2)
	{
		return NULL;
	}
	else
	{
		return hitObject.get();
	}
}
