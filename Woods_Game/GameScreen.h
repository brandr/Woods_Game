#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include "InputManager.h"
#include "xstring"             // for string
#include "utility"  // for pair
#include <allegro5/allegro_font.h>
#include "GameImage.h"
#include "Configurations.h"
#include "ImageLoader.h"
#include <memory>

class ImageLoader;

enum SCREEN_FLAGS { 
	FLAG_NONE, 
	FLAG_CHANGE_TO_MAIN_GAME, 
	FLAG_CHANGE_TO_TITLE, 
	FLAG_START_NEW_GAME, 
	FLAG_RESUME_GAME, 
	FLAG_QUIT_GAME
};
enum NUMBER_KEYS {NUM_0 = 27, NUM_1 = 28, NUM_2 = 29, NUM_3 = 30, NUM_4 = 31, 
	NUM_5 = 32, NUM_6 = 33, NUM_7 = 34, NUM_8 = 35, NUM_9 = 36};
enum FONT_KEYS {FONT_HOTBAR, FONT_DIALOG, FONT_CLOCK, FONT_CALENDAR_DAYS, FONT_CALENDAR_MONTH};

// font colors
const static std::string FONT_COLOR_HOTBAR = "333333";
const static std::string FONT_COLOR_CALENDAR_MONTH = "660000";
const static std::string FONT_COLOR_CALENDAR_DAYS = "333333";

// screen styles
/*
const static std::string SCREEN_STYLE_WINDOWED = "Windowed";
const static std::string SCREEN_STYLE_WINDOWED_FULLSCREEN = "Windowed Fullscreen";
const static std::string SCREEN_STYLE_FULLSCREEN = "Fullscreen";
*/
class GameScreen
{
//do this so we can call key press methods
//typedef void(*controlFunc)(GameScreen&, ALLEGRO_EVENT, bool);
protected:
	typedef void(*controlFunc)(GameScreen&, ALLEGRO_EVENT, bool);
	InputManager input;
	FileManager file_manager;
	std::unique_ptr<Configurations> config;
	/*
	std::vector<std::vector<int>> attributes;
	std::vector<std::vector<std::string>> contents;
	*/
	std::map<int, std::map<std::pair<int,int>, controlFunc>> control_map;	
	std::map<int, bool> input_map;
	std::map<int, std::pair<float, float>> joystick_pos_map;
	std::pair<int, int> mouse_pos;
	int screen_flag = FLAG_NONE;
	std::map<int, ALLEGRO_FONT*> font_map;
public:
	GameScreen();
	~GameScreen();
	virtual void load_content();
	virtual void unload_content();
	virtual void update();
	virtual void draw(ALLEGRO_DISPLAY *display);
	virtual int get_game_mode();
	virtual GameScreen& screen_receiving_input();
	virtual void set_input_map();
	virtual void set_input(int, bool);
	virtual void clear_input();
	virtual void call_keyboard_mappable_input(ALLEGRO_EVENT ev, bool toggle);
	virtual void call_controller_mappable_input(ALLEGRO_EVENT ev, bool toggle);
	virtual void set_joystick_pos(int stick, float x, float y);
	virtual std::pair<float, float> get_joystick_pos(int stick);
	virtual std::map<int, std::pair<float, float>> get_joystick_pos_map();
	virtual std::vector<GameImage> get_images();
	virtual int get_camera_offset_x();
	virtual int get_camera_offset_y();
	virtual void load_images(ImageLoader&);
	virtual void refresh();
	virtual bool taking_mappable_input();
	virtual void save_configurations();
	// controls
	virtual void pause_game();
	virtual void cancel_menu();
	virtual void resume_game();
	virtual void quit_game();
	virtual void dialog_advance();
	virtual void menu_up();
	virtual void menu_down();
	virtual void menu_left();
	virtual void menu_right();
	virtual void mouse_cursor_update();
	virtual void set_mouse_position(std::pair<int, int> pos);
	virtual void process_mouse_click_left(const int x, const int y);
	virtual void update_mouse_position(const int x, const int y);
	virtual void confirm_selection();
	virtual void select();
	virtual void process_number_input(int num);
	virtual void interact_action();
	virtual void use_item_action();
	virtual void open_inventory_action();
	virtual void hotbar_left_action();
	virtual void hotbar_right_action();
	virtual void a_button_action();
	virtual void b_button_action();
	virtual void x_button_action();
	virtual void y_button_action();
	virtual void left_bumper_action();
	virtual void right_bumper_action();
	virtual void process_event(ALLEGRO_EVENT ev);
	virtual void set_screen_flag(int);
	virtual int get_screen_flag();
};

#endif