#pragma once

#include <SDL2/SDL_gamecontroller.h>
#include <string>
#include "InputLibraryDefine.h"

using std::string;

class Gamepad {
public:
	Gamepad( SDL_GameController* controller );
	~Gamepad();

	void             Update();

	// Saves current state. Used for tracking state changes.
	void             SaveState();

	INPUT_API string GetName() const;

	INPUT_API bool   ButtonDown( SDL_GameControllerButton button ) const;
	INPUT_API bool   ButtonUpDown( SDL_GameControllerButton button ) const;
	INPUT_API bool   ButtonDownUp( SDL_GameControllerButton button ) const;
	INPUT_API bool   ButtonUp( SDL_GameControllerButton button ) const;

	INPUT_API float  GetRightStickX() const;
	INPUT_API float  GetRightStickY() const;
	INPUT_API float  GetLeftStickX() const;
	INPUT_API float  GetLeftStickY() const;
	INPUT_API float  GetLeftTrigger() const;
	INPUT_API float  GetRightTrigger() const;

private:

	SDL_GameController* m_Controller;

	Uint32 m_ButtonsMask     = 0;
	Uint32 m_PrevButtonsMask = 0;
	bool m_Connected         = false;

	float m_RightStickX  = 0.0f;
	float m_RightStickY  = 0.0f;
	float m_LeftStickX   = 0.0f;
	float m_LeftStickY   = 0.0f;
	float m_LeftTrigger  = 0.0f;
	float m_RightTrigger = 0.0f;
};
