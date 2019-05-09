#ifndef MAINGAMESCREEN_H
#define MAINGAMESCREEN_H

#include "InventoryScreen.h"
#include "CalendarScreen.h"
#include "allegro5/bitmap.h"   // for ALLEGRO_BITMAP
#include "allegro5/display.h"  // for ALLEGRO_DISPLAY
#include "allegro5/events.h"   // for ALLEGRO_EVENT
#include "Controls.h"          // for Controls
#include "GameImageManager.h"  // for GameImageManager
#include "GameScreen.h"        // for GameScreen
#include "PauseScreen.h"       // for PauseScreen
#include "Dialog.h"

#define DEFAULT_SCREEN_WIDTH 800
#define DEFAULT_SCREEN_HEIGHT 600


class MainGameScreen :
	public GameScreen
{
private:
	
	GameImageManager game_image_manager;
	PauseScreen pause_screen;
	InventoryScreen inventory_screen;
	CalendarScreen calendar_screen;
	Controls keyboard_controls;
	Controls controller_controls;
	//UI components
	ALLEGRO_BITMAP* hotbar_box;
	ALLEGRO_BITMAP* hotbar_box_selected;
	ALLEGRO_BITMAP* dialog_backdrop_full_width;
	ALLEGRO_BITMAP* clock_backdrop;
	ALLEGRO_BITMAP* option_arrow;
public:
	MainGameScreen();
	~MainGameScreen();
	void set_default_controls();
	void set_mappable_controls();
	void unset_mappable_controls();
	void set_mappable_input_list();
	void map_keyboard_control_action(int game_mode, std::string action_key, controlFunc action_func);
	void map_controller_control_action(int game_mode, std::string action_key, controlFunc action_func);
	void unmap_keyboard_control_action(int game_mode, std::string action_key);
	void unmap_controller_control_action(int game_mode, std::string action_key);
	virtual void set_input_map();
	virtual void load_content();
	void load_ui_content();
	void start_new_game(const std::string world_key);
	virtual void unload_content();
	void load_controls();
	void reset_controls();
	virtual void update();
	void pause_screen_update();
	void dialog_update();
	void cutscene_update();
	void calendar_update();
	virtual void draw(ALLEGRO_DISPLAY* display);
	void draw_ui(ALLEGRO_DISPLAY* display);
	void draw_ui_inventory(ALLEGRO_DISPLAY* display);
	void draw_ui_calendar(ALLEGRO_DISPLAY* display);
	void draw_ui_main_game(ALLEGRO_DISPLAY* display);
	void draw_hotbar(ALLEGRO_DISPLAY* display);
	void draw_clock(ALLEGRO_DISPLAY* display);
	void draw_ui_paused(ALLEGRO_DISPLAY* display);
	void draw_ui_dialog(ALLEGRO_DISPLAY* display);
	virtual int get_game_mode();
	virtual int get_camera_offset_x();
	virtual int get_camera_offset_y();
	virtual void refresh();
	virtual GameScreen& screen_receiving_input();
	void load_keyboard_input(std::string action_key);
	// controls
	virtual void pause_game();
	virtual void cancel_menu();
	virtual void resume_game();
	virtual void dialog_advance();
	virtual void menu_up();
	virtual void menu_down();
	virtual void menu_left();
	virtual void menu_right();
	virtual void confirm_selection();
	virtual void select();
	virtual void process_mouse_click_left(const int x, const int y);
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
	virtual void call_keyboard_mappable_input(ALLEGRO_EVENT ev, bool toggle);
};

#endif