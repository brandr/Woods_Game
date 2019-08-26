#ifndef AISTATE_H
#define AISTATE_H

#define AI_PATH_BLOCKED_WAIT_TIME 80  // wait frames if path is blocked
#define AI_FACE_OTHER_WAIT_TIME 200   // wait frames if we're facing some other being
#define AI_PUSH_OTHERS_TIME 300		  // how long we keep pushing others before going back to normal behavior
#define AI_FACE_OTHER_MAX_TILE_DIST 4

#include <string>
#include <map>

enum AI_STATE_KEYS { 
	AI_STATE_IDLE, AI_STATE_LOCKED, AI_STATE_WAITING, 
	AI_STATE_WALKING, AI_STATE_REQUESTING_PATH, AI_STATE_STARTING_PATH, 
	AI_STATE_WALKING_TO_NEXT_LEVEL, AI_STATE_WALKING_FROM_LEVEL,
	AI_STATE_REQUESTING_NEXT_LEVEL, AI_STATE_FACING_OTHER
};

enum AI_FLAG_KEYS {
	AI_FLAG_STOP_PUSHING_OTHERS
};

enum AI_TIMER_KEYS {
	AI_TIMER_WAIT, AI_TIMER_FACE_OTHER, AI_TIMER_PUSH_OTHERS, //AI_TIMER_DEPART_LEVEL, AI_TIMER_ARRIVE_LEVEL
};

class AIState {
private:
	std::map<int, int> ai_timer_map;
	std::map<int, int> ai_flag_map;
	int state_key = AI_STATE_IDLE;
	std::string current_destination_node_key = "";
	std::string requested_next_level_key = "";
	std::string requested_next_level_node_key = "";
	int next_level_x_dir = 0, next_level_y_dir = 0;
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
	void set_timer(const int timer_key, const int time);
	const bool is_waiting();
	void set_is_walking();
	const bool is_walking();
	const void set_is_requesting_path();
	const bool is_requesting_path();
	void set_is_requesting_next_level();
	void set_is_requesting_next_level(const std::string level_key, const std::string node_key);
	void set_is_requesting_next_level_key(const std::string level_key, const std::string node_key);
	void set_is_walking_to_next_level(const std::string level_key, const std::string node_key, const int x_dir, const int y_dir);
	void set_is_walking_from_level(const int x_dir, const int y_dir);
	const bool is_walking_to_next_level();
	const bool is_walking_from_level();
	const std::string get_requested_next_level_key();
	const std::string get_requested_next_level_node_key();
	const int get_next_level_x_dir();
	const int get_next_level_y_dir();
	const bool is_requesting_next_level();
	const std::string get_current_destination_node_key();
	void set_current_destination_node_key(const std::string key);
	void set_is_facing_other(const int wait_time);
	const bool is_facing_other();
	void set_flag(const int key, const int value);
	const int get_flag(const int key);
	const bool should_stop_pushing_others();
};
#endif