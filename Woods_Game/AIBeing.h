#define _USE_MATH_DEFINES
#ifndef AIBEING_H
#define AIBEING_H

#include "AIState.h"
#include "Being.h"
#include <math.h>

class Level;
class AIBeing : public Being
{
protected:
	//TODO: these bitmaps might be causing problems w/ performance, refactor and/or don't show
	ALLEGRO_BITMAP * check_rect_bitmap; //TEMP
	ALLEGRO_BITMAP * path_bitmap; //TEMP

	AIState ai_state;

	std::vector<std::pair<std::string, std::pair<int, int>>> primary_destinations;		// places the being actually wants to go and will resume trying to reach after an interruption
	std::vector<std::pair<std::string, std::pair<int, int>>> secondary_destinations;	// places the being paths to, such as when going around obstacles
	std::pair<std::string, std::pair<int, int>> forced_destination = std::pair<std::string, std::pair<int, int>>("", std::pair<int, int>(-1, -1));
	void ai_timer_update();
	void request_pathing_update(Level * level, GlobalTime * time);
	void destination_update(
		Level * level, GlobalTime * time);
	void walk_update();
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
	virtual const std::string calculate_destination_node_key(GlobalTime * time);
public:
	AIBeing();
	~AIBeing();
	virtual void update(Level * level, GlobalTime * time, const int game_mode);
	virtual void draw(ALLEGRO_DISPLAY* display, int x_offset, int y_offset);
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
	virtual const bool pathing_blocked_at(const int x, const int y, Level * level, const bool ignore_moving_obstacles);
	virtual const bool pathing_blocked_at(const int x, const int y, std::vector<Entity*> interactables);
	virtual void set_is_processing(const bool value);
	virtual const bool is_locked();
	virtual const bool needs_pathing_calculation();
	virtual void set_needs_pathing_calculation(const std::string dest_node_key);
	virtual void cancel_current_pathing();
	virtual void set_is_starting_path();
	virtual const bool is_requesting_next_level();
	virtual const std::string get_requested_next_level_key();
	virtual const std::string get_requested_next_level_node_key();
};

#endif