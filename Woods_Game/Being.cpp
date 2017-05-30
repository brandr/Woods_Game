#define _USE_MATH_DEFINES
#include<math.h>
#define NOMINMAX
#include "Being.h"
#include<iostream>
#include<algorithm>


Being::Being()
{
}


Being::~Being()
{
}

void Being::set_xvel(int vel)
{ 
	xvel = vel;
}

void Being::set_yvel(int vel)
{
	yvel = vel;
}

void Being::update(std::vector<Entity> interactables, std::pair<int, int> level_dimensions, int game_mode)
{
	
	//TODO: collide with the edge of the screen (keep in mind that for the player, this will sometimes lead to oher levels)
	movement_update(interactables, game_mode);
	animation_update(game_mode);
	Entity::update();
	//TODO: non-platform collisions (some beings may pass through some objects, so need a system to handle nuance instead of just checking is_solid)
}

void Being::animation_update(int game_mode)
{
	//auto it = image_map.find(std::pair<std::string, std::string>(full_filename, rect_to_string(subsection)));
	//if (it != image_map.end()) return;
	//.find(std::pair<int, int>(game_mode, direction)
	//TODO
}

void Being::movement_update(std::vector<Entity> interactables, int game_mode) {
	switch (game_mode) {
		case(TOP_DOWN):
			movement_update_top_down(interactables);
			break;
		case(SIDE_SCROLLING):
			movement_update_side_scrolling(interactables);
			break;
	}
}

void Being::movement_update_side_scrolling(std::vector<Entity> interactables)
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
		//Rect* below = new Rect(rect.x , rect.y + rect.height, rect.width, std::max(yvel + gravity, 2));
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

void Being::movement_update_top_down(std::vector<Entity> interactables)
{
	//TODO: consider having a grid of tiles on the level to make all interactions with grid-locked gameimages easier.
	// note that these can't have their collisions handled by empty_at()
	float xoff = xvel, yoff = yvel;
	Rect* check_rect = new Rect(rect.x + xoff, rect.y + yoff, rect.width, rect.height);
	if (!empty_at(*check_rect, interactables)) {	
		float mag = std::pow(std::pow(xoff, 2.0) + std::pow(yoff, 2), 0.5);
		while (!precise_empty_at(interactables, xoff, yoff) && mag > 1.0f) {
			float temp_mag = std::pow(std::pow(xoff, 2.0) + std::pow(yoff, 2), 0.5);
			xoff = (xvel / temp_mag)*mag, yoff = (yvel / temp_mag)*mag;
			mag -= 1.0f;
		}
		if (mag <= 1.0f) {
			//TODO: check different angles
			float angle = M_PI/6.0f;
			float xoff1 = xvel, yoff1 = yvel, xoff2 = xvel, yoff2 = yvel;
			while(!precise_empty_at(interactables, xoff1, yoff1) && !precise_empty_at(interactables, xoff2, yoff2) && angle < 5.0f*M_PI/12.0f){
				xoff1 = xvel * std::cos(angle) - yvel * std::sin(angle), yoff1 = xvel * std::sin(angle) + yvel * std::cos(angle);
				xoff2 = xvel * std::cos(-1.0f*angle) - yvel * std::sin(-1.0f*angle), yoff2 = xvel * std::sin(-1.0f*angle) + yvel * std::cos(-1.0f*angle);
				float mag1 = std::pow(std::pow(xoff1, 2.0) + std::pow(yoff1, 2), 0.5), mag2 = std::pow(std::pow(xoff2, 2.0) + std::pow(yoff2, 2), 0.5);
				if (precise_empty_at(interactables, xoff1, yoff1)) {
					xvel = xoff1, yvel = yoff1;
					break;
				}
				else if (precise_empty_at(interactables, xoff2, yoff2)) {
					xvel = xoff2, yvel = yoff2;
					break;
				}
				/*
				while (!precise_empty_at(interactables, xoff1, yoff1) && !precise_empty_at(interactables, xoff2, yoff2)) {
					float temp_mag1 = std::pow(std::pow(xoff1, 2.0) + std::pow(yoff1, 2), 0.5);
					float temp_mag2 = std::pow(std::pow(xoff2, 2.0) + std::pow(yoff2, 2), 0.5);
					xoff1 = (xvel / temp_mag1)*mag, yoff1 = (yvel / temp_mag1)*mag;
					mag1 -= 1.0f, mag2 -=1.0f;
				}
				*/
				angle += M_PI / 6.0f;
			}
			if (!precise_empty_at(interactables, xvel, yvel)) 
				xvel = 0, yvel = 0;
		}
		else
		{
			xvel = xoff, yvel = yoff;
		}
	}
	delete check_rect;
	check_rect = NULL;
	/*
	Rect* h_side = NULL;
	if (xvel < 0) h_side = new Rect(rect.x + xvel, rect.y, -1*xvel, rect.height);
	else if (xvel > 0) h_side = new Rect(rect.x + rect.width, rect.y, xvel, rect.height);
	if (h_side && !empty_at(*h_side, interactables)) {
		refresh_mask();
		for (int i = 0; i < interactables.size(); i++) 
			interactables[i].refresh_mask();	//TODO: in the event of slowdown, limit this to only interactables that are already rect-colliding
			
		//TODO: make this collision pixel perfect
		//h_side->width = 1;
		//while (empty_at(*h_side, interactables)) {
		//mask_t check_mask = 
		//while (Mask_Collide(mask, interactables[i].get_mask())) {
		while (empty_at(mask, interactables, HORIZONTAL)) {
			if (xvel < 0) {
				rect.x -= 1;
				h_side->x = rect.x - 1;
			}
			else {
				rect.x += 1;
				h_side->x = rect.x + rect.width;
			}
		}
		xvel = 0;
	}
	if (h_side)
	{
		delete h_side;
		h_side = NULL;
	}
	// check for vertical collisions 
	Rect* v_side = NULL;
	if (yvel < 0) v_side = new Rect(rect.x, rect.y + yvel, rect.width, -1*yvel);
	else if (yvel > 0) v_side = new Rect(rect.x, rect.y + rect.height, rect.width, yvel);
	if (v_side && !empty_at(*v_side, interactables)) {
		refresh_mask();
		for (int i = 0; i < interactables.size(); i++)
			interactables[i].refresh_mask();	//TODO: in the event of slowdown, limit this to only interactables that are already rect-colliding
		//TODO: make this collision pixel perfect
		v_side->height = 1;
		while (empty_at(mask, interactables, VERTICAL)) {
			if (yvel < 0) {
				rect.y -= 1;
				v_side->y = rect.y - 1;
			}
			else {
				rect.y += 1;
				v_side->y = rect.y + rect.height;
			}
		}
		yvel = 0;
	}
	if (v_side)
	{
		delete v_side;
		v_side = NULL;
	}
	*/
	rect.x += xvel;
	rect.y += yvel;
	//TODO
}

bool Being::on_ground(std::vector<Entity> interactables) {
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
}

//bool Being::empty_at(Rect r, std::vector<Entity> interactables, bool pixel_perfect) {
bool Being::empty_at(Rect r, std::vector<Entity> interactables) {
	//TODO: this currently does bounding box collision checks. Implement more sophisticated checks later.
	for (Entity e : interactables) {
		if (e.is_solid() && e.intersects_area(r)) {
			return false;
		}
	}
	return true;
}
bool Being::precise_empty_at(std::vector<Entity> interactables, int xoff, int yoff)
{
	if (!mask) return true;
	const int size = interactables.size();
	for (int i = 0; i < size; i++) {
		if (Mask_Collide(mask, interactables[i].get_mask(), (get_x() + xoff) - interactables[i].get_x(), (get_y() + yoff) - interactables[i].get_y()))
			return false;
	}
	return true;
}
/*
bool Being::empty_at(mask_t *mask, std::vector<Entity> interactables, int axis)
{
	const int size = interactables.size();
	for (int i = 0; i < size; i++) {
		int x_off = 0, y_off = 0;
		switch (axis) {
		case HORIZONTAL:
			x_off = xvel;
			break;
		case VERTICAL:
			y_off = yvel;
			break;
		}
		if (Mask_Collide(mask, interactables[i].get_mask(), (get_x() + x_off) - interactables[i].get_x(), (get_y() + y_off) - interactables[i].get_y()))
			return false;
	}
	
	//TODO
	return true;
}
*/