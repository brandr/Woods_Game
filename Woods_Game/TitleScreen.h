#ifndef TITLESCREEN_H
#define TITLESCREEN_H

#include "AudioManager.h"
#include "GameScreen.h"
#include "MenuManager.h"
#include "LoadGameMenuManager.h"

#include "allegro5/allegro_font.h"  // for ALLEGRO_FONT
#include "allegro5/display.h"       // for ALLEGRO_DISPLAY
#include "allegro5/events.h"        // for ALLEGRO_EVENT
#include "allegro5/allegro_ttf.h"    
#include "allegro5/bitmap.h"

enum TITLE_MENUS {
	TITLE_MAIN_MENU, TITLE_LOAD_GAME_MENU
};

class TitleScreen:
	public GameScreen
{
private:
	ALLEGRO_FONT *font;
	std::map<int, std::string> backdrop_filenames;
	std::map<int, std::unique_ptr<MenuManager>> menus;
	int menu_key = TITLE_MAIN_MENU;
	void play_title_music();
	void music_update();
protected:
	virtual const std::string get_mouse_cursor_key();
public:
	TitleScreen();
	~TitleScreen();
	virtual void load_content();
	virtual void unload_content();
	void initialize_load_game_menu();
	void set_default_controls();
	virtual int get_game_mode();
	virtual void update();
	virtual void draw(ALLEGRO_DISPLAY *display);
	ALLEGRO_BITMAP* current_backdrop();
	MenuManager& current_menu_manager();
	virtual void menu_up();
	virtual void menu_down();
	virtual void menu_left();
	virtual void menu_right();
	virtual void cancel_menu();
	virtual void confirm_selection();
	virtual void process_mouse_click_left(const int x, const int y);
	virtual void update_mouse_position(const int x, const int y, const int z);
	virtual void mouse_cursor_update();
};

#endif