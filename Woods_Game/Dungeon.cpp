#include "Dungeon.h"

Dungeon::Dungeon()
{
}

Dungeon::Dungeon(int width, int height)
{
	for (int y = 0; y < height; y++) {
		dungeon_grid.push_back(std::vector<Level*>());
		for (int x = 0; x < width; x++) {
			dungeon_grid[y].push_back(NULL);
		}
	}
	//TODO: add platforms differently using gameimagemanager
	//TEMP
	add_level(new Level("temp_level_A_1", "dungeon_1", "temp_level_A_1", 0, 0, 1, 1));
	//add_level(new Level("temp_level_2", "dungeon_1", "temp_level_2", 1, 0, 2, 1));	//TODO: fix level 2 map
	//add_level(new Level("temp_map_1", "Level_1", 0, 0, 1, 1));
	//add_level(new Level("temp_map_1", "Level_2", 1, 0, 2, 1));
	//TEMP
}


Dungeon::~Dungeon()
{
	/*
	std::vector<std::string> deleted_level_coords;	
	for (int y = 0; y < 0 + dungeon_grid.size(); y++) {
		for (int x = 0; x < 0+dungeon_grid[y].size(); x++) {
			std::string pos_str = x + "," + y;
			
			if (dungeon_grid[y][x]) {
				if (std::find(deleted_level_coords.begin(), deleted_level_coords.end(), pos_str) == deleted_level_coords.end()) {
					delete dungeon_grid[y][x];
					dungeon_grid[y][x] = NULL;
					deleted_level_coords.push_back(pos_str);
				}
			}
		}
		dungeon_grid[y].clear();
		dungeon_grid[y].swap(std::vector<Level*>());
	}
	dungeon_grid.clear();
	dungeon_grid.swap(std::vector<std::vector<Level*>>());
	*/
	//TODO: need to delete levels here, but be careful not to delete duplicates
}

void Dungeon::unload_content()
{
	//std::cout << "unloading content" << std::endl;
	std::vector<std::string> deleted_level_coords;
	for (int y = 0; y < 0 + dungeon_grid.size(); y++) {
		for (int x = 0; x < 0 + dungeon_grid[y].size(); x++) {
			std::string pos_str = std::to_string(x) + "," + std::to_string(y);
			//std::cout << "pos_str: " << pos_str << std::endl;
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
}

void Dungeon::load_images(ImageLoader& loader)
{
	//current_level->load_images(loader);
}

void Dungeon::add_level(Level* level)
{
	//std::cout << "width: " << level.get_dimensions().first << std::endl;
	int x_pos = level->get_grid_x();
	int y_pos = level->get_grid_y();
	int width = level->get_grid_width();
	int height = level->get_grid_height();
	//TODO: come up with a set of standard dimensions for a "room" and ensure that levels passed here match those dimensions)
	// multiply by 1.0 to avoid warnings
	if(y_pos + height >= 1.0*dungeon_grid.size())
		dungeon_grid.reserve(y_pos + height);
	for (int y = y_pos; y < y_pos + height; y++) {
		if (x_pos + width >= 1.0*dungeon_grid[y].size())
			dungeon_grid[y].reserve(x_pos + width);
		for (int x = x_pos; x < x_pos + width; x++) {
			dungeon_grid[y][x] = level;
		}
	}
	bool use_xml = true;	//temp-- set this some other way (maybe temporary menu
	if (use_xml) {
		level->load_from_xml();
	} else {
		level->load_from_map();
	}

	//level.set_dungeon_grid(&dungeon_grid);
}

Level * Dungeon::level_at(int x, int y)
{
	//TODO: out of bounds check
	Level* level = dungeon_grid[y][x];
	if (level == NULL)
		std::cout << "empty level " << std::endl;
	return level;
}
/*
void Dungeon::set_current_level(int x, int y)
{
	//TODO: out of bounds check
	//current_level = dungeon_grid[y][x];
}

void Dungeon::set_player(Player player)
{
	current_level->set_player(&player);
}


Level * Dungeon::get_current_level()
{
	return current_level;
}

Player * Dungeon::get_player()
{
	return current_level->get_player();
}

std::pair<int, int> Dungeon::get_current_level_dimensions()
{
	return current_level->get_dimensions();
}
*/