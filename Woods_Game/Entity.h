#ifndef ENTITY_H
#define ENTITY_H

#include "EntityEffect.h"
#include "allegro5/display.h"  // for ALLEGRO_DISPLAY
#include "GameImage.h"         // for GameImage
#include "InteractAction.h"
#include "InteractActionManager.h"
#include "map"                 // for map
#include "utility"             // for pair, swap
#include "vector"              // for vector
#include "xstddef"             // for addressof, less
#include "xstring"             // for string
#include <memory>              // for allocator
#include <filesystem>
#include <iostream>
#include "XMLSerialization.h"

struct EntityComponentAttribute : xmls::Serializable {
	xmls::xString name = "";
	EntityComponentAttribute();
	EntityComponentAttribute(std::string name);
};

struct EntityComponentData : xmls::Serializable {
	xmls::xString name = "";
	xmls::Collection<EntityComponentAttribute> attributes;
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
	xmls::Collection<InteractAction> contact_actions;
	xmls::Collection<InteractAction> interact_actions;
	xmls::xBool solid = false;
	xmls::xBool visible = true;
	xmls::Collection<EntityComponentData> components;
	EntityData();
	bool empty = true;
	void set_attributes(std::map<std::string, int> attributes);
	std::map<std::string, int> attribute_map();
	void set_root_offset(std::pair<int, int> offset);
	void set_center_offset(std::pair<int, int> offset);
	void set_components(std::vector<EntityComponentData*> components);
	std::vector<std::pair<std::string, std::string>> get_block_contact_action_data();
	std::vector<std::pair<std::string, std::string>> get_block_interact_action_data();
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

class Player;
class Entity :
	public GameImage
{
protected:
	xmls::xBool solid = false;
	xmls::xBool visible = true;
	xmls::Collection<EntityAttribute> entity_attributes;
	xmls::Collection<InteractAction> contact_actions;
	xmls::Collection<InteractAction> interact_actions;
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
	virtual void set_solid(const bool solid);
	virtual void set_visible(const bool visible);
	virtual bool is_solid();
	virtual bool is_visible();
	virtual void load_entity_effects(std::string filename, const std::string entity_key, int row, std::pair<int, int> frame_dimensions);
	virtual void load_content_from_attributes();
	virtual void draw(ALLEGRO_DISPLAY* display, int x_offset, int y_offset);
	virtual void update();
	void counter_update();
	virtual void durability_update();
	virtual const bool contact_action(Player * player);
	virtual const bool interact_action(Player * player);
	virtual void entity_break();
	virtual void set_entity_data_index(int index);
	virtual void set_entity_sheet_offset(int col, int row);
	virtual void set_entity_attributes(std::map<std::string, int> attributes);
	virtual void set_entity_attributes(std::vector<std::string> attributes);
	virtual void set_entity_attribute(std::string attr, int val);
	virtual void set_contact_actions(const std::vector<std::pair<std::string, std::string>> actions);
	virtual void set_interact_actions(const std::vector<std::pair<std::string, std::string>> actions);
	virtual void set_starting_pos(int x, int y);
	virtual int get_entity_data_index();
	virtual int get_entity_attribute(std::string attribute);
	virtual bool has_entity_attribute(std::string attribute);
	virtual int get_entity_sheet_col();
	virtual int get_entity_sheet_row();
	virtual void take_durability_damage(const int damage);
	static std::vector<std::string> get_entity_effect_names();
};
#endif