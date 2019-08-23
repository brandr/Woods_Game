#include "AIBeing.h"
#include "Level.h"

void AIBeing::ai_timer_update()
{
	this->ai_state.timer_update();
}

// TODO: consider moving this around/reorganizing into a more general order of operations for state flow
void AIBeing::request_pathing_update(Level * level, GlobalTime * time)
{
	if (this->ai_state.is_idle()) {
		std::string node_key = this->ai_state.get_current_destination_node_key();
		if (node_key == "") {;
			node_key = this->calculate_destination_node_key(time);
			this->ai_state.set_current_destination_node_key(node_key);
		}
		if (node_key != "") {
			bool close_enough = false;
			PathNode * matching_node = level->find_path_node_with_key(node_key);
			if (matching_node != NULL) {
				Rect * collide_rect = this->get_rect_for_collision();
				const float x_dist = collide_rect->x - matching_node->get_x(),
					y_dist = collide_rect->y - matching_node->get_y();
				close_enough =(std::abs(x_dist) < 2.0 * TILE_SIZE && std::abs(y_dist) < 2.0*TILE_SIZE);
			}
			if (!close_enough) {
				this->set_needs_pathing_calculation(node_key);
			}
		}
	}
}

void AIBeing::destination_update(Level * level, GlobalTime * time)
{
	Rect * collide_rect = this->get_rect_for_collision();
	const bool primary = this->secondary_destinations.empty();
	std::vector<std::pair<std::string, std::pair<int, int>>> destinations
		= primary ? this->primary_destinations : this->secondary_destinations;
	if (this->is_facing_other()) {
		return;
	} else if (!destinations.empty()) {
		const std::pair<std::string, std::pair<int, int>> next_dest = destinations[0];
		const std::pair<int, int> next_pos = next_dest.second;
		const float x_dist = (float) (next_pos.first) - collide_rect->x,
			y_dist = (float) (next_pos.second) - collide_rect->y;
		// when we reach our current destination, remove it
		if (std::abs(x_dist) < 1
			&& std::abs(y_dist) < 1) {
			if (primary) {
				const std::string dest_key = next_dest.first;
				this->mark_destination_reached(dest_key);
				this->primary_destinations.erase(this->primary_destinations.begin());
				this->last_primary_destination = next_dest;
				// if our path is taking us to another level, set a flag here saying we need to move the npc to that level
				PathNode * dest_node = level->find_path_node_with_key(dest_key);
				if (dest_node == NULL) {
					this->clear_primary_destinations();
				} else {
					std::vector<NextLevelNode *> next_level_nodes = dest_node->get_next_level_nodes();
					// only go to the next level if we just stepped onto the node
					//TODO: might need a better check here
					if (next_level_nodes.size() > 0) {
						NextLevelNode * next_level_node = next_level_nodes[0];
						const std::string next_level_key = next_level_node->level_id.value();
						const std::string next_level_node_key = next_level_node->node_id.value();
						this->ai_state.set_is_requesting_next_level(next_level_key, next_level_node_key);
					} else {
						// if we just reached a primary dest on the same level, set idle so we can request pathing
						this->set_ai_state(AI_STATE_IDLE);
					}
				}
			} else {
				this->secondary_destinations.erase(this->secondary_destinations.begin());
			}
			this->xvel = 0, this->yvel = 0;
			this->anim_state = ANIM_STATE_NEUTRAL;
		} else {
			// see if there is an obstacle in the tile we are walking into
			int start_x = collide_rect->x / TILE_SIZE, start_y = collide_rect->y / TILE_SIZE;
			const std::pair<int, int> start_pos = std::pair<int, int>(start_x, start_y);
			// cap xoff and yoff based on how far we're actually going to travel
			const int xoff_t = std::abs(this->xvel) > 0 ? (this->xvel / std::abs(this->xvel)) : 0;
			const int yoff_t = std::abs(this->xvel) == 0 && std::abs(this->yvel) > 0 ? (this->yvel / std::abs(this->yvel)) : 0;
			const int xoff = std::abs(this->xvel) > 0 ? (this->xvel / std::abs(this->xvel))
				* std::min(std::abs(xoff_t) * TILE_SIZE, (int)std::abs(x_dist)) : 0;
			const int yoff = std::abs(this->yvel) > 0 ? (this->yvel / std::abs(this->yvel))
				* std::min(std::abs(yoff_t) * TILE_SIZE, (int)std::abs(y_dist)) : 0;
			bool failed = false;
			Entity * blocking_entity = this->blocking_entity_at(collide_rect->x + xoff, collide_rect->y + yoff, level);
			if (blocking_entity == NULL || this->should_push_others) {
				//if we do path around, work backwards from the known path until the first blocked tile, then treat that as an intermediary destination
				if (!primary && this->secondary_destinations.size() > 1) {
					const std::pair<std::string, std::pair<int, int>> first_dest = this->secondary_destinations[0];
					const std::pair<std::string, std::pair<int, int>> second_dest = this->secondary_destinations[1];
					const std::pair<int, int> first_pos = first_dest.second;
					const std::pair<int, int> second_pos = second_dest.second;
					// this check tells us it's a straight shot to the the second destination and not blocked, so the first dest is redundant
					if ((std::abs(x_dist) < 1 && first_pos.first == second_pos.first)
						|| (std::abs(y_dist) < 1 && first_pos.second == second_pos.second)) {
						this->secondary_destinations.erase(this->secondary_destinations.begin());
					}
				}
			} else {
				this->secondary_destinations.clear();
				if (!primary_destinations.empty()) {
					const std::pair<std::string, std::pair<int, int>> primary_dest = this->primary_destinations[0];
					const std::pair<int, int> primary_pos = primary_dest.second;
					std::pair<int, int> pathing_dest(primary_pos.first / TILE_SIZE, primary_pos.second / TILE_SIZE);
					std::string forced_dest_key = primary_dest.first;
					// the primary destination is blocked, so we can't path directly to it
					if (this->pathing_blocked_by_moving_at(primary_pos.first, primary_pos.second, level)) {
						const float x_dist_p
							= primary_pos.first - collide_rect->x,
							y_dist_p = primary_pos.second - collide_rect->y;
						// we're next to the primary destination and it's blocked, so we can't go there at all
						if (std::abs(x_dist_p) < TILE_SIZE * 2 && std::abs(y_dist_p) < TILE_SIZE * 2) {
							failed = true;
						} else {
							const std::pair<int, int> closest_pos
								= this->find_closest_open_tile_pos(level, primary_pos);
							// there is no open space near the destination, so we can't go there
							if (closest_pos.first < 0 || closest_pos.second < 0) {
								failed = true;
							}
						}
					}
					if (!failed) {
						this->xvel = 0, this->yvel = 0;
						this->anim_state = ANIM_STATE_NEUTRAL;
						this->clear_primary_destinations();
						this->forced_destination = std::pair<std::string, std::pair<int, int>>(
							forced_dest_key, std::pair<int, int>(pathing_dest.first * TILE_SIZE, pathing_dest.second * TILE_SIZE));
						this->should_path_around_moving = true;
						const bool should_face_other = true; //TEMP. determine what to do via AIState, type of being, npc personality, relationship w/ being, etc
						if (should_face_other) {
							this->set_is_facing_other(blocking_entity);
						} else {
							this->ai_state.set_is_idle();
						}
					}
					if (failed) {
						this->failed_pathing_tally++;
						this->clear_primary_destinations();
						this->cancel_current_pathing(AI_PATH_BLOCKED_WAIT_TIME);
					} else {
						this->failed_pathing_tally = 0;
					}
				}
			}
		}
	}
}

void AIBeing::face_other_update(Level * level, GlobalTime * time)
{
	if (!this->is_facing_other()) {
		if (this->should_push_others) {
			return;
		}
		Rect * collide_rect = this->get_rect_for_collision();
		if (this->pathing_blocked_by_moving_at(collide_rect->x, collide_rect->y, level)) {
			this->failed_pathing_tally++;
			Entity * blocking_entity = this->blocking_entity_at(collide_rect->x, collide_rect->y, level);
			this->set_is_facing_other(blocking_entity);
			this->xvel = 0, this->yvel = 0;
			this->anim_state = ANIM_STATE_NEUTRAL;
			if (!this->primary_destinations.empty()) {
				this->forced_destination = this->primary_destinations[0];
			}
			this->clear_primary_destinations();
			this->secondary_destinations.clear();
		}
		return;
	} else {
		const int tile_dist = std::abs(this->facing_entity->get_x() - this->get_x()) / TILE_SIZE
			+ std::abs(this->facing_entity->get_y() - this->get_y()) / TILE_SIZE;
		if (tile_dist > AI_FACE_OTHER_MAX_TILE_DIST) {
			this->ai_state.set_is_idle();
			return;
		}
		this->direction = this->calculate_direction(this->facing_entity);
		//TODO: don't let the facing entity block us forever-- need to escape somehow
	}
}

void AIBeing::walk_update()
	{
		if (this->ai_state.is_requesting_path() || this->ai_state.is_idle() || 
			(this->primary_destinations.empty() && this->secondary_destinations.empty())) {
			this->xvel = 0, this->yvel = 0;
			// TODO: if we need the fact that we reached a destination as context, this may be incorrect
			if (this->ai_state.is_idle() || this->ai_state.is_walking()) {
				//TODO: this might be wrong since the AI could be standing still doing something
				this->ai_state.set_is_idle();
				anim_state = ANIM_STATE_NEUTRAL;
			}
			return;
		}
		Rect * collide_rect = this->get_rect_for_collision();
		const std::pair<std::string, std::pair<int, int>> next_dest = this->get_next_destination();
		const std::pair<int, int> next_pos = next_dest.second;
		const float x_dist = next_pos.first - collide_rect->x,
			y_dist = next_pos.second - collide_rect->y;
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

		if (this->xvel > 0) {
			if (this->xvel >= std::abs(this->yvel)) direction = DIR_RIGHT;
			else if (this->yvel > 0) direction = DIR_DOWN;
			else direction = DIR_UP;
		} else {
			if (std::abs(this->xvel) > 0 && std::abs(this->xvel) >= std::abs(this->yvel)) direction = DIR_LEFT;
			else if (this->yvel > 0) direction = DIR_DOWN;
			else if (this->yvel < 0) direction = DIR_UP;
		}
		if (std::abs(this->xvel) > 0 || std::abs(this->yvel) > 0) {
			this->ai_state.set_is_walking();
			anim_state = ANIM_STATE_WALKING;
		}
	}

void AIBeing::failed_pathing_update()
{
	if (this->failed_pathing_tally > 2) {
		this->should_push_others = true;
		this->failed_pathing_tally = 0;
	}
	// TODO: need to be able to set it back to false at some point, maybe after a timer
}

	const std::pair<int, int> AIBeing::find_closest_open_tile_pos(Level * level, const std::pair<int, int> destination)
	{
		for (int range = 1; range < 3; range++) {
			const std::pair<int, int> open_pos = this->find_closest_open_tile_pos(
				level, destination, range);
			if (open_pos.first >= 0 && open_pos.second >= 0) {
				return open_pos;
			}
		}
		return std::pair<int, int>(-1, -1);
	}

	const std::pair<int, int> AIBeing::find_closest_open_tile_pos(Level * level, const std::pair<int, int> destination, const int range)
	{
		const std::pair<int, int> level_dimensions = level->get_dimensions();
		for (int ty = -1 * range; ty < 1 + range; ty++) {
			for (int tx = -1 * range; tx < 1 + range; tx++) {
				const std::pair<int, int>
					tile_center(destination.first / TILE_SIZE + tx, destination.second / TILE_SIZE + ty);
				const int cx = std::max(
					0, std::min(level_dimensions.first - TILE_SIZE, destination.first + tx * TILE_SIZE));
				const int cy = std::max(
					0, std::min(level_dimensions.second - TILE_SIZE, destination.second + ty * TILE_SIZE));
				if (!this->pathing_blocked_at(cx, cy, level, this->should_push_others)) {
					return std::pair<int, int>(cx / TILE_SIZE, cy / TILE_SIZE);
				}
			}
		}
		return std::pair<int, int>(-1, -1);
	}


	const std::pair<std::string, std::pair<int, int>> AIBeing::get_next_destination()
	{
		if (!this->secondary_destinations.empty()) {
			return this->secondary_destinations[0];
		}
		if (!this->primary_destinations.empty()) {
			return this->primary_destinations[0];
		}
		return std::pair<std::string, std::pair<int, int>>("", std::pair<int, int>(-1, -1));
	}

	const bool AIBeing::pathing_blocked_at(const int x, const int y, std::vector<Entity*> interactables)
	{
		Rect * collide_rect = this->get_rect_for_collision();
		Rect check_rect(x, y,
			collide_rect->width, collide_rect->height);
		return !this->empty_at(check_rect, interactables);
	}

	void AIBeing::set_is_processing(const bool value)
	{
		if (value) {
			this->ai_state.set_is_locked();
		} else {
			this->ai_state.set_is_idle();
		}
	}

	const bool AIBeing::is_locked()
	{
		return this->ai_state.is_locked();
	}

	const bool AIBeing::needs_pathing_calculation()
	{
		return this->ai_state.is_requesting_path();
	}

	void AIBeing::set_needs_pathing_calculation(const std::string dest_node_key)
	{
		this->ai_state.set_is_requesting_path();
		this->ai_state.set_current_destination_node_key(dest_node_key);
	}

	void AIBeing::cancel_current_pathing(const int wait_time)
	{
		//TEMP -- might need more logic based on the context under which we're stopping
		this->xvel = 0, this->yvel = 0;
		this->anim_state = ANIM_STATE_NEUTRAL;
		if (wait_time > 0) {
			this->ai_state.set_is_waiting(wait_time);
		} else {
			this->ai_state.set_is_idle();
		}
		this->ai_state.set_current_destination_node_key("");
	}

	void AIBeing::clear_primary_destinations()
	{
		this->primary_destinations.clear();
	}

	void AIBeing::mark_destination_reached(const std::string dest_key)
	{
		// override in subclasses
	}

	void AIBeing::set_ai_state(const int state_key)
	{
		this->ai_state.set_state_key(state_key);
	}

	// abstract AIBeings do not use node pathing. Override this in subclasses.
	const std::string AIBeing::calculate_destination_node_key(GlobalTime * time)
	{
		return "";
	}

	void AIBeing::set_is_starting_path()
	{
		this->set_ai_state(AI_STATE_STARTING_PATH);
	}

	void AIBeing::set_is_facing_other(Entity * other)
	{
		this->ai_state.set_is_facing_other(AI_FACE_OTHER_WAIT_TIME);
		this->facing_entity = other;
	}

	const bool AIBeing::is_facing_other()
	{
		return this->ai_state.is_facing_other();
	}

	const bool AIBeing::is_requesting_next_level()
	{
		return this->ai_state.is_requesting_next_level();
	}

	const std::string AIBeing::get_requested_next_level_key()
	{
		return this->ai_state.is_requesting_next_level()
			? this->ai_state.get_requested_next_level_key() : "";
	}

	const std::string AIBeing::get_requested_next_level_node_key()
	{
		return this->ai_state.is_requesting_next_level()
			? this->ai_state.get_requested_next_level_node_key() : "";
	}

	void AIBeing::reset_failed_pathing_tally()
	{
		this->failed_pathing_tally = 0;
	}

	void AIBeing::increment_failed_pathing_tally()
	{
		this->failed_pathing_tally++;
	}

	AIBeing::AIBeing()
{
	direction = DIR_NEUTRAL;
	anim_state = ANIM_NEUTRAL;
	solid = true;
	this->set_ai_state(AI_STATE_IDLE);
}

AIBeing::~AIBeing()
{
}

void AIBeing::update(Level * level, GlobalTime * time, const int game_mode)
{
	// idea: "wandering" state when being has nothing else to do (how to determine allowed wander area?)
	// put destination calculations before walk update so we don't start walking in the wrong direction
	
	this->ai_timer_update();
	this->request_pathing_update(level, time);
	this->destination_update(level, time);
	this->face_other_update(level, time);
	this->walk_update();
	this->failed_pathing_update();
	Being::update(level, time, game_mode);
}

void AIBeing::draw(ALLEGRO_DISPLAY * display, int x_offset, int y_offset)
{
	//TEMP
	const bool should_draw_test_rects = true; //TEMP
	if (should_draw_test_rects) {
		this->draw_adjacent_rect(display, x_offset, y_offset);
		this->draw_destinations(display, x_offset, y_offset);
	}
	//TEMP
	Being::draw(display, x_offset, y_offset);
}

void AIBeing::draw_adjacent_rect(ALLEGRO_DISPLAY * display, int x_offset, int y_offset)
{
	Rect * collide_rect = this->get_rect_for_collision();
	const std::pair<std::string, std::pair<int, int>> next_dest = this->get_next_destination();
	const std::pair<int, int> next_pos = next_dest.second;
	const float x_dist = next_pos.first - collide_rect->x,
		y_dist = next_pos.second - collide_rect->y;

	const int xoff_t = std::abs(this->xvel) > 0 ? (this->xvel / std::abs(this->xvel)) : 0;
	const int yoff_t = std::abs(this->xvel) == 0 && std::abs(this->yvel) > 0 ? (this->yvel / std::abs(this->yvel)) : 0;
	const int xoff = std::abs(this->xvel) > 0 ? (this->xvel / std::abs(this->xvel))
		* std::min(std::abs(xoff_t) * TILE_SIZE, (int)std::abs(x_dist)) : 0;
	const int yoff = std::abs(this->yvel) > 0 ? (this->yvel / std::abs(this->yvel))
		* std::min(std::abs(yoff_t) * TILE_SIZE, (int)std::abs(y_dist)) : 0;

	
	if (this->check_rect_bitmap == NULL) {
		this->check_rect_bitmap = al_create_bitmap(collide_rect->width, collide_rect->height);
		al_set_target_bitmap(this->check_rect_bitmap);
		al_clear_to_color(al_map_rgba(100, 0, 0, 100));
		al_set_target_bitmap(al_get_backbuffer(display));
	}
	al_draw_bitmap(this->check_rect_bitmap, collide_rect->x + xoff + x_offset, collide_rect->y + yoff + y_offset, 0);
}

void AIBeing::draw_destinations(ALLEGRO_DISPLAY * display, int x_offset, int y_offset)
{
	if (this->path_bitmap == NULL) {
		this->path_bitmap = al_create_bitmap(TILE_SIZE, TILE_SIZE);
		al_set_target_bitmap(this->path_bitmap);
		al_clear_to_color(al_map_rgba(0, 100, 0, 100));
		al_set_target_bitmap(al_get_backbuffer(display));
	}
	for (std::pair<std::string, std::pair<int, int>> dest : this->secondary_destinations) {
		const std::pair<int, int> pos = dest.second;
		al_draw_bitmap(this->path_bitmap, pos.first + x_offset, pos.second + y_offset, 0);
	}
	for (std::pair<std::string, std::pair<int, int>> dest : this->primary_destinations) {
		const std::pair<int, int> pos = dest.second;
		al_draw_bitmap(this->path_bitmap, pos.first + x_offset, pos.second + y_offset, 0);
	}
}

void AIBeing::add_primary_destination(const std::pair<int, int> destination, const std::string dest_key)
{
	this->primary_destinations.push_back(std::pair<std::string, std::pair<int, int>>(dest_key, destination));
}

const std::pair<int, int> AIBeing::first_primary_destination()
{
	return this->primary_destinations.size() > 0 ? this->primary_destinations[0].second : std::pair<int, int>(-1, -1);
}

const bool AIBeing::has_primary_destinations()
{
	return !this->primary_destinations.empty();
}

const bool AIBeing::has_primary_destination(const std::pair<int, int> destination)
{
	for (std::pair<std::string, std::pair<int, int>> dest : this->primary_destinations) {
		const std::pair<int, int> pos = dest.second;
		if (pos.first == destination.first && pos.second == destination.second) {
			return true;
		}
	}
	return false;
}

const bool AIBeing::has_primary_destination_with_key(const std::string dest_key)
{
	for (std::pair<std::string, std::pair<int, int>> dest : this->primary_destinations) {
		if (dest.first == dest_key) {
			return true;
		}
	}
	return false;
}

void AIBeing::set_secondary_destinations(const std::vector<std::pair<std::string, std::pair<int, int>>> destinations)
{
	this->secondary_destinations.clear();
	this->secondary_destinations = destinations;
}

const std::pair<std::string, std::pair<int, int>> AIBeing::get_forced_destination()
{
	return this->forced_destination;
}

void AIBeing::set_forced_destination(std::pair<std::string, std::pair<int, int>> value)
{
	this->forced_destination = value;
}

const std::pair<std::string, std::pair<int, int>> AIBeing::get_last_primary_destination()
{
	return this->last_primary_destination;
}

void AIBeing::set_last_primary_destination(const std::pair<std::string, std::pair<int, int>> value)
{
	this->last_primary_destination = value;
}

const bool AIBeing::get_should_path_around_moving()
{
	return this->should_path_around_moving && !this->should_push_others;
}

void AIBeing::set_should_path_around_moving(const bool value)
{
	this->should_path_around_moving = value;
}

//TODO: depending on the context, should be looking at moving obstacles only, because we should already have chosen paths that account for blocked tiles
const bool AIBeing::pathing_blocked_at(const int x, const int y, Level * level, const bool ignore_moving_obstacles)
{
	Tile * t = level->get_tile(x / TILE_SIZE, y / TILE_SIZE);
	TileSet * tileset = level->get_tileset();
	if (!tileset->is_tile_npc_pathable(t->get_tile_type_index())) {
		return true;
	}
	Rect * collide_rect = this->get_rect_for_collision();
	Rect check_rect(x, y,
		collide_rect->width, collide_rect->height);
	return !this->empty_at(check_rect, level, ignore_moving_obstacles);
}

const bool AIBeing::pathing_blocked_by_moving_at(const int x, const int y, Level * level)
{
	if (this->should_push_others) {
		return false;
	}
	Rect * collide_rect = this->get_rect_for_collision();
	Rect check_rect(x, y,
		collide_rect->width, collide_rect->height);
	std::vector<Entity *> moving = level->get_moving_interactables(this);
	for (Entity *e : moving) {
		if (e != NULL && e != this && e->is_solid() && e->intersects_area(check_rect)) {
			return true;
		}
	}
	return false;
}

Entity * AIBeing::blocking_entity_at(const int x, const int y, Level * level)
{
	Rect * collide_rect = this->get_rect_for_collision();
	Rect check_rect(x, y,
		collide_rect->width, collide_rect->height);
	std::vector<Entity *> moving = level->get_moving_interactables(this);
	for (Entity *e : moving) {
		if (e != NULL && e != this && e->is_solid() && e->intersects_area(check_rect)) {
			return e;
		}
	}
	return NULL;
}
