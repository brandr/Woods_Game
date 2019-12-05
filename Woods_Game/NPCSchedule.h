#ifndef NPC_SCHEDULE_H
#define NPC_SCHEDULE_H

#include "GlobalTime.h"
#include "Qualifier.h"
#include "WanderZone.h"
#include "XMLSerialization.h"

struct ScheduleTimeBlock : public xmls::Serializable {
	ScheduleTimeBlock();
	~ScheduleTimeBlock();
	WanderZone wander_zone;
	xmls::Collection<Qualifier> qualifiers;
	xmls::xInt priority;
	xmls::xString node_key;
	xmls::xInt forced_anim_state;
	xmls::xInt forced_anim_dir;
	const bool matches_time(World * wold, Level * level, GlobalTime * time);
	const int get_priority();
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

};
#endif