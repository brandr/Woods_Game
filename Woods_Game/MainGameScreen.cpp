#include "MainGameScreen.h"
#include "Level.h"
#include<iostream>

//InputManager main_input;

// functions to be passed along to the level, usually to the player
void move_up(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	screen.set_input(INPUT_UP, toggle);
}

void move_down(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	screen.set_input(INPUT_DOWN, toggle);
}

void move_left(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	screen.set_input(INPUT_LEFT, toggle);
}

void move_right(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	screen.set_input(INPUT_RIGHT, toggle);
}

void move_joystick_axis(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		switch (ev.joystick.axis) {
		case 0:	// x axis
			screen.set_joystick_pos(ev.joystick.stick, ev.joystick.pos, screen.get_joystick_pos(ev.joystick.stick).second);
			break;
		case 1: // y axis
			screen.set_joystick_pos(ev.joystick.stick,  screen.get_joystick_pos(ev.joystick.stick).first, ev.joystick.pos);
			break;
		
		}
	}
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
	control_map.emplace(std::pair<int,int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_UP), &move_up);
	control_map.emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_DOWN), &move_down);
	control_map.emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_LEFT), &move_left);
	control_map.emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_RIGHT), &move_right);

	// joystick
	control_map.emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_AXIS, 0), &move_joystick_axis);
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
	//std::cout << "input map left: " << input_map[INPUT_LEFT] << std::endl;
	game_image_manager.update(input_map, joystick_pos_map);
	GameScreen::update();
}

void MainGameScreen::draw(ALLEGRO_DISPLAY * display)
{
	game_image_manager.draw(display);
	//TODO: uncomment after testing
}

int MainGameScreen::get_camera_offset_x()
{
	return 0;
}

int MainGameScreen::get_camera_offset_y()
{
	return 0;
}

void MainGameScreen::refresh()
{
	/*
	input_map[INPUT_UP] = false;
	input_map[INPUT_DOWN] = false;
	input_map[INPUT_LEFT] = false;
	input_map[INPUT_RIGHT] = false;
	*/
}
