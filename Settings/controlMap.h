#pragma once

#include "SDL_Handling/SDL_Handling.h"

enum class Action
{
	AccelForward,
	AccelBackward,
	AccelLeft,
	AccelRight,
	Decel,
	RotateRight,
	RotateLeft,
	RotateWeaponsRight,
	RotateWeaponsLeft,
	FireAllWeapons,
	FireAllAimedWeapons,
	FireAllNonAimedWeapons,

	ToggleControlScheme,
	OpenMenu,

	ACTION_COUNT
};

class ControlMap
{
	static Key actionToKeyMap[(int)Action::ACTION_COUNT];
	ControlMap();
public:
	static void init();
	static Key getActionKey(Action action);
};
