#include "Settings/controlMap.h"

using namespace std;

Key ControlMap::actionToKeyMap[(int)Action::ACTION_COUNT];

void ControlMap::init()
{
	actionToKeyMap[(int)Action::AccelForward] = Key::W;
	actionToKeyMap[(int)Action::AccelBackward] = Key::S;
	actionToKeyMap[(int)Action::AccelLeft] = Key::A;
	actionToKeyMap[(int)Action::AccelRight] = Key::D;
	actionToKeyMap[(int)Action::Decel] = Key::S;
	actionToKeyMap[(int)Action::RotateRight] = Key::D;
	actionToKeyMap[(int)Action::RotateLeft] = Key::A;
	actionToKeyMap[(int)Action::RotateWeaponsRight] = Key::J;
	actionToKeyMap[(int)Action::RotateWeaponsLeft] = Key::K;
	actionToKeyMap[(int)Action::FireAllWeapons] = Key::Space;
	actionToKeyMap[(int)Action::FireAllAimedWeapons] = Key::V;
	actionToKeyMap[(int)Action::FireAllNonAimedWeapons] = Key::B;

	actionToKeyMap[(int)Action::ToggleControlScheme] = Key::Tab;
	actionToKeyMap[(int)Action::OpenMenu] = Key::Escape;
}

Key ControlMap::getActionKey(Action action)
{
	return actionToKeyMap[(int)action];
}
