#include "MyGameObjects.h"
#include "ShipObjects.h"

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

// void Entity::updateGame_ControlLogic()
// {

// }

// void Entity::updateGame_GeneralLogic()
// {

// }

void Entity::updateEngine_Move()
{
	state.update();
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
			relativePosition.getX() * zoom * GameDrawEnvironment::getHeight() + GameDrawEnvironment::getWidth() / 2 + 0.5f - size,//x
			-(relativePosition.getY()) * zoom * GameDrawEnvironment::getHeight() + GameDrawEnvironment::getHeight() / 2 + 0.5f - size,//y
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
		relativePosition.getX() * zoom * GameDrawEnvironment::getHeight() + GameDrawEnvironment::getWidth() / 2 + 0.5f - size,//x
		-(relativePosition.getY()) * zoom * GameDrawEnvironment::getHeight() + GameDrawEnvironment::getHeight() / 2 + 0.5f - size,//y
		2 * size,//w
		2 * size//h
	));
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
	float worldX = pos.getX() + (x - GameDrawEnvironment::getWidth() / 2) / (zoom * GameDrawEnvironment::getHeight());
	float worldY = pos.getY() - (y - GameDrawEnvironment::getHeight() / 2) / (zoom * GameDrawEnvironment::getHeight());
	return GamePosition(worldX, worldY);
}


GamePosition Camera::mouseLocation() const
{
	return pixelLocation(Mouse::x(), Mouse::y());
}

/////--------------------Player-------------------\\\\\

Player::Player()
{
	Keyboard::subscribeToKeyPressed(
		function<void(Key)>([this](Key key) -> void
		{
			if (key == Key::X)
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
}

void Player::newShip(shared_ptr<Ship> newShip)
{
	ship = newShip;
}

void Player::updateGame_ControlLogic()
{
	// if (Keyboard::isDown(Key::W))
	// {
	// 	ship->accelForward();
	// }
	// else if (Keyboard::isDown(Key::S))
	// {
	// 	ship->accelBackward();
	// }
	// if (Keyboard::isDown(Key::D))
	// {
	// 	ship->rotateRight();
	// }
	// else if (Keyboard::isDown(Key::A))
	// {
	// 	ship->rotateLeft();
	// }
	GamePosition shipPos = ship->getKinetics().pos;
	auto cursorPos = Camera::getInstance().mouseLocation();
	auto relativePos = cursorPos - shipPos;
	float relativeAngleToCurser =
		atan2(relativePos.getY(), relativePos.getX()) - ship->getPhysics().state.angle;
	if (relativeAngleToCurser < M_PI) relativeAngleToCurser += 2 * M_PI;
	if (relativeAngleToCurser > M_PI) relativeAngleToCurser -= 2 * M_PI;
	ship->rotate(relativeAngleToCurser);

	if (Mouse::isDown(Button::Left))
	{
		ship->accelForward();
	}

	if (Keyboard::isDown(Key::Space))
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

void Player::draw() const
{
	ship->draw();
}

/////--------------------ParticleField-------------------\\\\\

ParticleField::ParticleField(size_t count, float maxSpeed, float maxRotation,
	float minZ, float maxZ, float minSize, float maxSize)
:
	// states(count),
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
			randValue() * drawSpace.getX() + minPoint.getX(),
			randValue() * drawSpace.getY() + minPoint.getY());
		states.push_back(PhysicsObject(
			Kinematic(pos, velocity, 0,  rotation), 1.0f, 1.0f, size));
	}
}

void ParticleField::updateGame_ControlLogic(){}
void ParticleField::updateGame_GeneralLogic(){}

void ParticleField::updateEngine_Move()
{
	for (int i = 0; i < states.size(); ++i)
	{
		states[i].update();

		Vector drawSpace(cam.drawSpace(zVals[i]));
		GamePosition minPoint(cam.getPos() - drawSpace / 2);
		Vector relativePosition(states[i].getPos() - minPoint);

		if (relativePosition.getX() < 0.1 * drawSpace.getX())
		{
			states[i].state.pos += Vector(1.2 * drawSpace.getX(), 0);
		}
		else if (relativePosition.getX() > 1.2 * drawSpace.getX())
		{
			states[i].state.pos += Vector(-1.2 * drawSpace.getX(), 0);
		}

		if (relativePosition.getY() < 0.1 * drawSpace.getY())
		{
			states[i].state.pos += Vector(0, 1.2 * drawSpace.getY());
		}
		else if (relativePosition.getY() > 1.2 * drawSpace.getY())
		{
			states[i].state.pos += Vector(0, -1.2 * drawSpace.getY());
		}
	}
}

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
			1,//hull count
			1,//engine count
			1,//weapon count
			1//shield count
		}));
	player->newShip(playerShip);

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

		try
		{
		updateGame_ControlLogic();
		}
		catch (exception e)
		{
			cout << "Exception in Control Logic" << endl;
			cout << e.what() << endl;
		}

		try{
		updateEngine_Move();
		}
		catch (exception e)
		{
			cout << "Exception in Move Logic" << endl;
			cout << e.what() << endl;
		}

		try{
		updateGame_GeneralLogic();
		}
		catch (exception e)
		{
			cout << "Exception in General Logic" << endl;
			cout << e.what() << endl;
		}

		try{
		draw();
		}
		catch (exception e)
		{
			cout << "Exception in Draw" << endl;
			cout << e.what() << endl;
		}
	}
}

void Game::updateGame_ControlLogic()
{
	player->updateGame_ControlLogic();
	for (auto&& item : updateList)
	{
		item->updateGame_ControlLogic();
	}
}

void Game::updateGame_GeneralLogic()
{
	player->updateGame_GeneralLogic();
	for (auto&& item : updateList)
	{
		item->updateGame_GeneralLogic();
	}
}

void Game::updateEngine_Move()
{
	field1->updateEngine_Move();
	player->updateEngine_Move();
	for (auto&& item : updateList)
	{
		item->updateEngine_Move();
	}
}

void Game::draw() const
{
	GameDrawEnvironment::clearRender();

	field1->draw();
	for (auto&& item : updateList)
	{
		item->draw();
	}
	player->draw();

	GameDrawEnvironment::render();
}

void Game::addToUpdateList(std::shared_ptr<GameObject> item)
{
	updateList.push_back(item);
}
