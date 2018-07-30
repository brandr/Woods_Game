#include "World.h"
#include "ImageLoader.h"

World::World()
{
	add_dungeon(new Dungeon("dungeon_1"));	//temp. probably want to pass in multiple dungeons and a starting one
}


World::~World()
{
	
	//delete dungeon;
}

void World::unload_content()
{

	for (int i = 0; i < dungeons.size(); i++) {
		if (dungeons[i]) {
			dungeons[i]->unload_content();
			dungeons[i].reset();
		}
	}
	dungeons.clear();
	std::vector<std::shared_ptr<Dungeon>>().swap(dungeons);
}

void World::load_images(ImageLoader& loader)
{
}

void World::add_dungeon(Dungeon *dungeon)
{
	current_dungeon = dungeon;
	dungeons.push_back(std::move(std::shared_ptr<Dungeon>(dungeon)));
}

Dungeon* World::get_current_dungeon()
{
	//std::cout << "dungeon size: " << current_dungeon->get_current_level();
	return dungeons[0].get();	//TEMP
}