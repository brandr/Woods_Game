#include "Item.h"



Item::Item()
{
}


Item::~Item()
{
}

Item::Item(Item * copy)
{
	load_content(copy->get_filename(), copy->get_item_key());
	this->filename = copy->get_filename();
	this->item_key = copy->get_item_key();
	set_item_attributes(copy->get_item_attributes());
}

void Item::load_content(std::string filename, int item_key)
{
	std::string full_filename = "resources/images/item_icons/" + filename + ".png";
	icon = al_load_bitmap(full_filename.c_str());
	al_convert_mask_to_alpha(icon, al_map_rgb(255, 0, 255));
	this->filename = filename;
	this->item_key = item_key;
}

void Item::unload_content()
{
	if (icon) {
		al_destroy_bitmap(icon);
		icon = NULL;
	}
}

void Item::draw(ALLEGRO_DISPLAY * display, float x, float y)
{
	al_draw_bitmap(icon, x, y, 0);
}

const int Item::get_item_key()
{
	return item_key;
}

const std::string Item::get_filename()
{
	return filename;
}

void Item::set_item_attributes(std::map<int, int> attributes)
{
	this->item_attributes = attributes;
}

void Item::set_item_attribute(int attr_key, int attr_val)
{
	item_attributes[attr_key] = attr_val;
}

const std::map<int, int> Item::get_item_attributes()
{
	return this->item_attributes;
}

const int Item::get_item_attribute(int attr_key)
{
	auto it = item_attributes.find(attr_key);
	if (it != item_attributes.end()) return it->second;
	return 0;
}
