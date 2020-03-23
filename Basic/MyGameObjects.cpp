#include "Basic/MyGameObjects.h"

#include "Basic/Player.h"

#include "Basic/AdditionalObjects.h"

using namespace std;

/////---------------I_ParentSpace-------------\\\\\

void I_ParentSpace::updateMovement()
{
	for (auto&& i:childSpaces)
	{
		i->updateMovement();
	}
}
void I_ParentSpace::updateControl()
{
	for (auto&& i:childSpaces)
	{
		i->updateControl();
	}
}
void I_ParentSpace::updateLogic()
{
	for (auto&& i:childSpaces)
	{
		i->updateLogic();
	}
}

void I_ParentSpace::updateCollisions()
{
	for (auto&& i:childSpaces)
	{
		i->updateCollisions();
	}
}

void I_ParentSpace::updateRemoval()
{
	for (auto&& removalItem : removalList)
	{
		for (auto it = childSpaces.begin();
			it < childSpaces.end(); ++it)
		// for (int i = 0; i < childSpaces.size(); ++i)
		{
			if (it->get() == removalItem)
			{
				childSpaces.erase(it);
				break;
			}
		}
	}
	removalList.clear();
	for (auto&& item : childSpaces)
	{
		item->updateRemoval();
	}
}

void I_ParentSpace::removeSpace(I_ChildSpace* space)
{
	removalList.push_back(space);
}

void I_ParentSpace::addSpace(
	shared_ptr<I_ChildSpace> space)
{
	childSpaces.push_back(space);
}

void I_ParentSpace::draw()
{
	for (auto&& i:childSpaces)
	{
		i->draw();
	}
}

/////---------------Projectile-------------\\\\\

Projectile::Projectile(
	weak_ptr<I_Composite> inParent,
	ImageTexture const& inTexture,
	PhysicsObject const& initState)
:
	I_Child(inParent),
	Pure_WorldPhysics(initState),
	Pure_Draw(inTexture),
	I_Collidable(HitBox{
		initState.radius, initState.state.pos})
{}

void Projectile::collide(I_Hittable* hit)
{
	hit->hit(*this);
}

void Projectile::updateMovement()
{
	Pure_WorldPhysics::update(
		GameUpdateEnvironment::getDT());
}

void Projectile::draw()
{
	worldTexture.draw(cam.stateToParameters(state));
}

/////------------------PlaySpace------------------\\\\\

PlaySpace::PlaySpace(shared_ptr<I_ParentSpace> inParent)
:
	I_ChildSpace(inParent)
{
	particleField = make_shared<ParticleField>(
		50,//count
		1.3f,//max speed
		1,//max rotation
		1e5,//min z
		5e5,//max z
		0.2,//min size
		0.5);//max size
}

void PlaySpace::setCenter(
	std::shared_ptr<I_WorldKinetic> newCenter)
{
	centerPoint = newCenter;
	stationaryVelocity = centerPoint->vel();
	particleField->setCenter(newCenter);
}

Vector const& PlaySpace::getStationaryVelocity() const
{
	return stationaryVelocity;
}

void PlaySpace::updateMovement()
{
	stationaryVelocity += centerPoint->vel();
	particleField->updateMovement();
	for (auto&& item : childEntities)
	{
		item->setPos(Vector(
			item->pos().x - centerPoint->pos().x,
			item->pos().y - centerPoint->pos().y));
		item->setVel(
			item->vel() - centerPoint->vel());
		item->updateMovement();
	}
	for (auto&& item : projectiles)
	{
		item->setPos(Vector(
			item->pos().x - centerPoint->pos().x,
			item->pos().y - centerPoint->pos().y));
		item->setVel(
			item->vel() - centerPoint->vel());
		item->updateMovement();
	}
}

void PlaySpace::updateControl()
{
	for (auto&& item : childEntities)
	{
		item->updateControl();
	}
	for (auto&& item : projectiles)
	{
		item->updateControl();
	}
}

void PlaySpace::updateLogic()
{
	for (auto&& item : childEntities)
	{
		item->updateLogic();
	}
	for (auto&& item : projectiles)
	{
		item->updateLogic();
	}
}

void PlaySpace::updateCollisions()
{
	for (auto&& projectile : projectiles)
	{
		for (auto&& item : childEntities)
		{
			float collisionTime = 
				getCollisionTime(*projectile,
					*item);
			if (collisionTime <= 1
				&& collisionTime > 0)
			{
				projectile->collide(item.get());
			}
		}
	}
}

void PlaySpace::updateRemoval()
{
	for (auto&& removalItem : removalList)
	{
		bool found = false;
		for (auto it = childEntities.begin();
			it < childEntities.end(); ++it)
		{
			if (it->get() == removalItem)
			{
				childEntities.erase(it);
				found = true;
				break;
			}
		}
		if (found)
			continue;
		for (auto it = projectiles.begin();
			it < projectiles.end(); ++it)
		{
			if (it->get() == removalItem)
			{
				projectiles.erase(it);
				break;
			}
		}
	}
	removalList.clear();
}

void PlaySpace::remove(I_Child* entity)
{
	removalList.push_back(entity);
}

void PlaySpace::addEntity(shared_ptr<I_Hittable> entity)
{
	childEntities.push_back(entity);
}

void PlaySpace::addProjectile(
	std::shared_ptr<Projectile> projectile)
{
	projectiles.push_back(projectile);
}

void PlaySpace::draw()
{
	particleField->draw();
	for (auto&& item : childEntities)
	{
		item->draw();
	}
	for (auto&& item : projectiles)
	{
		item->draw();
	}
}

// #include "Basic/Game.h"
// #include "Basic/Player.h"
// #include "ShipObjects/ShipObjects.h"
// #include "Basic/AdditionalObjects.h"

// using namespace std;

/////--------------------Camera-------------------\\\\\

Camera::Camera()
:
	pos(0, 0),
	zoom(0.01)
{}

void Camera::take(const I_CoreUpdate* newController)
{
	controlStack.push_back(newController);
}

bool Camera::setPosition(const I_CoreUpdate* requester, const Vector& newPos)
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

bool Camera::setZoom(const I_CoreUpdate* requester, float newZoom)
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

void Camera::release(const I_CoreUpdate* controller)
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

Vector Camera::pixelLocation(int x, int y) const
{
	float worldX = pos.x + (x - GameDrawEnvironment::getWidth() / 2) / (zoom * GameDrawEnvironment::getHeight());
	float worldY = pos.y - (y - GameDrawEnvironment::getHeight() / 2) / (zoom * GameDrawEnvironment::getHeight());
	return Vector(worldX, worldY);
}


Vector Camera::mouseLocation() const
{
	return pixelLocation(Mouse::x(), Mouse::y());
}


/////---------------------Game---------------------\\\\\

void Game::updateControl()
{
	I_ParentSpace::updateControl();
	player->updateControl();
}

void Game::updateLogic()
{
	I_ParentSpace::updateLogic();
	player->updateLogic();
}

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

// void Game::updateEngine_Collision()
// {
// 	field1->updateEngine_Collision(field1);
// 	player->updateEngine_Collision(player);
// 	for (auto&& item : hittableUpdateList)
// 	{
// 		item->updateEngine_Collision(item);
// 	}
// }

void Game::draw()
{
	GameDrawEnvironment::clearRender();

	I_ParentSpace::draw();

	// field1->draw();
	// for (auto&& item : children)
	// {
	// 	item->draw();
	// }
	player->draw();

	GameDrawEnvironment::render();
}

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
