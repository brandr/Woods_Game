#include "World.h"
#include "ImageLoader.h"

World::World()
{
	setClassName("World");
	Register("current_dungeon_key", &current_dungeon_key);
	Register("player_key", &player_key);
	Register("current_level_grid_x", &current_level_grid_x);
	Register("current_level_grid_y", &current_level_grid_y);
	Register("dungeon_data", &dungeon_data);
	//TODO: add dungeon based on dungeon data
	//add_dungeon(new Dungeon("dungeon_1"));	//temp. probably want to pass in multiple dungeons and a starting one
}


World::~World()
{

}

void World::load_dungeons()
{
	const int size = this->dungeon_data.size();
	for (int i = 0; i < size; i++) {
		const std::string dungeon_key = this->dungeon_data.getItem(i)->get_dungeon_key();
		this->add_dungeon(new Dungeon(dungeon_key));
	}
}

void World::load_player()
{
	//TODO
	//load_player_from_xml("resources/load/player", "default_player");
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
	return this->get_dungeon(this->current_dungeon_key.value());
}

Level * World::get_current_level()
{
	return this->get_current_dungeon()->level_at(this->current_level_grid_x.value(), this->current_level_grid_y.value());
}

Dungeon * World::get_dungeon(std::string dungeon_name)
{
	const int size = this->dungeons.size();
	for (int i = 0; i < size; i++) {
		Dungeon *d = this->dungeons[i].get();
		if (dungeon_name == d->get_dungeon_name()) {
			return d;
		}
	}
	return NULL;
}

std::string World::get_player_key()
{
	return this->player_key.value();
}
