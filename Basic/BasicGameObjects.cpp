// #include "Basic/BasicGameObjects.h"
// #include "Basic/Game.h"

// using namespace std;

// /////-------------------GameObject-------------------\\\\\

// /////--------------------Entity-------------------\\\\\

// Entity::Entity(const ImageTexture& inWorldTexture,
// 	const PhysicsObject& inState)
// :
// 	worldTexture(inWorldTexture),
// 	state(inState),
// 	currentHitBox{inState.radius, inState.state.pos},
// 	previousHitBox{inState.radius, inState.state.pos}
// {}

// void Entity::collide(shared_ptr<Projectile> p)
// {
// 	// game.removeFromUpdates((GameObject*)(&p));
// }

// void Entity::updateEngine_Move()
// {
// 	state.update();
// 	// float cAngle = cos(state.state.angle);
// 	// float sAngle = sin(state.state.angle);
// 	// for (auto&& child : children)
// 	// {
// 	// 	Vector relativePosition(
// 	// 		child->posToParent.x * cAngle - child->posToParent.y * sAngle,
// 	// 		child->posToParent.x * sAngle + child->posToParent.y * cAngle);
// 	// 	child->state.state.pos = state.state.pos + relativePosition;
// 	// 	child->state.state.vel = state.state.vel + Vector(relativePosition.y, - relativePosition.x);
// 	// 	if (!child->noAngleFlag)
// 	// 	{
// 	// 		child->state.state.angularVel = state.state.angularVel;
// 	// 		child->state.state.angle = state.state.angle + angleToParent;
// 	// 		if (child->state.state.angle > 2 * M_PI)
// 	// 			child->state.state.angle -= 2 * M_PI;//ensure angle is between 0 and 2pi
// 	// 	}
// 	// }
// }

// void Entity::draw() const
// {
// 	worldTexture.draw(cam.stateToParameters(state));
// }

// /////-----------------CompositeEntity---------------\\\\\

// CompositeEntity::CompositeEntity(const ImageTexture& inWorldTexture,
// 	const PhysicsObject& inState)
// :
// 	Entity(inWorldTexture, inState)
// {}

// void CompositeEntity::collide(std::shared_ptr<Projectile> p)
// {
// 	game.removeFromUpdates((Entity*)(&p));
// 	insideShots.push_back(p);

// 	// chassis->collide(p);
// 	// if (chassis->hp.health <= 0)
// 	// 	game.removeFromUpdates(this);
// }

// void CompositeEntity::updateEngine_Move()
// {
// 	state.update();
// 	float cAngle = cos(state.state.angle);
// 	float sAngle = sin(state.state.angle);
// 	for (int i = 0; i < children.size(); ++i)
// 	{
// 		Vector relativePosition(
// 			childrenLocations[i].x * cAngle
// 				- childrenLocations[i].y * sAngle,
// 			childrenLocations[i].x * sAngle
// 				+ childrenLocations[i].y * cAngle);

// 		children[i]->getKinetics().pos =
// 			state.state.pos + relativePosition;

// 		children[i]->getKinetics().vel =
// 			state.state.vel
// 			+ Vector(relativePosition.y, - relativePosition.x);

// 		// if (!childred[i]->noAngleFlag)
// 		// {
// 		// 	children[i]->getKinetics().angularVel =
// 		// 		state.state.angularVel;

// 		// 	children[i]->getKinetics().angle =
// 		// 		state.state.angle + angleToParent;

// 		// 	if (children[i]->getKinetics().angle > 2 * M_PI)
// 		// 		children[i]->getKinetics().angle -= 2 * M_PI;//ensure angle is between 0 and 2pi
// 		// }
// 	}

// 	updateEngine_Collision();
// }

// void CompositeEntity::updateEngine_Collision()
// {
// 	for (auto&& child : compositeChildren)
// 	{
// 		child->updateEngine_Collision();
// 	}
// }

// void CompositeEntity::draw() const
// {
// 	worldTexture.draw(cam.stateToParameters(state));
// 	for (auto&& child : children)
// 	{
// 		child->draw();
// 	}
// }

// /////---------------------Camera---------------------\\\\\

// Camera::Camera()
// :
// 	pos(0, 0),
// 	zoom(0.01)
// {}

// void Camera::take(const GameObject* newController)
// {
// 	controlStack.push_back(newController);
// }

// bool Camera::setPosition(const GameObject* requester, const GamePosition& newPos)
// {
// 	if (controlStack.size() == 0) return false;
// 	if (requester == controlStack.back())
// 	{
// 		pos = newPos;
// 		return true;
// 	}
// 	else
// 	{
// 		return false;
// 	}
// }

// bool Camera::setZoom(const GameObject* requester, float newZoom)
// {
// 	if (controlStack.size() == 0) return false;
// 	if (requester == controlStack.back())
// 	{
// 		zoom = newZoom;
// 		return true;
// 	}
// 	else
// 	{
// 		return false;
// 	}
// }

// void Camera::release(const GameObject* controller)
// {
// 	for (int i = controlStack.size() - 1; i >= 0; --i)
// 	{
// 		if (controller == controlStack[i])
// 		{
// 			controlStack.erase(controlStack.begin() + i);
// 		}
// 	}
// }

// DrawParameters Camera::stateToParameters(const PhysicsObject& state) const
// {
// 	//make relative position instead
// 	Vector relativePosition(state.state.pos - pos);
// 	int size = state.radius * zoom * GameDrawEnvironment::getHeight();
// 	return DrawParameters::Rectangle(ScreenRectangle(
// 			relativePosition.x * zoom * GameDrawEnvironment::getHeight() + GameDrawEnvironment::getWidth() / 2 + 0.5f - size,//x
// 			-(relativePosition.y) * zoom * GameDrawEnvironment::getHeight() + GameDrawEnvironment::getHeight() / 2 + 0.5f - size,//y
// 			2 * size,//w
// 			2 * size//h
// 		),
// 		state.state.angle);
// }

// DrawParameters Camera::stateToParameters(const PhysicsObject& state, float z) const
// {
// 	float camDist = 3440.0f / zoom;//dist from cam to player level
// 	float distToCam = camDist + z;//dist from cam to object
// 	float ratio = camDist / distToCam;

// 	Vector relativePosition(state.state.pos - pos);
// 	relativePosition *= ratio;
// 	int size = state.radius * zoom * GameDrawEnvironment::getHeight() * ratio;

// 	return DrawParameters::Rectangle(ScreenRectangle(
// 		relativePosition.x * zoom * GameDrawEnvironment::getHeight() + GameDrawEnvironment::getWidth() / 2 + 0.5f - size,//x
// 		-(relativePosition.y) * zoom * GameDrawEnvironment::getHeight() + GameDrawEnvironment::getHeight() / 2 + 0.5f - size,//y
// 		2 * size,//w
// 		2 * size//h
// 	),
// 		state.state.angle);
// }

// Vector Camera::drawSpace() const
// {
// 	return Vector((float)(GameDrawEnvironment::getWidth()) / GameDrawEnvironment::getHeight() / zoom,
// 		1.0f / zoom);
// }

// Vector Camera::drawSpace(float z) const
// {
// 	float camDist = 3440.0f / zoom;
// 	float distToCam = camDist + z;
// 	return drawSpace() * (distToCam / camDist);
// }

// GamePosition Camera::pixelLocation(int x, int y) const
// {
// 	float worldX = pos.x + (x - GameDrawEnvironment::getWidth() / 2) / (zoom * GameDrawEnvironment::getHeight());
// 	float worldY = pos.y - (y - GameDrawEnvironment::getHeight() / 2) / (zoom * GameDrawEnvironment::getHeight());
// 	return GamePosition(worldX, worldY);
// }


// GamePosition Camera::mouseLocation() const
// {
// 	return pixelLocation(Mouse::x(), Mouse::y());
// }
