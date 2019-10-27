#ifndef INVENTORY_H
#define INVENTORY_H

#include "Item.h"
#include "vector"  // for vector
#include <memory>  // for allocator
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include "XMLSerialization.h"

const static int HOTBAR_SIZE = 10;
const static int INVENTORY_COLS = 10, INVENTORY_ROWS = 3;

class Inventory : public xmls::Serializable
{
private:
	int hotbar_index = 0;
	xmls::xString inventory_key;
	xmls::Collection<Item> inventory_items;
	xmls::xFloat money = 0.0f;
	const std::pair<int, int> first_open_inventory_pos();
public:
	Inventory();
	~Inventory();
	void load_content();
	void load_content_from_attributes();
	void unload_content();
	void reset();
	void initialize_empty();
	const std::string get_inventory_key();
	void set_inventory_key(const std::string value);
	void set_hotbar_index(int num);
	void hotbar_index_left();
	void hotbar_index_right();
	void swap_items(Inventory * other_inv, const int x1, const int y1, const int x2, const int y2);
	void swap_items(int x1, int y1, int x2, int y2);
	std::vector<std::vector<Item*>> get_inventory_items();
	void set_item(Item* item, const int x, const int y);
	void add_item(Item* item);
	Item *get_selected_hotbar_item();
	Item *get_item(const int x, const int y);
	Item *get_hotbar_item(const int index);
	int get_hotbar_index();
	const bool is_full();
	const bool has_item_with_key(const int item_key);
	Item * get_item_with_key(const int item_key);
	void remove_item_with_key(const int item_key);
	// money
	const float get_money();
	const std::string money_display_text();
	void adjust_money(const float adjust_amount);
};

#endif