//#include "Entity.h"
#include "Tile.h"
#include<vector>
class Being:public Entity
{
protected:
	float xvel = 0.0f;
	float yvel = 0.0f;
	float gravity = 2.0f;
	float terminal_velocity = 14.0f;
	void movement_update(std::vector<Entity*>, std::vector<Tile>, int);
	void movement_update_side_scrolling(std::vector<Entity*>, std::vector<Tile>);
	void movement_update_top_down(std::vector<Entity*>, std::vector<Tile>);
	bool on_ground(std::vector<Entity*>);
	bool empty_at(Rect, std::vector<Entity*>);
	bool precise_empty_at(std::vector<Entity*>, int, int);
	float get_speed_multiplier(std::vector<Tile> tiles);
	//bool empty_at(mask_t*, std::vector<Entity>, int);
	//bool empty_at(Rect, std::vector<Entity>, bool);
public:
	Being();
	~Being();
	virtual void update(std::vector<Entity*>, std::vector<Tile>, std::pair<int, int>, int);
	virtual void animation_update(int game_mode);
	void set_xvel(int);
	void set_yvel(int);
};

