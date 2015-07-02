#include "KeyBindings.h"
#include <cassert>
#include <SDL2/SDL_keyboard.h>
#include <utility/ConfigManager.h>
#include "Input.h"
#include "LogInput.h"
#include "Gamepad.h"

KeyBindings& KeyBindings::GetInstance() {
	static KeyBindings keybindings;
	return keybindings;
}

KeyBindings::KeyBindings() {
}

void KeyBindings::ReadConfig( const rString& configPath ) {
	rString toRead = configPath == "" ? m_KeybindingsConfigPath : configPath;
	CallbackConfig* cfg = g_ConfigManager.GetConfig( toRead );
	for ( auto& title : m_ActionTitleToAction ) {
		rString keyName = cfg->GetString( "primary." + title.first, SDL_GetScancodeName( title.second.DefaultScancode ),
										  m_ActionDescriptions.at( static_cast<int>( title.second.Action ) ) );
		if ( keyName != "" ) {
			SDL_Scancode scanCode = SDL_GetScancodeFromName( keyName.c_str() );
			if ( scanCode == SDL_SCANCODE_UNKNOWN )
				LogInput( "Failed to interpret " + keyName + " as a scancode", "KeyBindings", LogSeverity::WARNING_MSG );
			else
				m_KeyBindingCollection.AddMappingWithScancode( scanCode, title.second.Action );
		}
	}
	for ( auto& title : m_ActionTitleToAction ) {
		rString keyName = cfg->GetString( "secondary." + title.first, "", m_ActionDescriptions.at( static_cast<int>( title.second.Action ) ) );
		if ( keyName != "" ) {
			SDL_Scancode scanCode = SDL_GetScancodeFromName( keyName.c_str() );
			if ( scanCode == SDL_SCANCODE_UNKNOWN )
				LogInput( "Failed to interpret " + keyName + " as a scancode", "KeyBindings", LogSeverity::WARNING_MSG );
			else
				m_KeyBindingCollection.AddMappingWithScancode( scanCode, title.second.Action );
		}
	}
	for ( auto& title : m_ActionTitleToAction ) {
		rString buttonName = cfg->GetString( "gamepad." + title.first, SDL_GameControllerGetStringForButton( title.second.DefaultButton ),
											 m_ActionDescriptions.at( static_cast<int>( title.second.Action ) ) );
		if ( buttonName != "" ) {
			SDL_GameControllerButton button = SDL_GameControllerGetButtonFromString( buttonName.c_str() );
			if ( button == SDL_CONTROLLER_BUTTON_INVALID )
				LogInput( "Failed to interpret " + buttonName + " as a button", "KeyBindings", LogSeverity::WARNING_MSG );
			else
				m_GamepadBindingCollection.AddMappingWithButton( button, title.second.Action );
		}
	}
}

void KeyBindings::ClearActions() {
	m_ActionDescriptions.clear();
	m_ActionTitleToAction.clear();
	m_KeyBindingCollection = KeyBindingCollection();
	m_GamepadBindingCollection = GamepadBindingCollection();
}

void KeyBindings::ReloadConfig() {
	m_KeyBindingCollection = KeyBindingCollection();
	ReadConfig();
}

void KeyBindings::SaveConfig( const rString& configPath ) {
	rString toSave = configPath == "" ? m_KeybindingsConfigPath : configPath;
	CallbackConfig* cfg = g_ConfigManager.GetConfig( toSave );
	for ( auto& keybinding : m_ActionTitleToAction ) {
		SDL_Scancode primary = m_KeyBindingCollection.GetPrimaryScancodeFromAction( keybinding.second.Action );
		cfg->SetString( "primary." + keybinding.first, SDL_GetScancodeName( primary ) );
	}
	for ( auto& keybinding : m_ActionTitleToAction ) {
		SDL_Scancode secondary = m_KeyBindingCollection.GetSecondaryScancodeFromAction( keybinding.second.Action );
		cfg->SetString( "secondary." + keybinding.first, SDL_GetScancodeName( secondary ) );
	}
	for ( auto& buttonBinding : m_ActionTitleToAction ) {
		SDL_GameControllerButton button = m_GamepadBindingCollection.GetButtonFromAction( buttonBinding.second.Action );
		cfg->SetString( "gamepad." + buttonBinding.first, SDL_GameControllerGetStringForButton( button ) );
	}
	cfg->SaveFile();
}

void KeyBindings::GetDefault( KeyBindingCollection& collection ) const {
	collection = KeyBindingCollection();

	for ( auto& title : m_ActionTitleToAction ) {
		collection.AddMappingWithScancode( title.second.DefaultScancode, title.second.Action );
	}
}

bool KeyBindings::ActionUpDown( ActionIdentifier action, INPUT_TYPE inputType, bool ignorePause ) const {
	assert( static_cast<int>( action ) < m_ActionDescriptions.size() );
	if ( inputType == INPUT_TYPE_KEYBOARD ) {
		return g_Input->KeyUpDown( m_KeyBindingCollection.GetPrimaryScancodeFromAction( action ), ignorePause ) ||
			   g_Input->KeyUpDown( m_KeyBindingCollection.GetSecondaryScancodeFromAction( action ), ignorePause );
	} else if ( inputType == INPUT_TYPE_ANY ){
		assert( false ); // TODOJM: Implement checking keyboard and all gamepads
		return false;
	} else {
		assert( inputType >= 0 && static_cast<int>( inputType ) < INPUT_MAX_NR_OF_GAMEPADS );
		Gamepad* gamepad = g_Input->GetGamepad( inputType );
		if ( gamepad == nullptr ) {
			return false;
		} else {
			return gamepad->ButtonUpDown( m_GamepadBindingCollection.GetButtonFromAction( action ) );
		}
	}
}

bool KeyBindings::ActionDownUp( ActionIdentifier action, INPUT_TYPE inputType, bool ignorePause ) const {
	assert( static_cast<int>( action ) < m_ActionDescriptions.size() );
	if ( inputType == INPUT_TYPE_KEYBOARD ) {
		return g_Input->KeyDownUp( m_KeyBindingCollection.GetPrimaryScancodeFromAction( action ), ignorePause ) ||
			   g_Input->KeyDownUp( m_KeyBindingCollection.GetSecondaryScancodeFromAction( action ), ignorePause );
	} else if ( inputType == INPUT_TYPE_ANY ){
		assert( false ); // TODOJM: Implement checking keyboard and all gamepads
		return false;
	} else {
		assert( inputType >= 0 && static_cast<int>( inputType ) < INPUT_MAX_NR_OF_GAMEPADS );
		Gamepad* gamepad = g_Input->GetGamepad( inputType );
		if ( gamepad == nullptr ) {
			return false;
		} else {
			return gamepad->ButtonDownUp( m_GamepadBindingCollection.GetButtonFromAction( action ) );
		}
	}
}

bool KeyBindings::ActionUp( ActionIdentifier action, INPUT_TYPE inputType, bool ignorePause ) const {
	assert( static_cast<int>( action ) < m_ActionDescriptions.size() );
	if ( inputType == INPUT_TYPE_KEYBOARD ) {
		return g_Input->KeyUp( m_KeyBindingCollection.GetPrimaryScancodeFromAction( action ), ignorePause ) ||
			   g_Input->KeyUp( m_KeyBindingCollection.GetSecondaryScancodeFromAction( action ), ignorePause );
	} else if ( inputType == INPUT_TYPE_ANY ){
		assert( false ); // TODOJM: Implement checking keyboard and all gamepads
		return false;
	} else {
		assert( inputType >= 0 && static_cast<int>( inputType ) < INPUT_MAX_NR_OF_GAMEPADS );
		Gamepad* gamepad = g_Input->GetGamepad( inputType );
		if ( gamepad == nullptr ) {
			return false;
		} else {
			return gamepad->ButtonUp( m_GamepadBindingCollection.GetButtonFromAction( action ) );
		}
	}
}

bool KeyBindings::ActionDown( ActionIdentifier action, INPUT_TYPE inputType, bool ignorePause ) const {
	assert( static_cast<int>( action ) < m_ActionDescriptions.size() );
	if ( inputType == INPUT_TYPE_KEYBOARD ) {
		return g_Input->KeyDown( m_KeyBindingCollection.GetPrimaryScancodeFromAction( action ), ignorePause ) ||
			   g_Input->KeyDown( m_KeyBindingCollection.GetSecondaryScancodeFromAction( action ), ignorePause );
	} else if ( inputType == INPUT_TYPE_ANY ){
		assert( false ); // TODOJM: Implement checking keyboard and all gamepads
		return false;
	} else {
		assert( inputType >= 0 && static_cast<int>( inputType ) < INPUT_MAX_NR_OF_GAMEPADS );
		Gamepad* gamepad = g_Input->GetGamepad( inputType );
		if ( gamepad == nullptr ) {
			return false;
		} else {
			return gamepad->ButtonDown( m_GamepadBindingCollection.GetButtonFromAction( action ) );
		}
	}
}

ActionIdentifier KeyBindings::AddAction( const rString& name, SDL_Scancode scancode, const rString& description, SDL_GameControllerButton defaultButton ) {
	ActionIdentifier identifier = static_cast<ActionIdentifier>( static_cast<int>( m_ActionDescriptions.size() ) );
	m_ActionDescriptions.push_back( description );
	m_ActionTitleToAction[name] = ActionTitleMapping{ identifier, scancode, defaultButton };
	return identifier;
}

const rMap<rString, KeyBindings::ActionTitleMapping>& KeyBindings::GetActionTitleToAction() const {
	return m_ActionTitleToAction;
}

const rVector<rString>& KeyBindings::GetActionDescriptions() const {
	return m_ActionDescriptions;
}

const KeyBindingCollection& KeyBindings::GetKeyBindCollection() const {
	return m_KeyBindingCollection;
}

KeyBindingCollection& KeyBindings::GetEditableKeyBindCollection() {
	return m_KeyBindingCollection;
}

void KeyBindings::SetKeyBindingCollection( const KeyBindingCollection& collection ) {
	m_KeyBindingCollection = collection;
}

bool KeyBindings::ConsumeFromPressStack( ActionIdentifier action ) {
	rVector<SDL_Scancode>* pressStack = &g_Input->GetEditablePressStack();

	if ( pressStack->size() > 0 ) {
		SDL_Scancode primaryGUIAbortScanCode = m_KeyBindingCollection.GetPrimaryScancodeFromAction( action );
		SDL_Scancode secondaryGUIAbortScaneCode = m_KeyBindingCollection.GetSecondaryScancodeFromAction( action );
		for ( rVector<SDL_Scancode>::iterator it = pressStack->begin(); it != pressStack->end(); ++it ) {
			if ( *it == primaryGUIAbortScanCode || *it == secondaryGUIAbortScaneCode ) {
				pressStack->erase( it );
				return true;
			}
		}
	}
	return false;
}

const rString& KeyBindings::GetDescription( ActionIdentifier action ) const {
	return m_ActionDescriptions.at( static_cast<int>( action ) );
}

const rString& KeyBindings::GetConfigPath() const {
	return m_KeybindingsConfigPath;
}
