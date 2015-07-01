#pragma once
#include <SDL2/SDL_events.h>
#include "Input.h"

#define g_TextInput TextInput::GetInstance()

class TextInput {
public:
	INPUT_API static TextInput& GetInstance( );

	INPUT_API void					Initialize	( );
	INPUT_API void					HandleEvents( const SDL_Event& event );

	// Starts inputting and resets the input string.
	INPUT_API void					StartInput	( rString* text, int cursor, unsigned int id );
	// Stops inputting and returns the inputted string.
	INPUT_API const rString*		StopInput	( );
	INPUT_API bool					IsInputting ( unsigned int id ) const;

	INPUT_API void					ResetString ();

	INPUT_API rString*				GetString( ) const;
	INPUT_API void					SetString( rString* input );
	
	INPUT_API const unsigned int	GetTextCursor( ) const;
	INPUT_API void					SetTextCursor( int cursor );
	
	INPUT_API void					MoveCursor( int direction );

private:
	//rString						m_InpurString = "";
	rString							m_Composition;
	bool							m_IsInputting = false;
	
	//const rString					m_EmptyString = "";
	//pMap<unsigned int, rString>		m_InputStrings;
	//pMap<unsigned int, unsigned int> m_TextCursors;
	
	//unsigned int					m_CurrentInputString;
	rString*						m_InputString = nullptr;
	int								m_Cursor = -1;
	unsigned int					m_CurrentOwner = -1;
};

