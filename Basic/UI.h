#pragma once

#include "Basic/Interfaces.h"
#include <vector>
#include <memory>

class HUD:
	public I_Draw
{
	std::vector<std::shared_ptr<I_Draw>> drawList;
	std::vector<std::shared_ptr<I_Draw>> dropList;
public:
	HUD();

	void draw() override;

	void addToDraw(std::shared_ptr<I_Draw> const& drawItem);
	void removeFromDraw(std::shared_ptr<I_Draw> const& drawItem);
};