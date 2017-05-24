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


std::pair<int, int> GameImageManager::get_camera_offset()
{
	int x = 0, y = 0;
	if (player) {
		if (current_level->get_width() <= DEFAULT_SCREEN_WIDTH)
			x = 0;
		else {
			x = -1 * std::min((std::max(0, player->get_x() - DEFAULT_SCREEN_WIDTH / 2)), current_level->get_width() - DEFAULT_SCREEN_WIDTH);
			//TODO
		}
		if (current_level->get_height() <= DEFAULT_SCREEN_HEIGHT)
			y = 0;
		else {
			y = -1 * std::min((std::max(0, player->get_y() - DEFAULT_SCREEN_HEIGHT / 2)), current_level->get_height() - DEFAULT_SCREEN_HEIGHT);
		}
	}
	//std::cout << player->get_x() << endl;
	//TODO
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
	load_level_content("resources/load/player.txt", "", PLAYER);
	//TEMP
	load_level(0, 0);
	//load_level_from_map("" , "temp_map_1");	//TODO: tie this into coord-based level loading
	//load_level_content("resources/load/temp_level_1.txt", "", PLATFORM);
	//TEMP
	//TODO: load dungeon content from somewhere
	//IDEA: put dungeon grid in this class instead of the dungeon class, and change it when changing world
	//TODO: load content for adjacent levels, or possibly for the current dungeon
}

void GameImageManager::load_level(int grid_x, int grid_y)
{
	Level *level = world.get_current_dungeon()->level_at(grid_x, grid_y);
	if (level) {
		load_level_from_map(level->get_id(), level->get_filename());	//TEMP. can store multiple maps in a file, so consider keeping track of a level's ID as well as its filename.
		current_level = level;
	}
	else
		std::cout << "ERROR: failed to load level." << std::endl; //TODO: error handling
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
			image = DBG_NEW Player();
			player = dynamic_cast<Player*>(image);
			entities.push_back(dynamic_cast<Entity*>(image));
			beings.push_back(dynamic_cast<Being*>(image));
			break;
		case PLATFORM:
			image = DBG_NEW Platform();
			entities.push_back(dynamic_cast<Entity*>(image));
			platforms.push_back(dynamic_cast<Platform*>(image));
			break;
		}
		game_images.push_back(image);
		game_images.back()->load_content(attributes[i], contents[i]);
	}
}

void GameImageManager::load_level_from_map(std::string layerID, std::string mapID)
{
	std::vector<std::vector<std::string>> attributes;
	std::vector<std::vector<std::string>> contents;
	FileManager file_manager;	//NOTE: see if filemanager can be chhanged to a static get_instance()
	std::string filename = "resources/load/maps/" + mapID + ".txt";
	file_manager.load_content(filename.c_str(), attributes, contents, layerID);
	int indexY = 0;
	int size = attributes.size();
	std::string tile_sheet_filename = "";
	for (int i = 0; i < size; i++) {
		std::pair<int, int> tile(0,0);
		const int isize = attributes[i].size();
		for (int j = 0; j < isize; j++) {
			if (attributes[i][j] == "Platform") {
				//platforms.push_back(make_tile(contents[i][j];
			}
			else if (attributes[i][j] == "TileSheet") {
				tile_sheet_filename = "tile_sheets/" + contents[i][j];	
			}
			else if (attributes[i][j] == "StartLayer") {	//TODO: figure out what to do with EndLayer tag
				const int contents_size = contents[i].size();
				for (int k = 0; k < contents_size; k++) {
					if (contents[i][k] != "---") {
						
						tile = tile_image_offset(contents[i][k]);
						//Rect* offset_rect = new Rect(tile.first*TILE_SIZE, tile.second*TILE_SIZE, TILE_SIZE, TILE_SIZE);
						Rect* offset_rect = DBG_NEW Rect(tile.first*TILE_SIZE, tile.second*TILE_SIZE, TILE_SIZE, TILE_SIZE);
						ImageLoader::get_instance().load_image(tile_sheet_filename, *offset_rect);
						std::pair<int, int> position(k*TILE_SIZE, indexY*TILE_SIZE);
						Platform* p = new Platform();
						p->set_content(tile_sheet_filename, offset_rect, position);
						add_platform(p);
						//TODO: need some way to draw a distinction between platforms and other elements-- may need contents to hold more info and load other data before
						
					}
				}
				indexY++;
			}
		}
	}
}

void GameImageManager::unload_content()
{
	world.unload_content();

	//int g_size = game_images.size();
	for (int i = 0; i < game_images.size(); i++) {
		if (game_images[i]) {
			game_images[i]->unload_content();
			delete game_images[i];
		}
	}
	game_images.clear();
	std::vector<GameImage*>().swap(game_images);
	//game_images.swap(std::vector<GameImage*>());
	entities.clear();
	//entities.swap(std::vector<Entity*>());
	std::vector<Entity*>().swap(entities);
	beings.clear();
	//beings.swap(std::vector<Being*>());
	std::vector<Being*>().swap(beings);
	platforms.clear();
	std::vector<Platform*>().swap(platforms);
	//player->unload_content();
	//delete player;
	player = NULL;
}

void GameImageManager::unload_level_content()
{
	//std::cout << "unloading level" << std::endl;
	int g_size = game_images.size();
	for (int i = 0; i < g_size; i++) {
		if (game_images[i]->get_type() != PLAYER) {
			game_images[i]->unload_content();
			delete game_images[i];
		}
	}
	game_images.swap(std::vector<GameImage*>());
	entities.swap(std::vector<Entity*>());
	beings.swap(std::vector<Being*>());
	platforms.swap(std::vector<Platform*>());

	//TODO: put these 3 lines somewhere else since we might not always want to add the player here
	game_images.push_back(player);
	entities.push_back(player);
	beings.push_back(player);
}

void GameImageManager::add_platform(Platform * p)
{
	entities.push_back(dynamic_cast<Entity*>(p));
	platforms.push_back(dynamic_cast<Platform*>(p));
	game_images.push_back(dynamic_cast<GameImage*>(p));
}


void GameImageManager::update(std::map<int, bool> input_map)
{
	//commented out for testing
	//TODO: if game slows down too much, store beings separately from gameimages and don't update all gameimages, just beings
	std::pair<int,int> dimensions = current_level->get_dimensions();
	if (player) {
		if (player->get_exit_level_flag()) {
			//TODO: check to make sure there is a next level in the given direction here
			change_player_level();
			return;
		}
		player->update_input(input_map);
	}
	std::vector<Entity> interactables = get_player_interactables();
	int size = beings.size();
	for (int i = 0; i < size; i++) {
		if (beings[i])	// note that the player's update is called here, so we don't need to call it above.
			beings[i]->update(interactables, dimensions);	//TODO: figure out how to pass necessary arguments, or make them unneccassary by changing other classes
		else
			std::cout << "NULL BEING" << std::endl;
	}
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
		x = grid_x, y = grid_y + current_level->grid_height();
		direction = DOWN;
	}
	//left
	else if (player->get_x() + player->get_width() < 0) {
		x = grid_x - 1, y = grid_y;
		direction = LEFT;
	}
		
	//right
	else if (player->get_x() > width) {
		x = grid_x + current_level->grid_width(), y = grid_y;
		direction = RIGHT;
	}
	
	unload_level_content();
	load_level(x, y);
	next_level = world.get_current_dungeon()->level_at(x, y);
	int px = 0, py = 0;
	const int level_width = Level::STANDARD_LEVEL_GRID_WIDTH, level_height = Level::STANDARD_LEVEL_GRID_HEIGHT;
	const int player_offset_x = player->get_x() % level_width, player_offset_y = player->get_y() % level_height;
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
}

std::vector<Entity> GameImageManager::get_player_interactables()
{
	std::vector<Entity> interactables;
	for (Platform* p : platforms)
		interactables.push_back(*p);
	return interactables;
}

void GameImageManager::draw(ALLEGRO_DISPLAY * display)
{
	//TODO: only draw things that are on screen
	int size = game_images.size();
	for (int i = 0; i < size; i++) {
		std::pair<int, int> off = get_camera_offset();
		game_images[i]->draw(display, off.first, off.second);
	}
}

