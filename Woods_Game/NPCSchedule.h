#ifndef NPC_SCHEDULE_H
#define NPC_SCHEDULE_H

#include "GlobalTime.h"
#include "XMLSerialization.h"

struct ScheduleTimeBlock : public xmls::Serializable {
	ScheduleTimeBlock();
	~ScheduleTimeBlock();
	xmls::xInt start_time;
	xmls::xInt stop_time;
	xmls::xString node_key; //TODO: either give this a more specific name to indicate we're forced to go there at htat time or rework scheduled location logic
	const bool matches_time(GlobalTime * time);
};

struct DaySchedule : public xmls::Serializable {
	DaySchedule();
	~DaySchedule();
	xmls::Collection<ScheduleTimeBlock> schedule_blocks;
	ScheduleTimeBlock * schedule_block_for_time(GlobalTime * time);
};

class NPCSchedule : public xmls::Serializable {
private:
	DaySchedule default_day_schedule;

	DaySchedule *day_schedule_for_time(GlobalTime * time);
public:
	NPCSchedule();
	~NPCSchedule();
	const std::string scheduled_node_key(GlobalTime * time);

};
#endif