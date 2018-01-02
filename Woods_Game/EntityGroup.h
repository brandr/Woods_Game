#ifndef ENTITY_GROUP_H
#define ENTITY_GROUP_H

#include "Entity.h"
#include "allegro5/display.h"  // for ALLEGRO_DISPLAY
#include "vector"              // for vector
#include "xmemory0"            // for _Destroy_range
#include "xstddef"             // for _Unfancy
#include "xstring"             // for string
#include <memory>              // for allocator

struct EntityComponentData {
	std::string name = "";
	std::vector<std::string> attributes;
	EntityComponentData();
	EntityComponentData(std::string name, std::vector<std::string> attributes);
};

class EntityGroup:
	public Entity
{
private:
	std::vector<Entity*> entities;
public:
	EntityGroup();
	//EntityGroup(std::pair<int, int> root_offset, std::vector<EntityComponentData> component_data);
	~EntityGroup();
	virtual int get_type();
	void set_entities(std::vector<Entity*> entities);
	virtual void draw(ALLEGRO_DISPLAY* display, int x_offset, int y_offset);
};

#endif