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

class EntityGroup :
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
	virtual void draw(ALLEGRO_DISPLAY* display, const int x_offset, const int y_offset, const int screen_w, const int screen_h);
	virtual const bool should_draw(const int x_offset, const int y_offset, const int screen_w, const int screen_h);
	virtual int get_type();
	void set_entity_group_name(std::string name);
	void set_sheet_pos(int col, int row);
	std::vector<Entity*> get_entities();
	void set_entities(std::vector<Entity*> entities);
	void set_root_pos(std::pair<int, int> root_pos);
	void set_collide_offset(const std::pair<int, int> offset);
	void set_collide_dimensions(const std::pair<int, int> dimensions);
	virtual const bool intersects_area(Rect collide_rect);
	std::string get_entity_group_name();
	std::pair<int, int> get_root_pos();
};

#endif