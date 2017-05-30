#ifndef ENTITY_H
#define ENTITY_H
//#include "Tile.h"
#include "GameImage.h"

class Entity :
	public GameImage
{
protected:
	bool solid;
	//Level* current_level;
public:
	Entity();
	~Entity();
	virtual bool is_solid();
};
#endif