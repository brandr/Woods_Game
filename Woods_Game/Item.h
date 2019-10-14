#ifndef ITEM_H
#define ITEM_H

#include "ItemBase.h"

#include "allegro5/bitmap.h"   // for ALLEGRO_BITMAP
#include "allegro5/allegro5.h"
#include "allegro5/display.h"  // for ALLEGRO_DISPLAY
#include "map"                 // for map
#include "xstddef"             // for less
#include "xstring"             // for string
#include <memory>              // for allocator
#include "XMLSerialization.h"
#include "ImageLoader.h"

enum ITEM_KEYS{ITEM_SHEARS = 0, ITEM_MALLET = 1};

class Item : 
	public ItemBase, public xmls::Serializable
{
public:
	static constexpr const char* ITEM_ATTR_POWER = "power";

	Item();
	~Item();
	Item(Item* copy);
	static Item * empty_item();
	virtual void load_content(const std::string filename, const int item_key);
	virtual void unload_content();
	void swap_with_item(Item * other);
	void set_inventory_pos(int x, int y);
	bool at_inventory_pos(int x, int y);
};

#endif