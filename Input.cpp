#include "Input.h"

#include <cassert>
#include <cstring> // Included for std::memcpy
#include <SDL2/SDL_events.h>
#include <SDL2/SDL.h>
#include "LogInput.h"
#include "Gamepad.h"

Input* Input::GetInstance() {
	static Input instance;
	return &instance;
}

Input::~Input() {
	pDeleteArray( m_PrevKeys );
	for (auto* gamepad : m_Gamepads) {
		if ( gamepad ) { 
			pDelete( gamepad );
		}
	}
}

void Input::Initialize() {
	if( SDL_InitSubSystem( SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER ) != 0 ) {
		LogInput( "Failed to initialize SDL input subsystem", "SDL", LogSeverity::ERROR_MSG );
		assert( false );
	}
	SDL_PumpEvents();
	m_CurrentKeys = SDL_GetKeyboardState( &m_KeyArrayLength );
	m_PrevKeys = pNewArray( Uint8, m_KeyArrayLength );

	m_Gamepads.resize( INPUT_MAX_NR_OF_GAMEPADS );
}

void Input::Update() {
	std::memcpy( m_PrevKeys, m_CurrentKeys, m_KeyArrayLength ); // Store previous keys for updown downup functionality

	// Update input states
	SDL_PumpEvents();
	m_CurrentKeys = SDL_GetKeyboardState( &m_KeyArrayLength );

	SDL_GetRelativeMouseState( &m_MouseDeltaX, &m_MouseDeltaY );		  // Track change in mouse position.
	m_PrevMouseMask = m_CurrentMouseMask;								  // Store previous button mask for updown downup functionality.
	m_CurrentMouseMask = SDL_GetMouseState( &m_MousePosX, &m_MousePosY ); // Get current mouse button mask.

	m_ScrollX = 0;
	m_ScrollY = 0;

	m_MouseInsideWindow = SDL_GetMouseFocus() != nullptr;

	m_ConsumedMouse = false;
	m_ConsumedKeys = false;
	m_PressStack.clear();
	m_ReleaseStack.clear();

	// Save gamepad states
	for( auto gamepad : m_Gamepads ) {
		if( gamepad ) {
			gamepad->SaveState();
		}
	}
	// Update gamepads
	for( auto gamepad : m_Gamepads ) {
		if( gamepad ) {
			gamepad->Update();
		}
	}
}

void Input::HandleEvent( const SDL_Event& event ) {
	switch( event.type ) {
		case SDL_MOUSEWHEEL: {
			m_ScrollX += event.wheel.x;
			m_ScrollY += event.wheel.y;
		} break;
		case SDL_KEYDOWN: {
			// Don't spam the button
			if( event.key.repeat == 0 ) {
				m_PressStack.push_back( event.key.keysym.scancode );
			}
		} break;
		case SDL_KEYUP: {
			// Don't spam the button
			if( event.key.repeat == 0 ) {
				m_ReleaseStack.push_back( event.key.keysym.scancode );
			}
		} break;
		case SDL_CONTROLLERDEVICEREMOVED: {
			Gamepad* gp = m_Gamepads.at( event.cdevice.which );
			if( gp != nullptr ) {
				LogInput( gp->GetName() + " " + std::to_string( event.cdevice.which ) + " removed", "Gamepad", LogSeverity::INFO_MSG );
				m_Gamepads.at( event.cdevice.which ) = nullptr;
				pDelete( gp );
			}
		} break;
		case SDL_CONTROLLERDEVICEADDED: {
			SDL_GameController* controller = nullptr;
			// Open controller so we can use it
			controller = SDL_GameControllerOpen( event.cdevice.which );
			if( controller ) {
				m_Gamepads.at( event.cdevice.which ) = pNew( Gamepad, controller );
				LogInput( pString( SDL_GameControllerName( controller ) ) + " " + rToString( event.cdevice.which ) + " added",
							 "Gamepad", LogSeverity::INFO_MSG );
			} else {
				LogInput( "Could not open gamecontroller " + rToString( event.cdevice.which ) + ": " + SDL_GetError(), "Gamepad",
							 LogSeverity::ERROR_MSG );
			}
		} break;
	}
}

bool Input::KeyUpDown( SDL_Scancode key, bool ignorePause ) const {
	return !m_PrevKeys[key] && m_CurrentKeys[key] && ( !m_Paused || ignorePause ) && !m_ConsumedKeys;
}

bool Input::KeyDownUp( SDL_Scancode key, bool ignorePause ) const {
	return m_PrevKeys[key] && !m_CurrentKeys[key] && ( !m_Paused || ignorePause ) && !m_ConsumedKeys;
}

bool Input::KeyDown( SDL_Scancode key, bool ignorePause ) const {
	return m_CurrentKeys[key] != 0 && ( !m_Paused || ignorePause ) && !m_ConsumedKeys;
}

bool Input::KeyUp( SDL_Scancode key, bool ignorePause ) const {
	return !m_CurrentKeys[key] && ( !m_Paused || ignorePause ) && !m_ConsumedKeys;
}

bool Input::MouseUpDown( unsigned int button, bool ignoreConsume ) {
	return !( m_PrevMouseMask & SDL_BUTTON( button ) ) &&										   // prev up
		   ( m_CurrentMouseMask & SDL_BUTTON( button ) ) && ( !m_ConsumedMouse || ignoreConsume ); // current down
}

bool Input::MouseDownUp( unsigned int button, bool ignoreConsume ) {
	return ( m_PrevMouseMask & SDL_BUTTON( button ) ) &&											// prev down
		   !( m_CurrentMouseMask & SDL_BUTTON( button ) ) && ( !m_ConsumedMouse || ignoreConsume ); // current up;
}

bool Input::MouseDown( unsigned int button ) {
	return ( m_CurrentMouseMask & SDL_BUTTON( button ) ) != 0 && !m_ConsumedMouse;
}

bool Input::MouseUp( unsigned int button ) {
	return !( m_CurrentMouseMask & SDL_BUTTON( button ) ) && !m_ConsumedMouse;
}

void Input::ConsumeMouseButtons() {
	m_ConsumedMouse = true;
}

void Input::ConsumeKeys() {
	m_ConsumedKeys = true;
}

int Input::GetMouseDeltaX() const {
	return m_MouseDeltaX;
}

int Input::GetMouseDeltaY() const {
	return m_MouseDeltaY;
}

int Input::GetMousePosX() const {
	return m_MousePosX;
}

int Input::GetMousePosY() const {
	return m_MousePosY;
}

int Input::GetScrollX( bool ignorePause ) const {
	if( ( !m_Paused || ignorePause ) && !m_ConsumedMouse )
		return m_ScrollX;
	else
		return 0;
}

int Input::GetScrollY( bool ignorePause ) const {
	if( ( !m_Paused || ignorePause ) && !m_ConsumedMouse )
		return m_ScrollY;
	else
		return 0;
}

bool Input::GetMouseInsideWindow() const {
	return m_MouseInsideWindow;
}

void Input::PauseKeyInput() {
	m_Paused = true;
}

void Input::UnPauseKeyInput() {
	m_Paused = false;
}

bool Input::IsPaused() const {
	return m_Paused;
}

const rVector<SDL_Scancode>& Input::GetPressStack() const {
	return m_PressStack;
}

rVector<SDL_Scancode>& Input::GetEditablePressStack() {
	return m_PressStack;
}

const rVector<SDL_Scancode>& Input::GetReleaseStack() const {
	return m_ReleaseStack;
}

rVector<SDL_Scancode>& Input::GetEditableReleaseStack() {
	return m_ReleaseStack;
}

Gamepad* Input::GetGamepad(unsigned int index) const {
	if (index >= m_Gamepads.size()) {
		return nullptr; 
	}
	return m_Gamepads.at( index );
}
