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
}


TileSet::~TileSet()
{
}

void TileSet::unload_content()
{
	this->tile_types.Clear();
	this->block_types.Clear();
	this->entity_group_types.Clear();
}

void TileSet::load_sheet_images(std::string filename, int sub_width, int sub_height)
{
	TileSet::load_full_sheet_image(filename);
	ALLEGRO_BITMAP *tile_sheet = TileSet::get_full_sheet_image(filename);
	if (tile_sheet == NULL) {
		std::cout << "ERROR: failed to get tile sheet for filename: " + filename;
		return;
	}
	const int image_width = al_get_bitmap_width(tile_sheet), image_height = al_get_bitmap_height(tile_sheet);
	const int sub_count_x = image_width / sub_width, sub_count_y = image_height / sub_height;
	for (int y = 0; y < sub_count_y; y++) {
		for (int x = 0; x < sub_count_x; x++) {
			Rect *sub_rect = new Rect(x * sub_width, y *sub_height, sub_width, sub_height);
			ImageLoader::get_instance().load_image("tile_sheets/" + filename, *sub_rect);
		}
	}
}

void TileSet::load_full_sheet_image(std::string filename)
{
	ImageLoader::get_instance().load_image("tile_sheets/" + filename);
}

ALLEGRO_BITMAP * TileSet::get_full_sheet_image(std::string filename)
{
	return ImageLoader::get_instance().get_image("tile_sheets/" + filename);
}

void TileSet::load_sheet_images()
{
	const int tile_type_count = this->tile_types.size();
	for (int i = 0; i < tile_type_count; i++) {
		TileType *tile = this->tile_types.getItem(i);
		std::string filename = this->tile_sheet_name.value() + "_" + tile->get_tile_sheet_key();
		std::string edge_filename = this->edge_tile_sheet_name.value() + "_" + tile->get_tile_sheet_key();
		TileSet::load_sheet_images(filename, TILE_SIZE, TILE_SIZE);
		TileSet::load_sheet_images(edge_filename, TILE_SIZE, TILE_SIZE);

	}
	const int block_type_count = this->block_types.size();
	for (int i = 0; i < block_type_count; i++) {
		EntityData *block = this->block_types.getItem(i);
		std::string filename = this->block_tile_sheet_name.value() + "_" + block->get_entity_data_key();
		TileSet::load_sheet_images(filename, TILE_SIZE, TILE_SIZE);
	}
	
	const int size = this->entity_group_types.size();
	for (int i = 0; i < size; i++) {
		EntityGroupData *e = this->entity_group_types.getItem(i);
		std::string group_name = e->get_entity_group_name();
		std::pair<int, int> image_dimensions = e->get_entity_group_image_dimensions();
		const std::string group_filename = entity_group_tile_sheet_name.value() + "_" + group_name;
		TileSet::load_sheet_images(group_filename, image_dimensions.first, image_dimensions.second);
		std::vector<EntityComponentData*> component_data = e->get_components();
		for (EntityComponentData* ecd : component_data) {
			std::string component_name = ecd->get_name();
			std::string comp_filename = group_filename + "_" + ecd->get_name();		
			TileSet::load_sheet_images(comp_filename, image_dimensions.first, image_dimensions.second);
		}
	}
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
			EntityGroupData *entity_group_type = new EntityGroupData();
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

std::string TileSet::get_tileset_key()
{
	return this->tileset_key;
}

std::string TileSet::get_tile_sheet_filename()
{
	return "tile_sheets/" + this->tile_sheet_name.value();
}

std::string TileSet::get_edge_tile_sheet_filename()
{
	return "tile_sheets/" + this->edge_tile_sheet_name.value();
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

std::string TileSet::get_full_tile_sheet_filename(int index)
{
	return this->get_tile_sheet_filename() + "_" + this->tile_types.getItem(index)->get_tile_sheet_key();
}

std::string TileSet::get_full_edge_tile_sheet_filename(int index)
{
	return this->get_edge_tile_sheet_filename() + "_" + this->tile_types.getItem(index)->get_tile_sheet_key();
}

std::string TileSet::get_tile_key(const int index)
{
	return this->tile_types.getItem(index)->get_tile_sheet_key();
}

ALLEGRO_BITMAP * TileSet::get_default_tile_bitmap(const int index)
{
	return ImageLoader::get_instance().get_default_tile_image(this->get_tile_sheet_filename(), this->tile_types.getItem(index));
}

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

std::vector<std::string> TileSet::all_tile_keys()
{
	std::vector<std::string> tile_keys;
	const int size = this->tile_types.size();
	for (int i = 0; i < size; i++) {
		TileType * tile_type = this->tile_types.getItem(i);
		tile_keys.push_back(tile_type->get_tile_sheet_key());
	}
	return tile_keys;
}

// block types

ALLEGRO_BITMAP * TileSet::get_default_block_bitmap(const int index)
{
	return ImageLoader::get_instance().get_default_block_image(this->get_block_tile_sheet_filename(), this->block_types.getItem(index));
}

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

EntityData * TileSet::get_block_data(const int index)
{
	return this->block_types.getItem(index);
}

std::string TileSet::get_full_block_sheet_filename(int index)
{
	return this->get_block_tile_sheet_filename() + "_" + this->block_types.getItem(index)->get_entity_data_key();
}

std::string TileSet::get_block_key(const int index)
{
	return this->block_types.getItem(index)->get_entity_data_key();
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

std::vector<std::string> TileSet::all_block_keys()
{
	std::vector<std::string> block_keys;
	const int size = this->block_types.size();
	for (int i = 0; i < size; i++) {
		EntityData * block_type = this->block_types.getItem(i);
		block_keys.push_back(block_type->entity_data_key.value());
	}
	return block_keys;
}

// entity group types

ALLEGRO_BITMAP * TileSet::get_default_entity_group_bitmap(const int index)
{
	//TODO: this is almost definitely not getting the image because
	// there is no "main" image for an entitygroup. 
	// debug, figure out filename and make a main image
	//TODO: ensure that we actually load the full group image
	return ImageLoader::get_instance().get_default_entity_group_image(this->get_entity_group_tile_sheet_filename(), this->entity_group_types.getItem(index));
}

void TileSet::set_entity_group_image_dimensions(std::string entity_group_name, std::pair<int, int> dimensions)
{
	EntityGroupData *egd = this->get_entity_group_data(entity_group_name);
	if (egd != NULL) {
		egd->set_entity_group_image_dimensions(dimensions);
	}
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

EntityGroupData * TileSet::get_entity_group_data(std::string name)
{
	const int size = this->entity_group_types.size();
	for (int i = 0; i < size; i++) {
		EntityGroupData *egd = this->entity_group_types.getItem(i);
		if (egd->get_entity_group_name() == name) {
			return egd;
		}
	}
	return NULL;
}

EntityGroupData * TileSet::get_entity_group_data_by_index(int index)
{
	const int size = this->entity_group_types.size();
	for (int i = 0; i < size; i++) {
		EntityGroupData *egd = this->entity_group_types.getItem(i);
		if (egd->get_entity_group_index() == index) {
			return egd;
		}
	}
	return NULL;
}

std::string TileSet::get_entity_group_key(const int index)
{
	return this->entity_group_types.getItem(index)->get_entity_group_name();
}

std::pair<int, int> TileSet::get_entity_group_image_dimensions(std::string name)
{
	EntityGroupData *egd = this->get_entity_group_data(name);
	if (egd == NULL) {
		return std::pair<int,int>(0,0);
	}
	return egd->get_entity_group_image_dimensions();
}

std::pair<int, int> TileSet::get_entity_group_image_dimensions_by_index(int index)
{
	EntityGroupData *egd = this->get_entity_group_data_by_index(index);
	if (egd == NULL) {
		return std::pair<int, int>(0, 0);
	}
	return egd->get_entity_group_image_dimensions();
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

std::vector<std::string> TileSet::all_entity_group_keys()
{
	std::vector<std::string> entity_group_keys;
	const int size = this->entity_group_types.size();
	for (int i = 0; i < size; i++) {
		EntityGroupData * entity_group_type = this->entity_group_types.getItem(i);
		entity_group_keys.push_back(entity_group_type->entity_group_name.value());
	}
	return entity_group_keys;
}
