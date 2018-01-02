#ifndef ENTITY_H
#define ENTITY_H

#include "EntityEffect.h"
#include "allegro5/display.h"  // for ALLEGRO_DISPLAY
#include "GameImage.h"         // for GameImage
#include "map"                 // for map
#include "utility"             // for pair, swap
#include "vector"              // for vector
#include "xstddef"             // for addressof, less
#include "xstring"             // for string
#include <memory>              // for allocator

struct EntityData {
	std::pair<int, int> root_offset;
	std::pair<int, int> center_offset;
	bool solid = false;
	std::map<std::string, int> attributes;
	EntityData();
};

class Entity :
	public GameImage
{
protected:
	bool solid = false;
	std::map<std::string, int> entity_attributes;
	std::map<int, int> counters;
	std::map<int, bool> entity_flags;
	std::map<std::string, EntityEffect> entity_effects;
public:
	Entity();
	~Entity();
	virtual void set_solid(bool solid);
	virtual bool is_solid();
	virtual void load_entity_effects(std::string filename, int row, std::pair<int, int> frame_dimensions);
	virtual void draw(ALLEGRO_DISPLAY* display, int x_offset, int y_offset);
	virtual void update();
	void counter_update();
	virtual void durability_update();
	virtual void entity_break();
	virtual void set_entity_attributes(std::map<std::string, int> attributes);
	virtual void set_entity_attributes(std::vector<std::string> attributes);
	virtual void set_entity_attribute(std::string attr, int val);
	virtual int get_entity_attribute(std::string attribute);
	virtual bool has_entity_attribute(std::string attribute);
	//virtual bool get_entity_flag(int flag_key);
	virtual void take_durability_damage(const int damage);
	static std::vector<std::string> get_entity_effect_names();
};
#endif