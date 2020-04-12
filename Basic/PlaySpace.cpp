#include "Basic/MyGameObjects.h"
#include "Basic/AdditionalObjects.h"

using namespace std;

PlaySpace::PlaySpace()
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
	Vector centerPos(centerPoint->pos());
	Vector centerVel(centerPoint->vel());

	stationaryVelocity += centerVel;
	particleField->updateMovement();

	for (auto&& item : childEntities)
	{
		item->setPos(
			item->pos() - centerPos);
		item->setVel(
			item->vel() - centerVel);
		item->updateMovement();
	}
	for (auto&& item : projectiles)
	{
		item->setPos(
			item->pos() - centerPos);
		item->setVel(
			item->vel() - centerVel);
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
				getCollisionTime(
					*projectile,
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
		for (auto it = childEntities.begin();
			it < childEntities.end(); ++it)
		{
			if (it->get() == removalItem)
			{
				childEntities.erase(it);
				break;
			}
		}
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

void PlaySpace::remove(I_Child* removalItem)
{
	removalList.push_back(removalItem);
}

void PlaySpace::addEntity(shared_ptr<I_FullWorldObject> entity)
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
