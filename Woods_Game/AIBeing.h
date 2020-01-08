#define _USE_MATH_DEFINES
#ifndef AIBEING_H
#define AIBEING_H

#include "AIState.h"
#include "Being.h"
#include "WanderZone.h"
#include <math.h>

class Level;
class World;
class AIBeing : public Being
{
protected:
	//TODO: better checks to determine whether we display these bitmaps, also should store them in imageloader
	ALLEGRO_BITMAP * check_rect_bitmap; //TEMP
	ALLEGRO_BITMAP * path_bitmap; //TEMP

	AIState ai_state;

	Entity * facing_entity;

	std::pair<std::string, std::pair<int, int>> last_primary_destination = std::pair<std::string, std::pair<int, int>>("", std::pair<int, int>(-1, -1));
	std::vector<std::pair<std::string, std::pair<int, int>>> primary_destinations;		// places the being actually wants to go and will resume trying to reach after an interruption
	std::vector<std::pair<std::string, std::pair<int, int>>> secondary_destinations;	// places the being paths to, such as when going around obstacles
	std::pair<std::string, std::pair<int, int>> forced_destination = std::pair<std::string, std::pair<int, int>>("", std::pair<int, int>(-1, -1));
	bool should_path_around_moving = false;

	int failed_pathing_tally = 0;
	void ai_timer_update();
	virtual void request_pathing_update(World * world, Level * level, GlobalTime * time);
	virtual void destination_update(
		Level * level, GlobalTime * time);
	virtual void face_other_update(
		Level * level, GlobalTime * time);
	virtual void walk_update();
	virtual void failed_pathing_update();
	virtual void push_others_update(Level * level);
	virtual void walk_to_next_level_update(Level * level);
	virtual void wander_update(World * world, Level * level, GlobalTime * time);
	virtual void forced_animation_update();
	const std::pair<int, int> find_closest_open_tile_pos(
		Level * level,
		const std::pair<int, int> destination);
	const std::pair<int, int> find_closest_open_tile_pos(
		Level * level, 
		const std::pair<int, int> destination,
		const int range);
	const std::pair<std::string, std::pair<int, int>> get_next_destination();
	virtual void mark_destination_reached(const std::string dest_key);
	virtual void set_ai_state(const int state_key);
	virtual const std::string calculate_destination_node_key(World * world, GlobalTime * time);
	virtual const bool calculate_close_enough_to_node(World * world, Level * level, GlobalTime * time, const std::string node_key);
	virtual const bool should_wander(World * world, Level * level, GlobalTime * time);
	virtual WanderZone * current_wander_zone(World * world, Level * level, GlobalTime * time);
	virtual const std::pair<int, int> get_wander_zone_center(World * world, Level * level, GlobalTime * time);
	virtual const int forced_animation_state(World * world, Level * level, GlobalTime * time);
	virtual const int forced_animation_direction(World * world, Level * level, GlobalTime * time);
	virtual const int get_seed_index();
public:
	AIBeing();
	~AIBeing();
	virtual void update(World * world, Level * level, GlobalTime * time, const int game_mode);
	virtual void update_animation_dir();
	virtual void draw(ALLEGRO_DISPLAY* display, const int x_offset, const int y_offset, const int screen_w, const int screen_h);
	virtual void draw_adjacent_rect(ALLEGRO_DISPLAY* display, int x_offset, int y_offset);
	virtual void draw_destinations(ALLEGRO_DISPLAY* display, int x_offset, int y_offset);
	virtual void clear_primary_destinations();
	virtual void add_primary_destination(const std::pair<int, int> destination, const std::string dest_key);
	virtual const std::pair<int, int> first_primary_destination();
	virtual const bool has_primary_destinations();
	virtual const bool has_primary_destination(const std::pair<int, int> destination);
	virtual const bool has_primary_destination_with_key(const std::string dest_key);
	virtual void set_secondary_destinations(const std::vector<std::pair<std::string, std::pair<int, int>>> destinations);
	virtual const std::pair<std::string, std::pair<int, int>> get_forced_destination();
	virtual void set_forced_destination(const std::pair<std::string, std::pair<int, int>> value);
	virtual const std::pair<std::string, std::pair<int, int>> get_last_primary_destination();
	virtual void set_last_primary_destination(const std::pair<std::string, std::pair<int, int>> value);
	virtual const bool get_should_path_around_moving();
	virtual void set_should_path_around_moving(const bool value);
	virtual const bool pathing_blocked_at(const int x, const int y, Level * level, const bool ignore_moving_obstacles);
	virtual const bool pathing_blocked_by_moving_at(const int x, const int y, Level * level);
	virtual const bool pathing_blocked_by_moving_at(const int x, const int y, Level * level, const bool ignore_should_push_others);
	virtual Entity * blocking_entity_at(const int x, const int y, Level * level);
	virtual const bool pathing_blocked_at(const int x, const int y, std::vector<Entity*> interactables);
	virtual void stand_still();
	virtual void set_is_processing(const bool value);
	virtual const bool is_locked();
	virtual const bool needs_pathing_calculation();
	virtual void set_needs_pathing_calculation(const std::string dest_node_key);
	virtual void cancel_current_pathing(const int wait_time);
	virtual void set_is_starting_path();
	virtual void set_is_facing_other(Entity * other);
	virtual const bool is_facing_other();
	virtual const bool is_requesting_next_level();
	virtual const std::string get_requested_next_level_key();
	virtual const std::string get_requested_next_level_node_key();
	virtual void reset_failed_pathing_tally();
	virtual void increment_failed_pathing_tally();
	virtual void set_is_walking_from_level(const int x_dir, const int y_dir);
	virtual const bool get_obeys_tile_rules();
};

#endif