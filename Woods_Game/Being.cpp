
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

void Being::update(std::vector<Entity> interactables, std::pair<int, int> level_dimensions)
{
	
	//TODO: collide with the edge of the screen (keep in mind that for the player, this will sometimes lead to oher levels)

	movement_update(interactables);
	GameImage::update();
	//TODO: non-platform collisions (some beings may pass through some objects, so need a system to handle nuance instead of just checking is_solid)
}

void Being::movement_update(std::vector<Entity> interactables) {

	//TODO: note that this update method implies the being is affected by gravity, which may not always be the case
	//check for side collisions
	Rect* side = NULL;
	int vel = yvel;
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
		//std::cout << "STOPPED" << std::endl;
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
			//Rect* above = new Rect(rect.x + std::min(xvel, 0), rect.y + yvel, rect.width + std::max(xvel, 0), yvel);
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
		//Rect* below = new Rect(rect.x + std::min(xvel, 0), rect.y + rect.height, rect.width + std::max(xvel, 0), std::max(yvel + gravity, 2));
		Rect* below = new Rect(rect.x , rect.y + rect.height, rect.width, std::max(yvel + gravity, 2));
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


bool Being::passable_at(int x, int y, std::vector<Entity> interactables)
{
	//TODO: might need to check that we aren't falling outside the level
	for (Entity e : interactables) {
		if (e.is_solid() && e.contains_point(x, y)) {
			return false;
		}
	}
	return true;
}

bool Being::empty_at(Rect r, std::vector<Entity> interactables) {
	//TODO: this currently does bounding box collision checks. Implement more sophisticated checks later.
	for (Entity e : interactables) {
		if (e.is_solid() && e.intersects_area(r)) {
			return false;
		}
	}
	return true;
}
