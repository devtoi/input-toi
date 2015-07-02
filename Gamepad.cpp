#include "Gamepad.h"
#include "LogInput.h"

// For making axises in the interval [-1,1]
#define GAMEPAD_AXIS_FACTOR 1 / 32768.0f

Gamepad::Gamepad(SDL_GameController* controller)
: m_Controller(controller) {
	
}

Gamepad::~Gamepad() {
	//SDL_GameControllerClose( m_controller );
	//delete m_controller;
}

void Gamepad::Update() {
	// Only update if controller is attached
	if ( SDL_GameControllerGetAttached( m_Controller ) == SDL_TRUE ) {
		// Reset mask to no buttons being down
		m_ButtonsMask = 0;
		// Loop through all SDL controller buttons
		for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; ++i) {
			// Set bit corresponding to the specific button
			m_ButtonsMask |=
				SDL_GameControllerGetButton( m_Controller, static_cast<SDL_GameControllerButton>( i ) ) << i;
		}

		// Axis													   Axis									Make [-1,1]
		m_RightStickX 	= SDL_GameControllerGetAxis( m_Controller, SDL_CONTROLLER_AXIS_RIGHTX 		) * GAMEPAD_AXIS_FACTOR;
		m_RightStickY 	= SDL_GameControllerGetAxis( m_Controller, SDL_CONTROLLER_AXIS_RIGHTY 		) * GAMEPAD_AXIS_FACTOR;
		m_LeftStickX 	= SDL_GameControllerGetAxis( m_Controller, SDL_CONTROLLER_AXIS_LEFTX 		) * GAMEPAD_AXIS_FACTOR;
		m_LeftStickY 	= SDL_GameControllerGetAxis( m_Controller, SDL_CONTROLLER_AXIS_LEFTY 		) * GAMEPAD_AXIS_FACTOR;
		m_LeftTrigger 	= SDL_GameControllerGetAxis( m_Controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT 	) * GAMEPAD_AXIS_FACTOR;
		m_RightTrigger 	= SDL_GameControllerGetAxis( m_Controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT ) * GAMEPAD_AXIS_FACTOR;

		if ( m_Connected == false ) {
			LogInput(string(SDL_GameControllerName( m_Controller )) + " connected", "Gamepad", LogSeverity::INFO_MSG);
			m_Connected = true;
		}
	} else {
		m_Connected = false;
	}
}

void Gamepad::SaveState() {
	// Store previous mask for updown downup functionality
	m_PrevButtonsMask = m_ButtonsMask;
}

string Gamepad::GetName() const {
	return string(SDL_GameControllerName( m_Controller ));
}

bool Gamepad::ButtonDown(SDL_GameControllerButton button) const {
	// Check the bit corresponding to the specified button
	return     (m_ButtonsMask         & (1 << button)) != 0;
}

bool Gamepad::ButtonUpDown(SDL_GameControllerButton button) const {
	return 	!(m_PrevButtonsMask	& (1 << button)) && // Previous up
			(m_ButtonsMask 		& (1 << button)); 	// Current down
}

bool Gamepad::ButtonDownUp(SDL_GameControllerButton button) const {
	return 	(m_PrevButtonsMask 	& (1 << button)) && // Previous down
			!(m_ButtonsMask 	& (1 << button)); 	// Current up
}

bool Gamepad::ButtonUp(SDL_GameControllerButton button) const {
	// Check the bit corresponding to the specified button.
	// It shouldn't be set.
	return 	!(m_ButtonsMask 	& (1 << button));
}

float Gamepad::RightStickX () const {
	return m_RightStickX;
}

float Gamepad::RightStickY () const {
	return m_RightStickY;
}

float Gamepad::LeftStickX () const {
	return m_LeftStickX;
}

float Gamepad::LeftStickY () const {
	return m_LeftStickY;
}

float Gamepad::LeftTrigger() const {
	return m_LeftTrigger;
}

float Gamepad::RightTrigger() const {
	return m_RightTrigger;
}
