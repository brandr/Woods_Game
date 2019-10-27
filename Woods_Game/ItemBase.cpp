#include "ItemBase.h"
#include "ImageLoader.h"

const std::string ItemBase::image_path_prefix()
{
	return "item_icons";
}

ItemBase::ItemBase()
{
}

ItemBase::~ItemBase()
{
}

void ItemBase::load_content(const std::string name, const int item_key)
{
	std::string full_filename = "item_icons/" + name;
	ImageLoader::get_instance().load_image(full_filename);
	this->item_name = name;
	this->item_key = item_key;
}

void ItemBase::load_content_from_attributes()
{
	if (!this->is_empty()) {
		std::string full_filename = this->image_path_prefix() + "/" + this->get_item_name();
		ImageLoader::get_instance().load_image(full_filename);
	}
}

void ItemBase::unload_content()
{
}

void ItemBase::draw(ALLEGRO_DISPLAY * display, float x, float y)
{
	if (!this->is_empty()) {
		std::string full_filename = this->image_path_prefix() + "/" + this->get_item_name();
		ALLEGRO_BITMAP * icon = ImageLoader::get_instance().get_image(full_filename);
		al_draw_bitmap(icon, x, y, 0);
	}
}

const int ItemBase::get_item_key()
{
	return this->item_key.value();
}

const std::string ItemBase::get_item_name()
{
	return this->item_name.value();
}

const std::string ItemBase::get_item_display_name()
{
	return this->item_display_name.value();
}

const std::string ItemBase::get_item_description()
{
	return this->item_description.value();
}

const int ItemBase::get_inventory_pos_x()
{
	return this->inventory_pos_x.value();
}

const int ItemBase::get_inventory_pos_y()
{
	return this->inventory_pos_y.value();
}

void ItemBase::set_item_attributes(xmls::Collection<ItemAttribute> attributes)
{
	this->item_attributes = attributes;
}

void ItemBase::set_item_attributes(std::vector<ItemAttribute*> attributes)
{
	this->item_attributes.Clear();
	const int size = attributes.size();
	for (int i = 0; i < size; i++) {
		this->item_attributes.addItem(attributes[i]);
	}
}
const xmls::Collection<ItemAttribute> ItemBase::get_item_attributes()
{
	return this->item_attributes;
}
const std::vector<ItemAttribute*> ItemBase::get_item_attributes_vec()
{
	std::vector<ItemAttribute*> attributes;
	const int size = this->item_attributes.size();
	for (int i = 0; i < size; i++) {
		attributes.push_back(this->item_attributes.getItem(i));
	}
	return attributes;
}

const std::vector<ItemAttribute*> ItemBase::get_item_attributes_vec_copy()
{
	std::vector<ItemAttribute*> attributes;
	const int size = this->item_attributes.size();
	for (int i = 0; i < size; i++) {
		ItemAttribute * attr = this->item_attributes.getItem(i);
		attributes.push_back(new ItemAttribute(attr->attribute_key.value(), attr->attribute_value.value()));
	}
	return attributes;
}

const int ItemBase::get_item_attribute(const std::string attr_key)
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

const bool ItemBase::is_empty()
{
	return this->get_item_key() < 0;
}
