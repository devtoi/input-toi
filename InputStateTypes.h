#pragma once

#include <functional>
#include <SDL2/SDL_events.h>
#include <utility/Handle.h>

struct InputEventCallbackHandle_tag { };

typedef Handle<InputEventCallbackHandle_tag, int, -1> InputEventCallbackHandle;
typedef HandleHasher<InputEventCallbackHandle_tag, int, -1> InputEventCallbackHandleHasher;
typedef std::function<bool ( const SDL_Event& event )>	  InputEventCallbackFunction;

struct MouseState {
	Uint32 ButtonState = 0;
	int	   PositionX   = 0;
	int	   PositionY   = 0;
};

typedef const Uint8* KeyboardState;

typedef Uint32 GamepadButtonState;
