#include "BindContext.h"
#include <SDL2/SDL_keyboard.h>
#include <utility/Config.h>
#include "LogInput.h"

BindContext::BindContext( const pString& name )
	: m_Name( name ) {
}

void BindContext::LoadFromConfig( Config& cfg, const pVector<pString> descriptions ) {
	for ( auto& title : m_ActionTitleToAction ) {
		pString keyName = cfg.GetString( m_Name + ".primary." + title.first, SDL_GetScancodeName( title.second.DefaultScancode ),
			descriptions.at( static_cast<int>( title.second.Action ) ) );
		if ( keyName != "" ) {
			SDL_Scancode scanCode = SDL_GetScancodeFromName( keyName.c_str() );
			if ( scanCode == SDL_SCANCODE_UNKNOWN ) {
				LogInput( "Failed to interpret " + keyName + " as a scancode", "KeyBindings", LogSeverity::WARNING_MSG );
			} else {
				m_KeyBindingCollection.AddMappingWithScancode( scanCode, title.second.Action );
			}
		}
	}
	for ( auto& title : m_ActionTitleToAction ) {
		pString keyName = cfg.GetString( m_Name + ".secondary." + title.first, "", descriptions.at( static_cast<int>( title.second.Action ) ) );
		if ( keyName != "" ) {
			SDL_Scancode scanCode = SDL_GetScancodeFromName( keyName.c_str() );
			if ( scanCode == SDL_SCANCODE_UNKNOWN ) {
				LogInput( "Failed to interpret " + keyName + " as a scancode", "KeyBindings", LogSeverity::WARNING_MSG );
			} else {
				m_KeyBindingCollection.AddMappingWithScancode( scanCode, title.second.Action );
			}
		}
	}
	for ( auto& title : m_ActionTitleToAction ) {
		pString buttonName =
			cfg.GetString( m_Name + ".gamepad." + title.first, title.second.DefaultButton != SDL_CONTROLLER_BUTTON_INVALID ? SDL_GameControllerGetStringForButton(
					title.second.DefaultButton ) : "",
				descriptions.at( static_cast<int>( title.second.Action ) ) );
		if ( buttonName != "" ) {
			SDL_GameControllerButton button = SDL_GameControllerGetButtonFromString( buttonName.c_str() );
			if ( button == SDL_CONTROLLER_BUTTON_INVALID ) {
				LogInput( "Failed to interpret " + buttonName + " as a button", "KeyBindings", LogSeverity::WARNING_MSG );
			} else {
				m_GamepadBindingCollection.AddMappingWithButton( button, title.second.Action );
			}
		}
	}
}

void BindContext::SaveToConfig( Config& cfg ) const {
	for ( auto& keybinding : m_ActionTitleToAction ) {
		SDL_Scancode primary = m_KeyBindingCollection.GetPrimaryScancodeFromAction( keybinding.second.Action );
		cfg.SetString( m_Name + ".primary." + keybinding.first, SDL_GetScancodeName( primary ) );
	}
	for ( auto& keybinding : m_ActionTitleToAction ) {
		SDL_Scancode secondary = m_KeyBindingCollection.GetSecondaryScancodeFromAction( keybinding.second.Action );
		cfg.SetString( m_Name + ".secondary." + keybinding.first, SDL_GetScancodeName( secondary ) );
	}
	for ( auto& buttonBinding : m_ActionTitleToAction ) {
		SDL_GameControllerButton button = m_GamepadBindingCollection.GetButtonFromAction( buttonBinding.second.Action );
		cfg.SetString( m_Name + ".gamepad." + buttonBinding.first, SDL_GameControllerGetStringForButton( button ) );
	}
}

void BindContext::ClearBindings() {
	m_KeyBindingCollection = KeyBindingCollection();
	m_GamepadBindingCollection = GamepadBindingCollection();
}

void BindContext::ClearActions() {
	ClearBindings();
	m_ActionTitleToAction.clear();
}

void BindContext::AddAction( ActionIdentifier actionIdentifier, const pString& name, SDL_Scancode defaultScancode, SDL_GameControllerButton defaultButton ) {
	m_ActionTitleToAction[name] = ActionTitleMapping {
		actionIdentifier, defaultScancode, defaultButton
	};
}

void BindContext::GetDefaultKeyBindings( KeyBindingCollection& collection ) const {
	collection = KeyBindingCollection();

	for ( auto& title : m_ActionTitleToAction ) {
		collection.AddMappingWithScancode( title.second.DefaultScancode, title.second.Action );
	}
}

void BindContext::GetDefaultGamepadBindings( GamepadBindingCollection& collection ) const {
	collection = GamepadBindingCollection();

	for ( auto& title : m_ActionTitleToAction ) {
		collection.AddMappingWithButton( title.second.DefaultButton, title.second.Action );
	}
}

const rMap<rString, BindContext::ActionTitleMapping>& BindContext::GetActionTitleToAction() const {
	return m_ActionTitleToAction;
}

const KeyBindingCollection& BindContext::GetKeyBindCollection( ) const {
	return m_KeyBindingCollection;
}

KeyBindingCollection& BindContext::GetEditableKeyBindCollection( ) {
	return m_KeyBindingCollection;
}

void BindContext::SetKeyBindingCollection( const KeyBindingCollection& collection ) {
	m_KeyBindingCollection = collection;
}

const GamepadBindingCollection& BindContext::GetGamepadBindCollection( ) const {
	return m_GamepadBindingCollection;
}

GamepadBindingCollection& BindContext::GetEditableGamepadBindCollection( ) {
	return m_GamepadBindingCollection;
}

void BindContext::SetGamepadBindingCollection( const GamepadBindingCollection& collection ) {
	m_GamepadBindingCollection = collection;
}

