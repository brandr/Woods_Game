#ifndef CRITTER_H
#define CRITTER_H

#include "AIBeing.h"
#include "SpritesheetAnimation.h"

#include "XMLSerialization.h"

// A critter is an animal or bug.
// Critters are not serialized directly because they can spawn at random, but we should serialize critter templates

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
};

class Level;
class Critter : public AIBeing
{
private:
	std::string critter_key;
protected:
	virtual void mark_destination_reached(const std::string dest_key);
	virtual void request_pathing_update(World * world, Level * level, GlobalTime * time);
	virtual void face_other_update(Level * level, GlobalTime * time);
	virtual void walk_to_next_level_update(Level * level);
public:
	Critter();
	Critter(CritterTemplate * critter_template);
	~Critter();
	virtual int get_type();
	virtual void update(World * world, Level * level, GlobalTime * time, const int game_mode);
	const std::string get_critter_key();
	const bool can_be_netted();
	
};

#endif