#ifndef NPC_SCHEDULE_H
#define NPC_SCHEDULE_H

#include "GlobalTime.h"
#include "Qualifier.h"
#include "XMLSerialization.h"

// TODO: remove start/stop time in favor of qualifiers
struct ScheduleTimeBlock : public xmls::Serializable {
	ScheduleTimeBlock();
	~ScheduleTimeBlock();
	//TODO: refactor and/or qualifiers because this serialization doesn't let us call superclass methods
	xmls::Collection<Qualifier> qualifiers;
	xmls::xInt priority;
	xmls::xString node_key; //TODO: either give this a more specific name to indicate we're forced to go there at that time or rework scheduled location logic
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

};
#endif