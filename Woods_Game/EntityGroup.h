#ifndef ENTITY_GROUP_H
#define ENTITY_GROUP_H

#include "Entity.h"
#include "allegro5/display.h"  // for ALLEGRO_DISPLAY
#include "vector"              // for vector
#include "xmemory0"            // for _Destroy_range
#include "xstddef"             // for _Unfancy
#include "xstring"             // for string
#include "XMLSerialization.h"
#include <memory>              // for allocator

//TODO: serializable stuff
class EntityGroup:
	public xmls::Serializable, public Entity
{
private:
	std::vector<Entity*> entities;
	xmls::xInt root_pos_x;
	xmls::xInt root_pos_y;
	xmls::xString entity_group_name;
public:
	EntityGroup();
	EntityGroup(std::pair<int, int> root_offset);
	~EntityGroup();
	virtual void draw(ALLEGRO_DISPLAY* display, int x_offset, int y_offset);
	void draw_bitmap_from_entities(std::vector<Entity *> entities);
	virtual int get_type();
	void set_entity_group_name(std::string name);
	void set_sheet_pos(int col, int row);
	std::vector<Entity*> get_entities();
	void set_entities(std::vector<Entity*> entities);
	void set_root_pos(std::pair<int, int> root_pos);
	std::string get_entity_group_name();
	std::pair<int, int> get_root_pos();
};

#endif