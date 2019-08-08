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

struct EntitySpawnTileRule : xmls::Serializable {
	xmls::xInt tile_type_index;
	EntitySpawnTileRule();
};

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
	xmls::xInt collide_offset_x;
	xmls::xInt collide_offset_y;
	xmls::xInt collide_width;
	xmls::xInt collide_height;
	xmls::xInt center_offset_x;
	xmls::xInt center_offset_y;
	xmls::Collection<EntityAttribute> attributes;
	xmls::Collection<InteractAction> contact_actions;
	xmls::Collection<InteractAction> interact_actions;
	xmls::Collection<InteractAction> load_day_actions;
	xmls::Collection<EntitySpawnTileRule> spawn_tile_rules;
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
	std::vector<std::pair<std::string, std::string>> get_block_load_day_action_data();
	const std::vector<EntitySpawnTileRule *> get_block_spawn_tile_rules();
	bool is_empty();
	std::string get_entity_data_key();
	std::pair<int, int> get_root_offset();
	const std::pair<int, int> get_collide_offset();
	const std::pair<int, int> get_collide_dimensions();
	std::pair<int, int> get_center_offset();
	std::vector<EntityComponentData*> get_components();
	const std::vector<std::string> image_filename_suffixes();
	const bool has_entity_attribute(const std::string attr_key);
	int get_entity_attribute(const std::string attribute);
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
	xmls::Collection<InteractAction> load_day_actions;
	xmls::Collection<EntitySpawnTileRule> spawn_tile_rules;
	xmls::xInt entity_data_index;
	xmls::xInt entity_sheet_col;
	xmls::xInt entity_sheet_row;
	xmls::xInt entity_starting_pos_x;
	xmls::xInt entity_starting_pos_y;
	const int get_collide_x_offset();
	const int get_collide_y_offset();
	const int get_collide_width();
	const int get_collide_height();
	std::map<int, int> counters;
	std::map<std::string, EntityEffect> entity_effects;
public:
	Entity();
	~Entity();
	virtual void copy_entity_attributes(Entity * other);
	virtual Rect *get_bitmap_subsection();
	virtual const std::string image_filename_suffix();
	virtual void set_solid(const bool solid);
	virtual void set_visible(const bool visible);
	virtual bool is_solid();
	virtual bool is_visible();
	virtual void load_entity_effects(std::string filename, const std::string entity_key, int row, std::pair<int, int> frame_dimensions);
	virtual void load_content_from_attributes();
	virtual void unload_content();
	virtual void draw(ALLEGRO_DISPLAY* display, int x_offset, int y_offset);
	virtual void update();
	void counter_update();
	virtual void durability_update();
	virtual void mark_needs_plant_day_update();
	virtual void unmark_needs_plant_day_update();
	virtual const bool needs_plant_day_update();
	virtual const int get_plant_growth_spread_range();
	virtual const bool update_new_day(Player * player);
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
	virtual void set_load_day_actions(const std::vector<std::pair<std::string, std::string>> actions);
	virtual void set_spawn_tile_rules(const std::vector<EntitySpawnTileRule *> rules);
	virtual void set_starting_pos(int x, int y);
	virtual int get_entity_starting_pos_x();
	virtual int get_entity_starting_pos_y();
	virtual const std::vector<int> get_allowed_spawn_tile_types();
	virtual int get_entity_data_index();
	virtual int get_entity_attribute(std::string attribute);
	virtual std::map<std::string, int> get_entity_attributes();
	virtual bool has_entity_attribute(std::string attribute);
	virtual int get_entity_sheet_col();
	virtual int get_entity_sheet_row();
	virtual void take_durability_damage(const int damage);
	static std::vector<std::string> get_entity_effect_names();
};
#endif