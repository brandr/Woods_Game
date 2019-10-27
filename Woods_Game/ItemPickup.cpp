#include "ItemPickup.h"
#include "ItemManager.h"
#include "Level.h"
#include "World.h"


ItemPickup::ItemPickup()
{
	
}

ItemPickup::ItemPickup(const int item_key, const int x_pos, const int y_pos)
{
	this->item = ItemManager::get_instance().create_item(item_key);
	this->image_filename = "item_icons/pickups/" + this->item->get_item_name();
	ImageLoader::get_instance().load_image(this->image_filename, Rect(0, 0, PICKUP_WIDTH, PICKUP_HEIGHT));
	ALLEGRO_BITMAP * pickup_bitmap = ImageLoader::get_instance().get_image(this->image_filename);
	this->rect = Rect(0, 0, PICKUP_WIDTH, PICKUP_HEIGHT);
	this->set_position(x_pos, y_pos);
	this->refresh_mask();
}


ItemPickup::~ItemPickup()
{
}

int ItemPickup::get_type()
{
	return ITEM_PICKUP;
}

Item * ItemPickup::get_pickup_item()
{
	return this->item;
}
