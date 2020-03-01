#include "NPCSchedule.h"
#include "Level.h"
#include "World.h"

ScheduleTimeBlock * NPCSchedule::schedule_block_for_time(World * world, Level * level, GlobalTime * time)
{
	ScheduleTimeBlock * block_for_time = NULL;
	const std::vector<ScheduleTimeBlock *> blocks = this->matching_blocks(world, level, time);
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
const std::vector<ScheduleTimeBlock*> NPCSchedule::matching_blocks(World * world, Level * level, GlobalTime * time)
{
	std::vector<ScheduleTimeBlock*> matches;
	const int size = this->schedule_blocks.size();
	for (int i = 0; i < size; i++) {
		ScheduleTimeBlock * block = this->schedule_blocks.getItem(i);
		if (block->matches_time(world, level, time)) {
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

const std::string NPCSchedule::scheduled_node_key(World * world, Level * level, GlobalTime * time)
{
	ScheduleTimeBlock * schedule_block = this->schedule_block_for_time(world, level, time);
	if (schedule_block != NULL) {
		return schedule_block->node_key.value();
	}
	return "";
}

WanderZone * NPCSchedule::scheduled_wander_zone(World * world, Level * level, GlobalTime * time)
{
	ScheduleTimeBlock * schedule_block = this->schedule_block_for_time(world, level, time);
	if (schedule_block != NULL) {
		return &schedule_block->wander_zone;
	}
	return NULL;
}

const int NPCSchedule::scheduled_forced_animation_state(World * world, Level * level, GlobalTime * time)
{
	ScheduleTimeBlock * schedule_block = this->schedule_block_for_time(world, level, time);
	if (schedule_block != NULL) {
		return schedule_block->forced_anim_state.value();
	}
	return -1;
}

const int NPCSchedule::scheduled_forced_animation_direction(World * world, Level * level, GlobalTime * time)
{
	ScheduleTimeBlock * schedule_block = this->schedule_block_for_time(world, level, time);
	if (schedule_block != NULL) {
		return schedule_block->forced_anim_dir.value();
	}
	return -1;
}

ScheduledAction * NPCSchedule::scheduled_action(World * world, Level * level, GlobalTime * time)
{
	ScheduleTimeBlock * schedule_block = this->schedule_block_for_time(world, level, time);
	if (schedule_block != NULL && schedule_block->has_scheduled_action()) {
		return &schedule_block->scheduled_action;
	}
	return NULL;
}

DaySchedule::DaySchedule()
{
	setClassName("DaySchedule");
	Register("schedule_blocks", &schedule_blocks);
}

DaySchedule::~DaySchedule()
{
}

ScheduleTimeBlock * DaySchedule::schedule_block_for_time(World * world, Level * level, GlobalTime * time)
{
	const int size = this->schedule_blocks.size();
	for (int i = 0; i < size; i++) {
		ScheduleTimeBlock * block = this->schedule_blocks.getItem(i);
		if (block->matches_time(world, level, time)) {
			return block;
		}
	}
	return NULL;
}

ScheduleTimeBlock::ScheduleTimeBlock()
{
	this->setClassName("ScheduleTimeBlock");
	Register("qualifiers", &qualifiers);
	Register("wander_zone", &wander_zone);
	Register("node_key", &node_key);  
	Register("priority", &priority);
	Register("forced_anim_state", &forced_anim_state);
	Register("forced_anim_dir", &forced_anim_dir);
	Register("scheduled_action", &scheduled_action);
}

ScheduleTimeBlock::~ScheduleTimeBlock()
{
}

//TODO: probably want other match methods like quest triggers
const bool ScheduleTimeBlock::matches_time(World * world, Level * level, GlobalTime * time)
{
	const int size = this->qualifiers.size();
	for (int i = 0; i < size; i++) {
		Qualifier * q = this->qualifiers.getItem(i);
		q->set_other_time(time);
		if (!q->evaluate(world, level)) {
			return false;
		}
	}
	return true;
}

const int ScheduleTimeBlock::get_priority()
{
	return this->priority.value();
}

const bool ScheduleTimeBlock::has_scheduled_action()
{
	return this->scheduled_action.has_action();
}

ScheduledAction::ScheduledAction()
{
	setClassName("ScheduledAction");
	Register("scheduled_action_key", &scheduled_action_key);
	Register("action", &action);
	Register("action_object_type", &action_object_type);
	Register("action_object_tile_x", &action_object_tile_x);
	Register("action_object_tile_y", &action_object_tile_y);
}

ScheduledAction::~ScheduledAction()
{
}

const bool ScheduledAction::has_action()
{
	return !this->action.get_interact_action_key().empty();
}

const std::string ScheduledAction::get_scheduled_action_key()
{
	return this->scheduled_action_key.value();
}

const std::string ScheduledAction::get_action_object_type()
{
	return this->action_object_type.value();
}

const int ScheduledAction::get_action_object_tile_x()
{
	return this->action_object_tile_x.value();
}

const int ScheduledAction::get_action_object_tile_y()
{
	return this->action_object_tile_y.value();
}

Entity * ScheduledAction::get_action_object(World * world, Level * level, GlobalTime * time)
{
	const std::string object_type = this->get_action_object_type();
	if (object_type == "Block") {
		const int tx = this->get_action_object_tile_x(), ty = this->get_action_object_tile_y();
		Tile * t = level->get_tile(tx, ty);
		if (t != NULL) {
			Block * b = t->get_block();
			if (b != NULL && !b->is_empty()) {
				return b;
			}
		}
	}
	return NULL;
}
