#define _CRTDBG_MAP_ALLOC
#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
//TODO: replace new with DBG_NEW when it is used
#else
#define DBG_NEW new
#endif
#define NOMINMAX
#include "GameImageManager.h"
//#include "Level.h"//TEMP


std::pair<int, int> GameImageManager::get_camera_offset(ALLEGRO_DISPLAY *display)
{
	int x = 0, y = 0;
	if (player) {
		if (current_level->get_width() <= al_get_display_width(display))
			x = 0;
		else {
			x = -1 * std::min((std::max(0.0f, player->get_x() - al_get_display_width(display) / 2)), (float)(current_level->get_width() - al_get_display_width(display)));
		}
		if (current_level->get_height() <= al_get_display_height(display))
			y = 0;
		else {
			y = -1 * std::min((std::max(0.0f, player->get_y() - al_get_display_height(display) / 2)), (float)(current_level->get_height() - al_get_display_height(display)));
		}
	}
	std::pair<int,int> off = std::make_pair(x, y);
	return off;
}

std::pair<int, int> GameImageManager::tile_image_offset(std::string tile_string)
{
	std::pair<int, int> tile;
	tile.first = atoi(tile_string.substr(0, tile_string.find(',')).c_str());
	tile.second = atoi(tile_string.substr(tile_string.find(',') + 1).c_str());
	return tile;
}

GameImageManager::GameImageManager()
{
}


GameImageManager::~GameImageManager()
{
}

void GameImageManager::load_content()
{
	current_level = world.get_current_dungeon()->level_at(0, 0);
	//TODO: load player through xml (and all other entities that start from tiles)
	load_player("resources/load/player.txt");


	//load_level_content("resources/load/player.txt", "", PLAYER);
	//TEMP
	//load_level_content("resources/load/temp_level_1.txt", "", PLATFORM);
	//TEMP
	//TODO: load dungeon content from somewhere
	//IDEA: put dungeon grid in this class instead of the dungeon class, and change it when changing world
	//TODO: load content for adjacent levels, or possibly for the current dungeon
}

void GameImageManager::set_game_mode(int game_mode)
{
	this->game_mode = game_mode;
}

int GameImageManager::get_game_mode()	//temp. figure out game mode some other way
{
	return game_mode;
}

void GameImageManager::load_level(int grid_x, int grid_y)
{
	Level *level = world.get_current_dungeon()->level_at(grid_x, grid_y);
	if (level) {
		load_level_from_map(level);
		current_level = level;
	}
	else
		std::cout << "ERROR: failed to load level." << std::endl; //TODO: error handling
}

void GameImageManager::load_player(std::string filename)
{
	std::vector<std::vector<std::string>> attributes;
	std::vector<std::vector<std::string>> contents;
	FileManager file_manager;
	file_manager.load_content(filename.c_str(), attributes, contents, "gameimage_properties");
	player = new Player();
	player->load_content(attributes[0], contents[0]);
	player->set_bitmap(ImageLoader::get_instance().get_current_image(player));
	attributes.clear(), contents.clear();
	file_manager.load_content(filename.c_str(), attributes, contents, "additional_masks");
	player->load_additional_masks(attributes[0], contents[0], "player");
	current_level->add_being(player);
}

void GameImageManager::load_level_content(std::string filename, std::string id, int type)
{
	//TODO: make a method that loads content into a level, not into the gamemanager itself
	//when loading into the gamemanager, can load into current_level
	std::vector<std::vector<std::string>> attributes;
	std::vector<std::vector<std::string>> contents;
	FileManager file_manager;
	if (id == "")
		file_manager.load_content(filename.c_str(), attributes, contents);
	else
		file_manager.load_content(filename.c_str(), attributes, contents, id);
	int size = attributes.size();
	for (int i = 0; i < size; i++) {
		GameImage *image;
		switch (type) {
		case PLAYER:
			image = new Player();
			player = dynamic_cast<Player*>(image);
			player->load_content(attributes[i], contents[i]);
			player->set_bitmap(ImageLoader::get_instance().get_current_image(player));
			current_level->add_being(player);
			break;
		}
	}
}

void GameImageManager::load_level_from_map(Level *level)
{
	//TODO?
}

void GameImageManager::load_player()
{
	
}

Player * GameImageManager::get_player()
{
	return player;
}

void GameImageManager::unload_content()
{
	world.unload_content();
	player = NULL;
}

void GameImageManager::unload_level_content()
{
}

void GameImageManager::update(std::map<int, bool> input_map, std::map<int, std::pair<float,float>> joystick_map)
{
	const int game_mode = get_game_mode();
	if (game_mode == MAIN_GAME_PAUSED) return;
	std::pair<int, int> dimensions = current_level->get_dimensions();
	if (player) {
		if (player->get_exit_level_flag()) {
			//TODO: check to make sure there is a next level in the given direction here
			change_player_level();
			return;
		}
		player->update_input(input_map, joystick_map, game_mode);
	}
	current_level->update(game_mode);

}

void GameImageManager::change_player_level()
{
	Level* next_level = NULL;
	int x = 0;
	int y = 0;
	int grid_x = current_level->get_grid_x();
	int grid_y = current_level->get_grid_y();
	int width = current_level->get_width();
	int height = current_level->get_height();

	enum { UP, DOWN, LEFT, RIGHT };
	int direction = 0;
	//up
	if (player->get_y() + player->get_height() < 0) {
		x = grid_x, y = grid_y - 1;
		direction = UP;
	}

	//down
	else if (player->get_y() > height) {
		x = grid_x, y = grid_y + current_level->get_grid_height();
		direction = DOWN;
	}
	//left
	else if (player->get_x() + player->get_width() < 0) {
		x = grid_x - 1, y = grid_y;
		direction = LEFT;
	}

	//right
	else if (player->get_x() > width) {
		x = grid_x + current_level->get_grid_width(), y = grid_y;
		direction = RIGHT;
	}
	else
		std::cout << "ERROR: no direction " << std::endl;
	
	current_level->remove_player();
	next_level = world.get_current_dungeon()->level_at(x, y);
	next_level->add_being(player);
	int px = 0, py = 0;
	const int level_width = Level::STANDARD_LEVEL_GRID_WIDTH, level_height = Level::STANDARD_LEVEL_GRID_HEIGHT;
	const float player_offset_x = fmod(player->get_x(), level_width), player_offset_y = fmod(player->get_y(), level_height);
	int grid_offset_x = next_level->get_grid_x() - current_level->get_grid_x();
	int grid_offset_y = next_level->get_grid_y() - current_level->get_grid_y();
	switch (direction) {
		case UP:
			px = grid_offset_x*level_width + player_offset_x;
			py = next_level->get_height() - player->get_height();
			break;
		case DOWN:
			px = grid_offset_x*level_width + player_offset_x;
			py = 0;
			break;
		case LEFT:
			px = next_level->get_width() - player->get_width();
			py = grid_offset_y*level_height + player_offset_y;
			break;
		case RIGHT:
			px = 0;
			py = grid_offset_y*level_height + player_offset_y;
			break;
	}
	player->set_position(px, py);
	player->set_exit_level_flag(false);
	current_level = next_level;
}

void GameImageManager::draw(ALLEGRO_DISPLAY * display)
{
	if (current_level) current_level->draw(display, get_camera_offset(display));
}

void GameImageManager::resume()
{
	player->clear_input();
}



