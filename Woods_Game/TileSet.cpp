#include "TileSet.h"

TileSet::TileSet()
{
	setClassName("TileSet");
	Register("tile_sheet_name", &tile_sheet_name);
	Register("tile_types", &tile_types);
	Register("block_types", &block_types);
	Register("entity_group_types", &entity_group_types);
	Register("tiled_image_types", &tiled_image_types);
	Register("spawner_types", &spawner_types);
	Register("path_node_types", &path_node_types);
	Register("location_marker_types", &location_marker_types);
}

TileSet::~TileSet()
{
}

void TileSet::unload_content()
{
	this->tile_types.Clear();
	this->block_types.Clear();
	this->entity_group_types.Clear();
	this->tiled_image_types.Clear();
}

void TileSet::load_sheet_images(const std::string filename, const int sub_width, const int sub_height)
{
	if (!get_full_sheet_image(filename)) {
		TileSet::load_full_sheet_image(filename);
	}
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
			if (!ImageLoader::get_instance().get_image("tile_sheets/" + filename, *sub_rect))
			{
				ImageLoader::get_instance().load_image("tile_sheets/" + filename, *sub_rect);
			}
		}
	}
}

void TileSet::load_full_sheet_image(std::string filename)
{
	ImageLoader::get_instance().load_image("tile_sheets/" + filename);
}

const bool TileSet::sheet_image_exists(const std::string filename)
{
	return ImageLoader::get_instance().image_exists("tile_sheets/" + filename);
}

ALLEGRO_BITMAP * TileSet::get_full_sheet_image(std::string filename)
{
	return ImageLoader::get_instance().get_image("tile_sheets/" + filename);
}

void TileSet::load_sheet_images()
{
	const std::string sheet_name = this->tile_sheet_name.value();
	const int tile_type_count = this->tile_types.size();
	for (int i = 0; i < tile_type_count; i++) {
		TileType *tile = this->tile_types.getItem(i);
		std::string filename = sheet_name + "/tiles/" + tile->get_tile_sheet_key();
		std::string edge_filename = this->tile_sheet_name.value() + "/tiles/edges/" + tile->get_tile_sheet_key();
		TileSet::load_sheet_images(filename, TILE_SIZE, TILE_SIZE);
		if (TileSet::sheet_image_exists(edge_filename)) {
			TileSet::load_sheet_images(edge_filename, TILE_SIZE, TILE_SIZE);
		}
	}
	const int block_type_count = this->block_types.size();
	for (int i = 0; i < block_type_count; i++) {
		EntityData *block = this->block_types.getItem(i);
		std::string filename = sheet_name + "/blocks/" + block->get_entity_data_key();
		TileSet::load_sheet_images(filename, TILE_SIZE, TILE_SIZE);
		const std::vector<std::string> suffixes = block->image_filename_suffixes();
		for (std::string suffix : suffixes) {
			TileSet::load_sheet_images(filename + suffix, TILE_SIZE, TILE_SIZE);
		}
	}
	
	const int size = this->entity_group_types.size();
	for (int i = 0; i < size; i++) {
		EntityGroupData *e = this->entity_group_types.getItem(i);
		std::string group_name = e->get_entity_group_name();
		std::pair<int, int> image_dimensions = e->get_entity_group_image_dimensions();
		const std::string group_filename = tile_sheet_name.value() + "/entity_groups/" + group_name;
		TileSet::load_sheet_images(group_filename, image_dimensions.first, image_dimensions.second);
		std::vector<EntityComponentData*> component_data = e->get_components();
		for (EntityComponentData* ecd : component_data) {
			std::string component_name = ecd->get_name();
			std::string comp_filename = group_filename + "_" + ecd->get_name();		
			TileSet::load_sheet_images(comp_filename, image_dimensions.first, image_dimensions.second);
		}
	}

	const int tiled_image_type_count = this->tiled_image_types.size();
	for (int i = 0; i < tiled_image_type_count; i++) {
		TiledImageData *tid = this->tiled_image_types.getItem(i);
		const std::string filename = this->tile_sheet_name.value() + "/tiled_images/" + tid->get_image_data_key();
		TileSet::load_sheet_images(filename, TILE_SIZE, TILE_SIZE);
	}

	const int spawner_type_count = this->spawner_types.size();
	for (int i = 0; i < spawner_type_count; i++) {
		EntityData * e = this->spawner_types.getItem(i);
		const std::string filename = this->tile_sheet_name.value() + "/spawners/" + e->get_entity_data_key();
		TileSet::load_sheet_images(filename, TILE_SIZE, TILE_SIZE);
	}

	const int path_node_type_count = this->path_node_types.size();
	for (int i = 0; i < path_node_type_count; i++) {
		EntityData * e = this->path_node_types.getItem(i);
		const std::string filename = this->tile_sheet_name.value() + "/path_nodes/" + e->get_entity_data_key();
		TileSet::load_sheet_images(filename, TILE_SIZE, TILE_SIZE);
	}

	const int location_marker_count = this->location_marker_types.size();
	for (int i = 0; i < location_marker_count; i++) {
		EntityData * e = this->location_marker_types.getItem(i);
		const std::string filename = this->tile_sheet_name.value() + "/location_markers/" + e->get_entity_data_key();
		TileSet::load_sheet_images(filename, TILE_SIZE, TILE_SIZE);
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

std::string TileSet::get_tileset_key()
{
	return this->tileset_key;
}

std::string TileSet::get_tile_sheet_filename()
{
	return "tile_sheets/" + this->tile_sheet_name.value();
}

// tile types

std::string TileSet::get_full_tile_sheet_filename(int index)
{
	return this->get_tile_sheet_filename() + "/tiles/" + this->tile_types.getItem(index)->get_tile_sheet_key();
}

std::string TileSet::get_tile_key(const int index)
{
	return this->tile_types.getItem(index)->get_tile_sheet_key();
}

ALLEGRO_BITMAP * TileSet::get_default_tile_bitmap(const int index)
{
	return ImageLoader::get_instance().get_default_tile_image(this->get_tile_sheet_filename(), this->tile_types.getItem(index));
}

ALLEGRO_BITMAP * TileSet::get_tile_bitmap_for_col(const int index, const int col)
{
	return ImageLoader::get_instance().get_tile_image_for_col(this
		->get_tile_sheet_filename(), this->tile_types.getItem(index), col);
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
	const int size = this->tile_types.size();
	if (row < 0 || row > size) {
		return 1.0f;
	}
	return this->tile_types.getItem(row)->get_speed_mod();
}

int TileSet::get_edge_priority(const int row)
{
	const int size = this->tile_types.size();
	if (row < 0 || row > size) {
		return 0;
	}
	return this->tile_types.getItem(row)->get_edge_priority();
}

const int TileSet::get_tile_sheet_image_cols_by_index(const int index)
{
	const int size = this->tile_types.size();
	if (index >= 0 && index < size) {
		const std::string filename = this->get_full_tile_sheet_filename(index);
		ALLEGRO_BITMAP * bitmap = ImageLoader::get_instance().get_image(filename);
		return al_get_bitmap_width(bitmap) / TILE_SIZE;
	}
	return 0;
}

const bool TileSet::is_tile_npc_pathable(const int index, const bool obeys_rules)
{
	const int size = this->tile_types.size();
	if (index >= 0 && index < size) {
		return this->tile_types.getItem(index)->is_npc_pathable(obeys_rules);
	}
	return false;
}

const int TileSet::get_block_sheet_image_cols_by_index(const int index)
{
	const int size = this->block_types.size();
	if (index >= 0 && index < size) {
		const std::string filename = this->get_full_block_sheet_filename(index);
		ALLEGRO_BITMAP * bitmap = ImageLoader::get_instance().get_image(filename);
		return al_get_bitmap_width(bitmap) / TILE_SIZE;
	}
	return 0;
}

const int TileSet::get_entity_group_sheet_image_cols_by_index(const int index)
{
	const int size = this->entity_group_types.size();
	if (index >= 0 && index < size) {
		std::vector<EntityComponentData*> comp_data = this->get_entity_group_components(index);
		if (comp_data.size() > 0) {
			EntityComponentData * comp = comp_data[0];
			EntityGroupData* group_data = this->get_entity_group_data_by_index(index);
			const std::string filename = get_tile_sheet_filename()
				+ "/entity_groups/" + group_data->get_entity_group_name() + "_"
				+ comp->name.value();
			ALLEGRO_BITMAP * bitmap = ImageLoader::get_instance().get_image(filename);
			const std::pair<int, int> dim 
				= this->get_entity_group_image_dimensions_by_index(index);
			return al_get_bitmap_width(bitmap) / dim.first;
		}
	}
	return 0;
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
	return ImageLoader::get_instance().get_default_block_image(this->get_tile_sheet_filename(), this->block_types.getItem(index));
}

ALLEGRO_BITMAP * TileSet::get_block_bitmap_for_col(const int index, const int col)
{
	return ImageLoader::get_instance().get_block_image_for_col(this
		->get_tile_sheet_filename(), this->block_types.getItem(index), col);
}

const std::vector<std::pair<std::string, std::string>> TileSet::get_block_interact_action_data(const int index)
{
	std::vector<std::pair<std::string, std::string>> data;
	const int size = this->block_types.size();
	if (index < 0 || index > size) {
		return data;
	}
	return this->block_types.getItem(index)->get_block_interact_action_data();
}

const std::vector<std::pair<std::string, std::string>> TileSet::get_block_contact_action_data(const int index)
{
	std::vector<std::pair<std::string, std::string>> data;
	const int size = this->block_types.size();
	if (index < 0 || index > size) {
		return data;
	}
	return this->block_types.getItem(index)->get_block_contact_action_data();
}

const std::vector<std::pair<std::string, std::string>> TileSet::get_block_load_day_action_data(const int index)
{
	std::vector<std::pair<std::string, std::string>> data;
	const int size = this->block_types.size();
	if (index < 0 || index > size) {
		return data;
	}
	return this->block_types.getItem(index)->get_block_load_day_action_data();
}

const std::vector<EntitySpawnTileRule *> TileSet::get_block_spawn_tile_rules(const int index)
{
	std::vector<EntitySpawnTileRule *> data;
	const int size = this->block_types.size();
	if (index < 0 || index > size) {
		return data;
	}
	return this->block_types.getItem(index)->get_block_spawn_tile_rules();
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
	return this->get_tile_sheet_filename() + "/blocks/" + this->block_types.getItem(index)->get_entity_data_key();
}

std::string TileSet::get_block_key(const int index)
{
	return this->block_types.getItem(index)->get_entity_data_key();
}

bool TileSet::get_block_solid(const int row)
{
	const int size = this->block_types.size();
	if (row < 0 || row > size) {
		return false;
	}
	return this->block_types.getItem(row)->solid.value();
}

bool TileSet::get_block_visible(const int row)
{
	const int size = this->block_types.size();
	if (row < 0 || row > size) {
		return false;
	}
	return this->block_types.getItem(row)->visible.value();
}

std::map<std::string, int> TileSet::get_block_attributes(const int row)
{
	const int size = this->block_types.size();
	if (row < 0 || row > size) {
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
	return ImageLoader::get_instance().get_default_entity_group_image(this->get_tile_sheet_filename(), this->entity_group_types.getItem(index));
}

ALLEGRO_BITMAP * TileSet::get_entity_group_bitmap_for_col(const int index, const int col)
{
	return ImageLoader::get_instance().get_entity_group_image_for_col(this
		->get_tile_sheet_filename(), this->entity_group_types.getItem(index), col);
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
	const int size = this->entity_group_types.size();
	if (row < 0 || row > size) {
		return std::pair<int, int>(0,0);
	}
	return this->entity_group_types.getItem(row)->get_root_offset();
}

const std::pair<int, int> TileSet::get_entity_group_collide_offset(const int row)
{
	const int size = this->entity_group_types.size();
	if (row < 0 || row > size) {
		return std::pair<int, int>(0, 0);
	}
	return this->entity_group_types.getItem(row)->get_collide_offset();
}

const std::pair<int, int> TileSet::get_entity_group_collide_dimensions(const int row)
{
	const int size = this->entity_group_types.size();
	if (row < 0 || row > size) {
		return std::pair<int, int>(0, 0);
	}
	return this->entity_group_types.getItem(row)->get_collide_dimensions();
}


std::pair<int, int> TileSet::get_entity_group_center_offset(const int row)
{
	const int size = this->entity_group_types.size();
	if (row < 0 || row > size) {
		return std::pair<int, int>(0, 0);
	}
	return this->entity_group_types.getItem(row)->get_center_offset();
}

std::vector<EntityComponentData*> TileSet::get_entity_group_components(const int row)
{
	const int size = this->entity_group_types.size();
	if (row < 0 || row > size) {
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

ALLEGRO_BITMAP * TileSet::get_full_tiled_image_sheet(const int index)
{
	return ImageLoader::get_instance().get_full_tiled_image_sheet(this->get_tile_sheet_filename(), this->tiled_image_types.getItem(index));
}

ALLEGRO_BITMAP * TileSet::get_default_tiled_image_bitmap(const int index)
{
	return ImageLoader::get_instance().get_default_tiled_image_image(this->get_tile_sheet_filename(), this->tiled_image_types.getItem(index));
}

TiledImageData * TileSet::get_tiled_image_data_by_index(const int index)
{
	const int size = this->tiled_image_types.size();
	for (int i = 0; i < size; i++) {
		TiledImageData *tid = this->tiled_image_types.getItem(i);
		if (tid->get_image_data_index() == index) {
			return tid;
		}
	}
	return NULL;
}

const std::string TileSet::get_tiled_image_name_by_index(const int index)
{
	const int size = this->tiled_image_types.size();
	for (int i = 0; i < size; i++) {
		TiledImageData *tid = this->tiled_image_types.getItem(i);
		if (tid->get_image_data_index() == index) {
			return tid->get_image_data_key();
		}
	}
	return NULL;
}

const std::string TileSet::get_full_tiled_image_sheet_filename(const int index)
{
	return this->get_tile_sheet_filename() + "/tiled_images/" + this->tiled_image_types.getItem(index)->get_image_data_key();
}

std::vector<std::string> TileSet::all_tiled_image_keys()
{
	std::vector<std::string> tiled_image_keys;
	const int size = this->tiled_image_types.size();
	for (int i = 0; i < size; i++) {
		TiledImageData * tiled_image_type = this->tiled_image_types.getItem(i);
		tiled_image_keys.push_back(tiled_image_type->get_image_data_key());
	}
	return tiled_image_keys;
}

const std::string TileSet::get_full_spawner_sheet_filename(int index)
{
	return this->get_tile_sheet_filename() + "/spawners/" + this->spawner_types.getItem(index)->get_entity_data_key();
}

ALLEGRO_BITMAP * TileSet::get_spawner_bitmap_for_col(const int index, const int col)
{
	return ImageLoader::get_instance().get_spawner_image_for_col(this
		->get_tile_sheet_filename(), this->spawner_types.getItem(index), col);
}

EntityData * TileSet::get_spawner_data_by_index(const int index)
{
	return this->spawner_types.getItem(index);
}

const std::vector<std::string> TileSet::all_spawner_keys()
{
	std::vector<std::string> spawner_keys;
	const int size = this->spawner_types.size();
	for (int i = 0; i < size; i++) {
		EntityData * spawner_data = this->spawner_types.getItem(i);
		spawner_keys.push_back(spawner_data->get_entity_data_key());
	}
	return spawner_keys;
}

const std::string TileSet::get_full_path_node_sheet_filename(const int index)
{
	return this->get_tile_sheet_filename() + "/path_nodes/" + this->path_node_types.getItem(index)->get_entity_data_key();
}

ALLEGRO_BITMAP * TileSet::get_path_node_bitmap_for_col(const int index, const int col)
{
	return ImageLoader::get_instance().get_path_node_image_for_col(this
		->get_tile_sheet_filename(), this->path_node_types.getItem(index), col);
}

EntityData * TileSet::get_path_node_data_by_index(const int index)
{
	return this->path_node_types.getItem(index);
}

const std::vector<std::string> TileSet::all_path_node_keys()
{
	std::vector<std::string> node_keys;
	const int size = this->path_node_types.size();
	for (int i = 0; i < size; i++) {
		EntityData * node_data = this->path_node_types.getItem(i);
		node_keys.push_back(node_data->get_entity_data_key());
	}
	return node_keys;
}

const std::string TileSet::get_full_location_marker_sheet_filename(const int index)
{
	return this->get_tile_sheet_filename() + "/location_markers/" + this->location_marker_types.getItem(index)->get_entity_data_key();
}

ALLEGRO_BITMAP * TileSet::get_location_marker_bitmap_for_col(const int index, const int col)
{
	return ImageLoader::get_instance().get_location_marker_image_for_col(this
		->get_tile_sheet_filename(), this->location_marker_types.getItem(index), col);
}

EntityData * TileSet::get_location_marker_data_by_index(const int index)
{
	return this->location_marker_types.getItem(index);
}

const std::vector<std::string> TileSet::all_location_marker_keys()
{
	std::vector<std::string> marker_keys;
	const int size = this->location_marker_types.size();
	for (int i = 0; i < size; i++) {
		EntityData * marker_data = this->location_marker_types.getItem(i);
		marker_keys.push_back(marker_data->get_entity_data_key());
	}
	return marker_keys;
}
