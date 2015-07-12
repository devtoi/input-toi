#include "InputContext.h"
#include "InputState.h"
#include <iostream>

InputContext& InputContext::GetInstance() {
	static InputContext inputContext;

	return inputContext;
}

void InputContext::Initialize() {
	m_InputEventCallbackHandle = g_InputState.RegisterEventInterest( std::bind( &InputContext::HandleEvent, this, std::placeholders::_1 ) );

	m_GamepadContexts.resize( INPUT_MAX_NR_OF_GAMEPADS );
}

void InputContext::Deinitialize() {
	g_InputState.UnregisterEventInterest( m_InputEventCallbackHandle );
}

void InputContext::Update() {
	m_KeyboardPressStack.clear();
	m_KeyboardReleaseStack.clear();
	m_MouseSingleClickPressStack.clear();
	m_MouseSingleClickReleaseStack.clear();
	m_MouseDoubleClickPressStack.clear();
	m_MouseDoubleClickReleaseStack.clear();

	m_MousePosDeltaX = g_InputState.GetMouseDeltaX( m_MousePosLastX );
	m_MousePosDeltaY = g_InputState.GetMouseDeltaY( m_MousePosLastY );
	m_MousePosLastX	 = g_InputState.GetMouseDeltaAccumulationX();
	m_MousePosLastY	 = g_InputState.GetMouseDeltaAccumulationY();

	m_MouseScrollDeltaX	  = g_InputState.GetMouseScrollDeltaX( m_MouseScrollLastPosX );
	m_MouseScrollDeltaY	  = g_InputState.GetMouseScrollDeltaY( m_MouseScrollLastPosY );
	m_MouseScrollLastPosX = g_InputState.GetMouseScrollAccumulationX();
	m_MouseScrollLastPosY = g_InputState.GetMouseScrollAccumulationY();
}

bool InputContext::KeyUpDown( SDL_Scancode scanCode ) const {
	for ( auto& pressed : m_KeyboardPressStack ) {
		if ( pressed == scanCode ) {
			return true;
		}
	}
	return false;
}

bool InputContext::KeyUpDownConsume( SDL_Scancode scanCode, INPUT_STATE state ) {
	for ( pVector<SDL_Scancode>::iterator it = m_KeyboardPressStack.begin(); it != m_KeyboardPressStack.end(); ) {
		if ( *it == scanCode ) {
			it = m_KeyboardPressStack.erase( it );
			if ( state != INPUT_STATE_IGNORE ) {
				g_InputState.SetKeyState( scanCode, state );
			}
			return true;
		} else {
			++it;
		}
	}
	return false;
}

int InputContext::KeyUpDownConsumeAll( SDL_Scancode scanCode, INPUT_STATE state ) {
	int nrOfKeysConsumed = 0;

	for ( pVector<SDL_Scancode>::iterator it = m_KeyboardPressStack.begin(); it != m_KeyboardPressStack.end(); ) {
		if ( *it == scanCode ) {
			it = m_KeyboardPressStack.erase( it );
			++nrOfKeysConsumed;
		} else {
			++it;
		}
	}
	if ( ( nrOfKeysConsumed > 0 ) && ( state != INPUT_STATE_IGNORE ) ) {
		g_InputState.SetKeyState( scanCode, state );
	}
	return nrOfKeysConsumed;
}

bool InputContext::KeyDownUp( SDL_Scancode scanCode ) const {
	for ( auto& released : m_KeyboardReleaseStack ) {
		if ( released == scanCode ) {
			return true;
		}
	}
	return false;
}

bool InputContext::KeyDownUpConsume( SDL_Scancode scanCode, INPUT_STATE state ) {
	for ( pVector<SDL_Scancode>::iterator it = m_KeyboardReleaseStack.begin(); it != m_KeyboardReleaseStack.end(); ) {
		if ( *it == scanCode ) {
			it = m_KeyboardReleaseStack.erase( it );
			if ( state != INPUT_STATE_IGNORE ) {
				g_InputState.SetKeyState( scanCode, state );
			}
			return true;
		} else {
			++it;
		}
	}
	return false;
}

int InputContext::KeyDownUpConsumeAll( SDL_Scancode scanCode, INPUT_STATE state ) {
	int nrOfKeysConsumed = 0;

	for ( pVector<SDL_Scancode>::iterator it = m_KeyboardReleaseStack.begin(); it != m_KeyboardReleaseStack.end(); ) {
		if ( *it == scanCode ) {
			it = m_KeyboardReleaseStack.erase( it );
			++nrOfKeysConsumed;
		} else {
			++it;
		}
	}
	if ( ( nrOfKeysConsumed > 0 ) && ( state != INPUT_STATE_IGNORE ) ) {
		g_InputState.SetKeyState( scanCode, state );
	}
	return nrOfKeysConsumed;
}

bool InputContext::KeyDown( SDL_Scancode scanCode ) const {
	return g_InputState.IsKeyDown( scanCode );
}

bool InputContext::KeyUp( SDL_Scancode scanCode ) const {
	return g_InputState.IsKeyUp( scanCode );
}

const pVector<SDL_Scancode>& InputContext::GetKeyboardPressStack() const {
	return m_KeyboardPressStack;
}

const pVector<SDL_Scancode>& InputContext::GetKeyboardReleaseStack() const {
	return m_KeyboardReleaseStack;
}

bool InputContext::MouseButtonDown( MOUSE_BUTTON button ) const {
	return g_InputState.IsMouseButtonDown( button );
}

bool InputContext::MouseButtonUp( MOUSE_BUTTON button ) const {
	return g_InputState.IsMouseButtonUp( button );
}

bool InputContext::MouseButtonUpDown( MOUSE_BUTTON button ) const {
	for ( auto& pressed : m_MouseSingleClickPressStack ) {
		if ( pressed == button ) {
			return true;
		}
	}
	return false;
}

bool InputContext::MouseButtonUpDownConsume( MOUSE_BUTTON button, INPUT_STATE state ) {
	return ConsumeMouseButton( m_MouseSingleClickPressStack, button, state );
}

int InputContext::MouseButtonUpDownConsumeAll( MOUSE_BUTTON button, INPUT_STATE state ) {
	return ConsumeMouseButtonAll( m_MouseSingleClickPressStack, button, state );
}

bool InputContext::MouseButtonDownUp( MOUSE_BUTTON button ) const {
	for ( auto& pressed : m_MouseSingleClickReleaseStack ) {
		if ( pressed == button ) {
			return true;
		}
	}
	return false;
}

bool InputContext::MouseButtonDownUpConsume( MOUSE_BUTTON button, INPUT_STATE state ) {
	return ConsumeMouseButton( m_MouseSingleClickReleaseStack, button, state );
}

int InputContext::MouseButtonDownUpConsumeAll( MOUSE_BUTTON button, INPUT_STATE state ) {
	return ConsumeMouseButtonAll( m_MouseSingleClickReleaseStack, button, state );
}

bool InputContext::MouseButtonDoubleUpDown( MOUSE_BUTTON button ) const {
	for ( auto& pressed : m_MouseDoubleClickPressStack ) {
		if ( pressed == button ) {
			return true;
		}
	}
	return false;
}

bool InputContext::MouseButtonDoubleUpDownConsume( MOUSE_BUTTON button, INPUT_STATE state ) {
	return ConsumeMouseButton( m_MouseDoubleClickPressStack, button, state );
}

int InputContext::MouseButtonDoubleUpDownConsumeAll( MOUSE_BUTTON button, INPUT_STATE state ) {
	return ConsumeMouseButtonAll( m_MouseDoubleClickPressStack, button, state );
}

bool InputContext::MouseButtonDoubleDownUp( MOUSE_BUTTON button ) const {
	for ( auto& pressed : m_MouseDoubleClickReleaseStack ) {
		if ( pressed == button ) {
			return true;
		}
	}
	return false;
}

bool InputContext::MouseButtonDoubleDownUpConsume( MOUSE_BUTTON button, INPUT_STATE stateToSet ) {
	return ConsumeMouseButton( m_MouseDoubleClickReleaseStack, button, stateToSet );
}

int InputContext::MouseButtonDoubleDownUpConsumeAll( MOUSE_BUTTON button, INPUT_STATE stateToSet ) {
	return ConsumeMouseButtonAll( m_MouseDoubleClickReleaseStack, button, stateToSet );
}

int InputContext::GetMousePosX() const {
	return g_InputState.GetMouseState().PositionX;
}

int InputContext::GetMousePosY() const {
	return g_InputState.GetMouseState().PositionY;
}

int InputContext::GetMousePosDeltaX() const {
	return m_MousePosDeltaX;
}

int InputContext::GetMousePosDeltaY() const {
	return m_MousePosDeltaY;
}

int InputContext::GetMouseScrollDeltaX() const {
	return m_MouseScrollDeltaX;
}

int InputContext::GetMouseScrollDeltaY() const {
	return m_MouseScrollDeltaY;
}

const pVector<MOUSE_BUTTON>& InputContext::GetMouseSingleClickPressStack() const {
	return m_MouseSingleClickPressStack;
}

const pVector<MOUSE_BUTTON>& InputContext::GetMouseSingleClickReleaseStack() const {
	return m_MouseSingleClickReleaseStack;
}

const pVector<MOUSE_BUTTON>& InputContext::GetMouseDoubleClickPressStack() const {
	return m_MouseDoubleClickPressStack;
}

const pVector<MOUSE_BUTTON>& InputContext::GetMouseDoubleClickReleaseStack() const {
	return m_MouseDoubleClickReleaseStack;
}

const GamepadContext& InputContext::GetGamepadContext( unsigned int gamepadIndex ) const {
	return m_GamepadContexts.at( gamepadIndex );
}

bool InputContext::HandleEvent( const SDL_Event& event ) {
	switch ( event.type ) {
		case SDL_KEYUP: {
			if ( event.key.repeat == 0 ) {
				m_KeyboardReleaseStack.push_back( event.key.keysym.scancode );
			}
		} break;
		case SDL_KEYDOWN: {
			if ( event.key.repeat == 0 ) {
				m_KeyboardPressStack.push_back( event.key.keysym.scancode );
			}
		} break;
		case SDL_CONTROLLERBUTTONDOWN: {
			m_GamepadContexts.at( event.cbutton.which ).HandleEvent( event );
		} break;
		case SDL_CONTROLLERBUTTONUP: {
			m_GamepadContexts.at( event.cbutton.which ).HandleEvent( event );
		} break;
		case SDL_CONTROLLERDEVICEADDED: {
			m_GamepadContexts.at( event.cdevice.which ).Initialize( event.cdevice.which );
		} break;
		case SDL_MOUSEBUTTONDOWN: {
			if ( event.button.clicks == 1 ) {
				m_MouseSingleClickPressStack.push_back( static_cast<MOUSE_BUTTON>( event.button.button ) );
			}
			if ( event.button.clicks == 2 ) {
				m_MouseDoubleClickPressStack.push_back( static_cast<MOUSE_BUTTON>( event.button.button ) );
			}
		} break;
		case SDL_MOUSEBUTTONUP: {
			if ( event.button.clicks == 1 ) {
				m_MouseSingleClickReleaseStack.push_back( static_cast<MOUSE_BUTTON>( event.button.button ) );
			}
			if ( event.button.clicks == 2 ) {
				m_MouseDoubleClickReleaseStack.push_back( static_cast<MOUSE_BUTTON>( event.button.button ) );
			}
		} break;
	}

	return false;
}

bool InputContext::ConsumeMouseButton( pVector<MOUSE_BUTTON>& stack, MOUSE_BUTTON button, INPUT_STATE stateToSet ) {
	for ( pVector<MOUSE_BUTTON>::iterator it = stack.begin(); it != stack.end(); ) {
		if ( *it == button ) {
			it = stack.erase( it );
			if ( stateToSet != INPUT_STATE_IGNORE ) {
				g_InputState.SetMouseButtonState( button, stateToSet );
			}
			return true;
		} else {
			++it;
		}
	}
	return false;
}

int InputContext::ConsumeMouseButtonAll( pVector<MOUSE_BUTTON>& stack, MOUSE_BUTTON button, INPUT_STATE stateToSet ) {
	int nrOfConsumedButtons = 0;

	for ( pVector<MOUSE_BUTTON>::iterator it = stack.begin(); it != stack.end(); ) {
		if ( *it == button ) {
			it = stack.erase( it );
			++nrOfConsumedButtons;
		} else {
			++it;
		}
	}
	if ( stateToSet != INPUT_STATE_IGNORE ) {
		g_InputState.SetMouseButtonState( button, stateToSet );
	}
	return nrOfConsumedButtons;
}

