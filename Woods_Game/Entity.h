#ifndef ENTITY_H
#define ENTITY_H

#include "EntityEffect.h"
#include "allegro5/display.h"  // for ALLEGRO_DISPLAY
#include "GameImage.h"         // for GameImage
//#include "EntityGroup.h"
#include "map"                 // for map
#include "utility"             // for pair, swap
#include "vector"              // for vector
#include "xstddef"             // for addressof, less
#include "xstring"             // for string
#include <memory>              // for allocator
#include "XMLSerialization.h"

struct EntityComponentAttribute : xmls::Serializable {
	xmls::xString name = "";
	EntityComponentAttribute();
	EntityComponentAttribute(std::string name);
};

struct EntityComponentData : xmls::Serializable {
	xmls::xString name = "";
	xmls::Collection<EntityComponentAttribute> attributes;
	//std::string name = "";
	//std::vector<std::string> attributes;
	EntityComponentData();
	EntityComponentData(std::string name, std::vector<std::string> attributes);
	void set_attributes(std::vector<std::string> attributes);
	std::string get_name();
	std::vector<std::string> get_attributes();
};

struct EntityAttribute : public xmls::Serializable {
	xmls::xString attribute_key;
	xmls::xInt attribute_value;
	EntityAttribute();
	EntityAttribute(std::string key, int value);
};



struct EntityData : public xmls::Serializable {
	xmls::xString entity_data_key;
	xmls::xInt entity_data_index;
	xmls::xInt root_offset_x;
	xmls::xInt root_offset_y;
	xmls::xInt center_offset_x;
	xmls::xInt center_offset_y;
	xmls::Collection<EntityAttribute> attributes;
	xmls::xBool solid = false;
	xmls::Collection<EntityComponentData> components;
	//std::map<std::string, int> attributes;
	EntityData();
	bool empty = true;
	void set_attributes(std::map<std::string, int> attributes);
	std::map<std::string, int> attribute_map();
	void set_root_offset(std::pair<int, int> offset);
	void set_center_offset(std::pair<int, int> offset);
	void set_components(std::vector<EntityComponentData*> components);
	bool is_empty();
	std::string get_entity_data_key();
	std::pair<int, int> get_root_offset();
	std::pair<int, int> get_center_offset();
	std::vector<EntityComponentData*> get_components();
};

struct EntityGroupData : public EntityData {
	xmls::xString entity_group_name;
	xmls::xInt entity_group_index;
	xmls::xInt entity_group_image_width, entity_group_image_height;
	EntityGroupData();
	std::string get_entity_group_name();
	int get_entity_group_index();
	void set_entity_group_image_dimensions(std::pair<int, int> dimensions);
	std::pair<int, int> get_entity_group_image_dimensions();
};

class Entity :
	public GameImage
{
protected:
	xmls::xBool solid = false;
	//std::map<std::string, int> entity_attributes;
	xmls::Collection<EntityAttribute> entity_attributes;
	xmls::xInt entity_data_index;
	xmls::xInt entity_sheet_col;	
	xmls::xInt entity_sheet_row;
	xmls::xInt entity_starting_pos_x;
	xmls::xInt entity_starting_pos_y;
	std::map<int, int> counters;
	std::map<int, bool> entity_flags;
	std::map<std::string, EntityEffect> entity_effects;
public:
	Entity();
	~Entity();
	virtual Rect *get_bitmap_subsection();
	virtual void set_solid(bool solid);
	virtual bool is_solid();
	virtual void load_entity_effects(std::string filename, int row, std::pair<int, int> frame_dimensions);
	virtual void load_content_from_attributes();
	virtual void draw(ALLEGRO_DISPLAY* display, int x_offset, int y_offset);
	virtual void update();
	void counter_update();
	virtual void durability_update();
	virtual void entity_break();
	virtual void set_entity_data_index(int index);
	virtual void set_entity_sheet_offset(int col, int row);
	virtual void set_entity_attributes(std::map<std::string, int> attributes);
	virtual void set_entity_attributes(std::vector<std::string> attributes);
	virtual void set_entity_attribute(std::string attr, int val);
	virtual void set_starting_pos(int x, int y);
	virtual int get_entity_data_index();
	virtual int get_entity_attribute(std::string attribute);
	virtual bool has_entity_attribute(std::string attribute);
	virtual int get_entity_sheet_col();
	virtual int get_entity_sheet_row();
	//virtual bool get_entity_flag(int flag_key);
	virtual void take_durability_damage(const int damage);
	static std::vector<std::string> get_entity_effect_names();
};
#endif