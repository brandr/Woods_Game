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
		} else {
			if (it.first == AI_TIMER_WAIT && this->is_waiting()) {
				this->set_is_idle();
			} else if (it.first == AI_TIMER_FACE_OTHER && this->is_facing_other()) {
				this->set_is_idle();
			}
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

const std::string AIState::get_requested_next_level_key()
{
	return this->requested_next_level_key;
}

const std::string AIState::get_requested_next_level_node_key()
{
	return this->requested_next_level_node_key;
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
