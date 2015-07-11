#pragma once

#include <map>
#include <SDL2/SDL_scancode.h>
#include "InputLibraryDefine.h"
#include "KeyBindingCollection.h"
#include "GamepadBindingCollection.h"
#include "Types.h"

class InputContext;

#define g_KeyBindings KeyBindings::GetInstance()

class KeyBindings {
public:
	struct ActionTitleMapping {
		ActionIdentifier		 Action;
		SDL_Scancode			 DefaultScancode;
		SDL_GameControllerButton DefaultButton;
	};

	INPUT_API static KeyBindings& GetInstance ();
	INPUT_API void				  ClearActions ();
	INPUT_API void				  ReadConfig ( const rString& configPath = "" );
	INPUT_API void				  ReloadConfig ();
	INPUT_API void				  SaveConfig ( const rString& configPath = "" );

	INPUT_API ActionIdentifier AddAction( const rString &name, SDL_Scancode scancode,
		const rString &description, SDL_GameControllerButton = SDL_CONTROLLER_BUTTON_INVALID );

	INPUT_API bool ActionUpDown ( InputContext& input, ActionIdentifier action, INPUT_TYPE inputType = INPUT_TYPE_KEYBOARD, bool ignorePause = false ) const;
	INPUT_API bool ActionDownUp ( InputContext& input, ActionIdentifier action, INPUT_TYPE inputType = INPUT_TYPE_KEYBOARD, bool ignorePause = false ) const;
	INPUT_API bool ActionUp     ( InputContext& input, ActionIdentifier action, INPUT_TYPE inputType = INPUT_TYPE_KEYBOARD, bool ignorePause = false ) const;
	INPUT_API bool ActionDown   ( InputContext& input, ActionIdentifier action, INPUT_TYPE inputType = INPUT_TYPE_KEYBOARD, bool ignorePause = false ) const;

	INPUT_API void GetDefault ( KeyBindingCollection& collection ) const;

	INPUT_API const rVector<rString>&				   GetActionDescriptions   () const;
	INPUT_API const rMap<rString, ActionTitleMapping>& GetActionTitleToAction  () const;

	INPUT_API const KeyBindingCollection& GetKeyBindCollection    () const;
	INPUT_API KeyBindingCollection&		  GetEditableKeyBindCollection ();
	INPUT_API void						  SetKeyBindingCollection ( const KeyBindingCollection& collection );

	// Returns true if action button was consumed from the press stack
	//INPUT_API bool ConsumeFromPressStack ( Input& input, ActionIdentifier action );

	INPUT_API const rString& GetDescription ( ActionIdentifier action ) const;
	INPUT_API const rString& GetConfigPath () const;

private:
	// No external instancing allowed
	KeyBindings();
	KeyBindings( const KeyBindings & rhs );
	~KeyBindings() { }

	KeyBindings& operator = ( const KeyBindings & rhs );

	const rString m_KeybindingsConfigPath = "keybindings.cfg";

	rMap<rString, ActionTitleMapping> m_ActionTitleToAction;
	rVector<rString> m_ActionDescriptions;

	KeyBindingCollection	 m_KeyBindingCollection;
	GamepadBindingCollection m_GamepadBindingCollection;
};

