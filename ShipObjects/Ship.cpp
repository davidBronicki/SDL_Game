#include "ShipObjects/ShipObjects.h"
#include "AI/ai_base.h"

using namespace std;

Vector Ship::trueVelocity() const
{
	return state.state.vel +
		dynamic_pointer_cast<PlaySpace>(playSpace.lock())
		->getStationaryVelocity();
}

Ship::Ship(
	weak_ptr<PlaySpace> inPlaySpace,
	const Vector& inPos)
:
	playSpace(inPlaySpace),
	Pure_WorldPhysics(inPos, 1, 1, 1),
	inertialDampenerEngaged(true)
{}

void Ship::setChassis(shared_ptr<ShipChassis> newChassis)
{
	chassis = newChassis;
	components.push_back(chassis);
	state.mass = chassis->getState().mass;
	state.momentInertia = chassis->getState().momentInertia;
	state.radius = chassis->getState().radius;
	engineBank.resize(chassis->engineCount());
	weaponBank.resize(chassis->weaponCount());
	shieldBank.resize(chassis->shieldCount());
}

void Ship::setHull(shared_ptr<Hull> newHull)
{
	hull = newHull;
}

void Ship::setShield(shared_ptr<Shield> newShield, size_t slot)
{
	if (slot <= shieldBank.size())
	{
		shieldBank[slot] = newShield;
		components.push_back(newShield);
	}
	else
	{
		throw length_error("Shield slot out of range.");
	}
}

void Ship::setWeapon(shared_ptr<Weapon> newWeapon, size_t slot)
{
	if (slot <= weaponBank.size())
	{
		weaponBank[slot] = newWeapon;
		components.push_back(newWeapon);
	}
	else
	{
		throw length_error("Weapon slot out of range.");
	}
}

void Ship::setEngine(shared_ptr<Engine> newEngine, size_t slot)
{
	if (slot <= engineBank.size())
	{
		engineBank[slot] = newEngine;
		components.push_back(newEngine);
	}
	else
	{
		throw length_error("Engine slot out of range.");
	}
}

void Ship::setShipController(
	std::shared_ptr<AI> newController)
{
	controller = newController;
}

void Ship::accelForward()
{
	state.force += 20 * Vector(cos(state.state.angle), sin(state.state.angle));
}

void Ship::accelBackward()
{
	state.force -= 20 * Vector(cos(state.state.angle), sin(state.state.angle));
}

void Ship::accelRight()
{
	state.force += Vector(5, 0);
}

void Ship::accelLeft()
{
	state.force += Vector(-5, 0);
}

void Ship::inertialBrake()
{
	state.force -= trueVelocity() * 2.0;
}

void Ship::rotateLeft()
{
	state.torque += 15;//counterclockwise motion is positive
}

void Ship::rotateRight()
{
	state.torque -= 15;
}
void Ship::rotate(float magnitude)
{
	state.torque += min(max(20 * magnitude, -20.0f), 20.0f);
}

void Ship::pointAtAngle(float angle)
{
	float relativeAngleToCurser =
		angle - state.state.angle;
	if (relativeAngleToCurser < M_PI) relativeAngleToCurser += 2 * M_PI;
	if (relativeAngleToCurser > M_PI) relativeAngleToCurser -= 2 * M_PI;
	rotate(relativeAngleToCurser);
	for (auto&& weapon : weaponBank)
	{
		weapon->pointTo(angle);
	}
}

void Ship::fire(size_t weaponSlot)
{
	if (weaponSlot <= weaponBank.size())
	{
		if (weaponBank[weaponSlot].get() == NULL) throw SDL_MiscException("weaponBank slot NULL");
		weaponBank[weaponSlot]->fire();
	}
	else
	{
		throw length_error("Weapon slot out of range.");
	}
}

// std::shared_ptr<std::vector<SensorDatum>>
// 	Ship::pullSensors()
// {
// 	auto output(make_shared<std::vector<SensorDatum>>());

// 	return output;
// }


shared_ptr<I_Composite> Ship::getParent() const
{
	return dynamic_pointer_cast<I_Composite>(
		playSpace.lock());
}

void Ship::hit(Projectile& hit)
{

	/*

While the projectile is inside the ship, it will
not be able to collide with anything in the environment.

	*/

	chassis->hit(hit);
	if (chassis->hp.health <= 0)
		playSpace.lock()->remove(this);
	playSpace.lock()->remove(&hit);
}

void Ship::updateMovement()
{
	if (inertialDampenerEngaged)
	{
		state.force -= trueVelocity() * 0.2;
		state.torque -= state.state.angularVel * 5.0;
	}
	Pure_WorldPhysics::update(
		GameUpdateEnvironment::getDT());
	updateHitBoxes({state.radius,state.state.pos});
}

void Ship::updateControl()
{
	controller->updateControl();
}

void Ship::updateLogic()
{
	controller->updateLogic();
}

void Ship::updateCollisions()
{

}

void Ship::updateRemoval()
{

}

void Ship::remove(I_Child* removeItem)
{
	
}

void Ship::draw()
{
	chassis->draw();
	weaponBank[0]->draw();
}
