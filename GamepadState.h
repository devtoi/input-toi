#pragma once

#include <SDL2/SDL_gamecontroller.h>
#include <string>
#include "InputLibraryDefine.h"

using std::string;

class GamepadState {
public:
	GamepadState( SDL_GameController* controller );
	~GamepadState();

	void Update ();

	INPUT_API string GetName () const;

	INPUT_API bool ButtonDown ( SDL_GameControllerButton button ) const;
	INPUT_API bool ButtonUp ( SDL_GameControllerButton button ) const;

	INPUT_API void ActivateStateTracking ();
	INPUT_API void DeactivateStateTracking ();

	INPUT_API float GetRightStickX () const;
	INPUT_API float GetRightStickY () const;
	INPUT_API float GetLeftStickX () const;
	INPUT_API float GetLeftStickY () const;
	INPUT_API float GetLeftTrigger () const;
	INPUT_API float GetRightTrigger () const;

private:
	void ZeroState ();

	SDL_GameController* m_Controller;

	bool m_TrackGamepadState = true;

	Uint32 m_ButtonsMask = 0;
	bool   m_Connected	 = false;

	float m_RightStickX	 = 0.0f;
	float m_RightStickY	 = 0.0f;
	float m_LeftStickX	 = 0.0f;
	float m_LeftStickY	 = 0.0f;
	float m_LeftTrigger	 = 0.0f;
	float m_RightTrigger = 0.0f;
};

