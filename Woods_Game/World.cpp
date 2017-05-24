#include "World.h"
#include "ImageLoader.h"

World::World()
{
	add_dungeon(new Dungeon(3, 1));
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
			//dungeons[i].
			//delete dungeons[i];
		}
	}
	dungeons.clear();
	std::vector<std::shared_ptr<Dungeon>>().swap(dungeons);
	//dungeons.swap(std::vector<std::shared_ptr<Dungeon>>());
}

void World::load_images(ImageLoader& loader)
{
	//loader.load_image(player.get_image_filename());
	/*
	std::vector<Platform> platforms = get_platforms();
	for (Platform pl : platforms) {
		loader.load_image((&pl)->get_image_filename());
	}
	*/
	//current_dungeon->load_images(loader);
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

/*
Player * World::get_player()
{
	return current_dungeon->get_player();
}


Level * World::get_current_level()
{
	return current_dungeon->get_current_level();
}

void World::set_current_level(int x, int y)
{
	current_dungeon->set_current_level(x, y);
}

std::pair<int, int> World::get_current_level_dimensions()
{
	return current_dungeon->get_current_level_dimensions();
}
*/