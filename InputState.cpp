#include "InputState.h"
#include <iostream>
#include <cassert>
#include <cstring>
#include <SDL2/SDL.h>
#include "GamepadState.h"
#include "LogInput.h"

InputState& InputState::GetInstance() {
	static InputState inputState;

	return inputState;
}

void InputState::Initialize() {
	if ( SDL_InitSubSystem( SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER ) != 0 ) {
		LogInput( "Failed to initialize SDL input subsystem", "SDL", LogSeverity::ERROR_MSG );
		assert( false );
	}

	m_Gamepads.resize( INPUT_MAX_NR_OF_GAMEPADS );
	std::fill( m_Gamepads.begin(), m_Gamepads.end(), nullptr );
}

void InputState::Deinitialize() {
	SDL_QuitSubSystem( SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER );
	if ( m_Callbacks.size() > 0 ) {
		rStringStream ss;
		for ( auto& callback : m_Callbacks ) {
			ss << static_cast<int>( callback.first ) << ", ";
		}
		LogInput( "Input state was destructed while still having callbacks registered to it. Callback values: " + ss.str(), "InputState", LogSeverity::WARNING_MSG );
	}
	if ( m_KeyboardState ) {
		pDelete( m_KeyboardState );
		m_KeyboardState = nullptr;
	}
}

void InputState::Update() {
	SDL_PumpEvents();

	if ( m_KeyboardStateTracking ) {
		int size;
		const Uint8* keyboardState = SDL_GetKeyboardState( &size );
		if ( m_KeyboardState == nullptr ) {
			m_KeyboardState = pNewArray( Uint8, size );
		}
		memcpy( m_KeyboardState, keyboardState, size );
	}

	m_MouseState.ButtonState = SDL_GetMouseState( &m_MouseState.PositionX, &m_MouseState.PositionY );
	m_MouseInsideWindow		 = SDL_GetMouseFocus() != nullptr;
	int mouseMoveX, mouseMoveY;
	SDL_GetRelativeMouseState( &mouseMoveX, &mouseMoveY );
	m_MouseMoveAccumulationX += mouseMoveX;
	m_MouseMoveAccumulationY += mouseMoveY;

	for ( auto gamepad : m_Gamepads ) {
		if ( gamepad ) {
			gamepad->Update();
		}
	}
}

void InputState::HandleEvent( const SDL_Event &event ) {
	// Process event for this class
	switch ( event.type ) {
		case SDL_MOUSEWHEEL: {
			m_MouseScrollAccumulationX += event.wheel.x;
			m_MouseScrollAccumulationY += event.wheel.y;
		} break;
		case SDL_CONTROLLERDEVICEADDED: {
			SDL_GameController* controller = nullptr;
			// Open controller so we can use it
			controller = SDL_GameControllerOpen( event.cdevice.which );
			if ( controller ) {
				m_Gamepads.at( event.cdevice.which ) = pNew( GamepadState, controller );
				LogInput( pString( SDL_GameControllerName( controller ) ) + " " + rToString( event.cdevice.which ) + " added",
					"GamepadState", LogSeverity::INFO_MSG );
			} else {
				LogInput( "Could not open gamecontroller " + rToString( event.cdevice.which ) + ": " + SDL_GetError(), "GamepadState",
					LogSeverity::ERROR_MSG );
			}
		} break;
		case SDL_CONTROLLERDEVICEREMOVED: {
			GamepadState* gp = m_Gamepads.at( event.cdevice.which );
			if ( gp != nullptr ) {
				LogInput( gp->GetName() + " " + std::to_string( event.cdevice.which ) + " removed", "GamepadState", LogSeverity::INFO_MSG );
				m_Gamepads.at( event.cdevice.which ) = nullptr;
				pDelete( gp );
			}
		} break;
	}
	// Relay event to callbacks
	switch ( event.type ) {
		case SDL_MOUSEWHEEL: { }
		case SDL_KEYUP:
		case SDL_KEYDOWN:
		case SDL_MOUSEMOTION:
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		case SDL_CONTROLLERAXISMOTION:
		case SDL_CONTROLLERBUTTONUP:
		case SDL_CONTROLLERBUTTONDOWN:
		case SDL_CONTROLLERDEVICEADDED:
		case SDL_CONTROLLERDEVICEREMOVED:
		case SDL_CONTROLLERDEVICEREMAPPED:
		case SDL_FINGERDOWN:
		case SDL_FINGERUP:
		case SDL_FINGERMOTION: {// If you know what i mean ;)
			for ( auto& callback : m_Callbacks ) {
				// Will return true if it wants to consume the event
				if ( callback.second( event ) ) {
					break;
				}
			}
		} break;
	}
}

InputEventCallbackHandle InputState::RegisterEventInterest( InputEventCallbackFunction callbackFunction ) {
	InputEventCallbackHandle handle = static_cast<InputEventCallbackHandle>( m_NextHandle++ );

	m_Callbacks.emplace( handle, callbackFunction );
	return handle;
}

void InputState::UnregisterEventInterest( InputEventCallbackHandle callbackHandle ) {
	auto it = m_Callbacks.find( callbackHandle );

	if ( it == m_Callbacks.end() ) {
		LogInput( "Tried to unregister invalid input event callback handle: " + rToString( static_cast<int>( callbackHandle ) ), "InputState", LogSeverity::WARNING_MSG );
	} else {
		m_Callbacks.erase( it );
	}
}

const MouseState& InputState::GetMouseState() const {
	return m_MouseState;
}

int InputState::GetMouseDeltaX( int previousValue ) const {
	return m_MouseMoveAccumulationX - previousValue;
}

int InputState::GetMouseDeltaY( int previousValue ) const {
	return m_MouseMoveAccumulationY - previousValue;
}

int InputState::GetMouseDeltaAccumulationX() const {
	return m_MouseMoveAccumulationX;
}

int InputState::GetMouseDeltaAccumulationY() const {
	return m_MouseMoveAccumulationY;
}

int InputState::GetMouseScrollDeltaX( int previousValue ) const {
	return m_MouseScrollAccumulationX - previousValue;
}

int InputState::GetMouseScrollDeltaY( int previousValue ) const {
	return m_MouseScrollAccumulationY - previousValue;
}

int InputState::GetMouseScrollAccumulationX() const {
	return m_MouseScrollAccumulationX;
}

int InputState::GetMouseScrollAccumulationY() const {
	return m_MouseScrollAccumulationY;
}

bool InputState::IsMouseButtonDown( MOUSE_BUTTON mouseButton ) const {
	return ( m_MouseState.ButtonState & SDL_BUTTON( mouseButton ) ) != 0;
}

bool InputState::IsMouseButtonUp( MOUSE_BUTTON mouseButton ) const {
	return !( m_MouseState.ButtonState & SDL_BUTTON( mouseButton ) );
}

void InputState::SetMouseButtonState( MOUSE_BUTTON mouseButton, INPUT_STATE state ) {
	// Set the corresponding bit
	m_MouseState.ButtonState ^= (-state ^ m_MouseState.ButtonState) & (1 << mouseButton);
}

bool InputState::IsKeyDown( SDL_Scancode scanCode ) const {
	return m_KeyboardState[scanCode] && m_KeyboardStateTracking;
}

bool InputState::IsKeyUp( SDL_Scancode scanCode ) const {
	return !m_KeyboardState[scanCode] && m_KeyboardStateTracking;
}

void InputState::ActivateKeyboardStateTracking() {
	m_KeyboardStateTracking = true;
}

void InputState::DeactivateKeyboardStateTracking() {
	m_KeyboardStateTracking = false;
}

bool InputState::IsKeyboardStateTrackingActivated() const {
	return m_KeyboardStateTracking;
}

void InputState::SetKeyState( SDL_Scancode scanCode, INPUT_STATE state ) {
	if ( state != INPUT_STATE_IGNORE ) {
		m_KeyboardState[ scanCode ] = static_cast<int>( state );
	}
}

const GamepadState* InputState::GetGamepadState( unsigned int gamepadIndex ) const {
	if ( gamepadIndex >= m_Gamepads.size() ) {
		return nullptr;
	}
	return m_Gamepads.at( gamepadIndex );
}

size_t InputState::GetNrOfGamepads() const {
	return m_Gamepads.size();
}

