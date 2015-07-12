#pragma once

#include INPUT_ALLOCATION_HEADER
#include "InputLibraryDefine.h"
#include "InputStateTypes.h"
#include "GamepadContext.h"
#include "Types.h"

#define g_Input InputContext::GetInstance()

class InputContext {
public:
	INPUT_API static InputContext& GetInstance ();

	INPUT_API void Initialize ();
	INPUT_API void Deinitialize ();
	INPUT_API void Update ();

	INPUT_API bool KeyUpDown ( SDL_Scancode scanCode ) const;
	INPUT_API bool KeyDownUp ( SDL_Scancode scanCode ) const;
	INPUT_API bool KeyDown ( SDL_Scancode scanCode ) const;
	INPUT_API bool KeyUp ( SDL_Scancode scanCode ) const;

	INPUT_API const pVector<SDL_Scancode>& GetKeyboardPressStack () const;
	INPUT_API const pVector<SDL_Scancode>& GetKeyboardReleaseStack () const;

	INPUT_API bool MouseButtonDown ( MOUSE_BUTTON button ) const;
	INPUT_API bool MouseButtonUp ( MOUSE_BUTTON button ) const;
	INPUT_API bool MouseButtonUpDown ( MOUSE_BUTTON button ) const;
	INPUT_API bool MouseButtonDownUp ( MOUSE_BUTTON button ) const;
	INPUT_API bool MouseButtonDoubleUpDown ( MOUSE_BUTTON button ) const;
	INPUT_API bool MouseButtonDoubleDownUp ( MOUSE_BUTTON button ) const;
	INPUT_API int GetMousePosX( ) const;
	INPUT_API int GetMousePosY( ) const;
	INPUT_API int GetMousePosDeltaX( ) const;
	INPUT_API int GetMousePosDeltaY( ) const;
	INPUT_API int GetMouseScrollDeltaX( ) const;
	INPUT_API int GetMouseScrollDeltaY( ) const;

	INPUT_API const pVector<MOUSE_BUTTON>& GetMouseSingleClickPressStack () const;
	INPUT_API const pVector<MOUSE_BUTTON>& GetMouseSingleClickReleaseStack () const;
	INPUT_API const pVector<MOUSE_BUTTON>& GetMouseDoubleClickPressStack () const;
	INPUT_API const pVector<MOUSE_BUTTON>& GetMouseDoubleClickReleaseStack () const;

	INPUT_API const GamepadContext& GetGamepadContext ( unsigned int gamepadIndex ) const;

private:
	bool HandleEvent ( const SDL_Event& event );

	InputEventCallbackHandle m_InputEventCallbackHandle;

	pVector<SDL_Scancode> m_KeyboardPressStack;
	pVector<SDL_Scancode> m_KeyboardReleaseStack;

	pVector<MOUSE_BUTTON> m_MouseSingleClickPressStack;
	pVector<MOUSE_BUTTON> m_MouseSingleClickReleaseStack;
	pVector<MOUSE_BUTTON> m_MouseDoubleClickPressStack;
	pVector<MOUSE_BUTTON> m_MouseDoubleClickReleaseStack; // Does this even make sense?

	int m_MousePosDeltaX = 0;
	int m_MousePosDeltaY = 0;
	int m_MousePosLastX = 0;
	int m_MousePosLastY = 0;

	int m_MouseScrollDeltaX = 0;
	int m_MouseScrollDeltaY = 0;
	int m_MouseScrollLastPosX = 0;
	int m_MouseScrollLastPosY = 0;

	pVector<GamepadContext> m_GamepadContexts;
};

