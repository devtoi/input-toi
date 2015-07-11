#pragma once

#include "InputLibraryDefine.h"
#include "InputStateTypes.h"
#include INPUT_ALLOCATION_HEADER
#include "Types.h"

#define g_InputState InputState::GetInstance()

class GamepadState;

class InputState {
public:
	INPUT_API static InputState& GetInstance ();

	INPUT_API void Initialize ();
	INPUT_API void Deinitialize ();

	INPUT_API void					   Update ();
	INPUT_API void					   HandleEvent ( const SDL_Event& event );
	INPUT_API InputEventCallbackHandle RegisterEventInterest ( InputEventCallbackFunction callbackFunction );
	INPUT_API void					   UnregisterEventInterest ( InputEventCallbackHandle callbackHandle );


	INPUT_API const MouseState& GetMouseState () const;
	INPUT_API int				GetMouseDeltaX ( int previousValue ) const;
	INPUT_API int				GetMouseDeltaY ( int previousValue ) const;
	INPUT_API int				GetMouseDeltaAccumulationX () const;
	INPUT_API int				GetMouseDeltaAccumulationY () const;
	INPUT_API int				GetMouseScrollDeltaX ( int previousValue ) const;
	INPUT_API int				GetMouseScrollDeltaY ( int previousValue ) const;
	INPUT_API int				GetMouseScrollAccumulationX () const;
	INPUT_API int				GetMouseScrollAccumulationY () const;
	INPUT_API bool				IsMouseButtonDown ( MOUSE_BUTTON mouseButton ) const;
	INPUT_API bool				IsMouseButtonUp ( MOUSE_BUTTON mouseButton ) const;

	INPUT_API bool IsKeyDown ( SDL_Scancode scanCode ) const;
	INPUT_API bool IsKeyUp ( SDL_Scancode scanCode ) const;
	INPUT_API void ActivateKeyboardStateTracking ();
	INPUT_API void DeactivateKeyboardStateTracking ();
	INPUT_API bool IsKeyboardStateTrackingActivated () const;

	INPUT_API const GamepadState* GetGamepadState ( unsigned int gamepadIndex ) const;
	INPUT_API size_t			  GetNrOfGamepads () const;

private:
	InputState() { }

	~InputState() { }

	InputState( const InputState& rhs );
	InputState& operator = ( const InputState& rhs );

	pUnorderedMap<InputEventCallbackHandle, InputEventCallbackFunction, InputEventCallbackHandleHasher> m_Callbacks;// TODOJM: Make priority queue when consume is needed
	int m_NextHandle = 0;

	KeyboardState m_KeyboardState		  = nullptr;
	bool		  m_KeyboardStateTracking = true;

	MouseState m_MouseState;
	bool	   m_MouseInsideWindow		  = true;
	int		   m_MouseMoveAccumulationX	  = 0;
	int		   m_MouseMoveAccumulationY	  = 0;
	int		   m_MouseScrollAccumulationX = 0;
	int		   m_MouseScrollAccumulationY = 0;

	rVector<GamepadState*> m_Gamepads;
};

