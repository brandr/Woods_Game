#ifndef INVENTORYSCREEN_H
#define INVENTORYSCREEN_H

#include "Inventory.h"
#include "GameScreen.h"
#include "ImageLoader.h"
#include "allegro5/bitmap.h"   // for ALLEGRO_BITMAP
#include "allegro5/display.h"  // for ALLEGRO_DISPLAY
#include "utility"             // for pair

class InventoryScreen :
	public GameScreen
{
private:
	
	//UI components
	ALLEGRO_BITMAP* inventory_backdrop;
	ALLEGRO_BITMAP* item_box_hotbar;
	ALLEGRO_BITMAP* item_box_selected;
	ALLEGRO_BITMAP* item_box_inventory;
	ALLEGRO_BITMAP* item_drag_selection;

	//inventory
	Inventory *inventory;
	//Item *dragging_item;

	//misc
	std::pair<int, int> inventory_selection = std::pair<int, int>(-1, -1);
	std::pair<int, int> dragging_selection = std::pair<int, int>(-1, -1);

public:
	InventoryScreen();
	~InventoryScreen();
	virtual void load_content();
	virtual void draw(ALLEGRO_DISPLAY *display);
	void draw_inventory(ALLEGRO_DISPLAY *display);
	void draw_hotbar(ALLEGRO_DISPLAY *display);
	void reset();
	virtual void menu_up();
	virtual void menu_down();
	virtual void menu_left();
	virtual void menu_right();
	virtual void select();
	void set_inventory(Inventory *inventory);
	bool selecting_internal_inventory();
	Item* selected_item();
	Item* dragging_item();
};

#endif