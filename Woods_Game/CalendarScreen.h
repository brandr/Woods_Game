#ifndef CALENDAR_SCREEN_H
#define CALENDAR_SCREEN_H

#include <Agui/Widget.hpp>
#include <Agui/Backends/Allegro5/Allegro5Image.hpp>
#include <Agui/Backends/Allegro5/Allegro5Graphics.hpp>

#include "Dialog.h"
#include "GlobalTime.h"
#include "GameScreen.h"
#include "ImageLoader.h"
#include "Player.h"
#include "allegro5/bitmap.h"   // for ALLEGRO_BITMAP
#include "allegro5/display.h"  // for ALLEGRO_DISPLAY
#include "utility"             // for pair
#include "MenuItem.h"
#include <allegro5/display.h>

class Level;
class World;
class CalendarScreen :
	public GameScreen
{
private:
	// UI components
	ALLEGRO_BITMAP * calendar_backdrop();
	ALLEGRO_BITMAP * calendar_backdrop_dialog();
	ALLEGRO_BITMAP* arrow_left();
	ALLEGRO_BITMAP* arrow_right();
	ALLEGRO_BITMAP* arrow_left_disabled();
	ALLEGRO_BITMAP* arrow_right_disabled();
	ALLEGRO_BITMAP* check_mark();

	GlobalTime * global_time;
	Player * player;
	World * world;
	Level * level;
	
	int month_index = -1;
	std::pair<int, int> select_pos = std::pair<int,int>(-1, -1);
	const int selected_month_index();
	const std::string month_str();
	const int num_days_in_month();
	const int first_day_index();
	const int last_day_index();
	const int day_at_pos(const int x, const int y);
	const bool may_select_day_at_pos(const int x, const int y);
protected:
	virtual const std::string get_mouse_cursor_key();
public:
	CalendarScreen();
	~CalendarScreen();
	virtual void load_content();
	virtual void load_fonts();
	virtual void draw(ALLEGRO_DISPLAY *display);
	void draw_day_labels(ALLEGRO_DISPLAY *display);
	void draw_month_arrows(ALLEGRO_DISPLAY *display);
	void draw_selection_ui(ALLEGRO_DISPLAY *display);
	void draw_symbols_ui(ALLEGRO_DISPLAY *display);
	void draw_confirm_dialog_ui(ALLEGRO_DISPLAY *display);
	virtual void update();
	void reset();
	virtual void process_mouse_click_left(const int mouse_x, const int mouse_y);
	void mouse_cursor_update(ALLEGRO_FONT * font, const int mouse_x, const int mouse_y, const int x_off, const int y_off);
	virtual void menu_up();
	virtual void menu_down();
	virtual void menu_left();
	virtual void menu_right();
	virtual void select();
	const bool may_decrement_month();
	const bool may_increment_month();
	void set_global_time(GlobalTime * time);
	const bool showing_dialog();
	void close_dialog();
	void set_player(Player * p);
	void set_level(Level * l);
	void set_world(World * w);
};

#endif