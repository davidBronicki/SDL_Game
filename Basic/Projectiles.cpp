#include "Basic/MyGameObjects.h"
#include "math.h"

/////---------------Projectile-------------\\\\\

using namespace std;
Projectile::Projectile(
	weak_ptr<I_Composite> inParent,
	ImageTexture const& inTexture,
	PhysicsObject const& initState)
:
	parent(inParent),
	Pure_WorldPhysics(initState),
	Pure_Draw(inTexture),
	I_Collidable(HitBox{
		initState.radius, initState.state.pos})
{}

void Projectile::collide(I_FullWorldObject* hit)
{
	hit->hit(*this);
}

shared_ptr<I_Composite> Projectile::getParent() const
{
	return dynamic_pointer_cast<I_Composite>(
		parent.lock());
}

void Projectile::updateMovement()
{
	Pure_WorldPhysics::update(
		GameUpdateEnvironment::getDT());
	updateHitBoxes({state.radius,state.state.pos});
}

void Projectile::updateLogic()
{
	if (state.state.pos.magnitudeSqr() > 10000)
		parent.lock()->remove(this);
}

void Projectile::draw()
{
	worldTexture.draw(cam.stateToParameters(state));
}
