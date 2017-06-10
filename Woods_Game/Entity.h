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
	virtual void set_solid(bool solid);
	virtual bool is_solid();
	virtual void draw(ALLEGRO_DISPLAY* display, int x_offset, int y_offset);
};
#endif