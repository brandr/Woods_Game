#include "World.h"
#include "ImageLoader.h"
#include "Player.h"

Level * World::get_level_with_key(const std::string level_key)
{
	const int size = this->dungeons.size();
	for (int i = 0; i < size; i++) {
		Dungeon *d = this->dungeons[i].get();
		Level *l = d->level_with_name(level_key);
		if (l) {
			return l;
		}
	}
	return NULL;
}

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

Level * World::get_npc_destination_level(NPC * npc)
{
	if (npc != NULL) {
		const std::string node_key = this->get_npc_destination_node_key(npc);
		return this->find_level_with_path_node_key(node_key);
	}
	return NULL;
}

Level * World::find_level_with_path_node_key(const std::string node_key)
{
	const int size = this->dungeons.size();
	for (int i = 0; i < size; i++) {
		Dungeon *d = this->dungeons[i].get();
		Level *l = d->find_level_with_path_node_key(node_key);
		if (l) {
			return l;
		}
	}
	return NULL;
}

PathNode * World::get_npc_destination_node(NPC * npc)
{
	if (npc != NULL) {
		const std::string node_key = this->get_npc_destination_node_key(npc);
		if (node_key != "") {
			return this->find_path_node_with_key(node_key);
		}
	}
	return NULL;
}

PathNode * World::find_path_node_with_key(const std::string node_key)
{
	if (node_key != "") {
		const int size = this->dungeons.size();
		for (int i = 0; i < size; i++) {
			Dungeon *d = this->dungeons[i].get();
			PathNode * n = d->find_path_node_with_key(node_key);
			if (n) {
				return n;
			}
		}
	}
	return NULL;
}

const std::string World::get_npc_destination_node_key(NPC * npc)
{
	if (npc) {
		return npc->get_current_destination_node_key();
	}
	return "";
}

void World::clear_all_beings()
{
	for (std::shared_ptr<Dungeon> d : this->dungeons) {
		d->clear_all_beings();
	}
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
		const std::string level_key = npc->get_start_level_key();
		const std::string spawner_key = npc->get_start_spawn_key();
		Level * level = this->get_level_with_key(level_key);
		if (level && level->spawner_for_key(spawner_key)) {
			level->add_npc_at_spawner(npc, spawner_key);
		}
	}
}

void World::load_player()
{
}

void World::generate_levels()
{
	const int size = dungeons.size();
	for (int i = 0; i < size; i++) {
		if (dungeons[i]) {
			dungeons[i]->generate_levels();
		}
	}
}

void World::unload_content()
{
	const int size = dungeons.size();
	for (int i = 0; i < size; i++) {
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
	// TODO: we don't actually need to do anything here, so either delete the method or refactor global image loading from level/gamimagemanager to here
}

// updates all NPCs, even if they aren't on the current level
//TODO: eventually this will incorporate NPC schedules, but just feed fake data for now
void World::npc_update()
{
	Level * active_level = this->get_current_level();
	const int size = this->npcs.size();
	for (int i = 0; i < size; i++) {
		NPC * npc = this->npcs.getItem(i);
		const std::string current_level_key = npc->get_current_level_key();
		if (current_level_key != "" && !npc->has_primary_destinations()) {
			Level * current_npc_level = this->get_level_with_key(current_level_key);
			// the NPC exists somewhere in the world
			if (current_npc_level != NULL) {
				Level * destination_level = this->get_npc_destination_level(npc);
				PathNode * destination_node = this->get_npc_destination_node(npc);
				if (destination_level != NULL && destination_node != NULL) {
					const std::string dest_node_id = destination_node->get_node_id();
					std::vector<PathNode *> current_level_path_nodes = current_npc_level->get_path_nodes();
					TileDjikstraPath tile_djikstra_path(current_npc_level, npc);
					const std::pair<int, int> npc_t_pos(npc->get_x() / TILE_SIZE, npc->get_y() / TILE_SIZE);
					int shortest_current_level_node_dist = -1;
					int second_shortest_current_level_node_dist = -1;
					PathNode * closest_node = NULL;
					PathNode * second_closest_node = NULL;
					for (PathNode * current_level_node : current_level_path_nodes) {
						const std::pair<int, int> current_level_node_pos(current_level_node->get_x() / TILE_SIZE, current_level_node->get_y() / TILE_SIZE);
						const std::pair<std::vector<std::pair<int, int>>, int> current_level_node_shortest_path
							= tile_djikstra_path.calculate_shortest_tile_path(npc_t_pos, current_level_node_pos);
						const int next_node_dist = current_level_node_shortest_path.second;
						std::vector<std::pair<int, int>> next_node_path = current_level_node_shortest_path.first;
						if (next_node_dist < 0) {
							continue;
						} else {
							if (shortest_current_level_node_dist < 0
								|| next_node_dist < shortest_current_level_node_dist) {
								shortest_current_level_node_dist = next_node_dist;
								closest_node = current_level_node;
							} else if (second_shortest_current_level_node_dist < 0
								|| next_node_dist < second_shortest_current_level_node_dist) {
								second_shortest_current_level_node_dist = next_node_dist;
								second_closest_node = current_level_node;
							}
						}
					}
					if (shortest_current_level_node_dist < 0 || closest_node == NULL) {
						npc->clear_pathing_destination(); //TEMP
						//TODO: we can't path anywhere, so tell the NPC not to path
					} else if (second_shortest_current_level_node_dist < 0 || second_closest_node == NULL) {
						// there is only one possible destination, so we'll path there
						const std::pair<int, int> primary_dest(closest_node->get_x(), closest_node->get_y());
						npc->add_primary_destination(primary_dest, closest_node->get_node_id());
					} else {
						// This is the only case where we have to use actual PathNode pathing,
						// because there is more than one relevant PathNode on the level.
						// First, we make a PathNode map so we know the edges and costs between path nodes, 
						//    including across levels
						PathNodeDjikstraPath node_djikstra_path(this, npc, destination_level, destination_node);
						std::pair<std::vector<DjikstraNode*>, int> shortest_path_from_closest
							= node_djikstra_path.calculate_shortest_node_path(this,
								closest_node, current_npc_level->get_filename(), destination_node, destination_level->get_filename());
						std::pair<std::vector<DjikstraNode*>, int> shortest_path_from_second_closest
							= node_djikstra_path.calculate_shortest_node_path(this,
								second_closest_node, current_npc_level->get_filename(), destination_node, destination_level->get_filename());

						std::vector<DjikstraNode*> node_list_from_closest = shortest_path_from_closest.first;
						std::vector<DjikstraNode*> node_list_from_second_closest = shortest_path_from_second_closest.first;

						const int shortest_from_closest_cost = shortest_path_from_closest.second;
						const int shortest_from_second_closest_cost = shortest_path_from_second_closest.second;

						const bool can_path_from_closest = shortest_from_closest_cost > 0
							&& !node_list_from_closest.empty();
						const bool can_path_from_second_closest = shortest_from_second_closest_cost > 0
							&& !node_list_from_second_closest.empty();

						std::vector<DjikstraNode*> path_to_execute;
						// we can just go straight to the destination node
						if (closest_node->get_node_id() == dest_node_id) {
							npc->clear_pathing_destination(); //TEMP
						} else if (!can_path_from_closest && !can_path_from_second_closest) {
							path_to_execute = node_list_from_closest;
						} else if (can_path_from_closest && !can_path_from_second_closest) {
							path_to_execute = node_list_from_closest;
						} else if (!can_path_from_closest && can_path_from_second_closest) {
							path_to_execute = node_list_from_second_closest;
						} else {
							path_to_execute = shortest_from_closest_cost <= shortest_from_second_closest_cost
								? node_list_from_closest : node_list_from_second_closest;
						}
						if (path_to_execute.empty()) {
							npc->clear_pathing_destination(); //TEMP
																  //TODO: we can't path anywhere, so tell the NPC not to path
						} else {
							// path to every node on the path that is on the current level.
							// we do not path to the next level because we can just recalculate after reaching it.
							for (DjikstraNode * node_to_execute : path_to_execute) {
								const std::string level_key = node_to_execute->level_key;
								if (level_key != current_level_key) {
									break;
								}
								npc->add_primary_destination(node_to_execute->position, node_to_execute->node_key);
							}
						}
					}
				}
			}
		}
	}
}

void World::update_new_day(Player * player)
{
	this->clear_all_beings();
	const int size = dungeons.size();
	for (int i = 0; i < size; i++) {
		if (dungeons[i]) {
			dungeons[i]->update_new_day(player);
		}
	}
	this->update_npcs_new_day();
}

void World::update_npcs_new_day()
{
	const int npc_count = this->npcs.size();
	for (int i = 0; i < npc_count; i++) {
		NPC * npc = this->npcs.getItem(i);
		//TODO: need to pass in global time or other global values?
		const std::string level_key = npc->get_current_spawn_level_key();
		const std::string spawner_key = npc->get_current_spawn_key();
		Level * level = this->get_level_with_key(level_key);
		if (level && level->spawner_for_key(spawner_key)) {
			level->add_npc_at_spawner(npc, spawner_key);
			break;
		}
	}
	//TODO: other NPC updates that should happen at the beginning of the day
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
