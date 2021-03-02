///////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: app_logic_template.cpp
// Defines the exported functions for the DLL.
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "../header/app_logic_template.h"


//////////////
// INCLUDES //
//////////////
#include <algorithm>
#include <string>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////


namespace logic
{

APP_LOGIC_TEMPLATE_API auto createLogic() -> std::unique_ptr<Logic>
{
	return std::make_unique<Logic>(Logic());
}


Logic::Logic() : 
	m_input(std::make_unique<Input>()),
	m_settings(std::make_unique<Settings>())
{
}


auto Logic::GetAppName() const -> const std::wstring&
{
	return m_app_name;
}


auto Logic::InitSettings() -> bool
{
	return m_settings->LoadSettings(m_settings_path + std::string("settings.txt"));
}


auto Logic::InitMenuScene(SceneManager& scene_mgr, graphics::Engine& engine) -> bool
{
	m_game_state = GAMESTATE::MAINMENU;
	const std::string menuname = "menu0.txt";
#ifdef _DEBUG
	const std::string model_path("../../app-logic/app-logic-template/app-logic-template/data/assets/models/");
	SaveGameManager::LoadScene(scene_mgr, engine, menuname, m_menu_path, model_path);
#else
	SaveGameManager::LoadScene(scene_mgr, engine, menuname, m_menu_path);
#endif
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Continuous update
///////////////////////////////////////////////////////////////////////////////////////////////////
// Create object
// Add model to obj with type
// Add shaderprog to obj -> default shader enum

// Create new shaderprog
// -> add shader
// -> add layout
// Returns index -> logic must save it for reuse
auto Logic::Update(
	SceneManager& scene_mgr, graphics::Engine& engine, float frametime
) -> bool
{
	m_input->Update();
	return ProcessInput(scene_mgr, engine, frametime);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Input
///////////////////////////////////////////////////////////////////////////////////////////////////
auto Logic::ProcessInput(
	SceneManager& scene_mgr, graphics::Engine& engine, float frametime
) -> bool
{
	auto result{ true };

	if (m_released_key != MAX_KEY) {
		if (m_game_state == GAMESTATE::MAINMENU) {
			// Closing
			if (m_released_key == m_settings->GetKey(VK_ESCAPE)) {
				m_game_state = GAMESTATE::APP_CLOSE;
			}
			// When in the main menu L is pressed, load the loading menu
			else if (m_released_key == m_settings->GetKey('L')) {
				m_game_state = GAMESTATE::LOADINGMENU;
			}
			else if (m_released_key == m_settings->GetKey('S')) {
				m_game_state = GAMESTATE::SAVEMENU;
			}
			else if (m_released_key == m_settings->GetKey('C')) {
				m_game_state = GAMESTATE::SETTINGSMENU;
			}
			else if (m_released_key == m_settings->GetKey('F')) {
				if (m_settings->GetGraphicSettings().fullscreen == TRUE) {
					m_settings->SetFullscreen(FALSE);
				} else {
					m_settings->SetFullscreen(TRUE);
				}
			}
			else if (m_released_key == m_settings->GetKey('M')) {
				auto& resolutions = engine.GetSupportedResolutions();
				auto w = m_settings->GetWindowWidth();
				auto h = m_settings->GetWindowHeight();
				
				auto it = std::find_if(
					resolutions.rbegin(), resolutions.rend(),
					[=] (const auto& r) {
						return std::get<0>(r) == w && std::get<1>(r) == h;
					}
				);
				auto idx = int(resolutions.size() - 1) - std::distance(resolutions.rbegin(), it);
				idx++;
				assert(idx >= 0);
				if (idx >= resolutions.size())
					idx = 0;
				m_settings->SetResolution(std::get<0>(resolutions[idx]), std::get<1>(resolutions[idx]));
			}
		}
		// When in the loading menu a digit is pressed load the corresponding savefile
		else if (m_game_state == GAMESTATE::LOADINGMENU) {
			if (m_released_key == m_settings->GetKey(VK_ESCAPE)) {
				m_game_state = GAMESTATE::MAINMENU;
			}
			else if (m_released_key == m_settings->GetKey('1')) {
				const std::string filename = "save1.txt";
				result = LoadSaveGame(scene_mgr, engine, filename);
			}
			else if (m_released_key == m_settings->GetKey('2')) {
				const std::string filename = "save2.txt";
				result = LoadSaveGame(scene_mgr, engine, filename);
			}
		}
		// When in the save menu a digit is pressed save to the corresponding savefile
		else if (m_game_state == GAMESTATE::SAVEMENU) {
			if (m_released_key == m_settings->GetKey(VK_ESCAPE)) {
				m_game_state = GAMESTATE::MAINMENU;
			}
			// Save the gamestate
			else if (m_released_key == m_settings->GetKey('1')) {
				const std::string filename = "save1.txt";
#ifdef _DEBUG
				const std::string save_path("../../app-logic/app-logic-template/app-logic-template/data/savegames/");
				result = SaveGameManager::SaveGameState(m_level, filename, save_path);
#else
				result = SaveGameManager::SaveGameState(m_level, filename);
#endif
			}
			// Save the level and how it is
			else if (m_released_key == m_settings->GetKey('2')) {
				const std::string levelname = "level" + std::to_string(m_level) + ".txt";
				result = SaveGameManager::SaveScene(scene_mgr.GetActiveScene(), m_level_path, levelname);
			}
		}
		else if (m_game_state == GAMESTATE::SETTINGSMENU) {
			const uint16_t RES = 900;
			if (m_released_key == m_settings->GetKey(VK_ESCAPE)) {
				m_game_state = GAMESTATE::MAINMENU;
			}
			else if (m_released_key == m_settings->GetKey('1')) {
				m_settings->SetResolution(RES / 2, RES / 2);
			}
			else if (m_released_key == m_settings->GetKey('2')) {
				m_settings->SetResolution(RES, RES);
			}
		}
	}

	if (m_game_state != GAMESTATE::INGAME) {
		m_released_key = MAX_KEY;
		return result;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	// In game controlls
	///////////////////////////////////////////////////////////////////////////////////////////////

	// Pause
	if (m_released_key == m_settings->GetKey('P')) {
		m_game_state = GAMESTATE::PAUSE;
	}

	if (m_released_key == m_settings->GetKey(VK_ESCAPE)) {
		m_game_state = GAMESTATE::MAINMENU;
	}

	MovePlayer(scene_mgr.GetActiveScene(), frametime);

	m_released_key = MAX_KEY;
	return true;
}


void Logic::MovePlayer(Scene& scene, float frametime)
{
	auto& user = scene.GetUser(0);

	// Mouse input
	constexpr double SCALE = 20.0F;
	if (m_mouse_dir_x > 1) {
		user.TurnRight(frametime, float(m_mouse_dir_x / SCALE));
	}
	else if (m_mouse_dir_x < -1) {
		user.TurnLeft(frametime, -float(m_mouse_dir_x / SCALE));
	}

	if (m_mouse_dir_y > 1) {
		user.TurnUp(frametime, float(m_mouse_dir_y / SCALE));
	}
	else if (m_mouse_dir_y < -1) {
		user.TurnDown(frametime, -float(m_mouse_dir_y / SCALE));
	}

	m_mouse_dir_x = 0;
	m_mouse_dir_y = 0;

	// Keyboard input
	if (m_input->IsKeyPressed('W')) {
		user.MoveForward(frametime);
	}
	else if (m_input->IsKeyPressed('S')) {
		user.MoveBackward(frametime);
	}

	if (m_input->IsKeyPressed('A')) {
		user.TurnLeft(frametime);
	}
	else if (m_input->IsKeyPressed('D')) {
		user.TurnRight(frametime);
	}

	if (m_input->IsKeyPressed(VK_UP)) {
		user.MoveUp(frametime);
	}
	else if (m_input->IsKeyPressed(VK_DOWN)) {
		user.MoveDown(frametime);
	}

	if (m_input->IsKeyPressed(VK_LEFT)) {
		user.MoveLeft(frametime);
	}
	else if (m_input->IsKeyPressed(VK_RIGHT)) {
		user.MoveRight(frametime);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Loading
///////////////////////////////////////////////////////////////////////////////////////////////////
auto Logic::LoadSaveGame(
	SceneManager& scene_mgr, graphics::Engine& engine, const std::string& filename
) -> bool
{
	bool result{ true };

	m_game_state = GAMESTATE::LOADING;
#ifdef _DEBUG
	const std::string save_path("../../app-logic/app-logic-template/app-logic-template/data/savegames/");
	result = SaveGameManager::LoadGameState(filename, m_level, save_path);
	if (!result) {
		return result;
	}
	const std::string levelname = "level" + std::to_string(m_level) + ".txt";
	const std::string model_path("../../app-logic/app-logic-template/app-logic-template/data/assets/models/");
	result = SaveGameManager::LoadScene(scene_mgr, engine, levelname, m_level_path, model_path);
#else
	result = SaveGameManager::LoadGameState(filename, m_level);
	if (!result) {
		return result;
	}
	std::string levelname = "level" + std::to_string(m_level) + ".txt";
	SaveGameManager::LoadScene(scene_mgr, engine, levelname, m_level_path);
#endif
	m_game_state = GAMESTATE::INGAME;

	return result;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Getter and setter
///////////////////////////////////////////////////////////////////////////////////////////////////
auto Logic::WindowChanged() -> bool
{
	return m_settings->WindowChanged();
}


auto Logic::GetWindowWidth() -> uint16_t
{
	return m_settings->GetWindowWidth();
}


auto Logic::GetWindowHeight() -> uint16_t
{
	return m_settings->GetWindowHeight();
}


auto Logic::GetGraphicSettings() -> const graphics::GraphicSettings&
{
	return m_settings->GetGraphicSettings();
}


auto Logic::CloseApp() const -> bool
{
	return m_game_state == GAMESTATE::APP_CLOSE;
}


void Logic::MouseMove(int32_t dir_x, int32_t dir_y)
{
	m_mouse_dir_x = dir_x;
	m_mouse_dir_y = dir_y;
}


void Logic::MouseClick(int p_x, int p_y)
{
	m_mouse_pos_x_now = p_x;
	m_mouse_pos_y_now = p_y;
}


void Logic::KeyIsPressed(uint16_t key) 
{
	m_is_pressed_key = key;
}


void Logic::KeyReleased(uint16_t key)
{
	m_released_key = key;
}

} // namespace logic
