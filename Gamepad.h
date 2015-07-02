#pragma once

#include <SDL2/SDL_gamecontroller.h>
#include <string>

using std::string;

class Gamepad {
public:
						Gamepad(SDL_GameController* controller);
						~Gamepad();

	void Update();
	// Saves current state. Used for tracking state changes.
	void SaveState();

	string GetName		( ) const;

	bool ButtonDown		( SDL_GameControllerButton button ) const;
	bool ButtonUpDown	( SDL_GameControllerButton button ) const;
	bool ButtonDownUp	( SDL_GameControllerButton button ) const;
	bool ButtonUp		( SDL_GameControllerButton button ) const;

	float RightStickX	( ) const;
	float RightStickY	( ) const;
	float LeftStickX	( ) const;
	float LeftStickY	( ) const;
	float LeftTrigger	( ) const;
	float RightTrigger	( ) const;

private:
	SDL_GameController* m_Controller;

	Uint32				m_ButtonsMask = 0;
	Uint32				m_PrevButtonsMask = 0;
	bool 				m_Connected = false;

	float				m_RightStickX;
	float				m_RightStickY;
	float				m_LeftStickX;
	float				m_LeftStickY;
	float				m_LeftTrigger;
	float				m_RightTrigger;
};
