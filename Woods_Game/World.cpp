#include "World.h"
#include "ImageLoader.h"
#include "Player.h"

Level * World::find_level_with_spawn_key(const std::string spawn_key)
{
	const int size = this->dungeons.size();
	for (int i = 0; i < size; i++) {
		Dungeon *d = this->dungeons[i].get();
		Level *l = d->find_level_with_spawn_key(spawn_key);
		if (l) {
			return l;
		}
	}
	return NULL;
}

World::World()
{
	setClassName("World");
	Register("world_key", &world_key); 
	Register("current_dungeon_key", &current_dungeon_key);
	Register("player_key", &player_key);
	Register("current_level_grid_x", &current_level_grid_x);
	Register("current_level_grid_y", &current_level_grid_y);
	Register("dungeon_data", &dungeon_data);
	Register("npcs", &npcs);
	Register("current_day", &current_day);
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

void World::reload_dungeons(const std::string dungeons_path)
{
	const int size = this->dungeons.size();
	for (int i = 0; i < size; i++) {
		std::shared_ptr<Dungeon> d = this->dungeons[i];
		FileManager filemanager;
		const std::string filename = dungeons_path + "/" + d->get_dungeon_name();
		std::vector<std::string> level_keys = filemanager.all_xml_keys(filename, "SerializableClass", "Level", "LevelKey");
		const std::vector<Level *> levels = d->get_level_list();
		const int size = levels.size();
		for (Level * level : levels) {
			const std::string level_key = level->get_filename();
			//level->reset_for_reload();
			Level copy_level(level_key, d->get_dungeon_name(), level_key);
			filemanager.load_xml_content(&copy_level, filename, "SerializableClass", "LevelKey", level_key);
			level->reload_from_xml(copy_level);
			copy_level.unload_content();
		}
	}
}

void World::load_npcs()
{
	const int npc_count = this->npcs.size();
	for (int i = 0; i < npc_count; i++) {
		NPC * npc = this->npcs.getItem(i);
		const std::string spawner_id = npc->get_spawn_key();
		Level * level = this->find_level_with_spawn_key(spawner_id);
		if (level) {
			level->add_npc_at_spawner(npc, spawner_id);
		}
	}
}

void World::load_player()
{
	//TODO
	//load_player_from_xml("resources/load/player", "default_player");
}

void World::generate_levels()
{
	for (int i = 0; i < dungeons.size(); i++) {
		if (dungeons[i]) {
			dungeons[i]->generate_levels();
		}
	}
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

void World::update_new_day(Player * player)
{
	for (int i = 0; i < dungeons.size(); i++) {
		if (dungeons[i]) {
			dungeons[i]->update_new_day(player);
		}
	}
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

Level * World::extract_current_level(Player * player)
{
	const std::string spawner_id = player->get_spawn_key();
	Level * level = this->find_level_with_spawn_key(spawner_id);
	if (level) {
		Spawner * spawner = level->spawner_for_key(spawner_id);
		player->set_starting_pos(spawner->get_x(), spawner->get_y());
		player->set_position(spawner->get_x(), spawner->get_y());
		return level;
	}
	//TODO: if we want the player to store the level without having a spawner, put the logic here
	return NULL;
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

std::vector<std::shared_ptr<Dungeon>> World::get_dungeons()
{
	return this->dungeons;
}

std::string World::get_player_key()
{
	return this->player_key.value();
}

const std::string World::get_world_key()
{
	return this->world_key.value();
}

const int World::get_current_day()
{
	return this->current_day.value();
}

void World::set_current_day(const int day)
{
	this->current_day = day;
}
