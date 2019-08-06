#include "NPCSchedule.h"

ScheduleTimeBlock * NPCSchedule::schedule_block_for_time(GlobalTime * time)
{
	ScheduleTimeBlock * block_for_time = NULL;
	const std::vector<ScheduleTimeBlock *> blocks = this->matching_blocks(time);
	if (blocks.size() == 1) {
		return blocks[0];
	} else if (blocks.size() > 1) {
		ScheduleTimeBlock * highest_block = NULL;
		int highest_priority = -1;
		for (ScheduleTimeBlock * block : blocks) {
			if (highest_priority < 0
				|| highest_block == NULL
				|| block->get_priority() > highest_priority) {
				highest_priority = block->get_priority();
				highest_block = block;
			}
		}
		block_for_time = highest_block;
	}
	return block_for_time;
}

DaySchedule * NPCSchedule::day_schedule_for_time(GlobalTime * time)
{
	//const std::vector<ScheduleTimeBlock *> blocks = this->matching_blocks(time);
	//TODO: quest trigger logic
	//TODO: series of override checks (draw a flowchart or maybe I drew one already)

	return &default_day_schedule; //temp
}

// TODO: other conditions besides time
const std::vector<ScheduleTimeBlock*> NPCSchedule::matching_blocks(GlobalTime * time)
{
	std::vector<ScheduleTimeBlock*> matches;
	const int size = this->schedule_blocks.size();
	for (int i = 0; i < size; i++) {
		ScheduleTimeBlock * block = this->schedule_blocks.getItem(i);
		if (block->matches_time(time)) {
			matches.push_back(block);
		}
	}
	return matches;
}

NPCSchedule::NPCSchedule()
{
	setClassName("NPCSchedule");
	Register("default_day_schedule", &default_day_schedule);
	Register("schedule_blocks", &schedule_blocks);
}

NPCSchedule::~NPCSchedule()
{
}

const std::string NPCSchedule::scheduled_node_key(GlobalTime * time)
{
	//TODO: probably want to pass in quest triggers here
	ScheduleTimeBlock * schedule_block = this->schedule_block_for_time(time);
	if (schedule_block != NULL) {
		return schedule_block->node_key.value();
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
	Register("qualifiers", &qualifiers);
	//Register("start_time", &start_time);
	//Register("stop_time", &stop_time);
	Register("node_key", &node_key);  
}

ScheduleTimeBlock::~ScheduleTimeBlock()
{
}

//TODO: probably want other match methods like quest triggers
const bool ScheduleTimeBlock::matches_time(GlobalTime * time)
{
	const int size = this->qualifiers.size();
	for (int i = 0; i < size; i++) {
		Qualifier * q = this->qualifiers.getItem(i);
		q->set_other_time(time);
		if (!q->evaluate()) {
			return false;
		}
	}
	return true;
	/*
	return time->get_total_minutes() 
		>= this->start_time.value() 
		&& time->get_total_minutes() < this->stop_time.value();
		*/
}

const int ScheduleTimeBlock::get_priority()
{
	return this->priority.value();
}
