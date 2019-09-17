#ifndef INVENTORYSCREEN_H
#define INVENTORYSCREEN_H

#define NUM_INVENTORY_TABS 3
#define MAP_CURRENT_LOCATION_BLINK_TIME 40

#include "Inventory.h"
#include "GameScreen.h"
#include "ImageLoader.h"
#include "allegro5/bitmap.h"   // for ALLEGRO_BITMAP
#include "allegro5/display.h"  // for ALLEGRO_DISPLAY
#include "utility"             // for pair

enum INVENTORY_SCREEN_TABS {
	INVENTORY_TAB_ITEMS,
	INVENTORY_TAB_MAP,
	INVENTORY_TAB_JOURNAL
};

class InventoryScreen :
	public GameScreen
{
private:

	std::string world_key;
	std::string dungeon_key;

	int current_location_x = -1, current_location_y = -1;

	int tab_index = INVENTORY_TAB_ITEMS;
	
	//UI components

	ALLEGRO_BITMAP* inventory_backdrop();
	ALLEGRO_BITMAP* inventory_tab_selected();
	ALLEGRO_BITMAP* inventory_tab_deselected();

	// items tab
	ALLEGRO_BITMAP* item_box_hotbar();
	ALLEGRO_BITMAP* item_box_selected();
	ALLEGRO_BITMAP* item_box_inventory();
	ALLEGRO_BITMAP* item_drag_selection();

	// map tab
	ALLEGRO_BITMAP* map_frame();
	ALLEGRO_BITMAP* map_current_location_icon();

	//inventory
	Inventory *inventory;

	std::pair<int, int> inventory_selection = std::pair<int, int>(-1, -1);
	std::pair<int, int> dragging_selection = std::pair<int, int>(-1, -1);

	// map

	int map_current_location_blink_index = 0;

	// journal tab

	// TODO

	const std::string label_for_tab(const int index);
	void items_tab_menu_up();
	void items_tab_menu_down();
	void items_tab_menu_left();
	void items_tab_menu_right();
	void items_tab_select();

public:
	InventoryScreen();
	~InventoryScreen();
	virtual void load_content();
	virtual void load_fonts();
	virtual void draw(ALLEGRO_DISPLAY *display);
	void draw_tab_selection(ALLEGRO_DISPLAY *display);
	void draw_inventory(ALLEGRO_DISPLAY *display);
	void draw_hotbar(ALLEGRO_DISPLAY *display);
	void draw_map(ALLEGRO_DISPLAY *display);
	virtual void update();
	void reset();
	virtual void menu_up();
	virtual void menu_down();
	virtual void menu_left();
	virtual void menu_right();
	virtual void tab_left();
	virtual void tab_right();
	virtual void select();
	void set_inventory(Inventory *inventory);
	bool selecting_internal_inventory();
	Item* selected_item();
	Item* dragging_item();
	void set_world_key(const std::string value);
	void set_dungeon_key(const std::string value);
	void set_current_location(const int x, const int y);

};

#endif