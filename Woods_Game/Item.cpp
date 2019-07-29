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

Item * Item::empty_item()
{
	return new Item();
}

void Item::load_content(std::string name, int item_key)
{
	std::string full_filename = "item_icons/" + name;
	ImageLoader::get_instance().load_image(full_filename);
	this->item_name = name;
	this->item_key = item_key;
}

void Item::load_content_from_attributes()
{
	if (!this->is_empty()) {
		std::string full_filename = "item_icons/" + this->get_filename();
		ImageLoader::get_instance().load_image(full_filename);
	}
}

void Item::unload_content()
{
}

void Item::swap_with_item(Item * other)
{
	if (this->is_empty() && other->is_empty()) {
		return;
	}
	const int key1 = this->get_item_key(), key2 = other->get_item_key();
	const std::string name1 = this->get_item_name(), name2 = other->get_item_name();
	const int xpos1 = this->get_inventory_pos_x(), xpos2 = other->get_inventory_pos_x();
	const int ypos1 = this->get_inventory_pos_y(), ypos2 = other->get_inventory_pos_y();
	const std::vector<ItemAttribute*> attr1 = this->get_item_attributes_vec(),
		attr2 = other->get_item_attributes_vec();
	this->unload_content();
	other->unload_content();
	// swap values
	this->item_key = key2, other->item_key = key1;
	this->item_name = name2, other->item_name = name1;
	this->set_item_attributes(attr2);
	other->set_item_attributes(attr1);
	this->load_content_from_attributes();
	other->load_content_from_attributes();
}

void Item::draw(ALLEGRO_DISPLAY * display, float x, float y)
{
	if (!this->is_empty()) {
		std::string full_filename = "item_icons/" + this->get_filename();
		ALLEGRO_BITMAP * icon = ImageLoader::get_instance().get_image(full_filename);
		al_draw_bitmap(icon, x, y, 0);
	}
}

bool Item::is_empty()
{
	return this->get_item_key() < 0;
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

void Item::set_item_attributes(std::vector<ItemAttribute*> attributes)
{
	this->item_attributes.Clear();
	const int size = attributes.size();
	for (int i = 0; i < size; i++) {
		this->item_attributes.addItem(attributes[i]);
	}
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

const std::vector<ItemAttribute*> Item::get_item_attributes_vec()
{
	std::vector<ItemAttribute*> attributes;
	const int size = this->item_attributes.size();
	for (int i = 0; i < size; i++) {
		attributes.push_back(this->item_attributes.getItem(i));
	}
	return attributes;
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
