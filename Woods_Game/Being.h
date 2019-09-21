#ifndef BEING_H
#define BEING_H

#define DEFAULT_FOOTSTEP_FILENAME "footstep_default"

#include "TileSet.h"
#include "Tile.h"
#include "Entity.h"  // for Entity
#include "utility"   // for pair

class Level;
class World;
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
	bool should_push_others = false;

	void movement_update(Level * level, const int game_mode);
	void movement_update_top_down(Level * level);
	virtual void draw(ALLEGRO_DISPLAY * display, int x_offset, int y_offset);
	void collision_update(Level * level, const int game_mode);
	const bool adjust_movement(Level * level, float xoff, float yoff, const bool snap);
	const bool adjust_movement(Level * level, std::vector<Entity*> collide_entities, float xoff, float yoff, const bool snap, const bool push_others);
	virtual void collide_with_entity(Entity* e);
	const bool on_ground(std::vector<Entity*>);
	const bool empty_at(Rect collide_rect, Level * level);
	
	const bool empty_at(Rect, std::vector<Entity*>);
	const float get_speed_multiplier(TileSet * tileset, std::vector<Tile*> tiles);
	std::vector<Tile *> get_colliding_tiles(TileSet * tileset, std::vector<Tile*> tiles);
	virtual void play_sounds_for_entity(Entity* e);
public:
	Being();
	~Being();
	virtual void update(World * world, Level * level, GlobalTime * time, const int game_mode);
	virtual void emit_sound_update(World * world, Level * level, GlobalTime * time, const int game_mode);
	virtual void play_sound_update(World * world, Level * level, GlobalTime * time, const int game_mode);
	virtual void animation_update(const int game_mode);
	void set_xvel(int);
	void set_yvel(int);
	const bool precise_empty_at(std::vector<Entity*>, const int, const int);
	const bool empty_at(Rect collide_rect, Level * level, const bool ignore_moving_obstacles);
	virtual const bool get_should_push_others();
	virtual const bool set_should_push_others(const bool value);
	virtual void push_back(Level * level, const float xvel, const float yvel);
};

#endif