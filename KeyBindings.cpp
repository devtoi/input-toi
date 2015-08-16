#include "KeyBindings.h"
#include <cassert>
#include <SDL2/SDL_keyboard.h>
#include <utility/ConfigManager.h>
#include "LogInput.h"
#include "InputContext.h"
#include "GamepadContext.h"
#include "BindContext.h"

KeyBindings& KeyBindings::GetInstance() {
	static KeyBindings keybindings;

	return keybindings;
}

KeyBindings::~KeyBindings() {
	for ( auto& context : m_BindContexts ) {
		if ( context ) {
			pDelete( context );
		}
	}
}

BindContextHandle KeyBindings::AllocateBindContext( const pString& name ) {
	BindContextHandle handle;
	for ( size_t i = 0; i < m_BindContexts.size(); ++i ) { // Reuse empty slot
		if ( m_BindContexts[i] == nullptr ) {
			handle = static_cast<BindContextHandle>( static_cast<int>( i ) );
			m_BindContexts[i] = pNew( BindContext, name );
			return handle;
		}
	}
	m_BindContexts.push_back( pNew( BindContext, name ) );
	return static_cast<BindContextHandle>( static_cast<int>( m_BindContexts.size() - 1 ) );
}

void KeyBindings::ReadConfig( const rString& configPath ) {
	pString			toRead = configPath == "" ? m_KeybindingsConfigPath : configPath;
	CallbackConfig* cfg	   = g_ConfigManager.GetConfig( toRead );

	for ( auto& context : m_BindContexts ) {
		if ( context ) {
			context->LoadFromConfig( *cfg, m_ActionDescriptions );
		}
	}
}

void KeyBindings::ClearActions() {
	m_ActionDescriptions.clear();
	for ( auto& context : m_BindContexts ) {
		if ( context ) {
			context->ClearActions();
		}
	}
}

void KeyBindings::ReloadConfig() {
	for ( auto& context : m_BindContexts ) {
		if ( context ) {
			context->ClearBindings();
		}
	}
	ReadConfig();
}

void KeyBindings::SaveConfig( const rString& configPath ) {
	rString			toSave = configPath == "" ? m_KeybindingsConfigPath : configPath;
	CallbackConfig* cfg	   = g_ConfigManager.GetConfig( toSave );

	for ( const auto& context : m_BindContexts ) {
		if ( context ) {
			context->SaveToConfig( *cfg );
		}
	}
	cfg->SaveFile();
}

bool KeyBindings::ActionUpDown( InputContext& input, BindContextHandle bindContextHandle, ActionIdentifier action, INPUT_TYPE inputType, bool ignorePause ) const {
	assert( static_cast<int>( action ) < m_ActionDescriptions.size() );
	const BindContext* context = GetBindContext( bindContextHandle );
	if ( inputType == INPUT_TYPE_KEYBOARD ) {
		return input.KeyUpDown( context->GetKeyBindCollection().GetPrimaryScancodeFromAction( action )/*, ignorePause*/ ) ||
			   input.KeyUpDown( context->GetKeyBindCollection().GetSecondaryScancodeFromAction( action )/*, ignorePause*/ );
	} else if ( inputType == INPUT_TYPE_ANY ) {
		if ( ActionUpDown( input, bindContextHandle, action, INPUT_TYPE_KEYBOARD, ignorePause ) ) {
			return true;
		}
		for ( int i = 0; i < INPUT_MAX_NR_OF_GAMEPADS; ++i ) {
			if ( ActionUpDown( input, bindContextHandle, action, static_cast<INPUT_TYPE>( i ), ignorePause ) ) {
				return true;
			}
		}
		return false;
	} else {
		assert( inputType >= 0 && static_cast<int>( inputType ) < INPUT_MAX_NR_OF_GAMEPADS );
		const GamepadContext& gamepad = input.GetGamepadContext( inputType );
		return gamepad.ButtonUpDown( context->GetGamepadBindCollection().GetButtonFromAction( action ) );
	}
}

bool KeyBindings::ActionDownUp( InputContext& input, BindContextHandle bindContextHandle, ActionIdentifier action, INPUT_TYPE inputType, bool ignorePause ) const {
	assert( static_cast<int>( action ) < m_ActionDescriptions.size() );
	const BindContext* context = GetBindContext( bindContextHandle );
	if ( inputType == INPUT_TYPE_KEYBOARD ) {
		return input.KeyDownUp( context->GetKeyBindCollection().GetPrimaryScancodeFromAction( action )/*, ignorePause*/ ) ||
			   input.KeyDownUp( context->GetKeyBindCollection().GetSecondaryScancodeFromAction( action )/*, ignorePause*/ );
	} else if ( inputType == INPUT_TYPE_ANY ) {
		if ( ActionDownUp( input, bindContextHandle, action, INPUT_TYPE_KEYBOARD, ignorePause ) ) {
			return true;
		}
		for ( int i = 0; i < INPUT_MAX_NR_OF_GAMEPADS; ++i ) {
			if ( ActionDownUp( input, bindContextHandle, action, static_cast<INPUT_TYPE>( i ), ignorePause ) ) {
				return true;
			}
		}
		return false;
	} else {
		assert( inputType >= 0 && static_cast<int>( inputType ) < INPUT_MAX_NR_OF_GAMEPADS );
		const GamepadContext& gamepad = input.GetGamepadContext( inputType );
		return gamepad.ButtonDownUp( context->GetGamepadBindCollection().GetButtonFromAction( action ) );
	}
}

bool KeyBindings::ActionUp( InputContext& input, BindContextHandle bindContextHandle, ActionIdentifier action, INPUT_TYPE inputType, bool ignorePause ) const {
	assert( static_cast<int>( action ) < m_ActionDescriptions.size() );
	const BindContext* context = GetBindContext( bindContextHandle );
	if ( inputType == INPUT_TYPE_KEYBOARD ) {
		return input.KeyUp( context->GetKeyBindCollection().GetPrimaryScancodeFromAction( action ) /*, ignorePause*/ ) ||
			   input.KeyUp( context->GetKeyBindCollection().GetSecondaryScancodeFromAction( action )	/*, ignorePause*/ );
	} else if ( inputType == INPUT_TYPE_ANY ) {
		if ( ActionUp( input, bindContextHandle, action, INPUT_TYPE_KEYBOARD, ignorePause ) ) {
			return true;
		}
		for ( int i = 0; i < INPUT_MAX_NR_OF_GAMEPADS; ++i ) {
			if ( ActionUp( input, bindContextHandle, action, static_cast<INPUT_TYPE>( i ), ignorePause ) ) {
				return true;
			}
		}
		return false;
	} else {
		assert( inputType >= 0 && static_cast<int>( inputType ) < INPUT_MAX_NR_OF_GAMEPADS );
		const GamepadContext& gamepad = input.GetGamepadContext( inputType );
		return gamepad.ButtonUp( context->GetGamepadBindCollection().GetButtonFromAction( action ) );
	}
}

bool KeyBindings::ActionDown( InputContext& input, BindContextHandle bindContextHandle, ActionIdentifier action, INPUT_TYPE inputType, bool ignorePause ) const {
	assert( static_cast<int>( action ) < m_ActionDescriptions.size() );
	const BindContext* context = GetBindContext( bindContextHandle );
	if ( inputType == INPUT_TYPE_KEYBOARD ) {
		// TODOJM: Implement ignore pause again if it is actually needed
		return input.KeyDown( context->GetKeyBindCollection().GetPrimaryScancodeFromAction( action )	/*, ignorePause*/ ) ||
			   input.KeyDown( context->GetKeyBindCollection().GetSecondaryScancodeFromAction( action ) /*, ignorePause*/ );
	} else if ( inputType == INPUT_TYPE_ANY ) {
		if ( ActionDown( input, bindContextHandle, action, INPUT_TYPE_KEYBOARD, ignorePause ) ) {
			return true;
		}
		for ( int i = 0; i < INPUT_MAX_NR_OF_GAMEPADS; ++i ) {
			if ( ActionDown( input, bindContextHandle, action, static_cast<INPUT_TYPE>( i ), ignorePause ) ) {
				return true;
			}
		}
		return false;
	} else {
		assert( inputType >= 0 && static_cast<int>( inputType ) < INPUT_MAX_NR_OF_GAMEPADS );
		const GamepadContext& gamepad = input.GetGamepadContext( inputType );
		return gamepad.ButtonDown( context->GetGamepadBindCollection().GetButtonFromAction( action ) );
	}
}

ActionIdentifier KeyBindings::CreateAction( BindContextHandle bindContextHandle, const pString& name, SDL_Scancode scancode, const pString& description, SDL_GameControllerButton defaultButton ) {
	BindContext* bindContext = GetBindContext( bindContextHandle );
	if ( bindContext ) {
		ActionIdentifier identifier = static_cast<ActionIdentifier>( static_cast<int>( m_ActionDescriptions.size() ) );

		m_ActionDescriptions.push_back( description );
		m_ActionNames.push_back( name );
		AddAction( identifier, bindContextHandle, scancode, defaultButton );
		return identifier;
	} else {
		LogInput( "Invalid bind context handle " + rToString( static_cast<int>( bindContextHandle ) ), "KeyBindings", LogSeverity::WARNING_MSG );
		return ActionIdentifier::invalid();
	}
}

void KeyBindings::AddAction( ActionIdentifier actionIdentifier, BindContextHandle bindContextHandle, SDL_Scancode scancode, SDL_GameControllerButton defaultButton ) {
	BindContext* bindContext = GetBindContext( bindContextHandle );
	if ( bindContext ) {
		const pString& name = m_ActionNames.at( static_cast<int>( actionIdentifier ) );
		bindContext->AddAction( actionIdentifier, name, scancode, defaultButton );
	} else {
		LogInput( "Invalid bind context handle " + rToString( static_cast<int>( bindContextHandle ) ), "KeyBindings", LogSeverity::WARNING_MSG );
	}
}

const rVector<rString>& KeyBindings::GetActionDescriptions() const {
	return m_ActionDescriptions;
}

const rString& KeyBindings::GetDescription( ActionIdentifier action ) const {
	return m_ActionDescriptions.at( static_cast<int>( action ) );
}

const rString& KeyBindings::GetConfigPath() const {
	return m_KeybindingsConfigPath;
}

BindContext* KeyBindings::GetBindContext( BindContextHandle bindContextHandle ) {
	return m_BindContexts.at( static_cast<int>( bindContextHandle ) );
}

const BindContext* KeyBindings::GetBindContext( BindContextHandle bindContextHandle ) const {
	return m_BindContexts.at( static_cast<int>( bindContextHandle ) );
}
