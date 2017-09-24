#include "GameScreen.h"

GameScreen::GameScreen()
{
	//TODO: set default values for control map
}


GameScreen::~GameScreen()
{

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
	for (auto const &it : input_map) {
		input_map[it.first] = false;
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
}

int GameScreen::get_game_mode()
{
	return 0;
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
	for (std::map<int, ALLEGRO_FONT*>::iterator it = font_map.begin(); it != font_map.end(); ++it) {
		al_destroy_font(it->second);
	}
	font_map.clear();
	control_map.clear();
	input_map.clear();
	joystick_pos_map.clear();
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

void GameScreen::pause_game()
{
}

void GameScreen::cancel_menu()
{
}

void GameScreen::resume_game()
{
}

void GameScreen::quit_game()
{
	screen_flag = FLAG_QUIT_GAME;
}

void GameScreen::menu_up()
{
}

void GameScreen::menu_down()
{
}

void GameScreen::menu_left()
{
}

void GameScreen::menu_right()
{
}

void GameScreen::mouse_cursor_update()
{
}

void GameScreen::set_mouse_position(std::pair<int, int> pos)
{
	mouse_pos = pos;
}

void GameScreen::process_mouse_click_left(const int x, const int y)
{
}

void GameScreen::update_mouse_position(const int x, const int y)
{
	mouse_pos.first = x, mouse_pos.second = y;
}

void GameScreen::confirm_selection()
{
}

void GameScreen::select()
{
}

void GameScreen::process_number_input(int num)
{
}

void GameScreen::a_button_action()
{
}

void GameScreen::b_button_action()
{
}

void GameScreen::x_button_action()
{
}

void GameScreen::y_button_action()
{
}

void GameScreen::left_bumper_action()
{
}

void GameScreen::right_bumper_action()
{
}

void GameScreen::process_event(ALLEGRO_EVENT ev)
{
	//TODO: note that this doesn't necessarily happen every frame
	//TODO: other types of events
	const int game_mode = get_game_mode();
	auto it1 = control_map.find(game_mode);
	if (it1 == control_map.end()) return;
	std::map<std::pair<int, int>, controlFunc> curr_control_map = it1->second;
	for (std::map<std::pair<int, int>, controlFunc>::iterator it2 = curr_control_map.begin(); it2 != curr_control_map.end(); ++it2) {
		controlFunc f = it2->second;
			if (input.is_input_pressed(ev, it2->first.first, it2->first.second)) {
				//std::cout << "key press" << std::endl;
				(*f)(*this, ev, true);
			}
			else if (input.is_input_released(ev, it2->first.first, it2->first.second)) {
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

