#define _USE_MATH_DEFINES
#ifndef AIBEING_H
#define AIBEING_H

#include "Being.h"
#include <math.h>

//class Player;
class AIBeing : public Being
{
protected:
	ALLEGRO_BITMAP * check_rect_bitmap; //TEMP
	ALLEGRO_BITMAP * path_bitmap; //TEMP

	std::vector<std::pair<int, int>> primary_destinations;		// places the being actually wants to go and will resume trying to reach after an interruption
	std::vector<std::pair<int, int>> secondary_destinations;	// places the being paths to, such as when going around obstacles
	void destination_update(
		TileSet * tileset, std::vector<Entity*> interactables, std::vector<Tile*> nearby_tiles, const std::pair<int, int> level_dimensions);
	void walk_update(
		TileSet * tileset, std::vector<Entity*> interactables, std::vector<Tile*> nearby_tiles, const std::pair<int, int> level_dimensions);
	const std::pair<std::vector<std::pair<int, int>>, int> calculate_shortest_path(
		const std::pair<int, int> origin_pos,
		const std::pair<int, int> start_pos,
		std::vector<Entity*> interactables,
		const std::pair<int, int> level_dimensions,
		const std::pair<int, int> destination_pos,
		std::map<
		std::pair<int, int>,
		std::map<
		std::pair<int, int>,
		int>> &distances,
		std::vector <std::pair<int, int>> ongoing_path,
		const int dist_tally);
	const std::vector<std::pair<int, int>> open_adjacent_pathable_tiles(std::vector<Entity*> interactables,
		const std::pair<int, int> level_dimensions,
		const std::pair<int, int> start_tile_pos,
		const std::vector<std::pair<int, int>> visited,
		const std::pair<int, int> dest_tile_pos);
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
	const std::pair<int, int> get_next_destination();
	const bool pathing_blocked_at(const int x, const int y, std::vector<Entity*> interactables);
public:
	AIBeing();
	~AIBeing();
	virtual void update(TileSet * tileset, std::vector<Entity*>, std::vector<Tile*>, const std::pair<int, int>, const int);
	virtual void draw(ALLEGRO_DISPLAY* display, int x_offset, int y_offset);
	virtual void draw_adjacent_rect(ALLEGRO_DISPLAY* display, int x_offset, int y_offset);
	virtual void draw_destinations(ALLEGRO_DISPLAY* display, int x_offset, int y_offset);
};

#endif