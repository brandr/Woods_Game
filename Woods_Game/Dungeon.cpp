#include "Dungeon.h"
#include "World.h"

Dungeon::Dungeon()
{
}

Dungeon::Dungeon(const std::string dungeon_name, const std::string filepath, const int default_level_width, const int default_level_height)
{
	this->dungeon_name = dungeon_name;
	this->intialize_levels(filepath, default_level_width, default_level_height);
}

Dungeon::~Dungeon()
{
	
}

void Dungeon::unload_content()
{
	std::vector<std::string> deleted_level_coords;
	for (int y = 0; y < 0 + dungeon_grid.size(); y++) {
		for (int x = 0; x < 0 + dungeon_grid[y].size(); x++) {
			std::string pos_str = std::to_string(x) + "," + std::to_string(y);
			Level *level = dungeon_grid[y][x];
			if (level) {
				if (std::find(deleted_level_coords.begin(), deleted_level_coords.end(), pos_str) == deleted_level_coords.end()) {
					for (int y2 = y; y2 < y + level->get_grid_height(); y2++){
						for (int x2 = x; x2 < x + level->get_grid_width(); x2++) {
							deleted_level_coords.push_back(std::to_string(x2) + "," + std::to_string(y2));
						}
					}
					dungeon_grid[y][x]->unload_content();
					delete dungeon_grid[y][x];
					dungeon_grid[y][x] = NULL;
				}
			}
		}
		dungeon_grid[y].clear();
		dungeon_grid[y].swap(std::vector<Level*>());
	}
	dungeon_grid.clear();
	dungeon_grid.swap(std::vector<std::vector<Level*>>());
	deleted_level_coords.clear();
	deleted_level_coords.swap(std::vector<std::string > ());
	const int level_size = this->level_list.size();
	for (int i = 0; i < level_size; i++) {
		Level * l = this->level_list[i];
		if (l != NULL) {
			l->unload_content();
			l = NULL;
		}
	}
	this->level_list.clear();
}

void Dungeon::intialize_levels(const int default_level_width, const int default_level_height)
{
	this->intialize_levels("resources/load/dungeons/", default_level_width, default_level_height);
}

void Dungeon::intialize_levels(const std::string filepath, int default_level_width, const int default_level_height)
{
	FileManager filemanager;
	std::string filename = filepath + dungeon_name;
	std::vector<std::string> level_keys = filemanager.all_xml_keys(filename, "SerializableClass", "Level", "LevelKey");
	const int size = level_keys.size();
	for (std::string level_key : level_keys) {
		Level *level = new Level(level_key, dungeon_name, level_key);
		filemanager.load_xml_content(level, filename, "SerializableClass", "LevelKey", level_key);
		level->intialize_dimensions(default_level_width, default_level_height);
		add_level(level);
	}
}

void Dungeon::generate_levels()
{
	for (Level * level : this->level_list) {
		level->generate_level();
	}
}

ALLEGRO_BITMAP * Dungeon::generate_map_image(const int standard_level_width, const int standard_level_height)
{
	const int grid_height = this->dungeon_grid.size();
	int grid_width = -1;
	for (int y = 0; y < grid_height; y++) {
		const int row_width = this->dungeon_grid[y].size();
		if (grid_width < row_width) {
			grid_width = row_width;
		}
	}

	const int cell_width = standard_level_width / LEVEL_MAP_SCALE;
	const int cell_height = standard_level_height/ LEVEL_MAP_SCALE;

	const int image_width = grid_width * cell_width;
	const int image_height = grid_height * cell_height;

	ALLEGRO_BITMAP * map_bitmap = al_create_bitmap(image_width, image_height);
	ALLEGRO_BITMAP *display = al_get_target_bitmap();
	al_set_target_bitmap(map_bitmap);
	al_clear_to_color(al_map_rgba(255, 255, 255, 255));
	for (int y = 0; y < grid_height; y++) {
		for (int x = 0; x < grid_width; x++) {
			if (x >= this->dungeon_grid[y].size()) {
				continue;
			}
			Level * level = this->dungeon_grid[y][x];
			if (level == NULL) {
				continue;
			}
			ALLEGRO_BITMAP * cell_bitmap = level->generate_cell_map_image(cell_width, cell_height, x, y);
			al_draw_bitmap(cell_bitmap, x * cell_width, y * cell_height, 0);
		}
	}
	al_set_target_bitmap(display);
	return map_bitmap;
}

Dungeon * Dungeon::load_dungeon(const std::string dungeon_name, const int default_level_width, const int default_level_height)
{
	Dungeon * dungeon = new Dungeon(dungeon_name, "resources/load/dungeons/", default_level_width, default_level_height);
	return dungeon;
}

void Dungeon::load_images(ImageLoader& loader)
{
}

void Dungeon::update_new_day(World * world, Player * player)
{
	for (Level *level : this->level_list) {
		level->update_new_day(world, player);
	}
}

void Dungeon::add_level(Level* level)
{
	int x_pos = level->get_grid_x();
	int y_pos = level->get_grid_y();
	int width = level->get_grid_width();
	int height = level->get_grid_height();
	// multiply by 1.0 to avoid warnings
	if (x_pos >= 0 && y_pos >= 0) {
		while (y_pos + height >= 1.0*dungeon_grid.size()) {
			dungeon_grid.push_back(std::vector<Level*>());
		}
		for (int y = y_pos; y < y_pos + height; y++) {
			while (x_pos + width >= 1.0*dungeon_grid[y].size()) {
				dungeon_grid[y].push_back(NULL);
			}
			for (int x = x_pos; x < x_pos + width; x++) {
				dungeon_grid[y][x] = level;
			}
		}
	}
	this->level_list.push_back(level);
	level->load_from_xml();
}

void Dungeon::remove_level(int index)
{
	this->level_list.erase(this->level_list.begin() + index);
}

Level * Dungeon::level_at(int x, int y)
{
	Level* level = NULL;
	if (x >= 0 && y >= 0) {
		level = dungeon_grid[y][x];
	}
	if (level == NULL) {
		std::cout << "empty level " << std::endl;
	}
	return level;
}

Level * Dungeon::level_at_index(const int index)
{
	return this->level_list[index];
}

Level * Dungeon::level_with_name(std::string name)
{
	for (Level * level : this->level_list) {
		if (name == level->get_filename())
		{
			return level;
		}
	}
	return NULL;
}

Level * Dungeon::find_level_with_spawn_key(const std::string spawn_key)
{
	for (Level * level : this->level_list) {
		if (level->has_spawner_for_key(spawn_key))
		{
			return level;
		}
	}
	return NULL;
}

Level * Dungeon::find_level_with_path_node_key(const std::string node_key)
{
	for (Level * level : this->level_list) {
		if (level->has_path_node_for_key(node_key))
		{
			return level;
		}
	}
	return NULL;
}

Level * Dungeon::find_any_level_with_node()
{
	for (Level * level : this->level_list) {
		if (level->has_any_path_node())
		{
			return level;
		}
	}
	return NULL;
}

PathNode * Dungeon::find_path_node_with_key(const std::string node_key)
{
	for (Level * level : this->level_list) {
		PathNode * node = level->find_path_node_with_key(node_key);
		if (node)
		{
			return node;
		}
	}
	return NULL;
}

std::vector<LocationMarker*> Dungeon::get_all_location_markers()
{
	std::vector<LocationMarker*> markers;
	for (Level * level : this->level_list) {
		std::vector<LocationMarker *> level_markers = level->get_location_markers();
		for (LocationMarker * lm : level_markers) {
			markers.push_back(lm);
		}
	}
	return markers;
}

LocationMarker * Dungeon::location_marker_matching_level(Level * l)
{
	for (Level * level : this->level_list) {
		LocationMarker * lm = level->find_location_marker_matching_level(l->get_filename());
		if (lm)
		{
			return lm;
		}
	}
	return NULL;
}

void Dungeon::set_dungeon_name(std::string name)
{
	this->dungeon_name = name;
}

std::string Dungeon::get_dungeon_name()
{
	return this->dungeon_name;
}

std::vector<Level*> Dungeon::get_level_list()
{
	return this->level_list;
}

const int Dungeon::get_level_list_count()
{
	return this->level_list.size();
}

void Dungeon::clear_all_beings()
{
	for (Level * l : this->level_list) {
		l->clear_all_beings();
	}
}

void Dungeon::spawn_critters(World * world, GlobalTime * time)
{
	for (Level * l : this->level_list) {
		l->generate_critters(world, time);
	}
}
