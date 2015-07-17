#include "GamepadState.h"
#include "LogInput.h"

// For making axises in the interval [-1,1]
#define GAMEPAD_AXIS_FACTOR 1 / 32768.0f

using std::string;
GamepadState::GamepadState( SDL_GameController* controller )
	: m_Controller( controller ) { }

GamepadState::~GamepadState() {
	// SDL_GameControllerClose( m_controller ); // TODOJM: Is this needed?
	// delete m_controller;
}

void GamepadState::Update() {
	if ( m_TrackGamepadState ) {
		// Only update if controller is attached
		if ( SDL_GameControllerGetAttached( m_Controller ) == SDL_TRUE ) {
			// Reset mask to no buttons being down
			m_ButtonsMask = 0;
			// Loop through all SDL controller buttons
			for ( int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; ++i ) {
				// Set bit corresponding to the specific button
				m_ButtonsMask |=
					SDL_GameControllerGetButton( m_Controller, static_cast<SDL_GameControllerButton>( i ) ) << i;
			}

			// Axis													   Axis									Make [-1,1]
			m_RightStickX  = SDL_GameControllerGetAxis( m_Controller, SDL_CONTROLLER_AXIS_RIGHTX       ) * GAMEPAD_AXIS_FACTOR;
			m_RightStickY  = SDL_GameControllerGetAxis( m_Controller, SDL_CONTROLLER_AXIS_RIGHTY       ) * GAMEPAD_AXIS_FACTOR;
			m_LeftStickX   = SDL_GameControllerGetAxis( m_Controller, SDL_CONTROLLER_AXIS_LEFTX        ) * GAMEPAD_AXIS_FACTOR;
			m_LeftStickY   = SDL_GameControllerGetAxis( m_Controller, SDL_CONTROLLER_AXIS_LEFTY        ) * GAMEPAD_AXIS_FACTOR;
			m_LeftTrigger  = SDL_GameControllerGetAxis( m_Controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT  ) * GAMEPAD_AXIS_FACTOR;
			m_RightTrigger = SDL_GameControllerGetAxis( m_Controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT ) * GAMEPAD_AXIS_FACTOR;

			if ( m_Connected == false ) {
				LogInput( string( SDL_GameControllerName( m_Controller ) ) + " connected", "GamepadState", LogSeverity::INFO_MSG );
				m_Connected = true;
			}
		} else {
			m_Connected = false;
		}
	} else {
		ZeroState();
	}
}

string GamepadState::GetName() const {
	return string( SDL_GameControllerName( m_Controller ) );
}

bool GamepadState::ButtonDown( SDL_GameControllerButton button ) const {
	// Check the bit corresponding to the specified button
	return ( m_ButtonsMask         & ( 1 << button ) ) != 0;
}

bool GamepadState::ButtonUp( SDL_GameControllerButton button ) const {
	// Check the bit corresponding to the specified button.
	// It shouldn't be set.
	return !( m_ButtonsMask     & ( 1 << button ) );
}

void GamepadState::ActivateStateTracking() {
	m_TrackGamepadState = true;
}

void GamepadState::DeactivateStateTracking() {
	m_TrackGamepadState = false;
	ZeroState();
}

float GamepadState::GetRightStickX() const {
	return m_RightStickX;
}

float GamepadState::GetRightStickY() const {
	return m_RightStickY;
}

float GamepadState::GetLeftStickX() const {
	return m_LeftStickX;
}

float GamepadState::GetLeftStickY() const {
	return m_LeftStickY;
}

float GamepadState::GetLeftTrigger() const {
	return m_LeftTrigger;
}

float GamepadState::GetRightTrigger() const {
	return m_RightTrigger;
}

void GamepadState::ZeroState() {
	m_RightStickX  = 0.0f;
	m_RightStickY  = 0.0f;
	m_LeftStickX   = 0.0f;
	m_LeftStickY   = 0.0f;
	m_RightTrigger = 0.0f;
	m_LeftTrigger  = 0.0f;
	m_ButtonsMask  = 0;
}

