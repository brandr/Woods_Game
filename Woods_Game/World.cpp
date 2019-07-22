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

PathNodeDjikstraPath * World::get_node_djikstra_path(NPC * npc)
{
	const std::string node_djikstra_path_key = this->get_node_djikstra_path_key(npc);
	auto it = this->node_djikstra_path_map.find(node_djikstra_path_key);
	if (it == this->node_djikstra_path_map.end()) {
		this->node_djikstra_path_map[node_djikstra_path_key] = new PathNodeDjikstraPath(this, npc);
	}
	return this->node_djikstra_path_map[node_djikstra_path_key];
}

const std::string World::get_node_djikstra_path_key(NPC * npc)
{
	return "npcKey-" + npc->get_npc_key();
}

TileDjikstraPath * World::get_tile_djikstra_path(Level * level, NPC * npc)
{
	const std::string path_key = this->get_tile_djikstra_path_key(level, npc);
	auto it = this->tile_djikstra_path_map.find(path_key);
	if (it == this->tile_djikstra_path_map.end()) {
		this->tile_djikstra_path_map[path_key] = new TileDjikstraPath(level, npc, true);
	}
	return this->tile_djikstra_path_map[path_key];
}

const std::string World::get_tile_djikstra_path_key(Level * level, NPC * npc)
{
	return "levelKey-" + level->get_filename() + ":npcKey-" + npc->get_npc_key();
}

void World::calculate_npc_pathing(NPC * npc, Level * current_npc_level)
{
	double start_time = al_get_time();
	double accounted_time = 0.0;

	bool enable_logging = true; //temp

	if (enable_logging) {
		std::cout << "--------------------------------------\n";
		std::cout << "START calculating npc pathing\n\n";
	}
	

	bool has_found_path = false;
	Level * destination_level = this->get_npc_destination_level(npc);
	PathNode * destination_node = this->get_npc_destination_node(npc);
	if (destination_level != NULL && destination_node != NULL) {
		const std::string dest_node_id = destination_node->get_node_id();
		std::vector<PathNode *> current_level_path_nodes = current_npc_level->get_path_nodes();

		double start_create_tile_path_time = al_get_time();

		TileDjikstraPath * tile_djikstra_path = this->get_tile_djikstra_path(current_npc_level, npc);
		tile_djikstra_path->mark_blocked_tiles(current_npc_level, npc);

		if (enable_logging) {
			double finish_create_tile_path_time = al_get_time();
			double delta_p2 = finish_create_tile_path_time - start_create_tile_path_time;
			std::cout << "Time to initalize tile path: " << std::to_string(delta_p2) << " seconds\n\n";
			accounted_time += delta_p2;
		}

		const std::pair<int, int> npc_t_pos(npc->get_x() / TILE_SIZE, npc->get_y() / TILE_SIZE);
		float shortest_current_level_node_dist = -1;
		float second_shortest_current_level_node_dist = -1;
		PathNode * closest_node = NULL;
		PathNode * second_closest_node = NULL;
		for (PathNode * current_level_node : current_level_path_nodes) {

			const std::pair<int, int> current_level_node_pos(current_level_node->get_x() / TILE_SIZE, current_level_node->get_y() / TILE_SIZE);
			
			double start_get_tile_path_time = al_get_time();

			// TODO: this is the currently the slowest part, so improve it and/or make it async
			const std::pair<std::vector<std::pair<int, int>>, float> current_level_node_shortest_path
				= tile_djikstra_path->calculate_shortest_tile_path(npc_t_pos, current_level_node_pos);

			if (enable_logging) {
				double end_get_tile_path_time = al_get_time();
				double delta_t2 = end_get_tile_path_time - start_get_tile_path_time;
				std::cout << "Time to calculate shortest tile path: " << std::to_string(delta_t2) << " seconds\n\n";
				accounted_time += delta_t2;
			}

			const float next_node_dist = current_level_node_shortest_path.second;
			std::vector<std::pair<int, int>> next_node_path = current_level_node_shortest_path.first;

			if (next_node_dist < 0) {
				continue;
			} else {
				if (shortest_current_level_node_dist < 0
					|| next_node_dist < shortest_current_level_node_dist) {

					// the closest becomes the second closest
					second_shortest_current_level_node_dist = shortest_current_level_node_dist;
					second_closest_node = closest_node;

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
			npc->cancel_current_pathing();
		} else if (second_shortest_current_level_node_dist < 0 || second_closest_node == NULL) {
			// there is only one possible destination, so we'll path there
			const std::pair<int, int> primary_dest(closest_node->get_x(), closest_node->get_y());
			npc->add_primary_destination(primary_dest, closest_node->get_node_id());
			has_found_path = true;
		} else {
			// This is the only case where we have to use actual PathNode pathing,
			// because there is more than one relevant PathNode on the level.

			double start_get_path_time = al_get_time();
			
			PathNodeDjikstraPath * node_djikstra_path = this->get_node_djikstra_path(npc);
			
			if (enable_logging) {
				double end_get_path_time = al_get_time();
				double delta_2 = end_get_path_time - start_get_path_time;
				std::cout << "Time to initialize node path: " << std::to_string(delta_2) << " seconds\n\n";
				accounted_time += delta_2;
			}

			double start_calculate_node_path_time = al_get_time();

			std::pair<std::vector<DjikstraNode*>, float> shortest_path_from_closest
				= node_djikstra_path->calculate_shortest_node_path(this,
					closest_node, current_npc_level->get_filename(), destination_node, destination_level->get_filename());

			std::pair<std::vector<DjikstraNode*>, float> shortest_path_from_second_closest
				= node_djikstra_path->calculate_shortest_node_path(this,
					second_closest_node, current_npc_level->get_filename(), destination_node, destination_level->get_filename());

			if (enable_logging) {
				double end_calculate_node_path_time = al_get_time();
				double delta_3 = end_calculate_node_path_time - start_calculate_node_path_time;
				std::cout << "Time to calculate shortest node path: " << std::to_string(delta_3) << " seconds\n\n";
				accounted_time += delta_3;
			}

			std::vector<DjikstraNode*> node_list_from_closest = shortest_path_from_closest.first;
			std::vector<DjikstraNode*> node_list_from_second_closest = shortest_path_from_second_closest.first;

			const float shortest_from_closest_cost = shortest_path_from_closest.second;
			const float shortest_from_second_closest_cost = shortest_path_from_second_closest.second;

			const bool can_path_from_closest = shortest_from_closest_cost >= 0
				&& !node_list_from_closest.empty();
			const bool can_path_from_second_closest = shortest_from_second_closest_cost >= 0
				&& !node_list_from_second_closest.empty();

			std::vector<DjikstraNode*> path_to_execute;
			// we can just go straight to the destination node
			if (closest_node->get_node_id() == dest_node_id) {
				npc->cancel_current_pathing();
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
				npc->cancel_current_pathing();
			} else {
				// path to every node on the path that is on the current level.
				// we do not path to the next level because we can just recalculate after reaching it.
				const int path_to_execute_size = path_to_execute.size();
				for (int i = 0; i < path_to_execute_size; i++) {
					DjikstraNode * node_to_execute = path_to_execute[i];
					const std::string level_key = node_to_execute->level_key;
					if (level_key != current_npc_level->get_filename()) {
						break;
					}
					// add the node as our next destination
					npc->add_primary_destination(node_to_execute->position, node_to_execute->node_key);
					has_found_path = true;
				}
			}
		}
	}
	if (has_found_path) {
		npc->set_is_starting_path();
	} else {
		//TODO: might need to tell the NPC not to bother looking for that path again
		npc->cancel_current_pathing();
	}
	
	if (enable_logging) {
		double end_time = al_get_time();
		double delta = end_time - start_time;
		std::cout << "FINISH calculating npc pathing. Time to calculate: " + std::to_string(delta) + "\n";
		std::cout << "Unaccounted time: " << std::to_string(delta - accounted_time) << "(" << std::to_string(100.0 * (accounted_time / delta)) << "% accounted for)\n";
		std::cout << "--------------------------------------\n";
	}
}

void World::move_npc_to_level(NPC * npc, Level * current_level, Level * dest_level, const std::pair<int, int> position)
{
	//TODO: eventually want an actual walk animation off the level, probably before this method
	npc->set_current_level_key(dest_level->get_filename());
	npc->set_position(position.first, position.second);
	current_level->remove_being(npc);
	dest_level->add_being(npc);
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

Level * World::find_any_level_with_node()
{
	const int size = this->dungeons.size();
	for (int i = 0; i < size; i++) {
		Dungeon *d = this->dungeons[i].get();
		Level *l = d->find_any_level_with_node();
		if (l) {
			return l;
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

void World::recalculate_npc_paths()
{
	this->node_djikstra_path_map.clear();
	this->tile_djikstra_path_map.clear();

	const int size = this->npcs.size();
	const int d_size = this->dungeons.size();
	for (int i = 0; i < size; i++) {
		NPC * npc = this->npcs.getItem(i);
		// calling this will update the cache
		this->get_node_djikstra_path(npc);
		for (int d = 0; d < d_size; d++) {
			const int level_count = this->dungeons[d]->get_level_list_count();
			for (int l = 0; l < level_count; l++) {
				Level * level = this->dungeons[d]->level_at_index(l);
				// calling this will update the cache
				this->get_tile_djikstra_path(level, npc);
			}
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

void World::save_game(World * world, GlobalTime * global_time)
{
	FileManager file_manager;
	//TODO: make sure "world" data can be managed across days in terms of things like NPC interactions
	//		might want different types of data files besides world to manage some NPC stuff
	const std::string save_file_name = world->get_world_key();
	const std::string day_str = "day_" + std::to_string(global_time->get_day());
	world->set_current_day(global_time->get_day());
	std::string save_file_dir = "resources/load/saves/" + save_file_name;
	if (!boost::filesystem::is_directory(save_file_dir)) {
		boost::filesystem::create_directory(save_file_dir);
	}
	save_file_dir += "/" + day_str;
	if (!boost::filesystem::is_directory(save_file_dir)) {
		boost::filesystem::create_directory(save_file_dir);
	}
	FileManager filemanager;
	const std::string world_file_path = "resources/load/saves/" + save_file_name + "/worlds";
	std::map<std::string, std::string> attributes;
	attributes["Type"] = "World";
	attributes["Version"] = "1";
	attributes["WorldKey"] = save_file_name;
	file_manager.create_xml_file(world_file_path);
	file_manager.save_xml_content(world_file_path, "SerializableClass", attributes);
	const std::string world_xml = world->toXML();
	//TODO: when to update NPCs? (I think these get serialized in world)
	file_manager.replace_xml_content(world_file_path, "SerializableClass", "WorldKey", save_file_name, world_xml);
	const std::string dungeon_dir = save_file_dir + "/dungeons";
	if (!boost::filesystem::is_directory(dungeon_dir)) {
		boost::filesystem::create_directory(dungeon_dir);
	}
	for (std::shared_ptr<Dungeon> d : world->get_dungeons()) {
		const std::string dungeon_path = dungeon_dir + "/" + d->get_dungeon_name();
		file_manager.create_xml_file(dungeon_path);
		std::vector<Level*> levels = d->get_level_list();
		for (Level *level : levels) {
			std::map<std::string, std::string> attributes;
			attributes["Type"] = "Level";
			attributes["Version"] = "1";
			attributes["LevelKey"] = level->get_filename();
			file_manager.save_xml_content(dungeon_path, "SerializableClass", attributes);
			const std::string level_xml = level->toXML();
			file_manager.replace_xml_content(dungeon_path, "SerializableClass", "LevelKey", level->get_filename(), level_xml);
		}
	}
	const std::string save_file_path = save_file_dir + "/" + day_str;
	file_manager.create_xml_file(save_file_path);
}

// updates all NPCs, even if they aren't on the current level
//TODO: eventually this will incorporate NPC schedules, but just feed fake data for now
void World::npc_update(GlobalTime * time, const int game_mode)
{
	Level * active_level = this->get_current_level();
	const int size = this->npcs.size();
	for (int i = 0; i < size; i++) {
		NPC * npc = this->npcs.getItem(i);
		const std::string current_level_key = npc->get_current_level_key();
		Level * current_npc_level = this->get_level_with_key(current_level_key);
		if (current_npc_level != NULL) {
			// the npc is on another level, so we need to update it separate from the level's being list
			// TODO: might want to always update NPCs here and just handle level beings differently in general
			if (current_level_key != active_level->get_filename()) {
				npc->update(current_npc_level, time, game_mode);
			}
			// transport the npc to the next level if appropriate
			if (npc->is_requesting_next_level()) {
				const std::string next_level_key = npc->get_requested_next_level_key();
				
				Level * npc_next_level = this->get_level_with_key(next_level_key);
				if (npc_next_level == NULL) {
					//TODO: error handling
				} else {
					// move the npc to the next level
					const std::string next_level_node_key = npc->get_requested_next_level_node_key();
					PathNode * next_level_node = npc_next_level->find_path_node_with_key(next_level_node_key);
					// TODO: error handling
					const std::pair<int, int> next_level_node_pos(next_level_node->get_x(), next_level_node->get_y());
					this->move_npc_to_level(npc, current_npc_level, npc_next_level, next_level_node_pos);
					npc->set_is_processing(false);
				}
			} else if (npc->needs_pathing_calculation()) { 
				//TODO: figure out if we can actually calculate asynchronously here
				// probably the main problem is that we can't read tiles/entities async
				npc->set_is_processing(true);
				this->calculate_npc_pathing(npc, current_npc_level);
				//std::future<void> fut_npc_pathing(
				//	std::async(
				//		launch::async, &World::calculate_npc_pathing, this, npc, current_npc_level));
			}
		}
	}
}

void World::update_new_day(Player * player, const std::string current_level_key)
{
	this->clear_all_beings();
	//TODO: make sure we do this in a way where the player doesn't jump suddenly
	// players position is updated in extract current level, so try passing the arg in there instead
	Level * current_level = this->extract_current_level(player, current_level_key);
	current_level->add_being(player);
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
		//may need to check schedule to see where the npc should spawn
		const std::string level_key = npc->get_current_spawn_level_key();
		const std::string spawner_key = npc->get_current_spawn_key();
		Level * level = this->get_level_with_key(level_key);
		if (level && level->spawner_for_key(spawner_key)) {
			level->add_npc_at_spawner(npc, spawner_key);
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

Level * World::extract_current_level(Player * player, const std::string previous_level_key)
{
	const std::string spawner_id = player->get_spawn_key();
	Level * level = this->find_level_with_spawn_key(spawner_id);
	if (level) {
		float x_pos = -1.0, y_pos = -1.0;
		if (previous_level_key != "" && previous_level_key == level->get_filename()) {
			x_pos = player->get_x(), y_pos = player->get_y();
		}
		// if the player is already in that level, no need to adjust position.
		// otherwise, grab a position that makes sense.
		if (x_pos >= 0 && y_pos >= 0) {
			player->set_position((int)x_pos, (int)y_pos);
		} else {
			Spawner * spawner = level->spawner_for_key(spawner_id);
			player->set_starting_pos(spawner->get_x(), spawner->get_y());
			player->set_position(spawner->get_x(), spawner->get_y());
		}
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
