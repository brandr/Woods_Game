#ifndef LEVEL_EDITOR_DATA_MANAGER_H
#define LEVEL_EDITOR_DATA_MANAGER_H

#include "TileSet.h"
#include "Level.h"
#include "Dungeon.h"
#include "FileManager.h"
#include <Agui/Backends/Allegro5/Allegro5Image.hpp>
#include <Agui/Backends/Allegro5/Allegro5Graphics.hpp>

#include <allegro5/display.h>

//TODO: make sure we avoid all memory leaks by filtering loading/unloading/etc through this class



enum OBJECT_TYPES {
	OBJECT_TYPE_TILE = 0, OBJECT_TYPE_BLOCK = 1, OBJECT_TYPE_ENTITY_GROUP = 2
};

class Level;
struct LevelDeleter
{
	void operator()(Level *p);
};
class TileSet;
struct TilesetDeleter
{

	void operator()(TileSet *p);
};

class LevelEditorDataManager {
public:
	static const std::vector<std::string> LEVEL_LAYERS;
	static constexpr const char* TILE_LAYER = "tile_layer";
	static constexpr const char* BLOCK_LAYER = "block_layer";
	static constexpr const char* ENTITY_GROUP_LAYER = "entity_group_layer";
	static constexpr const char* GRID_LINES_LAYER = "grid_lines_layer";
	
private:
	LevelEditorDataManager() {}
	FileManager file_manager;
	std::unique_ptr<Dungeon> active_dungeon;
	std::vector<std::unique_ptr<Level, LevelDeleter>> active_levels;
	std::vector<std::unique_ptr<TileSet, TilesetDeleter>> active_tilesets;
	int selected_level_index = -1;
	int selected_tileset_index = -1;
	int selected_tileset_tile_index = -1;
	int selected_tileset_block_index = -1;
	int selected_tileset_entity_group_index = -1;
	// for level editor layout
	int selected_object_tab_index = -1;
	int selected_object_select_index = -1;
	void reset_active_levels();
	void set_selected_level_tile(int index, std::pair<int, int> pos);
	void set_selected_level_block(int index, std::pair<int, int> pos);
	void add_selected_level_entity_group(int index, std::pair<int, int> pos);
	void delete_level_tile(std::pair<int, int> pos);
	void delete_level_block(std::pair<int, int> pos);
	void delete_level_entity_group(std::pair<int, int> pos);
public:
	LevelEditorDataManager(LevelEditorDataManager const&) = delete;
	void operator=(LevelEditorDataManager const&) = delete;
	static LevelEditorDataManager& get_instance();
	agui::Allegro5Image * load_image_layer(std::string layer);
	
	// dungeon
	void save_dungeon(std::string dungeon_name);
	void load_dungeon(std::string dungeon_name);
	void add_dungeon(std::string dungeon_name);
	void delete_dungeon(std::string dungeon_name);
	std::string get_active_dungeon_name();
	bool has_active_dungeon();

	//level
	void set_active_level_attributes(std::string level_name,
		::pair<int, int> pos, std::pair<int, int> dimensions);
	void add_level(std::string level_name);
	void delete_level(std::string level_name);
	std::vector<std::string> get_active_level_names();
	std::string get_selected_level_name();
	std::pair<int, int> selected_level_grid_pos();
	std::pair<int, int> selected_level_grid_dimensions();
	std::pair<int, int> selected_level_pixel_dimensions();
	bool has_selected_level();
	void set_selected_level_index(int index);
	int get_selected_level_index();

	// level objects
	void set_selected_object_tab_index(int index);
	int get_selected_object_tab_index();
	void set_selected_object_select_index(int index);
	int get_selected_object_select_index();
	bool add_level_object(std::pair<int, int> tile_pos);
	bool delete_level_object(std::pair<int, int> tile_pos);

	// tileset
	void load_all_tilesets();
	bool update_selected_tileset(int index);
	bool update_selected_tileset_tile(int index);
	bool update_selected_tileset_block(int index);
	bool update_selected_tileset_entity_group(int index);
	void set_selected_tileset_index(int index);
	std::vector<std::string> all_tileset_names();
	std::vector<std::string> all_selected_tile_keys();
	std::vector<std::string> all_selected_block_keys();
	std::vector<std::string> all_selected_entity_group_keys();
	std::string get_selected_tileset_name();
	std::string get_tileset_name(int index);
	ALLEGRO_BITMAP * get_default_tile_bitmap(int index);
	ALLEGRO_BITMAP * get_default_block_bitmap(int index);
	ALLEGRO_BITMAP * get_default_entity_group_bitmap(int index);
	std::pair<int, int> get_entity_group_image_dimensions_by_index(int index);
	bool has_selected_tileset();
};

//TODO: fix the memory leak by making these a part of the class
const std::vector<std::string> LEVEL_LAYERS({
	LevelEditorDataManager::TILE_LAYER,
	LevelEditorDataManager::BLOCK_LAYER,
	LevelEditorDataManager::ENTITY_GROUP_LAYER,
	LevelEditorDataManager::GRID_LINES_LAYER
});

#endif // !1
