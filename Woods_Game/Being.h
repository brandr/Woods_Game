#ifndef BEING_H
#define BEING_H

#include "TileSet.h"
#include "Tile.h"
#include "Entity.h"  // for Entity
#include "utility"   // for pair

class Level;
class Being: public Entity
{
protected:
	ALLEGRO_BITMAP * test_rect_bitmap; //TEMP

	xmls::xFloat base_walk_speed;
	float xvel = 0.0f;
	float yvel = 0.0f;
	float gravity = 2.0f;
	float terminal_velocity = 14.0f;
	std::vector<Entity*> colliding_entities;

	void movement_update(TileSet * tileset, std::vector<Entity*>, std::vector<Tile*>, const int);
	void movement_update(Level * level, const int game_mode);
	void movement_update_top_down(TileSet * tileset, std::vector<Entity*>, std::vector<Tile*>);
	void movement_update_top_down(Level * level);
	virtual void draw(ALLEGRO_DISPLAY * display, int x_offset, int y_offset);
	void collision_update(std::vector<Entity*>, std::vector<Tile*>, int); //TODO: look into replacing this
	void collision_update(Level * level, const int game_mode);
	const bool adjust_movement(std::vector<Entity*> interactables, float xoff, float yoff, const bool snap);
	virtual void collide_with_entity(Entity* e);
	const bool on_ground(std::vector<Entity*>);
	const bool empty_at(Rect collide_rect, Level * level);
	const bool empty_at(Rect collide_rect, Level * level, const bool ignore_moving_obstacles);
	const bool empty_at(Rect, std::vector<Entity*>);
	const bool precise_empty_at(std::vector<Entity*>, const int, const int);
	const float get_speed_multiplier(TileSet * tileset, std::vector<Tile*> tiles);
public:
	Being();
	~Being();
	virtual void update(Level * level, GlobalTime * time, const int game_mode);
	//virtual void update(TileSet * tileset, std::vector<Entity*>, std::vector<Tile*>, const std::pair<int, int>, const int);
	virtual void animation_update(const int game_mode);
	void set_xvel(int);
	void set_yvel(int);
};

#endif