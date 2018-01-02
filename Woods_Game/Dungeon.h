#ifndef DUNGEON_H
#define DUNGEON_H

#include "Level.h"
#include "vector"  // for vector
#include <memory>  // for allocator
#include "ImageLoader.h"

class Dungeon
{
private:
	//std::vector<Level> level_list;	// needed to store level objects since dungeon_grid only has pointers
	
public:
	std::vector<std::vector<Level*>> dungeon_grid;
	Dungeon();
	Dungeon(int, int);
	~Dungeon();
	void unload_content();
	void load_images(ImageLoader&);
	void add_level(Level*);
	Level* level_at(int, int);
};

#endif