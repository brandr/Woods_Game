#include "EntityGroup.h"



EntityGroup::EntityGroup()
{

}


EntityGroup::~EntityGroup()
{
}

int EntityGroup::get_type()
{
	return ENTITY_GROUP;
}

void EntityGroup::set_entities(std::vector<Entity*> entities)
{
	this->entities = entities;
}

void EntityGroup::draw(ALLEGRO_DISPLAY * display, int x_offset, int y_offset)
{
	int size = entities.size();
	for (int i = 0; i < size; i++) {
		entities[i]->draw(display, x_offset, y_offset);
	}
	//temp
	//Mask_Draw(mask, rect.x + x_offset, rect.y + y_offset);
		//al_draw_bitmap(temp_mask_image, rect.x + x_offset, rect.y + y_offset, 0);
	//temp
}