#include "Item.h"



Item::Item()
{
	setClassName("Item");
	Register("item_key", &item_key);
	Register("item_name", &item_name);
	Register("inventory_pos_x", &inventory_pos_x);
	Register("inventory_pos_y", &inventory_pos_y);
	Register("item_attributes", &item_attributes);
}


Item::~Item()
{
}

Item::Item(Item * copy)
{
	setClassName("Item");
	Register("item_key", &item_key);
	Register("item_name", &item_name);
	Register("inventory_pos_x", &inventory_pos_x);
	Register("inventory_pos_y", &inventory_pos_y);
	Register("item_attributes", &item_attributes);
	load_content(copy->get_filename(), copy->get_item_key());
	this->item_name = copy->get_filename();
	this->item_key = copy->get_item_key();
	set_item_attributes(copy->get_item_attributes());
}

void Item::load_content(std::string filename, int item_key)
{
	std::string full_filename = "resources/images/item_icons/" + filename + ".png";
	icon = al_load_bitmap(full_filename.c_str());
	al_convert_mask_to_alpha(icon, al_map_rgb(255, 0, 255));
	this->item_name = filename;
	this->item_key = item_key;
}

void Item::load_content_from_attributes()
{
	std::string full_filename = "resources/images/item_icons/" + this->get_filename() + ".png";
	icon = al_load_bitmap(full_filename.c_str());
	al_convert_mask_to_alpha(icon, al_map_rgb(255, 0, 255));
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

void Item::set_inventory_pos(int x, int y)
{
	this->inventory_pos_x = x;
	this->inventory_pos_y = y;
}

bool Item::at_inventory_pos(int x, int y)
{
	return this->inventory_pos_x.value() == x && this->inventory_pos_y.value() == y;
}

int Item::get_inventory_pos_x()
{
	return this->inventory_pos_x.value();
}

int Item::get_inventory_pos_y()
{
	return this->inventory_pos_y.value();
}

const int Item::get_item_key()
{
	return item_key.value();
}

const std::string Item::get_item_name()
{
	return this->item_name.value();
}

const std::string Item::get_filename()
{
	return this->item_name.value();
}


void Item::set_item_attributes(xmls::Collection<ItemAttribute> attributes)
{
	this->item_attributes = attributes;
}

/*void Item::set_item_attribute(int attr_key, int attr_val)
{
	item_attributes[attr_key] = attr_val;
}
*/
const xmls::Collection<ItemAttribute> Item::get_item_attributes()
{
	return this->item_attributes;
}

const int Item::get_item_attribute(std::string attr_key)
{
	const int size = this->item_attributes.size();
	for (int i = 0; i < size; i++) {
		ItemAttribute *attr = this->item_attributes.getItem(i);
		if (attr->attribute_key.value() == attr_key) {
			return attr->attribute_value.value();
		}
	}
	return 0;
}

ItemAttribute::ItemAttribute()
{
	setClassName("ItemAttribute");
	Register("attribute_key", &attribute_key);
	Register("attribute_value", &attribute_value);
}

ItemAttribute::ItemAttribute(std::string key, int value)
{
	setClassName("ItemAttribute");
	Register("attribute_key", &attribute_key);
	Register("attribute_value", &attribute_value);
	this->attribute_key = key;
	this->attribute_value = value;
}
