#pragma once
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_gamecontroller.h>
#include "Types.h"
#include "KeyBindingCollection.h"
#include "GamepadBindingCollection.h"

class Config;

class BindContext {
public:
	struct ActionTitleMapping;

	INPUT_API BindContext( const pString& name );

	INPUT_API void LoadFromConfig( Config& cfg, const pVector<pString> descriptions );
	INPUT_API void SaveToConfig( Config& cfg ) const;
	INPUT_API void ClearBindings();
	INPUT_API void ClearActions();
	INPUT_API void AddAction( ActionIdentifier actionIdentifier, const pString &name, SDL_Scancode scancode, SDL_GameControllerButton = SDL_CONTROLLER_BUTTON_INVALID );

	INPUT_API void									   GetDefaultKeyBindings ( KeyBindingCollection& collection ) const;
	INPUT_API void									   GetDefaultGamepadBindings ( GamepadBindingCollection& collection ) const;
	INPUT_API const rMap<rString, ActionTitleMapping>& GetActionTitleToAction  () const;
	INPUT_API const KeyBindingCollection&	  GetKeyBindCollection () const;
	INPUT_API KeyBindingCollection&			  GetEditableKeyBindCollection ();
	INPUT_API void							  SetKeyBindingCollection ( const KeyBindingCollection& collection );
	INPUT_API const GamepadBindingCollection& GetGamepadBindCollection () const;
	INPUT_API GamepadBindingCollection&		  GetEditableGamepadBindCollection ();
	INPUT_API void							  SetGamepadBindingCollection ( const GamepadBindingCollection& collection );

	struct ActionTitleMapping {
		ActionIdentifier		 Action;
		SDL_Scancode			 DefaultScancode;
		SDL_GameControllerButton DefaultButton;
	};

private:
	pString							  m_Name;
	pMap<pString, ActionTitleMapping> m_ActionTitleToAction;
	KeyBindingCollection			  m_KeyBindingCollection;
	GamepadBindingCollection		  m_GamepadBindingCollection;
};
