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
			x = -1 * std::min((std::max(0.0f, player->get_x() - DEFAULT_SCREEN_WIDTH / 2)), (float)(current_level->get_width() - DEFAULT_SCREEN_WIDTH));
			//TODO
		}
		if (current_level->get_height() <= DEFAULT_SCREEN_HEIGHT)
			y = 0;
		else {
			y = -1 * std::min((std::max(0.0f, player->get_y() - DEFAULT_SCREEN_HEIGHT / 2)), (float)(current_level->get_height() - DEFAULT_SCREEN_HEIGHT));
		}
	}
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
	current_level = world.get_current_dungeon()->level_at(0, 0);
	//load_player();
	load_level_content("resources/load/player.txt", "", PLAYER);
	//TEMP
	//load_level(0, 0);
	//load_level_from_map("" , "temp_map_1");	//TODO: tie this into coord-based level loading
	//load_level_content("resources/load/temp_level_1.txt", "", PLATFORM);
	//TEMP
	//TODO: load dungeon content from somewhere
	//IDEA: put dungeon grid in this class instead of the dungeon class, and change it when changing world
	//TODO: load content for adjacent levels, or possibly for the current dungeon
}

int GameImageManager::get_game_mode()	//temp. figure out game mode some other way
{
	return TOP_DOWN;
	//return SIDE_SCROLLING;	//TEMP
}

void GameImageManager::load_level(int grid_x, int grid_y)
{
	Level *level = world.get_current_dungeon()->level_at(grid_x, grid_y);
	if (level) {
		load_level_from_map(*level);
		//load_level_from_map(level->get_id(), level->get_filename());	//TEMP. can store multiple maps in a file, so consider keeping track of a level's ID as well as its filename.
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
			image = new Player();
			player = dynamic_cast<Player*>(image);
			player->load_content(attributes[i], contents[i]);
			player->set_bitmap(ImageLoader::get_instance().get_current_image(player));
			current_level->add_being(player);
			break;
			/*
			entities.push_back(dynamic_cast<Entity*>(image));
			beings.push_back(dynamic_cast<Being*>(image));
			break;
			*/
			/*
		case BLOCK:
			image = DBG_NEW Block();
			entities.push_back(dynamic_cast<Entity*>(image));
			blocks.push_back(dynamic_cast<Block*>(image));
			break;
			*/
		}
		/*
		game_images.push_back(image);
		game_images.back()->load_content(attributes[i], contents[i]);
		game_images.back()->set_bitmap(ImageLoader::get_instance().get_current_image(game_images.back()));
		*/
	}
}

void GameImageManager::load_level_from_map(Level level)
{

}

void GameImageManager::load_player()
{
	/*
	//load_level_content(, "", PLAYER);-
	std::vector<std::vector<std::string>> attributes;
	std::vector<std::vector<std::string>> contents;
	file_manager.load_content("resources/load/player.txt", attributes, contents);
	player = new Player();
	//GameImage *image;
	//	image = new Player();
	player->load_content(attributes[i], contents[i]);
		player->set_bitmap(ImageLoader::get_instance().get_current_image(player));
		current_level->add_being(player);
		*/
}

void GameImageManager::unload_content()
{
	world.unload_content();
	//int g_size = game_images.size();
	/*
	for (int i = 0; i < game_images.size(); i++) {
		if (game_images[i]) {
			game_images[i]->unload_content();
			delete game_images[i];
		}
	}
	for (int i = 0; i < tiles.size(); i++) {
		for (int j = 0; j < tiles[i].size(); j++) {
			tiles[i][j].unload_content();
		}
		tiles[i].clear();
		std::vector<Tile>().swap(tiles[i]);
	}
	std::vector<std::vector<Tile>>().swap(tiles);
	game_images.clear();
	std::vector<GameImage*>().swap(game_images);
	entities.clear();
	std::vector<Entity*>().swap(entities);
	beings.clear();
	std::vector<Being*>().swap(beings);
	player = NULL;
	*/
	player = NULL;
}

void GameImageManager::unload_level_content()
{
	/*
	for (int i = 0; i < tiles.size(); i++) {
		for (int j = 0; j < tiles[i].size(); j++) {
			tiles[i][j].unload_content();
		}
		tiles[i].clear();
		std::vector<Tile>().swap(tiles[i]);
	}
	std::vector<std::vector<Tile>>().swap(tiles);
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
	//blocks.swap(std::vector<Block*>());

	//TODO: put these 3 lines somewhere else since we might not always want to add the player here
	game_images.push_back(player);
	entities.push_back(player);
	beings.push_back(player);
	*/
}
/*
void GameImageManager::add_block(Block * b)
{
	entities.push_back(dynamic_cast<Entity*>(b));
	blocks.push_back(dynamic_cast<Block*>(b));
	game_images.push_back(dynamic_cast<GameImage*>(b));
}
*/

void GameImageManager::update(std::map<int, bool> input_map, std::map<int, std::pair<float,float>> joystick_map)
{
	//TODO: joystick controls
	//commented out for testing
	//TODO: if game slows down too much, store beings separately from gameimages and don't update all gameimages, just beings
	std::pair<int,int> dimensions = current_level->get_dimensions();
	if (player) {
		if (player->get_exit_level_flag()) {
			//TODO: check to make sure there is a next level in the given direction here
			change_player_level();
			return;
		}
		player->update_input(input_map, joystick_map, get_game_mode());
	}
	//std::vector<Entity> interactables = get_player_interactables();		//TODO: instead of just player interactables, get all interactables since other beings may interact with objects.
	current_level->update(get_game_mode());
	/*
	int size = beings.size();
	for (int i = 0; i < size; i++) {
		if (beings[i]) {	// note that the player's update is called here, so we don't need to call it above.
			std::vector<Entity> interactables = get_interactables(beings[i]);
			std::vector<Tile> tiles = get_nearby_tiles(beings[i]);
			beings[i]->update(interactables, tiles, dimensions, get_game_mode());	//TEMP. need to handle gamemode some other way, maybe by accessing it from level or storing it in GameImageManager
			beings[i]->set_bitmap(ImageLoader::get_instance().get_current_image(beings[i]));
		}
		else
			std::cout << "NULL BEING" << std::endl;
	}
	*/
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

/*
std::vector<Entity> GameImageManager::get_interactables(Entity* entity)
{	
	std::vector<Entity> interactables;
	
	//TODO: get interactables that could potentially interact with the entity this frame, based on location and other factors.
	int xpos = entity->get_x(), ypos = entity->get_y(), width = entity->get_width(), height = entity->get_height();
	int tx_max = tiles[0].size() - 1, ty_max = tiles.size() - 1;
	int tx1 = std::max(0, xpos / TILE_SIZE - 2), ty1 = std::max(0, ypos / TILE_SIZE - 2),
		tx2 = std::min(tx_max, (xpos + width) / TILE_SIZE + 2), ty2 = std::min(ty_max, (ypos + height) / TILE_SIZE + 2);
	for (int y = ty1; y < ty2; y++) {
		for (int x = tx1; x < tx2; x++) {
			Entity *e = tiles[y][x].get_block();
			if(e) interactables.push_back(*e);
		}
	}
	
	return interactables;
}
*/
/*
std::vector<Tile> GameImageManager::get_nearby_tiles(Entity* entity)
{

	std::vector<Tile> nearby_tiles;
	//TODO: get interactables that could potentially interact with the entity this frame, based on location and other factors.
	int xpos = entity->get_x(), ypos = entity->get_y(), width = entity->get_width(), height = entity->get_height();
	int tx_max = tiles[0].size() - 1, ty_max = tiles.size() - 1;
	int tx1 = std::max(0, xpos / TILE_SIZE - 2), ty1 = std::max(0, ypos / TILE_SIZE - 2),
		tx2 = std::min(tx_max, (xpos + width) / TILE_SIZE + 2), ty2 = std::min(ty_max, (ypos + height) / TILE_SIZE + 2);
	for (int y = ty1; y < ty2; y++) {
		for (int x = tx1; x < tx2; x++) {
			nearby_tiles.push_back(tiles[y][x]);
		}
	}
	return nearby_tiles;
}
*/
/*
std::vector<Entity> GameImageManager::get_player_interactables()
{	
	std::vector<Entity> interactables;
	//TODO: non-block interactables
	return interactables;
}
*/
/*
std::vector<std::vector<Tile>> GameImageManager::get_tiles()
{
	return std::vector<std::vector<Tile>>();
}

std::vector<GameImage*> GameImageManager::get_game_images()
{
	return std::vector<GameImage*>();
}

std::vector<Entity*> GameImageManager::get_entities()
{
	return std::vector<Entity*>();
}

std::vector<Being*> GameImageManager::get_beings()
{
	return std::vector<Being*>();
}
*/
void GameImageManager::draw(ALLEGRO_DISPLAY * display)
{
	if (current_level) current_level->draw(display, get_camera_offset());
		// only draw things that are on screen
	/*
	std::pair<int, int> off = get_camera_offset();
	const int start_x = std::max(0, (-1*off.first) / TILE_SIZE - 1);
	const int start_y = std::max(0, (-1*off.second) / TILE_SIZE - 1);
	const int x_size = tiles[0].size(), y_size = tiles.size();
	const int end_x = std::min(x_size, start_x + DEFAULT_SCREEN_WIDTH  / TILE_SIZE + 3);
	const int end_y = std::min(y_size, start_y + DEFAULT_SCREEN_HEIGHT / TILE_SIZE + 3);
	for (int y = start_y; y < end_y; y++) {
		for (int x = start_x; x < end_x; x++) {
			tiles[y][x].draw(display, off.first, off.second);
		}
	}
	int size = game_images.size();
	for (int i = 0; i < size; i++) {
		game_images[i]->draw(display, off.first, off.second);
	}
	*/
}

