#ifndef TILESET_H
#define TILESET_H

#include "XMLSerialization.h"
#include "TileType.h"
#include "Entity.h"
#include "EntityGroup.h"
#include "ImageLoader.h"
//#include "Tile.h"

class TileSet : public xmls::Serializable
	
{
	enum TYPE_KEYS {
		TILESET_TILE_TYPES, TILESET_BLOCK_TYPES, TILESET_ENTITY_GROUP_TYPES
	};
private:
	std::string tileset_key;
	xmls::xString tile_sheet_name;
	xmls::xString edge_tile_sheet_name;
	xmls::xString block_tile_sheet_name;
	xmls::xString entity_group_tile_sheet_name;
	xmls::Collection<TileType> tile_types;
	xmls::Collection<EntityData> block_types;
	xmls::Collection<EntityGroupData> entity_group_types;
	//xmls::xInt entity_group_image_width, entity_group_image_height;
	void ensure_room_for_type(const int type_key, const int row);
	//xmls::Collection<EntityData> entity_types;
public:
	TileSet();
	~TileSet();
	void unload_content();
	static void load_sheet_images(std::string filename, int sub_width, int sub_height);
	static void load_full_sheet_image(std::string filename);
	static ALLEGRO_BITMAP *get_full_sheet_image(std::string filename);
	void load_sheet_images();
	void set_tileset_key(std::string key);
	void set_tile_sheet_name(std::string name);
	void set_edge_tile_sheet_name(std::string name); 
	void set_block_tile_sheet_name(std::string name);
	void set_entity_group_tile_sheet_name(std::string name);
	
	std::string get_tile_sheet_filename();
	std::string get_edge_tile_sheet_filename();
	std::string get_block_tile_sheet_filename();
	std::string get_entity_group_tile_sheet_filename();
	// tile types	
	void set_tile_speed_mod(const int row, float speed_mod);
	void set_edge_priority(const int row, int edge_priority);
	std::string get_full_tile_sheet_filename(int index);
	std::string get_full_edge_tile_sheet_filename(int index);
	std::string get_tile_key(const int index);
	float get_tile_speed_mod(const int row);
	int get_edge_priority(const int row);
	// block types
	void set_block_solid(const int row, bool solid);
	void set_block_attributes(const int row, std::map<std::string, int> attributes);
	EntityData *get_block_data(const int index);
	std::string get_full_block_sheet_filename(int index);
	bool get_block_solid(const int row);
	std::map<std::string, int> get_block_attributes(const int row);
	// entity group types
	void set_entity_group_image_dimensions(std::string name, std::pair<int, int> dimensions);
	void set_entity_group_root_offset(const int row, std::pair<int, int> offset);
	void set_entity_group_center_offset(const int row, std::pair<int, int> offset);
	void set_entity_group_components(const int row, std::vector<EntityComponentData*> components);
	EntityGroupData *get_entity_group_data(std::string name);
	EntityGroupData *get_entity_group_data_by_index(int index);
	std::pair<int, int> get_entity_group_image_dimensions(std::string name);
	std::pair<int, int> get_entity_group_image_dimensions_by_index(int index);
	std::pair<int, int> get_entity_group_root_offset(const int row);
	std::pair<int, int> get_entity_group_center_offset(const int row);
	std::vector<EntityComponentData*> get_entity_group_components(const int row);
	
};
#endif