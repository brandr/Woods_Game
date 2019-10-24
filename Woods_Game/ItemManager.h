#ifndef ITEM_MANAGER_H
#define ITEM_MANAGER_H

#include <map>
#include <string>
#include "Item.h"
#include "FileManager.h"
#include "XMLSerialization.h"



//class Item;

struct ItemDefinitions : public xmls::Serializable {
	xmls::Collection<Item> items;
	ItemDefinitions();
	std::vector<Item *> get_items();
};

class ItemManager
{
private:
	ItemDefinitions item_definitions;
	ItemManager() {}
public:
	ItemManager(ItemManager const&) = delete;
	void operator=(ItemManager const&) = delete;
	static ItemManager& get_instance();
	~ItemManager();
	void load_item_templates();
	Item * create_item(const int item_key);
};

#endif