#pragma once
#include <SDL2/SDL_gamecontroller.h>
#include "InputLibraryDefine.h"
#include "Typedefs.h"

class GamepadBindingCollection {
public:
	INPUT_API GamepadBindingCollection();
	INPUT_API ~GamepadBindingCollection();

	INPUT_API bool AddMappingWithName( const rString& keyName, ActionIdentifier action, bool overwrite = false,
									   bool clearConflicting = false, rString* errorString = nullptr );

	INPUT_API bool AddMappingWithButton( SDL_GameControllerButton button, ActionIdentifier action, bool overwrite = false,
										 bool clearConflicting = false, rString* errorString = nullptr );

	INPUT_API bool BindAction( ActionIdentifier action, SDL_GameControllerButton button, bool overwrite );

	INPUT_API SDL_GameControllerButton GetButtonFromAction( ActionIdentifier action ) const;

private:
	void FillTheVoid( ActionIdentifier action );

	rMap<SDL_GameControllerButton, ActionIdentifier> m_ButtonToAction;
	rVector<SDL_GameControllerButton> m_ActionToButton;

	static const size_t mc_OverflowLimit = 200;
};
