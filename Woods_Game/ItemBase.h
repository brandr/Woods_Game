#ifndef ITEM_BASE_H
#define ITEM_BASE_H

#include "ImageLoader.h"

#include "allegro5/bitmap.h"   // for ALLEGRO_BITMAP
#include "allegro5/allegro5.h"
#include "allegro5/display.h"  // for ALLEGRO_DISPLAY
#include "XMLSerialization.h"

struct ItemAttribute : public xmls::Serializable {
	xmls::xString attribute_key;
	xmls::xInt attribute_value;
	ItemAttribute();
	ItemAttribute(std::string key, int value);
};

class ItemBase
{
protected:
	xmls::xInt item_key;
	xmls::xString item_name;
	xmls::xString item_display_name;
	xmls::xString item_description;
	xmls::xInt inventory_pos_x;
	xmls::xInt inventory_pos_y;
	xmls::xBool may_be_discarded = false;
	xmls::Collection<ItemAttribute> item_attributes;
	
	virtual const std::string image_path_prefix();
public:
	static constexpr const char* ITEM_ATTR_POWER = "power";

	ItemBase();
	~ItemBase();
	virtual void load_content(const std::string filename, const int item_key);
	virtual void load_content_from_attributes();
	virtual void unload_content();
	virtual void draw(ALLEGRO_DISPLAY *display, float x, float y);
	virtual const int get_item_key();
	virtual const std::string get_item_name();
	virtual const std::string get_item_display_name();
	virtual const std::string get_item_description();
	virtual void set_item_attributes(xmls::Collection<ItemAttribute> attributes);
	virtual void set_item_attributes(std::vector<ItemAttribute*> attributes);
	virtual const xmls::Collection<ItemAttribute> get_item_attributes();
	virtual const std::vector<ItemAttribute*> get_item_attributes_vec();
	virtual const std::vector<ItemAttribute*> get_item_attributes_vec_copy();
	virtual const int get_item_attribute(const std::string attr_key);
	virtual const bool is_empty();
	virtual const int get_inventory_pos_x();
	virtual const int get_inventory_pos_y();
};

#endif