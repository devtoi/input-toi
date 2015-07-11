#pragma once

#include "InputLibraryDefine.h"
#include <utility/Handle.h>
#include <SDL2/SDL_mouse.h>

#define ACTION_IDENTIFIER_INVALID -1;

struct ActionIdentifier_Tag {};

typedef Handle<ActionIdentifier_Tag, int, -1> ActionIdentifier;

enum class INPUT_API KeyBindingType {
	Primary,
	Secondary,
	Any
};

#define INPUT_MAX_NR_OF_GAMEPADS 16

enum INPUT_API MOUSE_BUTTON : Uint8 {
	MOUSE_BUTTON_LEFT	= SDL_BUTTON_LEFT,
	MOUSE_BUTTON_MIDDLE	= SDL_BUTTON_MIDDLE,
	MOUSE_BUTTON_RIGHT	= SDL_BUTTON_RIGHT,
	MOUSE_BUTTON_4		= SDL_BUTTON_X1,
	MOUSE_BUTTON_5		= SDL_BUTTON_X2,
};

enum INPUT_TYPE {
	INPUT_TYPE_NONE				= -3,
	INPUT_TYPE_ANY				= -2,
	INPUT_TYPE_KEYBOARD			= -1,
	INPUT_TYPE_GAMEPAD_FIRST	= 0,
	INPUT_TYPE_GAMEPAD_SECOND	= 1,
	INPUT_TYPE_GAMEPAD_THIRD	= 2,
	INPUT_TYPE_GAMEPAD_FOURTH	= 3,
};

static bool InputTypeIsGamepad( INPUT_TYPE inputType ) {
	return static_cast<int>( inputType ) >= 0;
}
