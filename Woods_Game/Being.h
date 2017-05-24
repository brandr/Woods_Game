//#include "GameScreen.h"
#include "Entity.h"
#include<vector>
class Being:public Entity
{
protected:
	int direction;
	int xvel = 0;
	int yvel = 0;
	int gravity = 2;
	int terminal_velocity = 14;
	void movement_update(std::vector<Entity>);
	bool on_ground(std::vector<Entity>);
	bool passable_at(int, int, std::vector<Entity>);
	bool empty_at(Rect, std::vector<Entity>);
public:
	Being();
	~Being();
	virtual void update(std::vector<Entity>, std::pair<int, int>);
	void set_xvel(int);
	void set_yvel(int);
};

