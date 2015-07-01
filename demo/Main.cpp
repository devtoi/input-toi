#include "../Alloc.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <utility/Logger.h>
#include <input/Input.h>
#include <input/TextInput.h>

void RegisterLogger( );

int main( int argc, char* argv[] )
{
	RegisterLogger( );
	Logger::IWantItAll( );
	SDL_Init( SDL_INIT_EVERYTHING );

	SDL_CreateWindow( "Input test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 200, 200, SDL_WINDOW_SHOWN );

	g_TextInput.Initialize( );
	g_Input->Initialize( );

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
	}
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
		{ "KeyBindings",			"Input" },
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
