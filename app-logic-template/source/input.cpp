///////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: input.cpp
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "..\header\input.h"


//////////////
// INCLUDES //
//////////////
#include <winuser.h>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////


Input::Input()
{
	constexpr int KEY_NUM = 256;
	m_key_states_now.resize(KEY_NUM);
	m_key_states_last.resize(KEY_NUM);
}


// https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getkeyboardstate
auto Input::Update() -> BOOL
{
	m_key_states_last.swap(m_key_states_now);
	return GetKeyboardState(m_key_states_now.data());
}


auto Input::IsKeyPressed(int key) -> bool
{
	constexpr uint16_t ACTIVE{ 0x8000 };
	uint16_t key_state = GetAsyncKeyState(key);
	return (key_state & ACTIVE) != 0;
}


auto Input::WasKeyPressed(int key) const -> bool
{
	constexpr uint8_t ACTIVE{ 0x80 };
	return (m_key_states_now[key] & ACTIVE) == 0 && (m_key_states_last[key] & ACTIVE) != 0;
}
