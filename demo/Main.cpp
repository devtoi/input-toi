#include "../Alloc.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <utility/Logger.h>
#include <utility/ConfigManager.h>
#include <input/Input.h>
#include <input/TextInput.h>
#include <input/KeyBindings.h>
#include <input/Gamepad.h>
#include <chrono>
#include <thread>

void RegisterLogger( );

int main( int argc, char* argv[] )
{
	RegisterLogger( );
	Logger::IWantItAll( );
	SDL_Init( SDL_INIT_EVERYTHING );

	SDL_CreateWindow( "Input test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 200, 200, SDL_WINDOW_SHOWN );

	g_TextInput.Initialize( );
	g_Input->Initialize( );

	ActionIdentifier testAction = g_KeyBindings.AddAction( "testaction", SDL_SCANCODE_W, "test desc", SDL_CONTROLLER_BUTTON_B );
	ActionIdentifier testAction2 = g_KeyBindings.AddAction( "testaction2", SDL_SCANCODE_S, "test desc", SDL_CONTROLLER_BUTTON_A  );

	g_KeyBindings.ReadConfig( );

	bool quit = false;
	while ( !quit )
	{
		SDL_Event event;
		g_Input->Update( );
		while ( SDL_PollEvent( &event ) > 0 )
		{
			g_TextInput.HandleEvents( event );
			g_Input->HandleEvent( event );
			switch ( event.type )
			{
			case SDL_KEYDOWN:
				if ( event.key.keysym.scancode == SDL_SCANCODE_ESCAPE )
				{
					quit = true;
				}
				break;
			}
		}

		while ( g_Input->GetEditablePressStack( ).size( ) > 0 )
		{
			std::cout << SDL_GetScancodeName( g_Input->GetEditablePressStack( ).back( ) ) << std::endl;
			g_Input->GetEditablePressStack( ).pop_back( );
		}
		if ( g_KeyBindings.ActionUpDown( testAction, INPUT_TYPE_GAMEPAD_FIRST ) )
		{
			std::cout << "action1 up down with gamepad" << std::endl;
		}
		if ( g_KeyBindings.ActionDownUp( testAction2, INPUT_TYPE_GAMEPAD_FIRST ) )
		{
			std::cout << "action2 down up with gamepad" << std::endl;
		}
		std::this_thread::sleep_for (std::chrono::milliseconds(17));
	}
	g_ConfigManager.SaveDirty( );
	SDL_Quit( );
	return 0;
}

void RegisterLogger( )
{
	struct
	{
		rString name;
		rString parent;
	} loggerRegisters[] =
	{
		// NAME						PARENT
		{ "Input", 					"" },
			{ "Gamepad", 					"Input" },
			{ "GamepadBindingCollection",	"Input" },
			{ "KeyBindings",				"Input" },
		{ "FileUtility",			"" },
		{ "Config",					"" },
		{ "CallbackConfig",			"" },
		{ "ConfigManager",			"" },
	};

	for ( auto& entry : loggerRegisters )
		Logger::RegisterType( entry.name, entry.parent );
	
	Logger::Initialize();

	Logger::RegisterInterestFromFile( );
}
