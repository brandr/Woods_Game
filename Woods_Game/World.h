#ifndef WORLD_H
#define WORLD_H
#include "Dungeon.h"
#include <memory>
class ImageLoader;

class World
{
private:
	std::vector <std::shared_ptr<Dungeon>> dungeons;
public:
	
	Dungeon* current_dungeon;
	World();
	~World();
	void unload_content();
	void load_images(ImageLoader&);
	void add_dungeon(Dungeon*);
	Dungeon* get_current_dungeon();
};

#endif