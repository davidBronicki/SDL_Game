#include "SDL_Handling/SDL_Handling.h"

#include <iostream>

#include <SDL.h>
#include <SDL_image.h>

using namespace std;

/////------------------GameDrawEnvironment-----------------/////

SDL_Window* GameDrawEnvironment::window = NULL;
SDL_Surface* GameDrawEnvironment::screen = NULL;
SDL_Renderer* GameDrawEnvironment::renderer = NULL;
int GameDrawEnvironment::width = 0;
int GameDrawEnvironment::height = 0;

bool GameDrawEnvironment::init(string name, int screenWidth, int screenHeight)
{
	//initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "failed to initialize! SDL Error: " << SDL_GetError() << endl;
		SDL_Quit();
		return false;
	}

	//initialize window
	width = screenWidth;
	height = screenHeight;

	window = SDL_CreateWindow(name.c_str(),
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		screenWidth, screenHeight,
		SDL_WINDOW_SHOWN);

	if (window == NULL)
	{
		cout << "failed to initialize window!" << endl;
		SDL_Quit();
		return false;
	}

	//initialize image library
	int imgFlags = IMG_INIT_PNG;
	if (IMG_Init(imgFlags) ^ imgFlags)
	{
		//there was at least one module which failed to load
		cout << "SDL image library failed to initialize! Error: " << IMG_GetError() << endl;
		return false;
	}

	//get screen renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
	{
		cout << "Renderer failed to load! SDL Error: " << SDL_GetError() << endl;
		return false;
	}

	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderClear(renderer);

	//get screen surface
	screen = SDL_GetWindowSurface(window);

	return true;
}

void GameDrawEnvironment::shutdown()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}

void GameDrawEnvironment::render()
{
	SDL_RenderPresent(renderer);
}

void GameDrawEnvironment::clearRender()
{
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
	SDL_RenderClear(renderer);
}

void GameDrawEnvironment::blit()
{
	SDL_UpdateWindowSurface(window);
}

void GameDrawEnvironment::renderRectFilled(const DrawParameters& params)
{
	SDL_SetRenderDrawColor(renderer,
		params.color.r, params.color.g, params.color.b, params.color.alpha);
	SDL_RenderFillRect(renderer, (SDL_Rect*)&(params.rect));
}

void GameDrawEnvironment::renderRectOutline(const DrawParameters& params)
{
	SDL_SetRenderDrawColor(renderer,
		params.color.r, params.color.g, params.color.b, params.color.alpha);
	SDL_RenderDrawRect(renderer, (SDL_Rect*)&(params.rect));
}

void GameDrawEnvironment::renderLine(const DrawParameters& params)
{
	SDL_SetRenderDrawColor(renderer,
		params.color.r, params.color.g, params.color.b, params.color.alpha);
	SDL_RenderDrawLine(renderer,
		params.rect.x, params.rect.y, params.rect.w, params.rect.h);
}

/////---------------GameControlEnvironment----------------/////

vector<int> SDL_KeycodeNumbers(410);
// vector<int> SDL_ScancodeNumbers(KEY_COUNT);

bool GameControlEnvironment::userQuit = false;
SDL_Event* GameControlEnvironment::e = new SDL_Event();

vector<bool> Keyboard::keyStates = vector<bool>((int)(Key::KEY_COUNT));//true if down
Basic_Event<Key> Keyboard::keyPressed;
Basic_Event<Key> Keyboard::keyReleased;

vector<bool> Mouse::buttonStates = vector<bool>(3);
Basic_Event<Button> Mouse::buttonPressed;
Basic_Event<Button> Mouse::buttonReleased;
Basic_Event<Motion, int, int> Mouse::mouseMotion;

// vector<int> SDL_ScancodeNames{
// 	SDL_SCANCODE_ESCAPE, SDL_SCANCODE_F1, SDL_SCANCODE_F2, SDL_SCANCODE_F3, SDL_SCANCODE_F4, SDL_SCANCODE_F5,
// 	SDL_SCANCODE_F6, SDL_SCANCODE_F7, SDL_SCANCODE_F8, SDL_SCANCODE_F9, SDL_SCANCODE_F10, SDL_SCANCODE_F11, SDL_SCANCODE_F12,

// 	SDL_SCANCODE_BACKQUOTE, SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3, SDL_SCANCODE_4, SDL_SCANCODE_5, SDL_SCANCODE_6,
// 	SDL_SCANCODE_7, SDL_SCANCODE_8, SDL_SCANCODE_9, SDL_SCANCODE_0, SDL_SCANCODE_MINUS, SDL_SCANCODE_EQUALS, SDL_SCANCODE_BACKSPACE,

// 	SDL_SCANCODE_TAB, SDL_SCANCODE_q, SDL_SCANCODE_w, SDL_SCANCODE_e, SDL_SCANCODE_r, SDL_SCANCODE_t, SDL_SCANCODE_y, SDL_SCANCODE_u,
// 	SDL_SCANCODE_i, SDL_SCANCODE_o, SDL_SCANCODE_p, SDL_SCANCODE_LEFTBRACKET, SDL_SCANCODE_RIGHTBRACKET, SDL_SCANCODE_BACKSLASH,

// 	SDL_SCANCODE_a, SDL_SCANCODE_s, SDL_SCANCODE_d, SDL_SCANCODE_f, SDL_SCANCODE_g, SDL_SCANCODE_h, SDL_SCANCODE_j,
// 	SDL_SCANCODE_k, SDL_SCANCODE_l, SDL_SCANCODE_SEMICOLON, SDL_SCANCODE_QUOTE, SDL_SCANCODE_RETURN,

// 	SDL_SCANCODE_LSHIFT, SDL_SCANCODE_z, SDL_SCANCODE_x, SDL_SCANCODE_c, SDL_SCANCODE_v, SDL_SCANCODE_b, SDL_SCANCODE_n,
// 	SDL_SCANCODE_m, SDL_SCANCODE_COMMA, SDL_SCANCODE_PERIOD, SDL_SCANCODE_SLASH, SDL_SCANCODE_RSHIFT,

// 	SDL_SCANCODE_LCTRL, SDL_SCANCODE_LALT, SDL_SCANCODE_SPACE, SDL_SCANCODE_RALT, SDL_SCANCODE_RCTRL,
// 	SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT
// };

vector<int> SDL_KeycodeNames{
	SDLK_ESCAPE, SDLK_F1, SDLK_F2, SDLK_F3, SDLK_F4, SDLK_F5,
	SDLK_F6, SDLK_F7, SDLK_F8, SDLK_F9, SDLK_F10, SDLK_F11, SDLK_F12,

	SDLK_BACKQUOTE, SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5, SDLK_6,
	SDLK_7, SDLK_8, SDLK_9, SDLK_0, SDLK_MINUS, SDLK_EQUALS, SDLK_BACKSPACE,

	SDLK_TAB, SDLK_q, SDLK_w, SDLK_e, SDLK_r, SDLK_t, SDLK_y, SDLK_u,
	SDLK_i, SDLK_o, SDLK_p, SDLK_LEFTBRACKET, SDLK_RIGHTBRACKET, SDLK_BACKSLASH,

	SDLK_a, SDLK_s, SDLK_d, SDLK_f, SDLK_g, SDLK_h, SDLK_j,
	SDLK_k, SDLK_l, SDLK_SEMICOLON, SDLK_QUOTE, SDLK_RETURN,

	SDLK_LSHIFT, SDLK_z, SDLK_x, SDLK_c, SDLK_v, SDLK_b, SDLK_n,
	SDLK_m, SDLK_COMMA, SDLK_PERIOD, SDLK_SLASH, SDLK_RSHIFT,

	SDLK_LCTRL, SDLK_LALT, SDLK_SPACE, SDLK_RALT, SDLK_RCTRL,
	SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT
};

void GameControlEnvironment::init()
{
	for (int i = 0; i < SDL_KeycodeNames.size(); ++i)
	{
		if (SDL_KeycodeNames[i] >> 30)
		{
			SDL_KeycodeNumbers[(SDL_KeycodeNames[i] & 0xffff) + 128] = i;
		}
		else
		{
			SDL_KeycodeNumbers[SDL_KeycodeNames[i]] = i;
		}
	}
}

void GameControlEnvironment::shutdown()
{
	delete e;
}

void GameControlEnvironment::quit()
{
	userQuit = true;
}

int getKeyIndex(int input)
{
	if (input >> 30)
	{
		return SDL_KeycodeNumbers[
			(input & 0xffff) + 128];
	}
	else
	{
		return SDL_KeycodeNumbers[input];
	}
}

int getMouseIndex(Uint8 input)
{
	switch (input)
	{
		case SDL_BUTTON_LEFT:
		return 0;
		break;
		case SDL_BUTTON_RIGHT:
		return 1;
		break;
		case SDL_BUTTON_MIDDLE:
		return 2;
		break;
	}
	throw SDL_MiscException("getMouseIndex called with invalid input");
}

void GameControlEnvironment::sortQueue()
{
	while (SDL_PollEvent(e) != 0)
	{
		switch (e->type)
		{
			int index;
			case SDL_QUIT:
			userQuit = true;
			break;

			case SDL_KEYDOWN:
			index = getKeyIndex(e->key.keysym.sym);
			Keyboard::keyStates[index] = true;
			Keyboard::keyPressed(static_cast<Key>(index));
			break;

			case SDL_KEYUP:
			index = getKeyIndex(e->key.keysym.sym);
			Keyboard::keyStates[index] = false;
			Keyboard::keyReleased(static_cast<Key>(index));
			break;

			case SDL_MOUSEBUTTONDOWN:
			index = getMouseIndex(e->button.button);
			Mouse::buttonStates[index] = true;
			Mouse::buttonPressed(static_cast<Button>(index));
			break;

			case SDL_MOUSEBUTTONUP:
			index = getMouseIndex(e->button.button);
			Mouse::buttonStates[index] = false;
			Mouse::buttonReleased(static_cast<Button>(index));
			break;

			case SDL_MOUSEWHEEL:
			index = e->wheel.direction == SDL_MOUSEWHEEL_NORMAL ? 1 : -1;//multipier in case scroll wheel is reversed
			Mouse::mouseMotion(static_cast<Motion>(index), e->wheel.x * index, e->wheel.y * index);
			break;
		}
	}
}


int Mouse::x()
{
	int output;
	SDL_GetMouseState(&output, NULL);
	return output;
}
int Mouse::y()
{
	int output;
	SDL_GetMouseState(NULL, &output);
	return output;
}
