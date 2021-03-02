///////////////////////////////////////////////////////////////////////////////////////////////////
// Filename: save_game_manager.cpp
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "..\header\save_game_manager.h"


//////////////
// INCLUDES //
//////////////
#include <fstream>
#include <iostream>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////


auto SaveGameManager::LoadGameState(
	const std::string& filename, uint16_t& gamestate, const std::string& path
) -> bool
{
	std::fstream in(path + filename, std::ios::in);
	auto res = in.is_open();
	if (res)
	{
		in >> gamestate;
		in.close();
	}
	return res;
}


auto SaveGameManager::SaveGameState(
	uint16_t gamestate,
	const std::string& filename, const std::string& path
) -> bool
{
	std::fstream out(path + filename, std::ios::out);
	auto res = out.is_open();
	if (res)
	{
		out << gamestate;
		out.close();
	}
	return res;
}


auto SaveGameManager::LoadScene(
	SceneManager& scene_mgr, graphics::Engine& engine,
	const std::string& scene_filename, const std::string& scene_path,
	const std::string& model_path
) -> bool
{
	auto scene_idx = scene_mgr.AddScene();
	scene_mgr.SetActiveScene(scene_idx);
	scene_mgr.AddUser(0.0F, 0.0F, USER_START_Z, scene_idx);

	std::fstream in(scene_path + scene_filename, std::ios::in);
	auto res = in.is_open();
	if (res)
	{
		size_t elem_count{ 0 };
		in >> elem_count;

		float x{ 0.0F };
		float y{ 0.0F };
		float z{ 0.0F };
		std::string model_filename;
		for (auto i{ 0 }; i < elem_count; ++i) {
			in >> x >> y >> z;
			in >> model_filename;

			size_t idx{ 0 };
			if (model_filename.rfind('-', 0) == 0) {
				auto proc_idx = uint8_t(abs(stoi(model_filename)));
				idx = engine.RegisterModelProcedural(proc_idx);
			} else {
				idx = engine.RegisterModel(model_path + model_filename);
			}

			auto& obj = scene_mgr.AddObjectToActive();
			obj.SetModelIdx(idx);
			obj.SetPosition(x, y, z);
		}
		in.close();
	}
	return res;
}


auto SaveGameManager::SaveScene(
	const Scene& scene, const std::string& scene_path, const std::string& scene_filename
) -> bool
{
	std::fstream out(scene_path + scene_filename, std::ios::out);
	auto res = out.is_open();
	if (res)
	{
		// For all tiles in this scene
		for (const auto& tile : scene.GetTiles()) {
			out << scene.GetObjects(tile.first).size() << '\n';
			
			// Get the tile coords to access the scene objects
			for (const auto& o : scene.GetObjects(tile.first)) {
				auto pos = o.GetPosition();
				out << pos.x << ' ' << pos.y << ' ' << pos.z << '\n';
			}
		}
		out.close();
	}
	return res;
}
