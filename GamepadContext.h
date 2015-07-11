#pragma once
#include <SDL2/SDL_gamecontroller.h>
#include <SDL2/SDL_events.h>
#include INPUT_ALLOCATION_HEADER
#include "InputLibraryDefine.h"

class GamepadContext {
public:
	INPUT_API GamepadContext( );

	INPUT_API void Initialize( int gamepadIndex );
	INPUT_API void Deinitialize( );
	INPUT_API void Update( );

	INPUT_API bool HandleEvent ( const SDL_Event& event );

	INPUT_API bool ButtonUpDown ( SDL_GameControllerButton button ) const;
	INPUT_API bool ButtonDownUp ( SDL_GameControllerButton button ) const;
	INPUT_API bool ButtonDown ( SDL_GameControllerButton button ) const;
	INPUT_API bool ButtonUp ( SDL_GameControllerButton button ) const;

private:
	const int INVALID_GAMEPAD_INDEX = -1;

	pVector<Uint8> m_PressStack;
	pVector<Uint8> m_ReleaseStack;
	int m_GamepadIndex = INVALID_GAMEPAD_INDEX;
};

