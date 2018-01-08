#include "TileSet.h"

TileSet::TileSet()
{
	setClassName("TileSet");
	Register("tile_sheet_name", &tile_sheet_name);
	Register("edge_tile_sheet_name", &edge_tile_sheet_name);
	Register("block_tile_sheet_name", &block_tile_sheet_name);
	Register("entity_group_tile_sheet_name", &entity_group_tile_sheet_name);
	Register("tile_types", &tile_types);
	Register("block_types", &block_types);
	Register("entity_group_types", &entity_group_types);
	Register("entity_group_image_width", &entity_group_image_width);
	Register("entity_group_image_height", &entity_group_image_height);
}


TileSet::~TileSet()
{
}

void TileSet::ensure_room_for_type(const int type_key, const int row)
{
	int size = 0;
	int index = 0;
	switch (type_key) {
	case TILESET_TILE_TYPES:
		size = this->tile_types.size();
		index = size;
		while (index <= row) {
			TileType *tile_type = new TileType();
			tile_type->set_tile_sheet_name(this->tile_sheet_name.value());
			tile_type->set_tile_sheet_row(row);
			this->tile_types.addItem(tile_type);
			index++;
		}
		break;
	case TILESET_BLOCK_TYPES:
		size = this->block_types.size();
		index = size;
		while (index <= row) {
			EntityData *block_type = new EntityData();
			this->block_types.addItem(block_type);
			index++;
		}
		break;
	case TILESET_ENTITY_GROUP_TYPES:
		size = this->entity_group_types.size();
		index = size;
		while (index <= row) {
			EntityData *entity_group_type = new EntityData();
			this->entity_group_types.addItem(entity_group_type);
			index++;
		}
		break;
	default:
		break;
	}
	if (row < 0) {
		return;
	}
	
}

void TileSet::set_tileset_key(std::string key)
{
	this->tileset_key = key;
}

void TileSet::set_tile_sheet_name(std::string name)
{
	this->tile_sheet_name = name;
}

void TileSet::set_edge_tile_sheet_name(std::string name)
{
	this->edge_tile_sheet_name = name;
}

void TileSet::set_block_tile_sheet_name(std::string name)
{
	this->block_tile_sheet_name = name;
}

void TileSet::set_entity_group_tile_sheet_name(std::string name)
{
	this->entity_group_tile_sheet_name = name;
}

std::string TileSet::get_tile_sheet_filename()
{
	return "tile_sheets/" + this->tile_sheet_name.value();
}

std::string TileSet::get_block_tile_sheet_filename()
{
	return "tile_sheets/" + this->block_tile_sheet_name.value();
}

std::string TileSet::get_entity_group_tile_sheet_filename()
{
	return "tile_sheets/" + this->entity_group_tile_sheet_name.value();
}

// tile types

void TileSet::set_tile_speed_mod(const int row, float speed_mod)
{ 
	this->ensure_room_for_type(TILESET_TILE_TYPES, row);
	this->tile_types.getItem(row)->set_speed_mod(speed_mod);
}

void TileSet::set_edge_priority(const int row, int priority)
{	
	this->ensure_room_for_type(TILESET_TILE_TYPES, row);
	this->tile_types.getItem(row)->set_edge_priority(priority);
}

float TileSet::get_tile_speed_mod(const int row)
{
	if (row < 0 || row > this->tile_types.size()) {
		return 1.0f;
	}
	return this->tile_types.getItem(row)->get_speed_mod();
}

int TileSet::get_edge_priority(const int row)
{
	if (row < 0 || row > this->tile_types.size()) {
		return 0;
	}
	return this->tile_types.getItem(row)->get_edge_priority();
}

// block types

void TileSet::set_block_solid(const int row, bool solid)
{
	this->ensure_room_for_type(TILESET_BLOCK_TYPES, row);
	this->block_types.getItem(row)->solid = solid;
}

void TileSet::set_block_attributes(const int row, std::map<std::string, int> attributes)
{
	this->ensure_room_for_type(TILESET_BLOCK_TYPES, row);
	this->block_types.getItem(row)->set_attributes(attributes);
}

bool TileSet::get_block_solid(const int row)
{
	if (row < 0 || row > this->block_types.size()) {
		return false;
	}
	return this->block_types.getItem(row)->solid.value();
}

std::map<std::string, int> TileSet::get_block_attributes(const int row)
{
	if (row < 0 || row > this->block_types.size()) {
		return std::map<std::string, int>();
	}
	return this->block_types.getItem(row)->attribute_map();
}

// entity group types

void TileSet::set_entity_group_image_dimensions(std::pair<int, int> dimensions)
{
	entity_group_image_width = dimensions.first;
	entity_group_image_height = dimensions.second;
}

void TileSet::set_entity_group_root_offset(const int row, std::pair<int, int> offset)
{
	this->ensure_room_for_type(TILESET_ENTITY_GROUP_TYPES, row);
	this->entity_group_types.getItem(row)->set_root_offset(offset);
}

void TileSet::set_entity_group_center_offset(const int row, std::pair<int, int> offset)
{
	this->ensure_room_for_type(TILESET_ENTITY_GROUP_TYPES, row);
	this->entity_group_types.getItem(row)->set_center_offset(offset);
}

void TileSet::set_entity_group_components(const int row, std::vector<EntityComponentData*> components)
{
	this->ensure_room_for_type(TILESET_ENTITY_GROUP_TYPES, row);
	this->entity_group_types.getItem(row)->set_components(components);
}

std::pair<int, int> TileSet::get_entity_group_image_dimensions()
{
	return std::pair<int, int>(entity_group_image_width.value(), entity_group_image_height.value());
}

std::pair<int, int> TileSet::get_entity_group_root_offset(const int row)
{
	if (row < 0 || row > this->entity_group_types.size()) {
		return std::pair<int, int>(0,0);
	}
	return this->entity_group_types.getItem(row)->get_root_offset();
}


std::pair<int, int> TileSet::get_entity_group_center_offset(const int row)
{
	if (row < 0 || row > this->entity_group_types.size()) {
		return std::pair<int, int>(0, 0);
	}
	return this->entity_group_types.getItem(row)->get_center_offset();
}

std::vector<EntityComponentData*> TileSet::get_entity_group_components(const int row)
{
	if (row < 0 || row > this->entity_group_types.size()) {
		return std::vector<EntityComponentData*>();
	}
	return this->entity_group_types.getItem(row)->get_components();
}
