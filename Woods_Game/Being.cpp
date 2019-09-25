#define _USE_MATH_DEFINES
#include<cmath>
#define NOMINMAX
#include "Being.h"
#include "Level.h"
#include "World.h"
#include<iostream>
#include<algorithm>


Being::Being()
{
}


Being::~Being()
{
}

void Being::update(World * world, Level * level, GlobalTime * time, const int game_mode)
{
	collision_update(world, level, game_mode);
	movement_update(level, game_mode);
	animation_update(game_mode);
	Entity::update();
	emit_sound_update(world, level, time, game_mode);
	play_sound_update(world, level, time, game_mode);
}

void Being::emit_sound_update(World * world, Level * level, GlobalTime * time, const int game_mode)
{
	this->clear_sounds();
	TileSet * tileset = level->get_tileset();
	std::vector<Tile*> nearby_tiles = level->get_nearby_tiles(this);
	std::vector<Tile*> colliding_tiles = this->get_colliding_tiles(tileset, nearby_tiles);
	Tile * t = colliding_tiles.size() > 0 ? colliding_tiles[0] : NULL; //TODO: do we want some priority for which tile we're stepping on?

	const std::string default_name = DEFAULT_FOOTSTEP_FILENAME;
	std::string footstep_filename = "entity_sounds/" + default_name;
	if (t != NULL) {
		const std::string tile_key = tileset->get_tile_key(t->get_tile_type_index());
		const std::string footstep_key = "footstep_" + tile_key;
		footstep_filename = "entity_sounds/" + footstep_key;
	}
	
	// TODO: also include information about the being in forming the key

	if (!AudioManager::get_instance().sfx_exists(footstep_filename, this->get_sound_key())) {
		footstep_filename = "entity_sounds/" + default_name;
	}

	// this is how many pixels we travel per frame
	const float speed = std::pow((float)std::pow(this->xvel, 2) + (float)std::pow(this->yvel, 2), 0.5f);
	if (speed > 0.0f) {
		const float step_px_size = (3.0f/4.0f) * TILE_SIZE; // TEMP. this is how many pixels a "step" should be
		const int duration = (int)(step_px_size / speed); //TODO: get number of frames sound should last

		if (this->anim_state == ANIM_STATE_WALKING) {
			this->emit_sound(footstep_filename, duration);
		}
		else {
			this->stop_sound(footstep_filename);
		}
	}
	else {
		this->stop_sound(footstep_filename);
	}
	
}

void Being::play_sound_update(World * world, Level * level, GlobalTime * time, const int game_mode)
{
	// probably just override this in player, unless there's some reason we have to play sounds elsewhere
}

void Being::set_xvel(int vel)
{
	xvel = vel;
}

void Being::set_yvel(int vel)
{
	yvel = vel;
}

void Being::animation_update(const int game_mode)
{
}

void Being::movement_update(Level * level, const int game_mode)
{
	switch (game_mode) {
	case(TOP_DOWN):
		movement_update_top_down(level);
		break;
	}
}

/*
void Being::movement_update_side_scrolling(std::vector<Entity*> interactables, std::vector<Tile*> nearby_tiles)
{
Rect* side = NULL;
if (xvel < 0) side = new Rect(rect.x + xvel, rect.y, xvel, rect.height);
else if (xvel > 0) side = new Rect(rect.x + rect.width, rect.y, xvel, rect.height);
if (side && !empty_at(*side, interactables)) {
//TODO: this is where we should handle climbing stairs/slopes. right now they will simply stop the player.
side->width = 1;
while (empty_at(*side, interactables)) {
if (xvel < 0) {
rect.x -= 1;
side->x = rect.x - 1;
}
else {
rect.x += 1;
side->x = rect.x + rect.width;
}
}
xvel = 0;
}
if (side)
{
delete side;
side = NULL;
}
// check for collisions above
if (yvel < 0) {
yvel = std::min(yvel + gravity, terminal_velocity);
if (yvel < 0) {
Rect* above = new Rect(rect.x, rect.y + yvel, rect.width, yvel);
if (!empty_at(*above, interactables)) {
yvel = 0;
above->height = 1;
above->y = rect.y - 1;
while (empty_at(*above, interactables)) {
rect.y -= 1;
above->y = rect.y - 1;
}
}
delete above;
above = NULL;
}
}
// check for collisions below
else {
Rect* below = new Rect(rect.x, rect.y + rect.height, rect.width, std::max(yvel, 2.0f));
if (empty_at(*below, interactables)) {
yvel = std::min(yvel + gravity, terminal_velocity);
}
else {
yvel = 0;
below->height = 1;
while (empty_at(*below, interactables)) {
rect.y += 1;
below->y = rect.y + rect.height;
}
}
delete below;
below = NULL;
}

// adjust the postition
rect.x += xvel;
rect.y += yvel;
}
*/

void Being::movement_update_top_down(Level * level)
{
	TileSet * tileset = level->get_tileset();
	std::vector<Tile*> nearby_tiles = level->get_nearby_tiles(this);
	float speed_multiplier = get_speed_multiplier(tileset, nearby_tiles);
	if (counters[BOUNCE] <= 0) {
		xvel *= speed_multiplier, yvel *= speed_multiplier;
	}
	float xoff = xvel, yoff = yvel;
	// this will check for blocked tiles
	this->adjust_movement(level, this->xvel, this->yvel, true);
	rect.x += xvel;
	rect.y += yvel;
	collide_rect.x += xvel;
	collide_rect.y += yvel;
}

void Being::draw(ALLEGRO_DISPLAY * display, int x_offset, int y_offset)
{
	//TEMP
	Rect * collide_rect = this->get_rect_for_collision();
	const bool should_draw_test_rect = true; //TEMP. probably want a launch arg or console commands or somehting
	if (should_draw_test_rect && this->test_rect_bitmap == NULL) {
		this->test_rect_bitmap = al_create_bitmap(collide_rect->width, collide_rect->height);
		al_set_target_bitmap(this->test_rect_bitmap);
		al_clear_to_color(al_map_rgba(0, 0, 100, 100));
		al_set_target_bitmap(al_get_backbuffer(display));

	}
	al_draw_bitmap(this->test_rect_bitmap, collide_rect->x + x_offset, collide_rect->y + y_offset, 0);

	//TEMP
	Entity::draw(display, x_offset, y_offset);
}

const bool Being::adjust_movement(Level * level, float xoff, float yoff, const bool snap)
{
	bool blocked = false;
	Rect check_rect(rect.x + xoff, rect.y + yoff, rect.width, rect.height);
	std::vector<Entity*> interactables = level->get_colliding_interactables(this, check_rect, false);
	std::vector<Tile*> nearby_tiles = level->get_nearby_tiles(this);
	int t_size = nearby_tiles.size();
	for (int i = 0; i < t_size; i++) {
		Block *b = nearby_tiles[i]->get_block();
		if (b) { 
			interactables.push_back(b); 
		}
	}
	// get pushed back by blocks and beings. if we can push others, do so to beings
	if (this->should_push_others) {
		std::vector<Entity*> moving_interactables = level->get_moving_interactables(this);
		this->adjust_movement(level, interactables, xoff, yoff, snap, true);
	}
	return this->adjust_movement(level, interactables, xoff, yoff, snap, false);
}

const bool Being::adjust_movement(Level * level, std::vector<Entity*> interactables, float xoff, float yoff, const bool snap, const bool push_others)
{
	bool blocked = false;
	float adj_xvel = this->xvel, adj_yvel = this->yvel; // TODO: I think might need to be different if we push others
	Rect check_rect(rect.x + xoff, rect.y + yoff, rect.width, rect.height);
	if (!empty_at(check_rect, interactables)) {
		if (push_others) {
			for (Entity * e : interactables) {
				if (e && e != this && e->is_solid() 
					&& !e->get_should_push_others()
					&& e->intersects_area(check_rect)
					&& e->get_mask()
					&& Mask_Collide(this->get_mask(), e->get_mask(),
					(get_x() + this->xvel) - e->get_x(),
						(get_y() + this->yvel) - e->get_y())) {
					e->push_back(level, this->xvel, this->yvel);
				}
			}
		} else {
			float mag = std::pow(std::pow(xoff, 2.0) + std::pow(yoff, 2), 0.5);
			if (snap) {
				while (!precise_empty_at(interactables, xoff, yoff) && mag > 1.0f) {
					float temp_mag = std::pow(std::pow(xoff, 2.0) + std::pow(yoff, 2), 0.5);
					xoff = (adj_xvel / temp_mag)*mag, yoff = (adj_yvel / temp_mag)*mag;
					mag -= 1.0f;
				}
			}
			if (mag <= 1.0f || !snap) {
				float angle = M_PI / 12.0f;
				float xoff1 = xoff, yoff1 = yoff, xoff2 = xoff, yoff2 = yoff;
				while (!precise_empty_at(interactables, xoff1, yoff1)
					&& !precise_empty_at(interactables, xoff2, yoff2)
					&& angle < 5.0*M_PI / 12.0f) {
					xoff1 = adj_xvel * std::cos(angle) - adj_yvel * std::sin(angle), yoff1 = adj_xvel * std::sin(angle) + adj_yvel * std::cos(angle);
					xoff2 = adj_xvel * std::cos(-1.0f*angle) - adj_yvel * std::sin(-1.0f*angle), yoff2 = adj_xvel * std::sin(-1.0f*angle) + adj_yvel * std::cos(-1.0f*angle);
					float mag1 = std::pow(std::pow(xoff1, 2.0) + std::pow(yoff1, 2), 0.5), mag2 = std::pow(std::pow(xoff2, 2.0) + std::pow(yoff2, 2), 0.5);
					if (precise_empty_at(interactables, xoff1, yoff1)) {
						adj_xvel = xoff1, adj_yvel = yoff1;
						break;
					}
					else if (precise_empty_at(interactables, xoff2, yoff2)) {
						adj_xvel = xoff2, adj_yvel = yoff2;
						break;
					}
					angle += M_PI / 12.0f;
				}
				if (!precise_empty_at(interactables, adj_xvel, adj_yvel)) {
					adj_xvel = 0, adj_yvel = 0;
					blocked = true;
				}
			} else {
				adj_xvel = xoff, adj_yvel = yoff;
			}
			this->xvel = adj_xvel, this->yvel = adj_yvel;
		}
	}
	
	return blocked;
}

void Being::collision_update(World * world, Level * level, const int game_mode)
{
	std::vector<Entity*> interactables = level->get_colliding_interactables(this, *(this->get_rect_for_collision()), false);
	std::vector<Tile*> nearby_tiles = level->get_nearby_tiles(this);
	this->colliding_entities.clear();
	int t_size = nearby_tiles.size();
	for (int i = 0; i < t_size; i++) {
		Block *b = nearby_tiles[i]->get_block();
		if (b) interactables.push_back(b);
	}
	const int size = interactables.size();
	for (int i = 0; i < size; i++) {
		Entity * other = interactables[i];
		if (other 
			&& this->intersects_area(*(other->get_rect_for_collision()))
			&& other->get_mask() && Mask_Collide(this->get_mask(), other->get_mask(),
			get_x() - other->get_x(),
			get_y() - other->get_y())) {
			Entity* e = other;
			this->colliding_entities.push_back(e);
			collide_with_entity(world, level, e);
		}
	}
}

void Being::collide_with_entity(World * world, Level * level, Entity * e)
{

	if (e->get_entity_attribute(E_ATTR_BROKEN) != 1 && e->has_entity_attribute(E_ATTR_CONTACT_DAMAGE)) {
		if (counters[BOUNCE] > 0) return;
		if (counters[SWING] > 0) return;
		//TODO: once health/stamina is implemented, apply damage as necessary. (will also need to figure out invincibility frames)
		std::pair<float, float> p1 = get_rect_center();
		std::pair<float, float> p2 = e->get_rect_center();
		std::pair<float, float> bounce_vec = std::pair<float, float>(p1.first - p2.first, p1.second - p2.second);
		int knockback = std::max(1, e->get_entity_attribute(E_ATTR_KNOCKBACK));
		float mag = std::pow(std::pow(bounce_vec.first, 2.0) + std::pow(bounce_vec.second, 2), 0.5);
		xvel = (bounce_vec.first / mag)*knockback, yvel = (bounce_vec.second / mag)*knockback;
		counters[BOUNCE] = knockback * 2;
	}
}

const bool Being::on_ground(std::vector<Entity*> interactables) {
	return false; //uncomment if we ever want platforming logic
				  /*
				  Rect* below = new Rect(rect.x, rect.y + rect.height, rect.width, 1);
				  if (empty_at(*below, interactables)) {
				  delete below;
				  below = NULL;
				  return false;
				  }
				  else {
				  delete below;
				  below = NULL;
				  return true;
				  }
				  */
}

const bool Being::empty_at(Rect collide_rect, Level * level)
{
	return this->empty_at(collide_rect, level, false);
}

const bool Being::empty_at(Rect collide_rect, Level * level, const bool ignore_moving_obstacles)
{
	return !level->has_rect_collisions(this, collide_rect, ignore_moving_obstacles);
}

const bool Being::get_should_push_others()
{
	return this->should_push_others;
}

const bool Being::set_should_push_others(const bool value)
{
	return this->should_push_others = value;
}

void Being::push_back(Level* level, const float adj_xvel, const float adj_yvel)
{
	this->rect.x += adj_xvel, this->rect.y += adj_yvel;
	this->collide_rect.x += adj_xvel, this->collide_rect.y += adj_yvel;
}

const bool Being::empty_at(Rect r, std::vector<Entity*> interactables) {
	for (Entity *e : interactables) {
		if (e && e != this && e->is_solid() && e->intersects_area(r)) {
			return false;
		}
	}
	return true;
}

const bool Being::precise_empty_at(std::vector<Entity*> interactables, const int xoff, const int yoff)
{
	if (!this->get_mask()) return true;
	const int size = interactables.size();
	for (int i = 0; i < size; i++) {
		if (interactables[i]
			&& interactables[i]->is_solid()
			&& !(this->get_should_push_others() && interactables[i]->get_should_push_others())
			&& interactables[i]->get_mask()
			&& Mask_Collide(this->get_mask(), interactables[i]->get_mask(),
			(get_x() + xoff) - interactables[i]->get_x(),
				(get_y() + yoff) - interactables[i]->get_y())) {
			return false;
		}
	}
	return true;
}

const float Being::get_speed_multiplier(TileSet* tileset, std::vector<Tile*> tiles)
{
	float mult = 1.0f;
	std::vector<Tile *> colliding_tiles = this->get_colliding_tiles(tileset, tiles);
	int t_size = colliding_tiles.size();
	for (int i = 0; i < t_size; i++) {
		mult = std::min(mult, tileset->get_tile_speed_mod(colliding_tiles[i]->get_tile_type_index()));
	}
	int e_size = this->colliding_entities.size();
	for (int i = 0; i < e_size; i++) {
		Entity* e = colliding_entities[i];
		if (e->get_entity_attribute(E_ATTR_BROKEN) != 1 && e->has_entity_attribute(E_ATTR_CONTACT_SLOW)) {
			mult = mult / e->get_entity_attribute(E_ATTR_CONTACT_SLOW);
		}
	}
	return mult;
}

std::vector<Tile *> Being::get_colliding_tiles(TileSet * tileset, std::vector<Tile*> tiles)
{
	std::vector<Tile *> colliding_tiles;
	int t_size = tiles.size();
	for (int i = 0; i < t_size; i++) {
		mask_t *tmask = tiles[i]->get_mask();
		if (tmask) {
			if (tmask && Mask_Collide(this->get_mask(),
				tmask, get_x() - tiles[i]->get_x(), get_y() - tiles[i]->get_y())) {
				colliding_tiles.push_back(tiles[i]);
			}
		}
	}
	return colliding_tiles;
}

void Being::play_sounds_for_entity(Entity * e)
{
	// probably just override in player
}