#ifndef NPC_SCHEDULE_H
#define NPC_SCHEDULE_H

#include "Entity.h"
#include "GlobalTime.h"
#include "InteractAction.h"
#include "Qualifier.h"
#include "WanderZone.h"
#include "XMLSerialization.h"

struct ScheduledAction : public xmls::Serializable {
	ScheduledAction();
	~ScheduledAction();
	InteractAction action;
	xmls::xString scheduled_action_key;
	xmls::xString action_object_type;
	xmls::xInt action_object_tile_x;
	xmls::xInt action_object_tile_y;
	const bool has_action();
	const std::string get_scheduled_action_key();
	const std::string get_action_object_type();
	const int get_action_object_tile_x();
	const int get_action_object_tile_y();
	Entity * get_action_object(World * world, Level * level, GlobalTime * time);
};

struct ScheduleTimeBlock : public xmls::Serializable {
	ScheduleTimeBlock();
	~ScheduleTimeBlock();
	WanderZone wander_zone;
	xmls::Collection<Qualifier> qualifiers;
	xmls::xInt priority;
	xmls::xString node_key;
	xmls::xInt forced_anim_state;
	xmls::xInt forced_anim_dir;
	ScheduledAction scheduled_action; // TODO: should this be a collection?
	const bool matches_time(World * wold, Level * level, GlobalTime * time);
	const int get_priority();
	const bool has_scheduled_action();
};

struct DaySchedule : public xmls::Serializable {
	DaySchedule();
	~DaySchedule();
	xmls::Collection<ScheduleTimeBlock> schedule_blocks;
	ScheduleTimeBlock * schedule_block_for_time(World * world, Level * level, GlobalTime * time);
};

class Level;
class World;
class NPCSchedule : public xmls::Serializable {
private:
	DaySchedule default_day_schedule;
	xmls::Collection<ScheduleTimeBlock> schedule_blocks;
	ScheduleTimeBlock * schedule_block_for_time(World * world, Level * level, GlobalTime * time);
	DaySchedule *day_schedule_for_time(GlobalTime * time);
	const std::vector<ScheduleTimeBlock *> matching_blocks(World * world, Level * level, GlobalTime * time);
public:
	NPCSchedule();
	~NPCSchedule();
	const std::string scheduled_node_key(World * world, Level * level, GlobalTime * time);
	WanderZone * scheduled_wander_zone(World * world, Level * level, GlobalTime * time);
	const int scheduled_forced_animation_state(World * world, Level * level, GlobalTime * time);
	const int scheduled_forced_animation_direction(World * world, Level * level, GlobalTime * time);
	ScheduledAction * scheduled_action(World * world, Level * level, GlobalTime * time);

};
#endif