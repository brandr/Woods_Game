#define _USE_MATH_DEFINES
#ifndef AIBEING_H
#define AIBEING_H

#include "Being.h"
#include <math.h>

class Level;
class AIBeing : public Being
{
protected:
	ALLEGRO_BITMAP * check_rect_bitmap; //TEMP
	ALLEGRO_BITMAP * path_bitmap; //TEMP

	std::vector<std::pair<std::string, std::pair<int, int>>> primary_destinations;		// places the being actually wants to go and will resume trying to reach after an interruption
	std::vector<std::pair<std::string, std::pair<int, int>>> secondary_destinations;	// places the being paths to, such as when going around obstacles
	void destination_update(
		Level * level);
	void walk_update();
	const std::pair<std::vector<std::pair<int, int>>, int> calculate_shortest_path(
		const std::pair<int, int> origin_pos,
		Level * level,
		const std::pair<int, int> destination_pos);
	const std::pair<int, int> find_closest_open_tile_pos(
		const std::pair<int, int> destination, 
		std::vector<Entity*> interactables, 
		std::vector<Tile*> nearby_tiles,
		const std::pair<int, int> level_dimensions);
	const std::pair<int, int> find_closest_open_tile_pos(
		const std::pair<int, int> destination,
		std::vector<Entity*> interactables,
		std::vector<Tile*> nearby_tiles,
		const std::pair<int, int> level_dimensions,
		const int range);
	const std::pair<std::string, std::pair<int, int>> get_next_destination();
	virtual void clear_primary_destinations();
	virtual void mark_destination_reached(const std::string dest_key);
public:
	AIBeing();
	~AIBeing();
	virtual void update(Level * level, const int game_mode);
	virtual void draw(ALLEGRO_DISPLAY* display, int x_offset, int y_offset);
	virtual void draw_adjacent_rect(ALLEGRO_DISPLAY* display, int x_offset, int y_offset);
	virtual void draw_destinations(ALLEGRO_DISPLAY* display, int x_offset, int y_offset);
	virtual void add_primary_destination(const std::pair<int, int> destination, const std::string dest_key);
	virtual const bool has_primary_destinations();
	virtual const bool has_primary_destination(const std::pair<int, int> destination);
	virtual const bool has_primary_destination_with_key(const std::string dest_key);
	virtual const bool pathing_blocked_at(const int x, const int y, Level * level);
	virtual const bool pathing_blocked_at(const int x, const int y, Level * level, const bool ignore_moving_obstacles);
	virtual const bool pathing_blocked_at(const int x, const int y, std::vector<Entity*> interactables);
};

#endif