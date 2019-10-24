#include "ItemManager.h"
#include "Item.h"

ItemManager & ItemManager::get_instance()
{
	static ItemManager manager;
	return manager;
}

ItemManager::~ItemManager()
{
}

void ItemManager::load_item_templates()
{
	FileManager filemanager;
	const std::string filename = "resources/load/items";
	filemanager.load_xml_content(&(this->item_definitions), filename, "SerializableClass", "ItemDefinitionsKey", "default_definitions");
	const std::vector<Item *> items = this->item_definitions.get_items();
	for (Item * item : items) {
		item->load_content_from_attributes();
	}
}

Item * ItemManager::create_item(const int item_key)
{
	const std::vector<Item *> items = this->item_definitions.get_items();
	for (Item * item : items) {
		if (item->get_item_key() == item_key) {
			return item;
		}
	}
	return NULL;
}

ItemDefinitions::ItemDefinitions()
{
	setClassName("ItemDefinitions");
	Register("items", &items);
}

std::vector<Item*> ItemDefinitions::get_items()
{
	std::vector<Item*> items_vec;
	const int size = this->items.size();
	for (int i = 0; i < size; i++) {
		items_vec.push_back(this->items.getItem(i));
	}
	return items_vec;
}
