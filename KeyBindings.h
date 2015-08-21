#pragma once

#include <map>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_gamecontroller.h>
#include "InputLibraryDefine.h"
#include "Types.h"

class InputContext;
class BindContext;

#define g_KeyBindings KeyBindings::GetInstance()

class KeyBindings {
public:

	KeyBindings& operator = ( const KeyBindings & rhs ) = delete;
	KeyBindings( const KeyBindings & rhs ) = delete;

	INPUT_API static KeyBindings& GetInstance ();

	INPUT_API BindContextHandle AllocateBindContext( const pString& name );
	INPUT_API void				  ClearActions ();
	INPUT_API void				  ReadConfig ( const rString& configPath = "" );
	INPUT_API void				  ReloadConfig ();
	INPUT_API void				  SaveConfig ( const rString& configPath = "" );

	INPUT_API ActionIdentifier CreateAction( BindContextHandle bindContextHandle, const pString &name, SDL_Scancode scancode,
		const pString &description, SDL_GameControllerButton = SDL_CONTROLLER_BUTTON_INVALID );
	INPUT_API void AddAction( ActionIdentifier actionIdentifier, BindContextHandle bindContextHandle, SDL_Scancode scancode, SDL_GameControllerButton = SDL_CONTROLLER_BUTTON_INVALID );

	INPUT_API bool ActionUpDown			( InputContext& input, BindContextHandle bindContextHandle, ActionIdentifier action, INPUT_TYPE inputType = INPUT_TYPE_KEYBOARD, bool ignorePause = false ) const;
	INPUT_API bool ActionDownUp			( InputContext& input, BindContextHandle bindContextHandle, ActionIdentifier action, INPUT_TYPE inputType = INPUT_TYPE_KEYBOARD, bool ignorePause = false ) const;
	INPUT_API bool ActionUpDownConsume	( InputContext& input, BindContextHandle bindContextHandle, ActionIdentifier action, INPUT_TYPE inputType = INPUT_TYPE_KEYBOARD, bool ignorePause = false ) const;
	INPUT_API bool ActionDownUpConsume	( InputContext& input, BindContextHandle bindContextHandle, ActionIdentifier action, INPUT_TYPE inputType = INPUT_TYPE_KEYBOARD, bool ignorePause = false ) const;
	INPUT_API bool ActionUp				( InputContext& input, BindContextHandle bindContextHandle, ActionIdentifier action, INPUT_TYPE inputType = INPUT_TYPE_KEYBOARD, bool ignorePause = false ) const;
	INPUT_API bool ActionDown			( InputContext& input, BindContextHandle bindContextHandle, ActionIdentifier action, INPUT_TYPE inputType = INPUT_TYPE_KEYBOARD, bool ignorePause = false ) const;

	INPUT_API const rVector<rString>& GetActionDescriptions   () const;

	INPUT_API const rString& GetDescription ( ActionIdentifier action ) const;
	INPUT_API const rString& GetConfigPath () const;
	INPUT_API BindContext* GetBindContext( BindContextHandle bindContextHandle );
	INPUT_API const BindContext* GetBindContext( BindContextHandle bindContextHandle ) const;

private:
	// No external instancing allowed
	KeyBindings(){ };
	~KeyBindings();


	const pString m_KeybindingsConfigPath = "keybindings.cfg";

	pVector<rString> m_ActionDescriptions;
	pVector<rString> m_ActionNames;

	pVector<BindContext*> m_BindContexts;
};

