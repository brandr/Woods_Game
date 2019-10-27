#include "Item.h"
#include "ImageLoader.h"

Item::Item()
{
	setClassName("Item");
	Register("item_key", &item_key);
	Register("inventory_pos_x", &inventory_pos_x);
	Register("inventory_pos_y", &inventory_pos_y);
	Register("may_be_discarded", &may_be_discarded);
	Register("item_description", &item_description);
	Register("item_name", &item_name);
	Register("item_display_name", &item_display_name);
	Register("item_attributes", &item_attributes);
}


Item::~Item()
{
}

Item::Item(Item * copy)
{
	setClassName("Item");
	Register("item_key", &item_key);
	Register("inventory_pos_x", &inventory_pos_x);
	Register("inventory_pos_y", &inventory_pos_y);
	Register("may_be_discarded", &may_be_discarded);
	Register("item_description", &item_description);
	Register("item_name", &item_name);
	Register("item_display_name", &item_display_name);
	Register("item_attributes", &item_attributes);
	load_content(copy->get_item_name(), copy->get_item_key());
	this->item_name = copy->get_item_name();
	this->item_display_name = copy->get_item_display_name();
	this->item_description = copy->get_item_description();
	this->item_key = copy->get_item_key();	
	this->may_be_discarded = copy->get_may_be_discarded();
	set_item_attributes(copy->get_item_attributes());
}

Item * Item::empty_item()
{
	Item * item = new Item();
	item->item_key = -1;
	return item;
}

void Item::load_content(const std::string name, const int item_key)
{
	ItemBase::load_content(name, item_key);
}

void Item::unload_content()
{
}

// TODO: this might be annoying to maintain if items have too many properties to swap
void Item::swap_with_item(Item * other)
{
	if (this->is_empty() && other->is_empty()) {
		return;
	}
	const int key1 = this->get_item_key(), key2 = other->get_item_key();
	const std::string name1 = this->get_item_name(), name2 = other->get_item_name();
	const std::string display_name1 = this->get_item_display_name(), display_name2 = other->get_item_display_name();
	const std::string desc1 = this->get_item_description(), desc2 = other->get_item_description();
	const bool discard1 = this->get_may_be_discarded(), discard2 = other->get_may_be_discarded();
	const int xpos1 = this->get_inventory_pos_x(), xpos2 = other->get_inventory_pos_x();
	const int ypos1 = this->get_inventory_pos_y(), ypos2 = other->get_inventory_pos_y();
	const std::vector<ItemAttribute*> attr1 = this->get_item_attributes_vec_copy(),
		attr2 = other->get_item_attributes_vec_copy();
	this->unload_content();
	other->unload_content();
	// swap values
	this->item_key = key2, other->item_key = key1;
	this->item_name = name2, other->item_name = name1;
	this->item_display_name = display_name2, other->item_display_name = display_name1;
	this->item_description = desc2, other->item_description = desc1;
	this->may_be_discarded = discard2, other->may_be_discarded = discard1;
	this->set_item_attributes(attr2);
	other->set_item_attributes(attr1);
	this->load_content_from_attributes();
	other->load_content_from_attributes();
}

void Item::copy_to_item(Item * other)
{
	if (this->is_empty() && other->is_empty()) {
		return;
	}
	other->item_key = this->get_item_key();
	other->item_name = this->get_item_name();
	other->item_description = this->get_item_description();
	other->item_display_name = this->get_item_display_name();
	other->may_be_discarded = this->get_may_be_discarded();
	other->set_item_attributes(this->get_item_attributes_vec_copy());
	other->load_content_from_attributes();
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

const bool Item::get_may_be_discarded()
{
	return this->may_be_discarded.value();
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

ItemDrop::ItemDrop()
{
	setClassName("ItemDrop");
	Register("item_key", &item_key);
	Register("drop_rate", &drop_rate);
}
