#ifndef PAUSESCREEN_H
#define PAUSESCREEN_H

#include "AudioManager.h"
#include "GameImageManager.h"
#include "GameScreen.h"
#include <memory>

enum PAUSE_MENUS{
	PAUSE_MAIN_MENU, 
	PAUSE_VIDEO_MENU, 
	PAUSE_AUDIO_MENU,
	PAUSE_GAMEPLAY_MENU,
	PAUSE_CONTROLS_MENU, PAUSE_CONTROLS_MENU_KEYBOARD, PAUSE_CONTROLS_MENU_CONTROLLER};
enum CONFIG_SETTINGS{ CONFIG_SCREEN_RESOLUTION, CONFIG_SCREEN_STYLE };

class PauseScreen:
	public GameScreen
{
private:
	std::map<int, std::string> backdrop_filenames;
	std::map<int, std::unique_ptr<MenuManager>> menus;
	int menu_key = PAUSE_MAIN_MENU;
protected:
	virtual const std::string get_mouse_cursor_key();
public:
	PauseScreen();
	~PauseScreen();
	void reset();
	void set_menus();
	ALLEGRO_BITMAP* current_backdrop();
	MenuManager& current_menu_manager();
	virtual void load_content();
	void load_menus();
	void load_config_settings();
	virtual void unload_content();
	virtual void update();
	virtual void draw(ALLEGRO_DISPLAY *display);
	virtual void process_mouse_click_left(const int x, const int y);
	virtual void process_mouse_click_right(const int x, const int y);
	virtual void update_mouse_position(const int x, const int y, const int z);
	virtual void mouse_cursor_update();
	virtual void cancel_menu();
	virtual void menu_up();
	virtual void menu_down();
	virtual void menu_left();
	virtual void menu_right();
	virtual void confirm_selection();
	virtual void update_video_settings(const std::string style_string, const std::string res_string);
	virtual void call_keyboard_mappable_input(ALLEGRO_EVENT ev, bool toggle);
	virtual void call_controller_mappable_input(ALLEGRO_EVENT ev, bool toggle);
	virtual bool taking_mappable_input();
	virtual void stop_taking_mappable_input_keyboard();
	virtual void stop_taking_mappable_input_controller();
};
#endif