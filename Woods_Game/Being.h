#ifndef BEING_H
#define BEING_H

#include "TileSet.h"
#include "Tile.h"
#include "Entity.h"  // for Entity
#include "utility"   // for pair

class Being: public Entity
{
protected:
	float xvel = 0.0f;
	float yvel = 0.0f;
	float gravity = 2.0f;
	float terminal_velocity = 14.0f;
	std::vector<Entity*> colliding_entities;

	void movement_update(TileSet * tileset, std::vector<Entity*>, std::vector<Tile*>, const int);
	//void movement_update_side_scrolling(std::vector<Entity*>, std::vector<Tile*>);
	void movement_update_top_down(TileSet * tileset, std::vector<Entity*>, std::vector<Tile*>);
	void collision_update(std::vector<Entity*>, std::vector<Tile*>, int);
	virtual void collide_with_entity(Entity* e);
	const bool on_ground(std::vector<Entity*>);
	const bool empty_at(Rect, std::vector<Entity*>);
	const bool precise_empty_at(std::vector<Entity*>, const int, const int);
	const float get_speed_multiplier(TileSet * tileset, std::vector<Tile*> tiles);
public:
	Being();
	~Being();
	virtual void update(TileSet * tileset, std::vector<Entity*>, std::vector<Tile*>, const std::pair<int, int>, const int);
	virtual void animation_update(const int game_mode);
	void set_xvel(int);
	void set_yvel(int);
};

#endif