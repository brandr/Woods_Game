#ifndef TILESET_H
#define TILESET_H

#include "XMLSerialization.h"
#include "TileType.h"
#include "Entity.h"
#include "EntityGroup.h"
#include "ImageLoader.h"

class TileSet : public xmls::Serializable
	
{
	enum TYPE_KEYS {
		TILESET_TILE_TYPES, TILESET_BLOCK_TYPES, TILESET_ENTITY_GROUP_TYPES, TILESET_TILED_IMAGE_TYPES
	};
private:
	std::string tileset_key;
	xmls::xString tile_sheet_name;
	xmls::Collection<TileType> tile_types;
	xmls::Collection<EntityData> block_types;
	xmls::Collection<EntityGroupData> entity_group_types;
	xmls::Collection<TiledImageData> tiled_image_types;
	xmls::Collection<EntityData> spawner_types;
	xmls::Collection<EntityData> path_node_types;
	xmls::Collection<EntityData> location_marker_types;
	void ensure_room_for_type(const int type_key, const int row);
public:
	TileSet();
	~TileSet();
	void unload_content();
	static void load_sheet_images(const std::string filename, const int sub_width, const int sub_height);
	static void load_full_sheet_image(std::string filename);
	const static bool sheet_image_exists(const std::string filename);
	static ALLEGRO_BITMAP *get_full_sheet_image(std::string filename);
	void load_sheet_images();
	void set_tileset_key(std::string key);
	void set_tile_sheet_name(std::string name);
	
	std::string get_tileset_key();
	std::string get_tile_sheet_filename();

	const std::string encyclopedia_entry_name_for_entity(Entity * e);
	const std::string encyclopedia_category_for_entity(Entity * e);
	
	// tile types	
	ALLEGRO_BITMAP * get_default_tile_bitmap(const int index);
	ALLEGRO_BITMAP * get_tile_bitmap_for_col(const int index, const int col);
	void set_tile_speed_mod(const int row, float speed_mod);
	void set_edge_priority(const int row, int edge_priority);
	std::string get_full_tile_sheet_filename(int index);
	std::string get_tile_key(const int index);
	float get_tile_speed_mod(const int row);
	int get_edge_priority(const int row);
	const int get_tile_sheet_image_cols_by_index(const int index);
	const bool is_tile_npc_pathable(const int index, const bool obeys_rules);
	std::vector<std::string> all_tile_keys();
	// block types
	ALLEGRO_BITMAP * get_default_block_bitmap(const int index);
	ALLEGRO_BITMAP * get_block_bitmap_for_col(const int index, const int col);
	const std::vector<std::pair<std::string, std::string>> get_block_interact_action_data(const int index);
	const std::vector<std::pair<std::string, std::string>> get_block_contact_action_data(const int index);
	const std::vector<std::pair<std::string, std::string>> get_block_load_day_action_data(const int index);
	std::vector<InteractAction *> get_block_interact_actions(const int index);
	std::vector<InteractAction *> get_block_contact_actions(const int index);
	std::vector<InteractAction *> get_block_load_day_actions(const int index);
	std::vector<ItemDrop *> get_block_item_drops(const int index);
	const std::vector<EntitySpawnTileRule *> get_block_spawn_tile_rules(const int index);
	void set_block_solid(const int row, bool solid);
	void set_block_attributes(const int row, std::map<std::string, int> attributes);
	EntityData *get_block_data(const int index);
	std::string get_full_block_sheet_filename(int index);
	std::string get_block_key(const int index);
	bool get_block_solid(const int row);
	bool get_block_visible(const int row);
	std::map<std::string, int> get_block_attributes(const int row);
	const int get_block_sheet_image_cols_by_index(const int index);
	std::vector<EntityEffectData *> get_block_entity_effect_data(const int index);
	std::vector<std::string> all_block_keys();
	// entity group types
	ALLEGRO_BITMAP * get_default_entity_group_bitmap(const int index);
	ALLEGRO_BITMAP * get_entity_group_bitmap_for_col(const int index, const int col);
	void set_entity_group_image_dimensions(std::string name, std::pair<int, int> dimensions);
	void set_entity_group_root_offset(const int row, std::pair<int, int> offset);
	void set_entity_group_center_offset(const int row, std::pair<int, int> offset);
	void set_entity_group_components(const int row, std::vector<EntityComponentData*> components);
	EntityGroupData *get_entity_group_data(std::string name);
	EntityGroupData *get_entity_group_data_by_index(int index);
	std::string get_entity_group_key(const int index);
	std::pair<int, int> get_entity_group_image_dimensions(std::string name);
	std::pair<int, int> get_entity_group_image_dimensions_by_index(int index);
	std::pair<int, int> get_entity_group_root_offset(const int row);
	const std::pair<int, int> get_entity_group_collide_offset(const int row);
	const std::pair<int, int> get_entity_group_collide_dimensions(const int row);
	std::pair<int, int> get_entity_group_center_offset(const int row);
	std::vector<EntityComponentData*> get_entity_group_components(const int row);
	const int get_entity_group_sheet_image_cols_by_index(const int index);
	std::vector<std::string> all_entity_group_keys();
	// tiled image types
	ALLEGRO_BITMAP * get_full_tiled_image_sheet(const int index);
	ALLEGRO_BITMAP * get_default_tiled_image_bitmap(const int index);
	TiledImageData * get_tiled_image_data_by_index(const int index);
	const std::string get_tiled_image_name_by_index(const int index);
	const std::string get_full_tiled_image_sheet_filename(const int index);
	std::vector<std::string> all_tiled_image_keys();
	// spawner types
	const std::string get_full_spawner_sheet_filename(int index);
	ALLEGRO_BITMAP * get_spawner_bitmap_for_col(const int index, const int col);
	EntityData * get_spawner_data_by_index(const int index);
	const std::vector<std::string> all_spawner_keys();
	// path node types
	const std::string get_full_path_node_sheet_filename(const int index);
	ALLEGRO_BITMAP * get_path_node_bitmap_for_col(const int index, const int col);
	EntityData * get_path_node_data_by_index(const int index);
	const std::vector<std::string> all_path_node_keys();
	// location marker types
	const std::string get_full_location_marker_sheet_filename(const int index);
	ALLEGRO_BITMAP * get_location_marker_bitmap_for_col(const int index, const int col);
	EntityData * get_location_marker_data_by_index(const int index);
	const std::vector<std::string> all_location_marker_keys();
};
#endif