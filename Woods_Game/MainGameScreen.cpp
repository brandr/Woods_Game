#include "MainGameScreen.h"
#include "Level.h"
#include<iostream>

InputManager main_input;

// functions to be passed along to the level, usually to the player
void move_up(GameScreen& screen, int key, bool toggle) {
	screen.set_input(INPUT_UP, toggle);
}

void move_down(GameScreen& screen, int key, bool toggle) {
	screen.set_input(INPUT_DOWN, toggle);
}

void move_left(GameScreen& screen, int key, bool toggle) {
	screen.set_input(INPUT_LEFT, toggle);
}

void move_right(GameScreen& screen, int key, bool toggle) {
	screen.set_input(INPUT_RIGHT, toggle);
}


MainGameScreen::MainGameScreen()
{
	set_default_controls();
	set_input_map();
}

MainGameScreen::~MainGameScreen()
{

}

void MainGameScreen::set_default_controls()
{
	// key presses
	control_map.emplace(ALLEGRO_KEY_UP, &move_up);
	control_map.emplace(ALLEGRO_KEY_DOWN, &move_down);
	control_map.emplace(ALLEGRO_KEY_LEFT, &move_left);
	control_map.emplace(ALLEGRO_KEY_RIGHT, &move_right);
}

void MainGameScreen::set_input_map()
{
	input_map[INPUT_UP] = false;
	input_map[INPUT_DOWN] = false;
	input_map[INPUT_LEFT] = false;
	input_map[INPUT_RIGHT] = false;
}

void MainGameScreen::load_content()
{
	//current_level = Level(0, 0, 800, 600);
	//std::cout << "Loading main screen content.." << std::endl;
	game_image_manager.load_content();
	//TODO: uncomment when necessary
	//TODO: copy necessary info from world/dungeon innto current level, or load it (the world structure may not even be necessary if we only load adjacent levels at any given time
	//TODO: load blocks and other level data via game_image_manager
}

void MainGameScreen::unload_content()
{
	game_image_manager.unload_content();
	GameScreen::unload_content();
	//TODO: unload everything that was loaded in
}

void MainGameScreen::update()
{
	//std::pair<int, int> dimensions = current_level.get_dimensions();
	/*
	if (game_image_manager.player->get_exit_level_flag()) {
		change_player_level();
		return;
	}
	*/
	game_image_manager.update(input_map);
	//TODO: uncomment after testing
	//TODO: tell the level to update too if necessary
	/*
	if (get_current_level()) {
		get_current_level()->update(input_map);
	}
	*/
}
/*

void MainGameScreen::draw(ImageLoader* image_loader)
{	
	std::list<GameImage> imageList = get_images();
	// display all gameimages
	//TODO: only draw objects that will actually appear on the screen (handle this when calling current_screen.get_images())
	int camera_offset_x = get_camera_offset_x(), camera_offset_y = get_camera_offset_y();
	for (GameImage image : imageList) {
		ALLEGRO_BITMAP *draw_image = image_loader->get_current_image(&image);
		if (draw_image == NULL) {
			//TODO: error handling
			//std::cout << "no image" << std::endl;
		}
		//TODO: use camera offset to determine x and y locations for drawing
		else
			al_draw_bitmap(draw_image, image.get_x(), image.get_y(), NULL);

	}
}
*/

void MainGameScreen::draw(ALLEGRO_DISPLAY * display)
{
	game_image_manager.draw(display);
	//TODO: uncomment after testing
}
/*
void MainGameScreen::set_world(World world)
{
	this->world = world;
}

void MainGameScreen::set_current_level(int x, int y)
{
	world.set_current_level(x, y);
	//Dungeon dungeon = (*(*world).get_current_dungeon());
	//std::cout << "HERE";
	//current_level = dungeon.level_at(x, y);
}

void MainGameScreen::change_player_level()
{
	//player->set_exit_level_flag(false);	//TODO
	Player *player = game_image_manager.player;
	Level* next_level = NULL;
	int x = 0;
	int y = 0;
	int grid_x = current_level.get_grid_x();
	int grid_y = current_level.get_grid_y();
	int width = current_level.get_width();
	int height = current_level.get_height();
	//up
	if (player->get_y() + player->get_height() < 0)
		x = grid_x, y = grid_y - 1;
	//down
	else if (player->get_y() > height)
		x = grid_x, y = grid_y + current_level.grid_height();
	//left
	else if (player->get_x() + player->get_width() < 0)
		x = grid_x - 1, y = grid_y;
	//right
	else if (player->get_x() > width)
		x = grid_x + current_level.grid_width(), y = grid_y;
	//next_level = dungeon_grid->at(y)[x];	//TODO: get next level another way
	//TODO: reset player location by figuring out where in the next level the player is
	//TODO: ensure that the screen changes the current level
	player->set_position(0, 0); //TEMP
	if (next_level != NULL) {
		next_level->set_player(player);
		//TODO: find some other way to set current level
		//screen->set_current_level(x, y);
		//screen = NULL;
		player = NULL;
	}
}


Level * MainGameScreen::get_current_level()
{
	return &current_level;//world.get_current_level();
}
*/
/*
std::list<GameImage> MainGameScreen::get_images()
{
	if (get_current_level() == NULL) {
		return std::list<GameImage>();
	}
	return get_current_level()->get_images();
	//TODO: return the proper images from the level.
	//TODO: use camera offset here. Only need to show images that will be visible.
}
*/
int MainGameScreen::get_camera_offset_x()
{
	return 0;
}

int MainGameScreen::get_camera_offset_y()
{
	return 0;
}
/*
std::pair<int, int> MainGameScreen::get_current_level_dimensions()
{
	return world.get_current_level_dimensions();
}
*/
/*
void MainGameScreen::load_images(ImageLoader& loader)
{
	//TODO: make sure this loads necesarry images ahead of time (i.e., before the player goes to a level with new images to load)

	//TODO: load all necessary images including background, player, etc.
	//may need to find some way to order them, though this can probably be handled when rendering by calling filenames.
	//Player *p = get_current_level()->get_player();
	//Player *p = get_player();
	//world.load_images(loader);
	//get_current_level()->load_images(loader);

	Player player = *(get_current_level()->get_player());
	loader.load_image((&player)->get_image_filename());
	std::vector<Platform> platforms = get_current_level()->get_platforms();
	for (Platform pl : platforms) {
		loader.load_image((&pl)->get_image_filename());
	}
	
}
*/
void MainGameScreen::refresh()
{
	/*
	input_map[INPUT_UP] = false;
	input_map[INPUT_DOWN] = false;
	input_map[INPUT_LEFT] = false;
	input_map[INPUT_RIGHT] = false;
	*/
}
