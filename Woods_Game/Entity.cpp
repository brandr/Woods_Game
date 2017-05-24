#include "Entity.h"



Entity::Entity()
{
	solid = false;
}


Entity::~Entity()
{
	//currentLevel = NULL;
}

bool Entity::is_solid()
{
	return solid;
}
