#ifndef CRITTER_H
#define CRITTER_H

#include "AIBeing.h"
#include "Dialog.h"
#include "DialogTree.h"
#include "Encyclopedia.h"
#include "SpritesheetAnimation.h"
#include "WanderZone.h"

#include "XMLSerialization.h"

// A critter is an animal or bug.
// Critters are not serialized directly because they can spawn at random, but we serialize critter templates.

struct CritterTemplate : public xmls::Serializable {
	xmls::xString critter_key;
	xmls::xString encyclopedia_category_name;
	xmls::xString encyclopedia_entry_name;
	xmls::xInt spritesheet_frame_width, spritesheet_frame_height;
	xmls::xInt collide_x_offset, collide_y_offset;
	xmls::xInt collide_width, collide_height;
	xmls::xBool solid;
	xmls::xFloat base_speed;
	xmls::Collection<AnimationData> animation_data;
	xmls::Collection<AnimationData> walk_animation_data;
	WanderZone wander_zone;
	DialogItem catch_dialog_item;
	CritterTemplate();
	~CritterTemplate();
	const std::string get_critter_key();
	const std::string get_encyclopedia_category_name();
	const std::string get_encyclopedia_entry_name();
	const int get_spritesheet_frame_width();
	const int get_spritesheet_frame_height();
	const int get_collide_x_offset();
	const int get_collide_y_offset();
	const int get_collide_width();
	const int get_collide_height();
	const bool get_solid();
	const float get_base_speed();
	Dialog * get_catch_dialog();
};

class Level;
class World;
class Critter : public AIBeing
{
private:
	std::string critter_key;
	int level_critter_index = 0; // to ensure == distinguishes between critters
	WanderZone * wander_zone;
	std::pair<int, int> critter_spawn_pos;
protected:
	virtual void request_pathing_update(World * world, Level * level, GlobalTime * time);
	virtual void face_other_update(Level * level, GlobalTime * time);
	virtual void walk_to_next_level_update(Level * level);
	virtual WanderZone * current_wander_zone(World * world, Level * level, GlobalTime * time);
	virtual const std::pair<int, int> get_wander_zone_center(World * world, Level * level, GlobalTime * time);
	virtual const int get_seed_index();
	virtual const std::string get_footstep_filename_suffix();
public:
	Critter();
	Critter(CritterTemplate * critter_template, const int index);
	~Critter();
	virtual int get_type();
	virtual void update(World * world, Level * level, GlobalTime * time, const int game_mode);
	const std::string get_critter_key();
	const bool can_be_netted();
	virtual void update_visibility(World * world, Level * level);
	virtual const bool has_visibility_actions(Level * level);
	void set_critter_spawn_pos(const int x, const int y);
	
};

#endif