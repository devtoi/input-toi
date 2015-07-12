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
	// Checks if key was pressed. Consumes one entry for the key from the press stack if it was pressed.
	INPUT_API bool KeyUpDownConsume ( SDL_Scancode scanCode, INPUT_STATE stateToSet = INPUT_STATE_IGNORE );
	// Checks if key was pressed. Consumes all entries for the key from the press stack if it was pressed.
	INPUT_API int  KeyUpDownConsumeAll ( SDL_Scancode scanCode, INPUT_STATE stateToSet = INPUT_STATE_IGNORE );
	INPUT_API bool KeyDownUp ( SDL_Scancode scanCode ) const;
	// Checks if key was released. Consumes one entry for the key from the release stack if it was released.
	INPUT_API bool KeyDownUpConsume ( SDL_Scancode scanCode, INPUT_STATE stateToSet = INPUT_STATE_IGNORE );
	// Checks if key was released. Consumes all entries for the key from the release stack if it was released.
	INPUT_API int  KeyDownUpConsumeAll ( SDL_Scancode scanCode, INPUT_STATE stateToSet = INPUT_STATE_IGNORE );
	INPUT_API bool KeyDown ( SDL_Scancode scanCode ) const;
	INPUT_API bool KeyUp ( SDL_Scancode scanCode ) const;

	INPUT_API const pVector<SDL_Scancode>& GetKeyboardPressStack () const;
	INPUT_API const pVector<SDL_Scancode>& GetKeyboardReleaseStack () const;

	INPUT_API bool MouseButtonDown                  ( MOUSE_BUTTON button ) const;
	INPUT_API bool MouseButtonUp                    ( MOUSE_BUTTON button ) const;
	INPUT_API bool MouseButtonUpDown                ( MOUSE_BUTTON button ) const;
	// Checks if button was pressed. Consumes one entry for the button from the press stack if it was pressed.
	INPUT_API bool MouseButtonUpDownConsume         ( MOUSE_BUTTON button, INPUT_STATE stateToSet = INPUT_STATE_IGNORE );
	// Checks if button was pressed. Consumes all entries for the button from the press stack if it was pressed.
	INPUT_API int  MouseButtonUpDownConsumeAll      ( MOUSE_BUTTON button, INPUT_STATE stateToSet = INPUT_STATE_IGNORE );
	INPUT_API bool MouseButtonDownUp                ( MOUSE_BUTTON button ) const;
	// Checks if button was released. Consumes one entry for the button from the release stack if it was released.
	INPUT_API bool MouseButtonDownUpConsume         ( MOUSE_BUTTON button, INPUT_STATE stateToSet = INPUT_STATE_IGNORE );
	// Checks if button was released. Consumes all entries for the button from the release stack if it was released.
	INPUT_API int  MouseButtonDownUpConsumeAll      ( MOUSE_BUTTON button, INPUT_STATE stateToSet = INPUT_STATE_IGNORE );
	INPUT_API bool MouseButtonDoubleUpDown          ( MOUSE_BUTTON button ) const;
	// Checks if button was double pressed. Consumes one entry for the button from the release stack if it was released.
	INPUT_API bool MouseButtonDoubleUpDownConsume   ( MOUSE_BUTTON button, INPUT_STATE stateToSet = INPUT_STATE_IGNORE );
	// Checks if button was double pressed. Consumes all entries for the button from the release stack if it was released.
	INPUT_API int  MouseButtonDoubleUpDownConsumeAll ( MOUSE_BUTTON button, INPUT_STATE stateToSet = INPUT_STATE_IGNORE );
	INPUT_API bool MouseButtonDoubleDownUp          ( MOUSE_BUTTON button ) const;
	// Checks if button was double released. Consumes one entry for the button from the release stack if it was released.
	INPUT_API bool MouseButtonDoubleDownUpConsume   ( MOUSE_BUTTON button, INPUT_STATE stateToSet = INPUT_STATE_IGNORE );
	// Checks if button was double released. Consumes all entries for the button from the release stack if it was released.
	INPUT_API int MouseButtonDoubleDownUpConsumeAll ( MOUSE_BUTTON button, INPUT_STATE stateToSet = INPUT_STATE_IGNORE );
	INPUT_API int GetMousePosX () const;
	INPUT_API int GetMousePosY () const;
	INPUT_API int GetMousePosDeltaX () const;
	INPUT_API int GetMousePosDeltaY () const;
	INPUT_API int GetMouseScrollDeltaX () const;
	INPUT_API int GetMouseScrollDeltaY () const;

	INPUT_API const pVector<MOUSE_BUTTON>& GetMouseSingleClickPressStack () const;
	INPUT_API const pVector<MOUSE_BUTTON>& GetMouseSingleClickReleaseStack () const;
	INPUT_API const pVector<MOUSE_BUTTON>& GetMouseDoubleClickPressStack () const;
	INPUT_API const pVector<MOUSE_BUTTON>& GetMouseDoubleClickReleaseStack () const;

	INPUT_API const GamepadContext& GetGamepadContext ( unsigned int gamepadIndex ) const;

private:
	bool HandleEvent ( const SDL_Event& event );
	bool ConsumeMouseButton ( pVector<MOUSE_BUTTON>& stack, MOUSE_BUTTON button, INPUT_STATE stateToSet );
	int	 ConsumeMouseButtonAll ( pVector<MOUSE_BUTTON>& stack, MOUSE_BUTTON button, INPUT_STATE stateToSet );

	InputEventCallbackHandle m_InputEventCallbackHandle;

	pVector<SDL_Scancode> m_KeyboardPressStack;
	pVector<SDL_Scancode> m_KeyboardReleaseStack;

	pVector<MOUSE_BUTTON> m_MouseSingleClickPressStack;
	pVector<MOUSE_BUTTON> m_MouseSingleClickReleaseStack;
	pVector<MOUSE_BUTTON> m_MouseDoubleClickPressStack;
	pVector<MOUSE_BUTTON> m_MouseDoubleClickReleaseStack;	// Does this even make sense?

	int m_MousePosDeltaX = 0;
	int m_MousePosDeltaY = 0;
	int m_MousePosLastX	 = 0;
	int m_MousePosLastY	 = 0;

	int m_MouseScrollDeltaX	  = 0;
	int m_MouseScrollDeltaY	  = 0;
	int m_MouseScrollLastPosX = 0;
	int m_MouseScrollLastPosY = 0;

	pVector<GamepadContext> m_GamepadContexts;
};

