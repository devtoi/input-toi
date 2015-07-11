#include "GamepadContext.h"
#include "InputState.h"
#include "GamepadState.h"

GamepadContext::GamepadContext( ) { }

void GamepadContext::Initialize( int gamepadIndex ) {
	m_GamepadIndex = gamepadIndex;
}

void GamepadContext::Deinitialize( ) {
	m_GamepadIndex = INVALID_GAMEPAD_INDEX;
}

void GamepadContext::Update() {
	m_PressStack.clear();
	m_ReleaseStack.clear();
}

bool GamepadContext::HandleEvent( const SDL_Event& event ) {
	switch ( event.type ) {
		case SDL_CONTROLLERBUTTONDOWN: {
			if ( event.cbutton.which == m_GamepadIndex ) {
				m_PressStack.push_back( event.cbutton.button );
			}
		} break;
		case SDL_CONTROLLERBUTTONUP: {
			if ( event.cbutton.which == m_GamepadIndex ) {
				m_ReleaseStack.push_back( event.cbutton.button );
			}
		} break;
	}
	return false;
}

bool GamepadContext::ButtonUpDown( SDL_GameControllerButton button ) const {
	for ( auto& pressed : m_PressStack ) {
		if ( pressed == button ) {
			return true;
		}
	}
	return false;
}

bool GamepadContext::ButtonDownUp( SDL_GameControllerButton button ) const {
	for ( auto& pressed : m_ReleaseStack ) {
		if ( pressed == button ) {
			return true;
		}
	}
	return false;
}

bool GamepadContext::ButtonDown( SDL_GameControllerButton button ) const {
	const GamepadState* state = g_InputState.GetGamepadState( m_GamepadIndex );

	if ( state ) {
		return state->ButtonDown( button );
	} else {
		return false;
	}
}

bool GamepadContext::ButtonUp( SDL_GameControllerButton button ) const {
	const GamepadState* state = g_InputState.GetGamepadState( m_GamepadIndex );

	if ( state ) {
		return state->ButtonUp( button );
	} else {
		return false;
	}
}

