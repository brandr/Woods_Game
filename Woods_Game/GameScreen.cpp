#include "GameScreen.h"

GameScreen::GameScreen()
{
	//TODO: set default values for control map
}


GameScreen::~GameScreen()
{
	//std::cout << "deleting gamescreen" << std::endl;
	
	/*
	InputManager input;
	std::vector<std::vector<int>> attributes;
	std::vector<std::vector<std::string>> contents;
	std::map<int, controlFunc> control_map;	//TODO: method
	std::map<int, bool> input_map;
	int screen_flag = NONE;*/
}

void GameScreen::set_input_map()
{
	
}

void GameScreen::set_input(int key, bool on)
{
	input_map[key] = on;
}

void GameScreen::clear_input()
{
	for (auto const &it : joystick_pos_map) {
		joystick_pos_map[it.first] = std::pair<float, float>(0.0f, 0.0f);
	}
}

void GameScreen::set_joystick_pos(int stick, float x, float y)
{
	joystick_pos_map[stick] = std::pair<float, float>(x, y);
}

std::pair<float, float> GameScreen::get_joystick_pos(int stick)
{
	auto it = joystick_pos_map.find(stick);
	if (it == joystick_pos_map.end()) return std::pair<float,float>(0.0f, 0.0f);
	return it->second;
}

std::map<int, std::pair<float, float>> GameScreen::get_joystick_pos_map()
{
	return joystick_pos_map;
}

void GameScreen::update()
{
}

void GameScreen::draw(ALLEGRO_DISPLAY *display)
{
	//TODO
}

std::list<GameImage> GameScreen::get_images()
{
	return std::list<GameImage>();
}
int GameScreen::get_camera_offset_x()
{
	return 0;
}
int GameScreen::get_camera_offset_y()
{
	return 0;
}

void GameScreen::load_content()
{
	//joystick_map[LEFT_STICK] = al_get_joystick(LEFT_STICK);
	//joystick_map[RIGHT_STICK] = al_get_joystick(RIGHT_STICK);
}

void GameScreen::unload_content()
{
	for (std::vector<int> attribute : attributes)
		attribute.clear();
	attributes.clear();
	attributes.swap(std::vector<std::vector<int>>());
	for (std::vector<std::string> content : contents)
		content.clear();
	contents.clear();
	contents.swap(std::vector<std::vector<std::string>>());
	for (std::map<std::pair<int,int>, controlFunc>::iterator it = control_map.begin(); it != control_map.end(); ++it) {
		it->second = NULL;
	}
	control_map.clear();
	input_map.clear();
	joystick_pos_map.clear();
	//for (auto const &it : joystick_map) {
	//	delete it.second;
	//}
	//joystick_map.clear();
}

void GameScreen::load_images(ImageLoader& loader)
{
}

void GameScreen::refresh()
{
	for (std::map<int, bool>::iterator it = input_map.begin(); it != input_map.end(); ++it) {
		it->second = false;
	}
}

void GameScreen::process_event(ALLEGRO_EVENT ev)
{
	//TODO: note that this doesn't necessarily happen every frame
	//TODO: other types of events

	for (std::map<std::pair<int, int>, controlFunc>::iterator it = control_map.begin(); it != control_map.end(); ++it) {
		controlFunc f = it->second;
			if (input.is_input_pressed(ev, it->first.first, it->first.second)) {
				//std::cout << "key press" << std::endl;
				(*f)(*this, ev, true);
			}
			else if (input.is_input_released(ev, it->first.first, it->first.second)) {
				//std::cout << "key release" << std::endl;
				(*f)(*this, ev, false);
			}

	}
	//TODO: need to handle different control schemes
}

void GameScreen::set_screen_flag(int flag)
{
	screen_flag = flag;
}

int GameScreen::get_screen_flag()
{
	return screen_flag;
}

