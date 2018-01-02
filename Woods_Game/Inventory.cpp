#include "Inventory.h"

Inventory::Inventory()
{
	for (int y = 0; y < INVENTORY_ROWS; y++) {
		inventory_items.push_back(std::vector<Item*>());
		for (int x = 0; x < INVENTORY_COLS; x++) {
			inventory_items[y].push_back(NULL);
		}
	}
}

Inventory::~Inventory()
{
}

void Inventory::load_content()
{
	for (int i = 0; i < HOTBAR_SIZE; i++) {
		hotbar.push_back(NULL);
	}
	//TEMP
	// shears
	Item *shears = new Item();
	shears->load_content("shears", ITEM_SHEARS);
	shears->set_item_attribute(ITEM_ATTR_POWER, 1);
	hotbar[0] = shears;

	// mallet
	Item *mallet = new Item();
	mallet->load_content("mallet", ITEM_MALLET);
	mallet->set_item_attribute(ITEM_ATTR_POWER, 1);
	hotbar[1] = mallet;
	//TODO: figure out a more long-term item loading system
	//TEMP
}

void Inventory::unload_content()
{
	for (int i = 0; i < hotbar.size(); i++) {
		if (hotbar[i]) {
			hotbar[i]->unload_content();
			delete hotbar[i];
		}
	}
	hotbar.clear();
}

std::vector<Item*> &Inventory::get_hotbar()
{
	return hotbar;
}

void Inventory::set_hotbar_index(int num)
{
	if (num >= 0 && num < hotbar.size()) hotbar_index = num;
}

void Inventory::hotbar_index_left()
{
	hotbar_index = hotbar_index > 0 ? hotbar_index - 1 : hotbar.size() - 1;
}

void Inventory::hotbar_index_right()
{
	hotbar_index = (hotbar_index + 1) % hotbar.size();
}

void Inventory::swap_items(int x1, int y1, int x2, int y2)
{
	if (x1 == x2 && y1 == y2) return;
	Item *item_1 = get_item(x1, y1);
	Item *item_2 = get_item(x2, y2);
	if (!item_1 || !item_2) return;
	Item *swapped_item_1 = new Item(item_1);
	Item *swapped_item_2 = new Item(item_2);
	item_1->unload_content(), item_2->unload_content();
	delete item_1, delete item_2;
	set_item(swapped_item_1, x2, y2);
	set_item(swapped_item_2, x1, y1);
}

std::vector<std::vector<Item*>> Inventory::get_inventory_items()
{
	return inventory_items;
}

void Inventory::set_item(Item * item, const int x, const int y)
{
	if (y < 0) {
		if (x < 0) return;
		hotbar[x] = item;
		return;
	}
	inventory_items[y][x] = item;
}

Item * Inventory::get_selected_hotbar_item()
{
	return hotbar[hotbar_index];
}

Item * Inventory::get_item(const int x, const int y)
{
	if (x < 0) return NULL;
	if(y >= 0) return inventory_items[y][x];
	else return get_hotbar_item(x);
}

Item * Inventory::get_hotbar_item(const int index)
{
	return hotbar[index];
}

int Inventory::get_hotbar_index()
{
	return hotbar_index;
}


