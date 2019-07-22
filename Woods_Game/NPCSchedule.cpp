#include "NPCSchedule.h"

DaySchedule * NPCSchedule::day_schedule_for_time(GlobalTime * time)
{
	//TODO: quest trigger logic
	//TODO: series of override checks (draw a flowchart or maybe I drew one already)

	return &default_day_schedule; //temp
}

NPCSchedule::NPCSchedule()
{
	setClassName("NPCSchedule");
	Register("default_day_schedule", &default_day_schedule);
}

NPCSchedule::~NPCSchedule()
{
}

const std::string NPCSchedule::scheduled_node_key(GlobalTime * time)
{
	//TODO: probably want to pass in quest triggers here
	DaySchedule * day_schedule = this->day_schedule_for_time(time);
	if (day_schedule != NULL) {
		ScheduleTimeBlock * schedule_block = day_schedule->schedule_block_for_time(time);
		if (schedule_block != NULL) {
			return schedule_block->node_key.value();
		}
	}
	return "";
}

DaySchedule::DaySchedule()
{
	setClassName("DaySchedule");
	Register("schedule_blocks", &schedule_blocks);
}

DaySchedule::~DaySchedule()
{
}

ScheduleTimeBlock * DaySchedule::schedule_block_for_time(GlobalTime * time)
{
	const int size = this->schedule_blocks.size();
	for (int i = 0; i < size; i++) {
		ScheduleTimeBlock * block = this->schedule_blocks.getItem(i);
		if (block->matches_time(time)) {
			return block;
		}
	}
	return NULL;
}

ScheduleTimeBlock::ScheduleTimeBlock()
{
	this->setClassName("ScheduleTimeBlock");
	Register("start_time", &start_time);
	Register("stop_time", &stop_time);
	Register("node_key", &node_key);  
}

ScheduleTimeBlock::~ScheduleTimeBlock()
{
}

const bool ScheduleTimeBlock::matches_time(GlobalTime * time)
{
	return time->get_time() >= this->start_time.value() && time->get_current_minutes() < this->stop_time.value();
}
