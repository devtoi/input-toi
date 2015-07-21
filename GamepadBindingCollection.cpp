#include "GamepadBindingCollection.h"
#include "LogInput.h"
#include <cassert>
#include "KeyBindings.h"

GamepadBindingCollection::GamepadBindingCollection() {
}

GamepadBindingCollection::~GamepadBindingCollection() {
}

bool GamepadBindingCollection::AddMappingWithName( const rString& keyName, ActionIdentifier action, bool overwrite, bool clearConflicting,
												   rString* errorString ) {
	SDL_GameControllerButton button = SDL_GameControllerGetButtonFromString( keyName.c_str() );
	if ( button != SDL_CONTROLLER_BUTTON_INVALID ) {
		return AddMappingWithButton( button, action, overwrite, clearConflicting, errorString );
	} else {
		LogInput( "Failed to get scancode from name: " + keyName, "GamepadBindings", LogSeverity::WARNING_MSG );
		if ( errorString != nullptr ) {
			*errorString = "Failed to get scancode from name: " + keyName;
		}
		return false;
	}
}

bool GamepadBindingCollection::AddMappingWithButton( SDL_GameControllerButton button, ActionIdentifier action, bool overwrite,
													 bool clearConflicting, rString* errorString ) {
	FillTheVoid( action );
	auto buttonIt = m_ButtonToAction.find( button );
	// Warn about overwriting duplicate gamepad bindings
	if ( buttonIt != m_ButtonToAction.end() && !clearConflicting ) {
		LogInput( "Can't bind button: \"" + rString( SDL_GameControllerGetStringForButton( button ) ) + "\" to action " +
				  g_KeyBindings.GetDescription( action ) + " because it is already bound to action \"" +
				  g_KeyBindings.GetDescription( buttonIt->second ) + "\"",
				  "KeyBindings", LogSeverity::WARNING_MSG );
		if ( errorString != nullptr ) {
			*errorString = "Can't bind key: \"" + rString( SDL_GameControllerGetStringForButton( button ) ) + "\" to action " +
						   g_KeyBindings.GetDescription( action ) + " because it is already bound to action \"" +
						   g_KeyBindings.GetDescription( buttonIt->second ) + "\"";
		}
		return false;
	} else {
		if ( BindAction( action, button, overwrite ) ) {
			// Bound button
			LogInput( "Bound button \"" + rString( SDL_GameControllerGetStringForButton( button ) ) + "\" to action \"" +
					  g_KeyBindings.GetDescription( action ) + "\"",
					  "KeyBindings", LogSeverity::DEBUG_MSG );
			if ( errorString != nullptr ) {
				*errorString = "Bound button \"" + rString( SDL_GameControllerGetStringForButton( button ) ) + "\" to action \"" +
							   g_KeyBindings.GetDescription( action ) + "\"";
			}
			return true;
		} else {
			// Failed to bind button
			LogInput( "Can't bind button: \"" + rString( SDL_GameControllerGetStringForButton( button ) ) + "\" to action \"" +
					  g_KeyBindings.GetDescription( action ) + "\" because no free bind slots are avaliable",
					  "KeyBindings", LogSeverity::WARNING_MSG );
			if ( errorString != nullptr ) {
				*errorString = "Can't bind button: \"" + rString( SDL_GameControllerGetStringForButton( button ) ) + "\" to action \"" +
							   g_KeyBindings.GetDescription( action ) + "\" because no free bind slots are avaliable";
			}
			return false;
		}
	}
}

void GamepadBindingCollection::FillTheVoid( ActionIdentifier action ) {
	if ( static_cast<int>( action ) >= static_cast<int>( m_ActionToButton.size() ) ) {
		size_t prevSize = m_ActionToButton.size();
		m_ActionToButton.resize( static_cast<int>( action ) + 1 );
		std::fill( m_ActionToButton.begin() + prevSize, m_ActionToButton.end(), SDL_CONTROLLER_BUTTON_INVALID );
	}
}

bool GamepadBindingCollection::BindAction( ActionIdentifier action, SDL_GameControllerButton button, bool overwrite ) {
	FillTheVoid( action );
	auto freePrevious = [this, action, button]() {
		SDL_GameControllerButton prevButton = m_ActionToButton[static_cast<int>( action )];
		if ( prevButton != SDL_CONTROLLER_BUTTON_INVALID ) {
			auto it = m_ButtonToAction.find( prevButton );
			if ( it != m_ButtonToAction.end() ) {
				m_ButtonToAction.erase( it );
			}
		}
	};
	auto addBinding = [this, action, button, &freePrevious]() { 
		freePrevious();
		m_ActionToButton.at( static_cast<int>( action ) ) = button;
		m_ButtonToAction[button] = action;
	};

	if ( overwrite ) {
		addBinding();
		return true;
	} else {
		// No overwriting
		// Only bind unbound action
		if ( m_ActionToButton[static_cast<int>( action )] == SDL_CONTROLLER_BUTTON_INVALID ) {
			addBinding();
			return true;
		}
		// Failed to bind
		return false;
	}
}

SDL_GameControllerButton GamepadBindingCollection::GetButtonFromAction( ActionIdentifier action ) const {
	if ( static_cast<size_t>( static_cast<int>( action ) ) >= m_ActionToButton.size( ) ) {
		return SDL_CONTROLLER_BUTTON_INVALID;
	}
	return m_ActionToButton.at( static_cast<int>( action ) );
}
