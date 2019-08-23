#ifndef AISTATE_H
#define AISTATE_H

#define AI_PATH_BLOCKED_WAIT_TIME 140 // wait frames if path is blocked
#define AI_FACE_OTHER_WAIT_TIME 200   // wait frames if we're facing some other being

#define AI_FACE_OTHER_MAX_TILE_DIST 4

#include <string>
#include <map>

enum AI_STATE_KEYS { 
	AI_STATE_IDLE, AI_STATE_LOCKED, AI_STATE_WAITING, 
	AI_STATE_WALKING, AI_STATE_REQUESTING_PATH, AI_STATE_STARTING_PATH, AI_STATE_REQUESTING_NEXT_LEVEL, AI_STATE_FACING_OTHER
};

enum AI_TIMER_KEYS {
	AI_TIMER_WAIT, AI_TIMER_FACE_OTHER
};

class AIState {
private:
	std::map<int, int> ai_timer_map;
	int state_key = AI_STATE_IDLE;
	std::string current_destination_node_key = "";
	std::string requested_next_level_key = "";
	std::string requested_next_level_node_key = "";
public:
	AIState();
	~AIState();
	void set_state_key(const int value);
	void timer_update();
	void set_is_idle();
	const bool is_idle();
	void set_is_locked();
	const bool is_locked();
	void set_is_waiting(const int wait_time);
	const bool is_waiting();
	void set_is_walking();
	const bool is_walking();
	const void set_is_requesting_path();
	const bool is_requesting_path();
	void set_is_requesting_next_level(const std::string level_key, const std::string node_key);
	void set_is_requesting_next_level_key(const std::string level_key, const std::string node_key);
	const std::string get_requested_next_level_key();
	const std::string get_requested_next_level_node_key();
	const bool is_requesting_next_level();
	const std::string get_current_destination_node_key();
	void set_current_destination_node_key(const std::string key);
	void set_is_facing_other(const int wait_time);
	const bool is_facing_other();
};

#endif