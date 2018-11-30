#ifndef ITEM_H
#define ITEM_H

#include "allegro5/bitmap.h"   // for ALLEGRO_BITMAP
#include "allegro5/allegro5.h"
#include "allegro5/display.h"  // for ALLEGRO_DISPLAY
#include "map"                 // for map
#include "xstddef"             // for less
#include "xstring"             // for string
#include <memory>              // for allocator
#include "XMLSerialization.h"

enum ITEM_KEYS{ITEM_SHEARS = 0, ITEM_MALLET = 1};

struct ItemAttribute : public xmls::Serializable {
	xmls::xString attribute_key;
	xmls::xInt attribute_value;
	ItemAttribute();
	ItemAttribute(std::string key, int value);
};

class Item : public xmls::Serializable
{
private:
	ALLEGRO_BITMAP *icon;
	xmls::xInt item_key;
	xmls::xString item_name;
	xmls::xInt inventory_pos_x;
	xmls::xInt inventory_pos_y;
	xmls::Collection<ItemAttribute> item_attributes;
public:
	static constexpr const char* ITEM_ATTR_POWER = "power";

	Item();
	~Item();
	Item(Item* copy);
	static Item * empty_item();
	void load_content(std::string filename, int item_key);
	void load_content_from_attributes();
	void unload_content();
	void swap_with_item(Item * other);
	void draw(ALLEGRO_DISPLAY *display, float x, float y);
	bool is_empty();
	void set_inventory_pos(int x, int y);
	bool at_inventory_pos(int x, int y);
	int get_inventory_pos_x();
	int get_inventory_pos_y();
	const int get_item_key();
	const std::string get_item_name();
	const std::string get_filename();
	void set_item_attributes(xmls::Collection<ItemAttribute> attributes);
	void set_item_attributes(std::vector<ItemAttribute*> attributes);
	//void set_item_attribute(int attr_key, int attr_val);
	const xmls::Collection<ItemAttribute> get_item_attributes();
	const std::vector<ItemAttribute*> get_item_attributes_vec();
	const int get_item_attribute(std::string attr_key);
};

#endif