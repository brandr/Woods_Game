#include "AIBeing.h"

void AIBeing::destination_update(TileSet * tileset, 
	std::vector<Entity*> interactables, 
	std::vector<Tile*> nearby_tiles, 
	const std::pair<int, int> level_dimensions)
{
	std::vector<Entity*> all_interactables;
	all_interactables.insert(all_interactables.end(), interactables.begin(), interactables.end());
	for (Tile * t : nearby_tiles) {
		Block * b = t->get_block();
		if (b != NULL && !b->is_empty() && b->is_solid()) {
			all_interactables.push_back(b);
		}
	}
	const bool primary = this->secondary_destinations.empty();
	std::vector<std::pair<int, int>> destinations
		= primary ? this->primary_destinations : this->secondary_destinations;
	if (!destinations.empty()) {
		const std::pair<int, int> pos_2 = destinations[0];
		const float x_dist = pos_2.first - this->get_x(),
			y_dist = pos_2.second - this->get_y();
		// when we reach our current destination, remove it
		if (std::abs(x_dist) < 1
			&& std::abs(y_dist) < 1) {
			this->xvel = 0, this->yvel = 0;
			if (primary) {
				this->primary_destinations.erase(this->primary_destinations.begin());
			}
			else {
				this->secondary_destinations.erase(this->secondary_destinations.begin());
			}
		} else {
			// see if there is an obstacle in the tile we are walking into
			int start_x = this->get_x() / TILE_SIZE, start_y = this->get_y() / TILE_SIZE;
			const std::pair<int, int> start_pos = std::pair<int, int>(start_x, start_y);
			// cap xoff and yoff based on how far we're actually going to travel
			const int xoff_t = std::abs(this->xvel) >= 1 ? (this->xvel / std::abs(this->xvel)) : 0;
			const int yoff_t = std::abs(this->xvel) < 1 && std::abs(this->yvel) >= 1 ? (this->yvel / std::abs(this->yvel)) : 0;
			const int xoff = std::abs(this->xvel) > 0 ? (this->xvel / std::abs(this->xvel))
				* std::min(std::abs(xoff_t) * TILE_SIZE, (int)std::abs(x_dist)) : 0;
			const int yoff = std::abs(this->yvel) > 0 ? (this->yvel / std::abs(this->yvel))
				* std::min(std::abs(yoff_t) * TILE_SIZE, (int)std::abs(y_dist)) : 0;
			bool failed = false;
			const bool blocked = this->pathing_blocked_at(this->get_x() + xoff, this->get_y() + yoff, all_interactables);
			if (!blocked) {
				if (!primary && this->secondary_destinations.size() > 1) {
					const std::pair<int, int> first_dest = this->secondary_destinations[0];
					const std::pair<int, int> second_dest = this->secondary_destinations[1];
					// this check tells us it's a straight shot to the the second destination and not blocked, so the first dest is redundant
					if ((std::abs(x_dist) < 1 && first_dest.first == second_dest.first)
						|| (std::abs(y_dist) < 1 && first_dest.second == second_dest.second)) {
						this->secondary_destinations.erase(this->secondary_destinations.begin());
					}
				}
			} else {
				// if we have pathing destinations, reset them and find a new path
				this->secondary_destinations.clear();
				if (!primary_destinations.empty()) {
					const std::pair<int, int> primary_pos = this->primary_destinations[0];
					std::pair<int, int> pathing_dest(primary_pos.first / TILE_SIZE, primary_pos.second / TILE_SIZE);
					// the primary destination is blocked, so we can't path directly to it
					if (this->pathing_blocked_at(primary_pos.first, primary_pos.second, all_interactables)) {
						const float x_dist_p
							= primary_pos.first - this->get_x(),
							y_dist_p = primary_pos.second - this->get_y();
						// we're next to the primary destination and it's blocked, so we can't go there at all
						if (std::abs(x_dist_p) < TILE_SIZE * 2 && std::abs(y_dist_p < TILE_SIZE * 2)) {
							failed = true;
						} else {
							const std::pair<int, int> closest_pos
								= this->find_closest_open_tile_pos(
									primary_pos, all_interactables, nearby_tiles, level_dimensions);
							if (closest_pos.first >= 0 && closest_pos.second >= 0) {
								pathing_dest = closest_pos;
							} else {
								failed = true;
							}
						}
					}
					if (!failed) {
						const std::pair<std::vector<std::pair<int, int>>, int> path_nodes
							= this->calculate_shortest_path(start_pos,
								start_pos,
								all_interactables,
								level_dimensions,
								pathing_dest,
								std::map<
								std::pair<int, int>,
								std::map<
								std::pair<int, int>,
								int>>(),
								std::vector<std::pair<int, int>>(),
								0);
						if (path_nodes.first.empty() || path_nodes.second == -1) {
							failed = true;
						}
						else {
							for (std::pair<int, int> p_node : path_nodes.first) {
								this->secondary_destinations.push_back(std::pair<int, int>(p_node.first*TILE_SIZE, p_node.second*TILE_SIZE));
							}
						}
					}
					if (failed) {
						if (primary) {
							// TODO: instead of just giving up completely, 
							//     set a timer, then recalculate in case an obstacle was moved out of the way
							this->primary_destinations.clear();
						}
						return;
					}
				}
			}
		}
	}
}

	void AIBeing::walk_update(TileSet * tileset, std::vector<Entity*> interactables, std::vector<Tile*> nearby_tiles, const std::pair<int, int> level_dimensions)
	{
		if (this->primary_destinations.empty() && this->secondary_destinations.empty()) {
			this->xvel = 0, this->yvel = 0;
			this->anim_state = ANIM_STATE_NEUTRAL;
			return;
		}
		const std::pair<int, int> pos_2 = this->get_next_destination();
		const float x_dist = pos_2.first - this->get_x(),
			y_dist = pos_2.second - this->get_y();
		const float b_speed = this->base_walk_speed.value();

		int xoff = std::abs(x_dist) >= 1 ? (x_dist / std::abs(x_dist)) : 0;
		int yoff = std::abs(x_dist) < 1 && std::abs(y_dist) >= 1 ? (y_dist / std::abs(y_dist)) : 0;

		this->xvel = (float)xoff * b_speed, this->yvel = (float)yoff * b_speed;

		// don't overshoot the destination
		if (std::abs(this->xvel) > 0 && std::abs(this->xvel) > std::abs(x_dist)) {
			this->xvel = (this->xvel / std::abs(this->xvel)) * std::abs(x_dist);
		}

		if (std::abs(this->yvel) > 0 && std::abs(this->yvel) > std::abs(y_dist)) {
			this->yvel = (this->yvel / std::abs(this->yvel)) * std::abs(y_dist);
		}

		if (this->xvel >= 0) {
			if (this->xvel >= std::abs(this->yvel)) direction = DIR_RIGHT;
			else if (this->yvel > 0) direction = DIR_DOWN;
			else direction = DIR_UP;
		}
		else {
			if (std::abs(this->xvel) >= std::abs(this->yvel)) direction = DIR_LEFT;
			else if (this->yvel > 0) direction = DIR_DOWN;
			else direction = DIR_UP;
		}
		if (this->xvel > 0 || this->yvel > 0) {
			anim_state = ANIM_STATE_WALKING;
		}
	}

	const std::pair<std::vector<std::pair<int, int>>, int> AIBeing::calculate_shortest_path(
		const std::pair<int, int> origin_pos, 
		const std::pair<int, int> start_pos, 
		std::vector<Entity*> interactables, 
		const std::pair<int, int> level_dimensions, 
		const std::pair<int, int> dest_tile_pos, 
		std::map<std::pair<int, int>, std::map<std::pair<int, int>, int>>& distances, 
		std::vector<std::pair<int, int>> ongoing_path, const int dist_tally)
	{
		std::vector<std::pair<int, int>>
		shortest_path;
		const int dest_tx_dist = std::abs(start_pos.first - dest_tile_pos.first),
			dest_ty_dist = std::abs(start_pos.second - dest_tile_pos.second);
		const bool will_stop = dest_tx_dist == 0 && dest_ty_dist < 2 || dest_tx_dist < 2 && dest_ty_dist == 0;
	std::vector<std::pair<int, int>> open_adjacent_tiles
		= this->open_adjacent_pathable_tiles(interactables,
			level_dimensions,
			start_pos, ongoing_path,
			dest_tile_pos,
			!will_stop);
	if (open_adjacent_tiles.empty() || this->pathing_blocked_at(start_pos.first*TILE_SIZE, start_pos.second*TILE_SIZE, interactables)) {
		return std::pair<std::vector<std::pair<int, int>>, int>(shortest_path, -1);
	}
	ongoing_path.push_back(start_pos);
	shortest_path.push_back(start_pos);
	auto it_start = distances.find(start_pos);
	if (it_start == distances.end()) {
		distances[start_pos] = std::map<std::pair<int, int>, int>();
		distances[start_pos][start_pos] = 0;
	}
	std::map<std::pair<int, int>, int> origin_dists = distances[origin_pos];
	std::map<std::pair<int, int>, int> start_dists = distances[start_pos];
	int shortest_dist = -1;
	for (std::pair<int, int> adj_t : open_adjacent_tiles) {
		auto it_adj = distances.find(adj_t);
		if (it_adj == distances.end()) {
			distances[adj_t] = std::map<std::pair<int, int>, int>();
			distances[adj_t][adj_t] = 0;
		}
		if (adj_t == dest_tile_pos) {
			distances[adj_t][dest_tile_pos] = 0;
			distances[start_pos][dest_tile_pos] = 1;
			shortest_path.push_back(std::pair<int, int>(dest_tile_pos.first, dest_tile_pos.second));
			// the destination is adjacent, so just go there
			return std::pair<std::vector<std::pair<int, int>>, int>(shortest_path, 1);
		}
		
		// mark the adjacent tile as 1 away
		distances[adj_t][start_pos] = 1;
		distances[start_pos][adj_t] = 1;
		std::map<std::pair<int, int>, int> adj_dists = distances[adj_t];
		auto it_start_adj = start_dists.find(adj_t);
		auto it_origin_adj = origin_dists.find(adj_t);
		bool found_new_shortest = it_origin_adj == origin_dists.end();
		if (!found_new_shortest) {
			const int old_shortest = distances[origin_pos][adj_t];
			found_new_shortest = old_shortest < 0 || dist_tally + 1 < old_shortest;
		}
		// we found a new shortest path to the adjacent tile, so add it to the map
		if (found_new_shortest) {
			distances[origin_pos][adj_t] = dist_tally + 1;
			std::vector<std::pair<int, int>> ongoing_adjacent_path = ongoing_path;
			// the vector represents the path starting from the beginning and the int represents the distance starting from this tile
			std::pair<std::vector<std::pair<int, int>>, int> adj_t_path = this->calculate_shortest_path(
				origin_pos,
				adj_t,
				interactables,
				level_dimensions,
				dest_tile_pos,
				distances,
				ongoing_path,
				dist_tally + 1); // +1 to include the traversal to the adjacent tile
			// an empty path means it's impossible
			if (!adj_t_path.first.empty() && adj_t_path.second >= 0) {
				const int adj_dist = adj_t_path.second;
				distances[adj_t][dest_tile_pos] = adj_dist;
				const int new_shortest_full = dist_tally + adj_dist;
				auto it_origin_dest = distances[origin_pos].find(dest_tile_pos);
				bool found_new_shortest_full = shortest_dist < 0 || it_origin_dest == distances[origin_pos].end();
				if (!found_new_shortest_full) {
					const int old_shortest_full = it_origin_dest->second;
					found_new_shortest_full = new_shortest_full > 0 && (old_shortest_full < 0
						|| (adj_dist >= 0 && new_shortest_full < old_shortest_full));
				}
				// if we have a possible path shorter than those starting from the other adjacent tiles, that's our new shortest path
				if (found_new_shortest_full) {
					distances[origin_pos][dest_tile_pos] = new_shortest_full;
					// erase the previous shortest path and replace it with the new path
					shortest_path.clear();
					shortest_path.push_back(start_pos);
					shortest_path.insert(shortest_path.end(), adj_t_path.first.begin(), adj_t_path.first.end());
					shortest_dist = adj_dist;
				}
			}
		}
	}
	if (shortest_dist > 0 && !shortest_path.empty()) {
		// we add 1 to the shortest dist to denote that we have to travel to the adjacent tile to reach it
		return std::pair<std::vector<std::pair<int, int>>, int>(shortest_path, shortest_dist + 1);
	}
	return std::pair<std::vector<std::pair<int, int>>, int>(shortest_path, -1);
}

const std::vector<std::pair<int, int>> AIBeing::open_adjacent_pathable_tiles(
	std::vector<Entity*> interactables,
	const std::pair<int, int> level_dimensions,
	const std::pair<int, int> start_tile_pos,
	const std::vector<std::pair<int, int>> visited,
	const std::pair<int, int> dest_tile_pos)
{
	const int x_dist = dest_tile_pos.first - start_tile_pos.first;
	const int y_dist = dest_tile_pos.second - start_tile_pos.second;
	const int x_dir = x_dist != 0 ? (x_dist / std::abs(x_dist)) : 0;
	const int y_dir = y_dist != 0 ? (y_dist / std::abs(y_dist)) : 0;

	std::vector<std::pair<int, int>> open_tiles;
	// we sort the tiles from closest to furthest from destination
	const int x1 = start_tile_pos.first, y1 = start_tile_pos.second;
	for (int i = 0; i < 4; i++) {
		int c_x_dir = x_dir, c_y_dir = y_dir;
		// 0 -> 0, 1 -> -1, 2 -> 1, 3 -> 0
		if (c_x_dir == 0) {
			switch (i) {
			case 1:
				c_x_dir = -1;
				if (c_y_dir != 0) {
					c_y_dir = 0;
				}
				break;
			case 2:
				c_x_dir = 1;
				if (c_y_dir != 0) {
					c_y_dir = 0;
				}
				break;
			case 3:
				c_y_dir *= -1;
			default:
				break;
			}
		} else if (c_y_dir == 0) {
			switch (i) {
			case 1:
				c_y_dir = -1;
				if (c_x_dir != 0) {
					c_x_dir = 0;
				}
				break;
			case 2:
				c_y_dir = 1;
				if (c_x_dir != 0) {
					c_x_dir = 0;
				}
				break;
			case 3:
				c_x_dir *= -1;
			default:
				break;
			}
		}
		// don't travel diagonally
		if (c_x_dir != 0 && c_y_dir != 0) {
			if (i % 2 == 0) {
				c_x_dir = 0;
			}
			else {
				c_y_dir = 0;
			}
		}
		if (x1 + c_x_dir > 0 && y1 + c_y_dir > 0
			&& x1 + c_x_dir < level_dimensions.first / TILE_SIZE - 1
			&& y1 + c_y_dir < level_dimensions.second / TILE_SIZE - 1
			&& !this->pathing_blocked_at((start_tile_pos.first + c_x_dir)*TILE_SIZE,
			(start_tile_pos.second + c_y_dir)*TILE_SIZE, interactables)) {
				const std::pair<int, int> pos(x1 + c_x_dir, y1 + c_y_dir);
				open_tiles.push_back(pos);
		}
	}
	return open_tiles;
}

const std::pair<int, int> AIBeing::find_closest_open_tile_pos(
	const std::pair<int, int> destination,
	std::vector<Entity*> interactables,
	std::vector<Tile*> nearby_tiles,
	const std::pair<int, int> level_dimensions)
{
	for (int range = 1; range < 3; range++) {
		const std::pair<int, int> open_pos = this->find_closest_open_tile_pos(
			destination, interactables, nearby_tiles, level_dimensions, range);
		if (open_pos.first >= 0 && open_pos.second >= 0) {
			return open_pos;
		}
	}
	return std::pair<int, int>(-1, -1);
}

const std::pair<int, int> AIBeing::find_closest_open_tile_pos(const std::pair<int, int> destination, std::vector<Entity*> interactables, std::vector<Tile*> nearby_tiles, const std::pair<int, int> level_dimensions, const int range)
{
	for (int ty = -1*range; ty < 1 + range; ty++) {
		for (int tx = -1*range; tx < 1 + range; tx++) {
			const std::pair<int, int>
				tile_center(destination.first / TILE_SIZE + tx, destination.second / TILE_SIZE + ty);
			const int cx = std::max(
				0, std::min(level_dimensions.first - TILE_SIZE, destination.first + tx * TILE_SIZE));
			const int cy = std::max(
				0, std::min(level_dimensions.second - TILE_SIZE, destination.second + ty * TILE_SIZE));
			if (!this->pathing_blocked_at(cx, cy, interactables)) {
				return std::pair<int, int>(cx / TILE_SIZE, cy / TILE_SIZE);
			}
		}
	}
	return std::pair<int, int>(-1, -1);
}


	const std::pair<int, int> AIBeing::get_next_destination()
	{
		if (!this->secondary_destinations.empty()) {
			return this->secondary_destinations[0];
		}
		if (!this->primary_destinations.empty()) {
			return this->primary_destinations[0];
		}
		return std::pair<int, int>(-1, -1);
	}

	const bool AIBeing::pathing_blocked_at(const int x, const int y, std::vector<Entity*> interactables)
	{
		Rect check_rect(x, y,
			this->get_width(), this->get_height());
		return !this->empty_at(check_rect, interactables);
	}

	AIBeing::AIBeing()
{
	direction = DIR_NEUTRAL;
	anim_state = ANIM_NEUTRAL;
	solid = true;

	this->primary_destinations.push_back(std::pair<int, int>(256, 640));
	this->primary_destinations.push_back(std::pair<int, int>(640, 640));
}

AIBeing::~AIBeing()
{
}

void AIBeing::update(TileSet * tileset, 
	std::vector<Entity*> interactables, 
	std::vector<Tile*> nearby_tiles, 
	const std::pair<int, int> level_dimensions, 
	const int game_mode)
{
	// idea: "wandering" state when being has nothing else to do (how to determine allowed wander area?)
	// put destination calculations before walk update so we don't start walking in the wrong direction
	this->destination_update(tileset, interactables, nearby_tiles, level_dimensions);
	this->walk_update(tileset, interactables, nearby_tiles, level_dimensions);
	Being::update(tileset, interactables, nearby_tiles, level_dimensions, game_mode);
}

void AIBeing::draw(ALLEGRO_DISPLAY * display, int x_offset, int y_offset)
{
	//TEMP
	this->draw_adjacent_rect(display, x_offset, y_offset);
	this->draw_destinations(display, x_offset, y_offset);
	//TEMP
	Being::draw(display, x_offset, y_offset);
}

void AIBeing::draw_adjacent_rect(ALLEGRO_DISPLAY * display, int x_offset, int y_offset)
{
	const std::pair<int, int> pos_2 = this->get_next_destination();
	const float x_dist = pos_2.first - this->get_x(),
		y_dist = pos_2.second - this->get_y();

	const int xoff_t = std::abs(this->xvel) >= 1 ? (this->xvel / std::abs(this->xvel)) : 0;
	const int yoff_t = std::abs(this->xvel) < 1 && std::abs(this->yvel) >= 1 ? (this->yvel / std::abs(this->yvel)) : 0;
	const int xoff = std::abs(this->xvel) > 0 ? (this->xvel / std::abs(this->xvel))
		* std::min(std::abs(xoff_t) * TILE_SIZE, (int)std::abs(x_dist)) : 0;
	const int yoff = std::abs(this->yvel) > 0 ? (this->yvel / std::abs(this->yvel))
		* std::min(std::abs(yoff_t) * TILE_SIZE, (int)std::abs(y_dist)) : 0;

	if (this->check_rect_bitmap == NULL) {
		this->check_rect_bitmap = al_create_bitmap(this->get_width(), this->get_height());
		al_set_target_bitmap(this->check_rect_bitmap);
		al_clear_to_color(al_map_rgba(100, 0, 0, 100));
		al_set_target_bitmap(al_get_backbuffer(display));
	}
	al_draw_bitmap(this->check_rect_bitmap, this->get_x() + xoff + x_offset, this->get_y() + yoff + y_offset, 0);
}

void AIBeing::draw_destinations(ALLEGRO_DISPLAY * display, int x_offset, int y_offset)
{
	if (this->path_bitmap == NULL) {
		this->path_bitmap = al_create_bitmap(TILE_SIZE, TILE_SIZE);
		al_set_target_bitmap(this->path_bitmap);
		al_clear_to_color(al_map_rgba(0, 100, 0, 100));
		al_set_target_bitmap(al_get_backbuffer(display));
	}
	for (std::pair<int, int> dest : this->secondary_destinations) {
		al_draw_bitmap(this->path_bitmap, dest.first + x_offset, dest.second + y_offset, 0);
	}
	for (std::pair<int, int> dest : this->primary_destinations) {
		al_draw_bitmap(this->path_bitmap, dest.first + x_offset, dest.second + y_offset, 0);
	}
}
