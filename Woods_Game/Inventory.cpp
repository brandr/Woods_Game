#include "Inventory.h"

Inventory::Inventory()
{
	setClassName("Inventory");
	Register("inventory_items", &inventory_items);
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


