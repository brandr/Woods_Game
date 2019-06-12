#ifndef PAUSESCREEN_H
#define PAUSESCREEN_H

#include "GameImageManager.h"
#include "GameScreen.h"
#include <memory>

enum PAUSE_MENUS{
	PAUSE_MAIN_MENU, 
	PAUSE_VIDEO_MENU, 
	PAUSE_CONTROLS_MENU, PAUSE_CONTROLS_MENU_KEYBOARD, PAUSE_CONTROLS_MENU_CONTROLLER};
enum CONFIG_SETTINGS{ CONFIG_SCREEN_RESOLUTION, CONFIG_SCREEN_STYLE };

class PauseScreen:
	public GameScreen
{
private:
	std::map<int, ALLEGRO_BITMAP*> backdrops;
	std::map<int, std::unique_ptr<MenuManager>> menus;
	int menu_key = PAUSE_MAIN_MENU;
public:
	PauseScreen();
	~PauseScreen();
	void reset();
	void set_menus();
	ALLEGRO_BITMAP* current_backdrop();
	MenuManager& current_menu_manager();
	virtual void load_content();
	void load_config_settings();
	virtual void unload_content();
	virtual void update();
	virtual void draw(ALLEGRO_DISPLAY *display);
	virtual void process_mouse_click_left(const int x, const int y);
	virtual void mouse_cursor_update();
	virtual void cancel_menu();
	virtual void menu_up();
	virtual void menu_down();
	virtual void menu_left();
	virtual void menu_right();
	virtual void confirm_selection();
	virtual void call_keyboard_mappable_input(ALLEGRO_EVENT ev, bool toggle);
	virtual void call_controller_mappable_input(ALLEGRO_EVENT ev, bool toggle);
	virtual bool taking_mappable_input();
	virtual void stop_taking_mappable_input_keyboard();
	virtual void stop_taking_mappable_input_controller();
};
#endif