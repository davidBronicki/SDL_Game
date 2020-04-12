#include "Basic/MyGameObjects.h"
#include "Basic/Player.h"

using namespace std;

void Game::updateControl()
{
	player->updateControl();
	playSpace->updateControl();
}

void Game::updateLogic()
{
	player->updateLogic();
	playSpace->updateLogic();
}

void Game::updateMovement()
{
	player->updateMovement();
	playSpace->updateMovement();
}

void Game::updateCollisions()
{
	playSpace->updateCollisions();
}

void Game::updateRemoval()
{
	playSpace->updateRemoval();
}

void Game::remove(I_Child* removeItem)
{
	
}

void Game::draw()
{
	GameDrawEnvironment::clearRender();

	playSpace->draw();
	player->draw();

	GameDrawEnvironment::render();
}
