///////////////////////////////////////////////////////////////////////////////////////////////////
// app_logic_template.h
//
// Part of a Gamelogic Template, under the MIT licence.
// See LICENSE for license information.
// SPDX-License-Identifier: MIT
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
// This file declares the SelectionDAG class, and transitively defines the
// SDNode class and subclasses.
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#ifdef APPLOGICTEMPLATE_EXPORTS
#define APP_LOGIC_TEMPLATE_API __declspec(dllexport)
#else
#define APP_LOGIC_TEMPLATE_API __declspec(dllimport)
#endif

//////////////
// INCLUDES //
//////////////
#include <cstdint>
#include <memory>
#include <winuser.h>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "input.h"
#include "save_game_manager.h"
#include "settings.h"

// OBACHT: This does only work if the additional includes are set as intended
#include "header/scene_manager.h"
#include "header/ubrotengine_dx11.h"


namespace logic
{
	constexpr unsigned int SCREEN_WIDTH{ 1920 };
	constexpr unsigned int SCREEN_HEIGHT{ 1080 };
	constexpr uint16_t MAX_KEY{ 0xFF };

class Logic
{
	enum class GAMESTATE : uint16_t
	{
		INIT_MENU = 0,
		LOGOS,
		MAINMENU,
		LOADINGMENU,
		SAVEMENU,
		SETTINGSMENU,
		PAUSE,
		LOADING,
		INGAME,
		APP_CLOSE
	};

public:
	Logic();

	[[nodiscard]] APP_LOGIC_TEMPLATE_API auto GetAppName() const -> const std::wstring&;

	APP_LOGIC_TEMPLATE_API auto InitMenuScene(
		SceneManager& scene_mgr, graphics::Engine& engine
	) -> bool;

	APP_LOGIC_TEMPLATE_API auto InitSettings() -> bool;

	APP_LOGIC_TEMPLATE_API auto Update(
		SceneManager& scene_mgr, graphics::Engine& engine, float frametime
	) -> bool;

	APP_LOGIC_TEMPLATE_API auto WindowChanged() -> bool;
	APP_LOGIC_TEMPLATE_API auto GetWindowWidth() -> uint16_t;
	APP_LOGIC_TEMPLATE_API auto GetWindowHeight() -> uint16_t;
	APP_LOGIC_TEMPLATE_API auto GetGraphicSettings() -> const graphics::GraphicSettings&;

	[[nodiscard]] APP_LOGIC_TEMPLATE_API auto CloseApp() const -> bool;

	void APP_LOGIC_TEMPLATE_API MouseMove(int32_t dir_x, int32_t dir_y);
	void APP_LOGIC_TEMPLATE_API MouseClick(int p_x, int p_y);
	void APP_LOGIC_TEMPLATE_API KeyIsPressed(uint16_t key);
	void APP_LOGIC_TEMPLATE_API KeyReleased(uint16_t key);

private:
	auto ProcessInput(
		SceneManager& scene_mgr, graphics::Engine& engine, float frametime
	) -> bool;

	void MovePlayer(Scene& scene, float frametime);

	auto LoadSaveGame(
		SceneManager& scene_mgr, graphics::Engine& engine, const std::string& filename
	) -> bool;

//private:
	GAMESTATE m_game_state{ GAMESTATE::INIT_MENU };
	uint16_t m_level{ 0 };

	const std::wstring m_app_name = L"App Logic Template";

#ifdef _DEBUG
	const std::string m_level_path = "../../app-logic/app-logic-template/app-logic-template/data/level/";
	const std::string m_menu_path = "../../app-logic/app-logic-template/app-logic-template/data/menus/";
	const std::string m_settings_path = "../../app-logic/app-logic-template/app-logic-template/data/";
#else
	const std::string m_level_path = "./data/level/";
	const std::string m_menu_path = "./data/menus/";
	const std::string m_settings_path = "./data/";
#endif

	int m_mouse_pos_x_now{ 0 };
	int m_mouse_pos_y_now{ 0 };
	int32_t m_mouse_dir_x{ 0 };
	int32_t m_mouse_dir_y{ 0 };

	uint16_t m_is_pressed_key{ MAX_KEY };
	uint16_t m_released_key{ MAX_KEY };

	std::unique_ptr<Input> m_input;
	std::unique_ptr<Settings> m_settings;

};

extern APP_LOGIC_TEMPLATE_API auto createLogic() -> std::unique_ptr<Logic>;

}; // namespace logic