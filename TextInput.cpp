#include "TextInput.h"
#include <SDL2/SDL_clipboard.h>
#include "InputState.h"

TextInput& TextInput::GetInstance() {
	static TextInput textInput;

	return textInput;
}

void TextInput::Initialize() {
	// Default to not start entering text
	SDL_StopTextInput();
	m_TextInputEventCallbackHandle = g_InputState.RegisterEventInterest( std::bind( &TextInput::HandleEvents, this, std::placeholders::_1 ) );
}

void TextInput::Deinitialize() {
	g_InputState.UnregisterEventInterest( m_TextInputEventCallbackHandle );
}

bool TextInput::HandleEvents( const SDL_Event& event ) {
	if ( !m_InputString ) {
		return false;
	}

	if ( m_Cursor > m_InputString->size() ) {
		m_Cursor = static_cast<int>( m_InputString->size() );
	}

	switch ( event.type ) {
		case SDL_KEYDOWN:
			if ( event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE ) {
				if ( m_InputString->size() > 0 ) {
					m_InputString->erase( m_Cursor - 1, 1 );
					--m_Cursor;
				}
			}
			if ( event.key.keysym.scancode == SDL_SCANCODE_LEFT ) {
				if ( m_IsInputting ) {
					MoveCursor( -1 );
				}
			}
			if ( event.key.keysym.scancode == SDL_SCANCODE_RIGHT ) {
				if ( m_IsInputting ) {
					MoveCursor( 1 );
				}
			}
			// Paste
			if ( ( event.key.keysym.scancode == SDL_SCANCODE_V ) && SDL_GetModState() & KMOD_CTRL ) {
				size_t strSize = m_InputString->size();
				m_InputString->insert( m_Cursor, SDL_GetClipboardText() );
				MoveCursor( ( int )( m_InputString->size() - strSize ) );
			}
			// Copy
			if ( ( event.key.keysym.scancode == SDL_SCANCODE_C ) && SDL_GetModState() & KMOD_CTRL ) {
				SDL_SetClipboardText( m_InputString->c_str() );
			}
			// Cut
			if ( ( event.key.keysym.scancode == SDL_SCANCODE_X ) && SDL_GetModState() & KMOD_CTRL ) {
				SDL_SetClipboardText( m_InputString->c_str() );
				m_InputString->assign( "" );
				m_Cursor = 0;
			}
			return IsInputtableKey( event.key );
			break;
		case SDL_TEXTINPUT:
			if ( m_Cursor != -1 ) {
				m_InputString->insert( m_Cursor, event.text.text );
				++m_Cursor;
			}
			break;
		case SDL_TEXTEDITING:

			break;
	}
	return false;
}

void TextInput::StartInput( rString* text, int cursor, unsigned int id ) {
	m_InputString  = text;
	m_Cursor	   = cursor;
	m_CurrentOwner = id;
	g_InputState.DeactivateKeyboardStateTracking();
	SDL_StartTextInput();
	m_IsInputting = true;
}

const rString* TextInput::StopInput() {
	g_InputState.ActivateKeyboardStateTracking();

	SDL_StopTextInput();
	m_IsInputting  = false;
	m_Cursor	   = -1;
	m_CurrentOwner = -1;
	m_InputString  = nullptr;
	return GetString();
}

void TextInput::ResetString() {
	if ( m_InputString ) {
		m_InputString->assign( "" );
	}
}

bool TextInput::IsInputting( unsigned int id ) const {
	if ( id == m_CurrentOwner ) {
		return m_IsInputting;
	} else {
		return false;
	}
}

rString* TextInput::GetString() const {
	return m_InputString;
}

void TextInput::SetString( rString* input ) {
	m_InputString = input;
}

unsigned int TextInput::GetTextCursor() const {
	return m_Cursor;
}

void TextInput::SetTextCursor( int cursor ) {
	m_Cursor = cursor;
}

void TextInput::MoveCursor( int direction ) {
	if ( m_InputString ) {
		if ( ( m_Cursor + direction >= 0 ) && ( m_Cursor + direction <= m_InputString->length() ) ) {
			m_Cursor += direction;
		}
	}
}

bool TextInput::IsInputtableKey( const SDL_KeyboardEvent& keyboardEvent ) const {
	return keyboardEvent.keysym.sym >= 32 && keyboardEvent.keysym.sym < 132; // TODOIA Add more key exceptions
}

