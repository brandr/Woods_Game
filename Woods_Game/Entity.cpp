#include "Entity.h"

Entity::Entity()
{
	solid = false;
}


Entity::~Entity()
{
	//currentLevel = NULL;
}

void Entity::set_solid(bool solid)
{
	this->solid = solid;
}

bool Entity::is_solid()
{
	return solid;
}

void Entity::draw(ALLEGRO_DISPLAY * display, int x_offset, int y_offset)
{
	GameImage::draw(display, x_offset, y_offset);
}
