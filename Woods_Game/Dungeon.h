#include "Level.h"

class Dungeon
{
private:
	//std::vector<Level> level_list;	// needed to store level objects since dungeon_grid only has pointers
	
public:
	std::vector<std::vector<Level*>> dungeon_grid;
	//Level* current_level;
	Dungeon();
	Dungeon(int, int);
	~Dungeon();
	void unload_content();
	void load_images(ImageLoader&);
	void add_level(Level*);
	Level* level_at(int, int);
	//void set_current_level(int, int);
	//void set_player(Player);
	//Level* get_current_level();
	//Player* get_player();
	//std::pair<int, int> get_current_level_dimensions();
};

