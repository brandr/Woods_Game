#include "LevelEditorDataManager.h"


void LevelEditorDataManager::reset_active_levels()
{
	for (int i = 0; i < this->active_levels.size(); i++) {
		this->active_levels[i]->unload_content();
		this->active_levels[i].release();
	}
	this->active_levels.clear();
	if (this->active_dungeon != NULL) {
		std::vector<Level*> levels = this->active_dungeon->get_level_list();
		for (Level * level : levels) {
			this->active_levels.push_back(unique_ptr<Level, LevelDeleter>(level));
		}
	}
}

LevelEditorDataManager & LevelEditorDataManager::get_instance()
{
	static LevelEditorDataManager manager;
	return manager;
}

void LevelEditorDataManager::save_dungeon(std::string dungeon_name)
{
	if (this->active_dungeon.get() != NULL) {
		if (this->active_dungeon->get_dungeon_name() != dungeon_name) {
			this->active_dungeon->set_dungeon_name(dungeon_name);
		}
		const std::string filename = "resources/load/dungeons/" + dungeon_name;
		file_manager.create_xml_file(filename);
		std::vector<Level*> levels = this->active_dungeon->get_level_list();
		for (Level *level : levels) {
			std::map<std::string, std::string> attributes;
			attributes["Type"] = "Level";
			attributes["Version"] = "1";
			attributes["LevelKey"] = level->get_filename();
			file_manager.save_xml_content(filename, "SerializableClass", attributes);
			const std::string level_xml = level->toXML();
			file_manager.replace_xml_content(filename, "SerializableClass", "LevelKey", level->get_filename(), level_xml);
		}
	}
}

void LevelEditorDataManager::load_dungeon(std::string dungeon_path)
{
	size_t pos = 0;
	while ((pos = dungeon_path.find("\\")) != std::string::npos) {
		dungeon_path.erase(0, pos + 1);
	}
	const std::string dungeon_name = dungeon_path;
	if (this->active_dungeon != NULL) {
		this->active_dungeon->unload_content();
		this->active_dungeon.release();
	}
	this->active_dungeon.reset(Dungeon::load_dungeon(dungeon_name));
	this->reset_active_levels();
}

void LevelEditorDataManager::add_dungeon(std::string dungeon_name)
{
	if (dungeon_name.length() > 0) {
		const std::string dungeon_filepath = "resources/load/dungeons/" + dungeon_name + ".xml";
		FILE * filepoint;
		errno_t err;
		if (err = fopen_s(&filepoint, dungeon_filepath.c_str(), "r")) {
			Dungeon *dungeon = new Dungeon();
			dungeon->set_dungeon_name(dungeon_name);
			if (this->active_dungeon != NULL) {
				this->active_dungeon->unload_content();
				this->active_dungeon.release();
			}
			this->active_dungeon.reset(dungeon);
			this->save_dungeon(this->active_dungeon->get_dungeon_name());
		} else {
			// file	already exists, so don't do anything
		}
	}
}

void LevelEditorDataManager::delete_dungeon(std::string dungeon_path)
{
	
	if (dungeon_path.length() > 0) {
		std::string dungeon_name = dungeon_path;
		size_t pos = 0;
		while ((pos = dungeon_name.find("\\")) != std::string::npos) {
			dungeon_name.erase(0, pos + 1);
		}
		const std::string dungeon_filepath = dungeon_path + ".xml";
		if (this->active_dungeon != NULL && dungeon_name == this->active_dungeon->get_dungeon_name()) {
			this->active_dungeon->unload_content();
			this->active_dungeon.release();
			this->reset_active_levels();
		}
		remove(dungeon_filepath.c_str());
	}
}

void LevelEditorDataManager::set_active_level_attributes(std::string level_name, std::pair<int, int> pos, std::pair<int, int> dimensions)
{
	if (this->selected_level_index >= 0 
		&& this->active_levels[this->selected_level_index] != NULL) {
		if (level_name.length() > 0) {
			this->active_levels[this->selected_level_index]->set_filename(level_name);
		}
		this->active_levels[this->selected_level_index]->set_grid_x(pos.first);
		this->active_levels[this->selected_level_index]->set_grid_y(pos.second);
		this->active_levels[this->selected_level_index]->set_grid_width(dimensions.first);
		this->active_levels[this->selected_level_index]->set_grid_height(dimensions.second);
	}
}

void LevelEditorDataManager::add_level(std::string level_name)
{
	if (level_name.length() > 0 && this->has_active_dungeon()) {
		const int size = this->active_levels.size();
		for (int i = 0; i < size; i++) {
			if (this->active_levels[i]->get_filename() == level_name) {
				return;
			}
		}
		Level *level = new Level(level_name, this->get_active_dungeon_name(), level_name);
		level->set_grid_x(0), level->set_grid_y(0), level->set_grid_width(1), level->set_grid_height(1);
		level->set_tileset_key(this->get_selected_tileset_name());
		level->load_tileset();
		level->intialize_dimensions();
		level->initialize_empty();
		this->active_dungeon->add_level(level);
		this->active_levels.push_back(unique_ptr<Level, LevelDeleter>(level));
	}
}

//FIXME: are we actually using the level name?
void LevelEditorDataManager::delete_level(std::string level_name)
{
	if (level_name.length() > 0 && this->has_active_dungeon()) {
		const int size = this->active_levels.size();
		this->active_dungeon->remove_level(this->selected_level_index);
		this->reset_active_levels();
	}
}

void LevelEditorDataManager::reset_tile_edges()
{
	if (this->has_selected_level()) {
		this->active_levels[this->selected_level_index]->remove_tile_edges();
		this->active_levels[this->selected_level_index]->load_tile_edges();
		this->active_levels[this->selected_level_index]->draw_tile_edge_bitmaps();
	}
}

void LevelEditorDataManager::load_all_tilesets()
{
	for (int i = 0; i < this->active_tilesets.size(); i++) {
		this->active_tilesets[i]->unload_content();
		this->active_tilesets[i].release();
	}
	this->active_tilesets.clear();
	FileManager filemanager;
	const std::string filename = "resources/load/tilesets";
	std::vector<std::string> tileset_keys = filemanager.all_xml_keys(filename, "SerializableClass", "TileSet", "TilesetKey");
	for (std::string tileset_key : tileset_keys) {
		TileSet *tile_set = new TileSet();
		filemanager.load_xml_content(tile_set, filename, "SerializableClass", "TilesetKey", tileset_key);
		tile_set->set_tileset_key(tileset_key);
		tile_set->load_sheet_images();
		this->active_tilesets.push_back(unique_ptr<TileSet, TilesetDeleter>(tile_set));
	}
}

bool LevelEditorDataManager::update_selected_tileset(int index)
{
	bool update_tileset = false;
	if (index >= 0) {
		if (index != this->selected_tileset_index) {
			update_tileset = true;
			this->set_selected_tileset_index(index);
		}
	}
	return update_tileset;
}

bool LevelEditorDataManager::update_selected_tileset_tile(int index)
{
	bool update_tile = false;
	if (index >= 0) {
		if (index != this->selected_tileset_tile_index) {
			update_tile = true;
			this->selected_tileset_tile_index = index;
		}
	}
	return update_tile;
}

bool LevelEditorDataManager::update_selected_tileset_block(int index)
{
	bool update_block = false;
	if (index >= 0) {
		if (index != this->selected_tileset_block_index) {
			update_block = true;
			this->selected_tileset_block_index = index;
		}
	}
	return update_block;
}

bool LevelEditorDataManager::update_selected_tileset_entity_group(int index)
{
	bool update_eg = false;
	if (index >= 0) {
		if (index != this->selected_tileset_entity_group_index) {
			update_eg = true;
			this->selected_tileset_entity_group_index = index;
		}
	}
	return update_eg;
}

bool LevelEditorDataManager::update_selected_tileset_tiled_image(int index)
{
	bool update_image = false;
	if (index >= 0) {
		if (index != this->selected_tileset_tile_image_index) {
			update_image = true;
			this->selected_tileset_tile_image_index = index;
		}
	}
	return update_image;
}

agui::Allegro5Image * LevelEditorDataManager::load_image_layer(std::string layer)
{
	const std::string level_name = this->get_selected_level_name();
	agui::Allegro5Image *image_layer = new agui::Allegro5Image();
	const int pixel_width = this->active_levels[this->selected_level_index]->get_width();
	const int pixel_height = this->active_levels[this->selected_level_index]->get_height();
	ALLEGRO_BITMAP* image_bitmap = al_create_bitmap(pixel_width, pixel_height);
	if (TILE_LAYER == layer) {
		this->active_levels[this->selected_level_index]->draw_tiles_onto_bitmap(image_bitmap);
	}
	else if (BLOCK_LAYER == layer) {
		this->active_levels[this->selected_level_index]->draw_blocks_onto_bitmap(image_bitmap);
	}
	else if (ENTITY_GROUP_LAYER == layer) {
		this->active_levels[this->selected_level_index]->draw_entity_groups_onto_bitmap(image_bitmap);
	}
	else if (TILED_IMAGE_LAYER == layer) {
		this->active_levels[this->selected_level_index]->draw_tiled_images_onto_bitmap(image_bitmap);
	}
	else if (GRID_LINES_LAYER == layer) {
		ALLEGRO_BITMAP *display = al_get_target_bitmap();
		al_set_target_bitmap(image_bitmap);
		const int grid_width = pixel_width / TILE_SIZE;
		const int grid_height = pixel_height / TILE_SIZE;
		// vertical lines
		for (int x = 0; x <= grid_width; x++) {
			al_draw_line(x*TILE_SIZE, 0, x*TILE_SIZE, pixel_height, al_map_rgb(20, 20, 20), 2.0);
		}
		// horizontal lines
		for (int y = 0; y <= grid_height; y++) {
			al_draw_line(0, y*TILE_SIZE, pixel_width, y*TILE_SIZE, al_map_rgb(20, 20, 20), 2.0);
		}
		al_set_target_bitmap(display);
	}
	image_layer->setBitmap(image_bitmap, true);
	return image_layer;
}

void LevelEditorDataManager::set_selected_level_index(int index)
{
	this->selected_level_index = index;
}

int LevelEditorDataManager::get_selected_level_index()
{
	return this->selected_level_index;
}

void LevelEditorDataManager::set_selected_object_tab_index(int index)
{
	this->selected_object_tab_index = index;
}

int LevelEditorDataManager::get_selected_object_tab_index()
{
	return this->selected_object_tab_index;
}

void LevelEditorDataManager::set_selected_object_select_index(int index)
{
	this->selected_object_select_index = index;
}

int LevelEditorDataManager::get_selected_object_select_index()
{
	return this->selected_object_select_index;
}

bool LevelEditorDataManager::add_level_object(std::pair<int, int> tile_pos)
{
	bool update = false;
	if (this->has_active_dungeon() 
		&& this->has_selected_level()
		&& this->has_selected_tileset()) {
		const std::pair<int, int> level_pixel_dim = this->selected_level_pixel_dimensions();
		const std::pair<int, int> level_tile_dim(level_pixel_dim.first / TILE_SIZE, 
			level_pixel_dim.second / TILE_SIZE);
		if (tile_pos.first >= 0 && tile_pos.first < level_tile_dim.first
			&& tile_pos.second >= 0 && tile_pos.second < level_tile_dim.second) {
			if (this->selected_object_tab_index >= 0 && this->selected_object_select_index >= 0) {
				switch (this->selected_object_tab_index) {
				case OBJECT_TYPE_TILE:
					this->set_selected_level_tile(this->selected_object_select_index, tile_pos);
					update = true;
					break;
				case OBJECT_TYPE_BLOCK:
					this->set_selected_level_block(this->selected_object_select_index, tile_pos);
					update = true;
					break;
				case OBJECT_TYPE_ENTITY_GROUP:
					this->add_selected_level_entity_group(this->selected_object_select_index, tile_pos);
					update = true;
					break;
				default:
					break;
				}
			}
		}
	}
	return update;
}

bool LevelEditorDataManager::delete_level_object(std::pair<int, int> tile_pos)
{
	bool update = false;
	if (this->has_active_dungeon() 
		&& this->has_selected_level()
		&& this->has_selected_tileset()) {
		const std::pair<int, int> level_pixel_dim = this->selected_level_pixel_dimensions();
		const std::pair<int, int> level_tile_dim(level_pixel_dim.first / TILE_SIZE,
			level_pixel_dim.second / TILE_SIZE);
		if (tile_pos.first >= 0 && tile_pos.first < level_tile_dim.first
			&& tile_pos.second >= 0 && tile_pos.second < level_tile_dim.second) {
			if (this->selected_object_tab_index >= 0) {
				if (this->selected_object_tab_index >= 0) {
					switch (this->selected_object_tab_index) {
					case OBJECT_TYPE_TILE:
						this->delete_level_tile(tile_pos);
						update = true;
						break;
					case OBJECT_TYPE_BLOCK:
						this->delete_level_block(tile_pos);
						update = true;
						break;
					case OBJECT_TYPE_ENTITY_GROUP:
						this->delete_level_entity_group(tile_pos);
						update = true;
						break;
					default:
						break;
					}
				}
			}
		}
	}
	return update;
}

void LevelEditorDataManager::select_level_object(std::pair<int, int> tile_pos)
{
	//TODO: need to track both selected pos (-1, -1 if nothing) and selected entity type
	this->selected_object_grid_index = this->selected_object_tab_index;
	this->selected_object_grid_pos = tile_pos;
}


void LevelEditorDataManager::set_selected_level_tile(int index, std::pair<int, int> pos)
{
	//TEMP: need to set spritesheet pos here
	this->active_levels[this->selected_level_index]->replace_tile(index, std::pair<int, int>(0, 0), pos);
}

void LevelEditorDataManager::set_selected_level_block(int index, std::pair<int, int> pos)
{
	//TEMP: need to set spritesheet pos here
	this->active_levels[this->selected_level_index]->replace_block(index, std::pair<int, int>(0, 0), pos);
}

void LevelEditorDataManager::add_selected_level_entity_group(int index, std::pair<int, int> pos)
{
	//TEMP: need to set spritesheet pos here
	this->active_levels[this->selected_level_index]->add_entity_group(index, std::pair<int, int>(0, 0), pos);
}

void LevelEditorDataManager::delete_level_tile(std::pair<int, int> pos)
{
	this->active_levels[this->selected_level_index]->remove_tile(pos);
}

void LevelEditorDataManager::delete_level_block(std::pair<int, int> pos)
{
	this->active_levels[this->selected_level_index]->remove_block(pos);
}

void LevelEditorDataManager::delete_level_entity_group(std::pair<int, int> pos)
{
	this->active_levels[this->selected_level_index]->remove_entity_group(pos);
}

std::string LevelEditorDataManager::get_active_dungeon_name()
{
	return this->active_dungeon != NULL ? this->active_dungeon->get_dungeon_name() : "";
}

bool LevelEditorDataManager::has_active_dungeon()
{
	return this->active_dungeon != NULL;
}

std::vector<std::string> LevelEditorDataManager::get_active_level_names()
{
	std::vector<std::string> names;
	for (int i = 0; i < this->active_levels.size(); i++) {
		const std::string filename = this->active_levels[i]->get_filename();
		names.push_back(filename);
	}
	return names;
}

std::string LevelEditorDataManager::get_selected_level_name()
{
	return this->active_levels[this->selected_level_index] != NULL 
		? this->active_levels[this->selected_level_index]->get_filename() : "";
}

std::pair<int, int> LevelEditorDataManager::selected_level_grid_pos()
{
	if (this->active_levels[this->selected_level_index] != NULL) {
		return std::pair<int, int>(this->active_levels[this->selected_level_index]->get_grid_x(),
			this->active_levels[this->selected_level_index]->get_grid_y());
	}
	return std::pair<int, int>();

}

std::pair<int, int> LevelEditorDataManager::selected_level_grid_dimensions()
{
	if (this->active_levels[this->selected_level_index] != NULL) {
		return std::pair<int, int>(this->active_levels[this->selected_level_index]->get_grid_width(),
			this->active_levels[this->selected_level_index]->get_grid_height());
	}
	return std::pair<int, int>();
}

std::pair<int, int> LevelEditorDataManager::selected_level_pixel_dimensions()
{
	if (this->active_levels[this->selected_level_index] != NULL) {
		return std::pair<int, int>(this->active_levels[this->selected_level_index]->get_width(),
			this->active_levels[this->selected_level_index]->get_height());
	}
	return std::pair<int, int>();
}

void LevelEditorDataManager::set_selected_tileset_index(int index)
{
	this->selected_tileset_index = index;
}

std::vector<std::string> LevelEditorDataManager::all_tileset_names()
{
	std::vector<std::string> tileset_names;
	const int size = this->active_tilesets.size();
	for (int i = 0; i < size; i++) {
		tileset_names.push_back(this->active_tilesets[i]->get_tileset_key());
	}
	return tileset_names;
}

std::vector<std::string> LevelEditorDataManager::all_selected_tile_keys()
{

	if (this->has_selected_tileset()) {
		return this->active_tilesets[this->selected_tileset_index]->all_tile_keys();
	}
	return std::vector<std::string>();
}

std::vector<std::string> LevelEditorDataManager::all_selected_block_keys()
{
	if (this->has_selected_tileset()) {
		return this->active_tilesets[this->selected_tileset_index]->all_block_keys();
	}
	return std::vector<std::string>();
}

std::vector<std::string> LevelEditorDataManager::all_selected_entity_group_keys()
{
	if (this->has_selected_tileset()) {
		return this->active_tilesets[this->selected_tileset_index]->all_entity_group_keys();
	}
	return std::vector<std::string>();
}

std::string LevelEditorDataManager::get_selected_tileset_name()
{
	return this->get_tileset_name(this->selected_tileset_index);
}

std::string LevelEditorDataManager::get_tileset_name(int index)
{
	if (index >= 0 && index < this->active_tilesets.size()) {
		return this->active_tilesets[index]->get_tileset_key();
	}
	return NULL;
}

ALLEGRO_BITMAP * LevelEditorDataManager::get_default_tile_bitmap(int index)
{
	if (index >= 0 
		&&this->active_tilesets[this->selected_tileset_index] != NULL) {
		return this->active_tilesets[this->selected_tileset_index]->get_default_tile_bitmap(index);
	}
	return NULL;
}

ALLEGRO_BITMAP * LevelEditorDataManager::get_default_block_bitmap(int index)
{
	if (this->active_tilesets[this->selected_tileset_index] != NULL) {
		return this->active_tilesets[this->selected_tileset_index]->get_default_block_bitmap(index);
	}
	return NULL;
}

ALLEGRO_BITMAP * LevelEditorDataManager::get_default_entity_group_bitmap(int index)
{
	if (this->active_tilesets[this->selected_tileset_index] != NULL) {
		return this->active_tilesets[this->selected_tileset_index]->get_default_entity_group_bitmap(index);
	}
	return NULL;
}

ALLEGRO_BITMAP * LevelEditorDataManager::get_default_tiled_image_bitmap(int index)
{
	if (this->active_tilesets[this->selected_tileset_index] != NULL) {
		return this->active_tilesets[this->selected_tileset_index]->get_default_tiled_image_bitmap(index);
	}
	return NULL;
}

std::pair<int, int> LevelEditorDataManager::get_entity_group_image_dimensions_by_index(int index)
{
	if (this->active_tilesets[this->selected_tileset_index] != NULL) {
		return this->active_tilesets[this->selected_tileset_index]
			->get_entity_group_image_dimensions_by_index(index);
	}
	return std::pair<int, int>();
}

bool LevelEditorDataManager::has_selected_level()
{
	return this->selected_level_index >= 0 
		&& this->active_levels[this->selected_level_index] != NULL;
}

bool LevelEditorDataManager::has_selected_tileset()
{
	return this->selected_tileset_index >= 0
		&& this->active_tilesets[this->selected_tileset_index] != NULL;
}

void LevelDeleter::operator()(Level * p)
{
	if (p != NULL) {
		p->unload_content();
		delete p;
	}
}

void TilesetDeleter::operator()(TileSet * p)
{
	if (p != NULL) {
		p->unload_content();
		delete p;
	}
}
