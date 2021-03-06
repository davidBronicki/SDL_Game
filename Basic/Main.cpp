#include <iostream>
#include <string>
#include <exception>
#include <memory>

#include "SDL_Handling/SDL_Handling.h"
#include "Basic/MyGameObjects.h"

using namespace std;

int main(int argc, char* args[])
{
	if (!GameDrawEnvironment::init(
		"First Window", 1080, 640)) return 1;

	GameControlEnvironment::init();

	Game::makeInstance();
	game.initialize();
	game.run();

	GameControlEnvironment::shutdown();
	GameDrawEnvironment::shutdown();

	return 0;
}

