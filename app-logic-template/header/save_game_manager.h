///////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: save_game_manager.h
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once


//////////////
// INCLUDES //
//////////////
#include <string>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "header/scene_manager.h"
#include "header/ubrotengine_dx11.h"

constexpr float USER_START_Z = -5.0F;

///////////////////////////////////////////////////////////////////////////////////////////////////
// Class name: SaveGameManager
///////////////////////////////////////////////////////////////////////////////////////////////////
class SaveGameManager
{

public:
	SaveGameManager() = delete;
	SaveGameManager(const SaveGameManager& other) = delete;
	SaveGameManager(SaveGameManager&& other) noexcept = delete;
	auto operator=(const SaveGameManager& other) -> SaveGameManager = delete;
	auto operator=(SaveGameManager&& other) -> SaveGameManager & = delete;
	~SaveGameManager() = default;

	static auto LoadGameState(
		const std::string& filename, uint16_t& gamestate, const std::string& path = "./data/savegames/"
	) -> bool;
	static auto SaveGameState(
		uint16_t gamestate,
		const std::string& filename, const std::string& path = "./data/savegames/"
	) -> bool;

	/// <summary>
	/// Used for loading a level or a menu.
	/// </summary>
	/// <param name="scene_mgr"></param>
	/// <param name="engine"></param>
	/// <param name="path"></param>
	/// <param name="filename"></param>
	static auto LoadScene(
		SceneManager& scene_mgr, graphics::Engine& engine,
		const std::string& scene_filename, 
		const std::string& scene_path,
		const std::string& model_path = "./data/assets/models/"
	) -> bool;

	static auto SaveScene(
		const Scene& scene, const std::string& scene_path, const std::string& scene_filename
	) -> bool;

private:

};
