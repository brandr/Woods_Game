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
	if (state_key != AI_STATE_FORCING_ANIMATION) {
		this->forced_anim_state = -1;
		this->forced_anim_dir = -1;
	}
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
	this->set_state_key(AI_STATE_IDLE);
}

const bool AIState::is_idle()
{
	return this->state_key == AI_STATE_IDLE;
}

void AIState::set_is_locked()
{
	this->set_state_key(AI_STATE_LOCKED);
}

const bool AIState::is_locked()
{
	return this->state_key == AI_STATE_LOCKED;
}

void AIState::set_is_waiting(const int wait_time)
{
	this->set_state_key(AI_STATE_WAITING);
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
	this->set_state_key(AI_STATE_WALKING);
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
	this->set_state_key(AI_STATE_REQUESTING_NEXT_LEVEL);
}

const void AIState::set_is_requesting_path()
{
	this->set_state_key(AI_STATE_REQUESTING_PATH);
}

void AIState::set_is_requesting_next_level(const std::string level_key, const std::string node_key)
{
	this->set_state_key(AI_STATE_REQUESTING_NEXT_LEVEL);
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
	this->set_state_key(AI_STATE_WALKING_TO_NEXT_LEVEL);
	this->requested_next_level_key = level_key;
	this->requested_next_level_node_key = node_key;
	this->next_level_x_dir = x_dir, next_level_y_dir = y_dir;
}

void AIState::set_is_walking_from_level(const int x_dir, const int y_dir)
{
	this->set_state_key(AI_STATE_WALKING_FROM_LEVEL);
	this->next_level_x_dir = x_dir, next_level_y_dir = y_dir;
}

const bool AIState::is_walking_to_next_level()
{
	return this->state_key == AI_STATE_WALKING_TO_NEXT_LEVEL;
}

const bool AIState::is_walking_from_level()
{
	return this->state_key == AI_STATE_WALKING_FROM_LEVEL;
}

const bool AIState::is_wandering()
{
	return this->state_key == AI_STATE_WANDERING;
}

const bool AIState::may_wander()
{
	return this->ai_timer_map.find(AI_TIMER_DELAY_WANDER) == this->ai_timer_map.end()
		|| this->ai_timer_map[AI_TIMER_DELAY_WANDER] <= 0;
}

const bool AIState::is_forcing_animation()
{
	return this->state_key == AI_STATE_FORCING_ANIMATION;
}

void AIState::set_forced_anim_state(const int anim_state)
{
	this->state_key = AI_STATE_FORCING_ANIMATION;
	this->forced_anim_state = anim_state;
}

const int AIState::get_forced_anim_state()
{
	return this->forced_anim_state;
}

void AIState::set_forced_anim_dir(const int anim_dir)
{
	this->state_key = AI_STATE_FORCING_ANIMATION;
	this->forced_anim_dir = anim_dir;
}

const int AIState::get_forced_anim_dir()
{
	return this->forced_anim_dir;
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
