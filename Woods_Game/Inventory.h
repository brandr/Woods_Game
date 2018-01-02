#ifndef INVENTORY_H
#define INVENTORY_H

#include "Item.h"
#include "vector"  // for vector
#include <memory>  // for allocator
#include <iostream>

const static int HOTBAR_SIZE = 10;
const static int INVENTORY_COLS = 10, INVENTORY_ROWS = 3;

class Inventory
{

private:
	std::vector<Item*> hotbar;
	int hotbar_index = 0;
	std::vector<std::vector<Item*>> inventory_items;
public:
	Inventory();
	~Inventory();
	void load_content();
	void unload_content();
	std::vector<Item*> &get_hotbar();
	void set_hotbar_index(int num);
	void hotbar_index_left();
	void hotbar_index_right();
	void swap_items(int x1, int y1, int x2, int y2);
	std::vector<std::vector<Item*>> get_inventory_items();
	void set_item(Item* item, const int x, const int y);
	Item *get_selected_hotbar_item();
	Item *get_item(const int x, const int y);
	Item *get_hotbar_item(const int index);
	int get_hotbar_index();
};

#endif