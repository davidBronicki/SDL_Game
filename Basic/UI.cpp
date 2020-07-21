#include "Basic/UI.h"

#include "Basic/MyGameObjects.h"

using namespace std;

HUD::HUD(){}

void HUD::draw()
{
	for (auto&& removalItem : dropList)
	{
		for (auto it = drawList.begin();
			it < drawList.end(); ++it)
		{
			if (it->get() == removalItem.get())
			{
				drawList.erase(it);
				break;
			}
		}
	}
	dropList.clear();

	for (auto&& item : drawList)
	{
		item->draw();
	}
}

void HUD::addToDraw(std::shared_ptr<I_Draw> const& drawItem)
{
	drawList.push_back(drawItem);
}
void HUD::removeFromDraw(std::shared_ptr<I_Draw> const& drawItem)
{
	dropList.push_back(drawItem);
}



TargetElement::TargetElement(){}

// TargetElement::updatePosition(ScreenPoint const& newScreenPosition, int newSize)
// {
// 	screenPosition = newScreenPosition;
// 	size = newSize;
// }

void TargetElement::updatePosition(std::shared_ptr<PhysicsObject> const& newObject)
{
	object = newObject;
}

void TargetElement::draw()
{
	// DrawParameters drawParameters(DrawParameters::Rectangle(ScreenRectangle(
	// 	screenPosition.x - size, screenPosition.y - size,
	// 	screenPosition.x + size, screenPosition.y + size)));
	// GameDrawEnvironment::renderRectOutline(drawParameters);

	GameDrawEnvironment::renderRectOutline(
		cam.stateToParameters(*object));
}
