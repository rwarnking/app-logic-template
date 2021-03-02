///////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: tile.h
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once


//////////////
// INCLUDES //
//////////////
#include <array>
#include <string>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "header/ubrotengine_dx11.h"

constexpr uint16_t MAX_KEYS{ 0xFF };

///////////////////////////////////////////////////////////////////////////////////////////////////
// Class name: Settings
///////////////////////////////////////////////////////////////////////////////////////////////////
class Settings
{
public:
	enum class settings : uint8_t
	{
		FULLSCREEN = 0,
		RESOLUTION = 1,
		NR_ITEMS
	};

	Settings();
	Settings(const Settings& other) = delete;
	Settings(Settings&& other) noexcept = delete;
	auto operator=(const Settings& other) -> Settings = delete;
	auto operator=(Settings&& other) -> Settings & = delete;
	~Settings() = default;

	auto LoadSettings(const std::string& filename) -> bool;
	void SaveSettings(const std::string& path) const;

	[[nodiscard]] auto GetWindowWidth() const-> uint16_t;
	[[nodiscard]] auto GetWindowHeight() const -> uint16_t;
	[[nodiscard]] auto GetGraphicSettings() const -> const graphics::GraphicSettings&;
	void SetResolution(uint16_t window_width, uint16_t window_height);
	void SetFullscreen(BOOL fullscreen);
	auto WindowChanged() -> bool;

	[[nodiscard]] auto GetSetting(settings setting) const -> bool;

	[[nodiscard]] auto GetKey(uint16_t key) const -> uint16_t;

private:
	std::array<bool, int(settings::NR_ITEMS)> m_setting_list;

	//const uint16_t DEFAULT_RES{ 900 };
	//uint16_t m_window_width{ DEFAULT_RES };
	//uint16_t m_window_height{ DEFAULT_RES };

	graphics::GraphicSettings m_graphic_settings{};

	std::array<uint16_t, MAX_KEYS> m_keybindings{};

};
