///////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: settings.cpp
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "..\header\settings.h"


//////////////
// INCLUDES //
//////////////
#include <algorithm>
#include <fstream>
#include <iostream>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////


Settings::Settings() : m_setting_list()
{
	// https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
	m_setting_list.fill(false);
	std::generate(m_keybindings.begin(), m_keybindings.end(), [n = uint16_t(0)]() mutable { return n++; });
}


auto Settings::LoadSettings(const std::string& filename) -> bool
{
	std::fstream in(filename, std::ios::in);
	bool res = in.is_open();
	if (res)
	{
		in >> m_graphic_settings;
		in.close();
	}

	m_setting_list[int(settings::RESOLUTION)] = false;
	return res;
}


void Settings::SaveSettings(const std::string& path) const
{
	std::fstream out(path + std::string("settings.txt"), std::ios::out);
	if (out.is_open())
	{
		out << m_graphic_settings;
		out.close();
	}
}


auto Settings::GetWindowWidth() const -> uint16_t
{
	return m_graphic_settings.window_width;
}


auto Settings::GetWindowHeight() const -> uint16_t
{
	return m_graphic_settings.window_height;
}


auto Settings::GetGraphicSettings() const -> const graphics::GraphicSettings&
{
	return m_graphic_settings;
}


void Settings::SetResolution(uint16_t window_width, uint16_t window_height)
{
	m_graphic_settings.window_width = window_width;
	m_graphic_settings.window_height = window_height;
	m_setting_list[int(settings::RESOLUTION)] = true;
}


void Settings::SetFullscreen(const BOOL fullscreen)
{
	m_graphic_settings.fullscreen = fullscreen;
	m_setting_list[int(settings::RESOLUTION)] = true;
}


auto Settings::WindowChanged() -> bool
{
	auto res = m_setting_list[int(settings::RESOLUTION)];
	if (res) {
		m_setting_list[int(settings::RESOLUTION)] = false;
	}
	return res;
}


auto Settings::GetSetting(settings setting) const -> bool
{
	return m_setting_list.at(int(setting));
}


auto Settings::GetKey(uint16_t key) const -> uint16_t
{
	assert(key < m_keybindings.size());
	return m_keybindings.at(key);
}
