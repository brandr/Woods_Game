#include "Inventory.h"

const std::pair<int, int> Inventory::first_open_inventory_pos()
{
	for (int y = 0; y < INVENTORY_ROWS; y++) {
		for (int x = 0; x < INVENTORY_COLS; x++) {
			Item * item = this->get_item(x, y);
			if (item->is_empty()) {
				return std::pair<int, int>(x, y);
			}
		}
	}
	for (int x = 0; x < INVENTORY_COLS; x++) {
		Item * item = this->get_item(x, -1);
		if (item->is_empty()) {
			return std::pair<int, int>(x, -1);
		}
	}
	return std::pair<int, int>(-1, -1);
}

Inventory::Inventory()
{
	setClassName("Inventory");
	Register("inventory_key", &inventory_key);
	Register("inventory_items", &inventory_items);
	Register("money", &money);
}

Inventory::~Inventory()
{
}

void Inventory::load_content()
{

}

void Inventory::load_content_from_attributes()
{
	const int size = this->inventory_items.size();
	for (int i = 0; i < size; i++) {
		Item *item = this->inventory_items.getItem(i);
		item->load_content_from_attributes();
	}
}

void Inventory::unload_content()
{

}

void Inventory::reset()
{
	this->money = 0.0f;
	this->inventory_items.Clear();
	//this->initialize_empty();
}

void Inventory::initialize_empty()
{
	// start at -1 for hotbar
	for (int y = -1; y < INVENTORY_ROWS; y++) {
		for (int x = 0; x < INVENTORY_COLS; x++) {
			Item * item = Item::empty_item();
			item->set_inventory_pos(x, y);
			this->inventory_items.addItem(item);
		}
	}
}

const std::string Inventory::get_inventory_key()
{
	return this->inventory_key.value();
}

void Inventory::set_inventory_key(const std::string value)
{
	this->inventory_key = value;
}

void Inventory::set_hotbar_index(int num)
{
	hotbar_index = num;
}

void Inventory::hotbar_index_left()
{
	hotbar_index = hotbar_index > 0 ? hotbar_index - 1 : HOTBAR_SIZE - 1;
}

void Inventory::hotbar_index_right()
{
	hotbar_index = (hotbar_index + 1) % HOTBAR_SIZE;
}

void Inventory::swap_items(Inventory * other_inv, const int x1, const int y1, const int x2, const int y2)
{
	if (x1 == x2 && y1 == y2 && other_inv == this) {
		return;
	}
	Item *item_1 = this->get_item(x1, y1);
	Item *item_2 =other_inv->get_item(x2, y2);
	if (!item_1 || !item_2) return;
	item_1->swap_with_item(item_2);
}

void Inventory::swap_items(int x1, int y1, int x2, int y2)
{
	if (x1 == x2 && y1 == y2) return;
	Item *item_1 = get_item(x1, y1);
	Item *item_2 = get_item(x2, y2);
	if (!item_1 || !item_2) return;
	item_1->swap_with_item(item_2);
}

std::vector<std::vector<Item*>> Inventory::get_inventory_items()
{
	std::vector<std::vector<Item*>> items;
	for (int y = 0; y < INVENTORY_ROWS; y++) {
		items.push_back(std::vector<Item*>());
		for (int x = 0; x < INVENTORY_COLS; x++) {
			items[y].push_back(NULL);
		}
	}
	const int size = this->inventory_items.size();
	for (int i = 0; i < size; i++) {
		Item *item = this->inventory_items.getItem(i);
		int x = item->get_inventory_pos_x(), y = item->get_inventory_pos_y();
		if (item != NULL && x >= 0 && y >= 0) {
			items[y][x] = item;
		}
	}
	return items;
}

void Inventory::set_item(Item * item, const int x, const int y)
{
	if (item != NULL) {
		item->set_inventory_pos(x, y);
	}
}

void Inventory::add_item(Item * item)
{
	if (item != NULL && !this->is_full()) {
		const std::pair<int, int> item_pos = this->first_open_inventory_pos();
		Item * blank_item = this->get_item(item_pos.first, item_pos.second);
		item->copy_to_item(blank_item);
	}
}

Item * Inventory::get_selected_hotbar_item()
{
	const int size = this->inventory_items.size();
	for (int i = 0; i < size; i++) {
		Item * item = this->inventory_items.getItem(i);
		if (item->get_inventory_pos_y() == -1 && item->get_inventory_pos_x() == this->hotbar_index) {
			return item;
		}
	}
	return NULL;
}

Item * Inventory::get_hotbar_item(int index)
{
	const int size = this->inventory_items.size();
	for (int i = 0; i < size; i++) {
		Item * item = this->inventory_items.getItem(i);
		if (item->get_inventory_pos_y() == -1 && item->get_inventory_pos_x() == index) {
			return item;
		}
	}
	return NULL;
}

Item * Inventory::get_item(const int x, const int y)
{
	if (x < 0) return NULL;
	if (y >= 0) {
		const int size = this->inventory_items.size();
		for (int i = 0; i < size; i++) {
			Item *item = this->inventory_items.getItem(i);
			if (item->at_inventory_pos(x, y)) {
				return item;
			}
		}
		return NULL;
	} else {
		return get_hotbar_item(x);
	}
}

int Inventory::get_hotbar_index()
{
	return hotbar_index;
}

const float Inventory::get_money()
{
	return this->money.value();
}

const std::string Inventory::money_display_text()
{
	std::stringstream ss;
	ss.imbue(std::locale(""));
	// money is in dollars, so convert to cents
	ss << std::showbase << std::put_money(this->get_money() * 100.0f);
	return ss.str();
}

const bool Inventory::is_full()
{
	const std::pair<int, int> open_pos = this->first_open_inventory_pos();
	return open_pos.first < 0 && open_pos.second < 0;
}

const bool Inventory::has_item_with_key(const int item_key)
{
	return this->get_item_with_key(item_key) != NULL;
}

Item * Inventory::get_item_with_key(const int item_key)
{
	const int size = this->inventory_items.size();
	for (int i = 0; i < size; i++) {
		Item * item = this->inventory_items.getItem(i);
		if (item != NULL && !item->is_empty() && item_key == item->get_item_key()) {
			return item;
		}
	}
	return NULL;
}

void Inventory::remove_item_with_key(const int item_key)
{
	Item * item = this->get_item_with_key(item_key);
	if (item != NULL) {
		item->swap_with_item(Item::empty_item());
	}
}

void Inventory::adjust_money(const float adjust_amount)
{
	this->money = std::max(0.0f, this->money.value() + adjust_amount);
}
