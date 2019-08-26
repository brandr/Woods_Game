#include "AIState.h"

AIState::AIState()
{
}

AIState::~AIState()
{
}

void AIState::set_state_key(const int value)
{
	this->state_key = value;
}

void AIState::timer_update()
{
	for (auto const &it : this->ai_timer_map) {
		const int time = it.second;
		if (time > 0) {
			this->ai_timer_map[it.first] = time - 1;
		} else if (time == 0) {
			if (it.first == AI_TIMER_WAIT && this->is_waiting()) {
				this->set_is_idle();
			} else if (it.first == AI_TIMER_FACE_OTHER && this->is_facing_other()) {
				this->set_is_idle();
			} else if (it.first == AI_TIMER_PUSH_OTHERS) {
				this->set_flag(AI_FLAG_STOP_PUSHING_OTHERS, 1);
			}
			this->ai_timer_map[it.first] = -1;
		}
	}
}

void AIState::set_is_idle()
{
	this->state_key = AI_STATE_IDLE;
}

const bool AIState::is_idle()
{
	return this->state_key == AI_STATE_IDLE;
}

void AIState::set_is_locked()
{
	this->state_key = AI_STATE_LOCKED;
}

const bool AIState::is_locked()
{
	return this->state_key == AI_STATE_LOCKED;
}

void AIState::set_is_waiting(const int wait_time)
{
	this->state_key = AI_STATE_WAITING;
	this->ai_timer_map[AI_TIMER_WAIT] = wait_time;
}

void AIState::set_timer(const int timer_key, const int time)
{
	this->ai_timer_map[timer_key] = time;
}

const bool AIState::is_waiting()
{
	return this->state_key == AI_STATE_WAITING;
}

void AIState::set_is_walking()
{
	this->state_key = AI_STATE_WALKING;
}

const bool AIState::is_walking()
{
	return this->state_key == AI_STATE_WALKING;
}

const bool AIState::is_requesting_path()
{
	return this->state_key == AI_STATE_REQUESTING_PATH;
}

void AIState::set_is_requesting_next_level()
{
	this->state_key = AI_STATE_REQUESTING_NEXT_LEVEL;
}

const void AIState::set_is_requesting_path()
{
	this->state_key = AI_STATE_REQUESTING_PATH;
}

void AIState::set_is_requesting_next_level(const std::string level_key, const std::string node_key)
{
	this->state_key = AI_STATE_REQUESTING_NEXT_LEVEL;
	this->requested_next_level_key = level_key;
	this->requested_next_level_node_key = node_key;
}

void AIState::set_is_requesting_next_level_key(const std::string level_key, const std::string node_key)
{
	this->requested_next_level_key = level_key;
	this->requested_next_level_node_key = node_key;
}

void AIState::set_is_walking_to_next_level(const std::string level_key, const std::string node_key, const int x_dir, const int y_dir)
{
	this->state_key = AI_STATE_WALKING_TO_NEXT_LEVEL;
	this->requested_next_level_key = level_key;
	this->requested_next_level_node_key = node_key;
	this->next_level_x_dir = x_dir, next_level_y_dir = y_dir;
	//this->set_timer(AI_TIMER_DEPART_LEVEL, AI_DEPART_LEVEL_TIME);
}

void AIState::set_is_walking_from_level(const int x_dir, const int y_dir)
{
	this->state_key = AI_STATE_WALKING_FROM_LEVEL;
	this->next_level_x_dir = x_dir, next_level_y_dir = y_dir;
	//this->set_timer(AI_TIMER_ARRIVE_LEVEL, AI_ARRIVE_LEVEL_TIME);
}

const bool AIState::is_walking_to_next_level()
{
	return this->state_key == AI_STATE_WALKING_TO_NEXT_LEVEL;
}

const bool AIState::is_walking_from_level()
{
	return this->state_key == AI_STATE_WALKING_FROM_LEVEL;
}

const std::string AIState::get_requested_next_level_key()
{
	return this->requested_next_level_key;
}

const std::string AIState::get_requested_next_level_node_key()
{
	return this->requested_next_level_node_key;
}

const int AIState::get_next_level_x_dir()
{
	return this->next_level_x_dir;
}

const int AIState::get_next_level_y_dir()
{
	return this->next_level_y_dir;
}

const bool AIState::is_requesting_next_level()
{
	return this->state_key == AI_STATE_REQUESTING_NEXT_LEVEL;
}

const std::string AIState::get_current_destination_node_key()
{
	return this->current_destination_node_key;
}

void AIState::set_current_destination_node_key(const std::string key)
{
	this->current_destination_node_key = key;
}

void AIState::set_is_facing_other(const int wait_time)
{
	this->state_key = AI_STATE_FACING_OTHER;
	this->ai_timer_map[AI_TIMER_FACE_OTHER] = wait_time;
}

const bool AIState::is_facing_other()
{
	return this->state_key == AI_STATE_FACING_OTHER;
}

void AIState::set_flag(const int key, const int value)
{
	this->ai_flag_map[key] = value;
}

const int AIState::get_flag(const int key)
{
	auto it = this->ai_flag_map.find(key);
	return it != this->ai_flag_map.end() ? it->second : 0;
}

const bool AIState::should_stop_pushing_others()
{
	return this->get_flag(AI_FLAG_STOP_PUSHING_OTHERS) > 0;
}
