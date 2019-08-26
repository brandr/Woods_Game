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
	const std::string node_djikstra_path_key = this->get_node_djikstra_path_key(npc->get_npc_key());
	auto it = this->node_djikstra_path_map.find(node_djikstra_path_key);
	if (it == this->node_djikstra_path_map.end()) {
		this->node_djikstra_path_map[node_djikstra_path_key] = new PathNodeDjikstraPath(this, npc);
	}
	return this->node_djikstra_path_map[node_djikstra_path_key];
}

PathNodeDjikstraPath * World::get_mapped_node_djikstra_path(const std::string npc_key)
{
	const std::string node_djikstra_path_key = this->get_node_djikstra_path_key(npc_key);
	auto it = this->node_djikstra_path_map.find(node_djikstra_path_key);
	if (it == this->node_djikstra_path_map.end()) {
		return NULL;
	}
	return this->node_djikstra_path_map[node_djikstra_path_key];
}

const std::string World::get_node_djikstra_path_key(const std::string npc_key)
{
	return "npcKey-" + npc_key;
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

TileDjikstraPath * World::get_mapped_tile_djikstra_path(Level * level, NPC * npc)
{
	const std::string path_key = this->get_tile_djikstra_path_key(level, npc);
	auto it = this->tile_djikstra_path_map.find(path_key);
	if (it == this->tile_djikstra_path_map.end()) {
		return NULL;
	}
	return this->tile_djikstra_path_map[path_key];
}

const std::string World::get_tile_djikstra_path_key(Level * level, NPC * npc)
{
	return "levelKey-" + level->get_filename() + ":npcKey-" + npc->get_npc_key();
}

// TODO: something in here causes a lot of slowdown that persists even when this is not being called
// TODO: make as much of this method async as possible
//		first, need to refactor out places where we use NPC and current level and replace with cached values
const bool World::calculate_npc_pathing(NPC * npc, Level * current_npc_level)
{
	bool has_found_path = false;
	Level * destination_level = this->get_npc_destination_level(npc);
	PathNode * destination_node = this->get_npc_destination_node(npc);
	TileDjikstraPath * tile_djikstra_path = NULL;
	if (destination_level != NULL && destination_node != NULL) {
		npc->clear_primary_destinations();
		double start_create_tile_path_time = al_get_time();
		tile_djikstra_path = this->get_mapped_tile_djikstra_path(current_npc_level, npc);
		tile_djikstra_path->mark_blocked_tiles(current_npc_level, npc, npc->get_should_push_others());
		has_found_path 
			= this->calculate_npc_pathing(npc, current_npc_level, tile_djikstra_path, destination_node->get_node_id(), destination_level->get_filename());	
	}
	return has_found_path;
}

//TODO: make this method or stuff inside it async
//could store primary dests to add, whether we should cancel npc pathing, etc in thread data
const bool World::calculate_npc_pathing(
	NPC * npc, Level * current_npc_level, TileDjikstraPath * tile_djikstra_path, const std::string dest_node_id, const std::string dest_level_key)
{
	double start_time = al_get_time();
	double accounted_time = 0.0;

	bool enable_logging = false; //temp

	bool has_found_path = false;

	Rect * collide_rect = npc->get_rect_for_collision();
	const std::pair<int, int> npc_t_pos(collide_rect->x / TILE_SIZE, collide_rect->y / TILE_SIZE);
	const std::string current_npc_level_key = current_npc_level->get_filename();
	std::vector<PathNode *> current_level_path_nodes = current_npc_level->get_path_nodes();
	const std::string npc_key = npc->get_npc_key();
	const std::pair<int, int> start_npc_pos((int)collide_rect->x, collide_rect->y);

	bool should_force = false;
	bool should_unset_forced_destination = false;
	std::vector<std::pair<std::string, std::pair<int, int>>> primary_destinations;

	const std::pair<std::string, std::pair<int, int>> forced_dest = npc->get_forced_destination();
	const bool path_around_moving = npc->get_should_path_around_moving();
	npc->set_should_path_around_moving(false);

	const std::pair<std::string, std::pair<int, int>> last_primary_dest = npc->get_last_primary_destination();
	npc->set_last_primary_destination(std::pair<std::string, std::pair<int, int>>("", std::pair<int, int>()));

	//TODO: store the node key and level key in data so we can use the cached path and not have to calculate on the fly.
	std::string starting_node_key = "";

	for (PathNode * n : current_level_path_nodes) {
		if (n->get_x() / TILE_SIZE == start_npc_pos.first / TILE_SIZE
			&& n->get_y() / TILE_SIZE == start_npc_pos.second / TILE_SIZE) {
			starting_node_key = n->get_node_id();
			break;
		}
	}

	PathNodeDjikstraPath * node_djikstra_path = this->get_mapped_node_djikstra_path(npc_key);

	auto it = this->djikstra_path_data_map.find(npc_key);
	if (it != this->djikstra_path_data_map.end()) {
		delete it->second;
	}

	DjikstraPathData * path_data = new DjikstraPathData();

	path_data->current_level_key = current_npc_level_key;
	path_data->npc_key = npc_key;
	path_data->dest_level_key = dest_level_key;
	path_data->dest_node_id = dest_node_id;
	path_data->start_npc_pos = start_npc_pos;
	path_data->npc_t_pos = npc_t_pos;			//TODO: this might be redundant given npc_t_pos, also it should probably be collide pos
	path_data->current_level_path_nodes = current_level_path_nodes;
	path_data->forced_dest = forced_dest;
	path_data->last_primary_dest = last_primary_dest;
	path_data->should_path_around_moving = path_around_moving;
	path_data->tile_djikstra_path = tile_djikstra_path;
	path_data->node_djikstra_path = node_djikstra_path;
	path_data->starting_node_key = starting_node_key;
	path_data->ready = false;

	this->djikstra_path_data_map[npc_key] = path_data;


	npc->set_is_processing(true);

	//TODO: during NPC update, need to check for ready data to see if we need to unlock
	//TODO: copy the stuff below the commented section to a separate method we call when unlocking the NPC after we finish pathing

	ALLEGRO_THREAD * pathing_thread = al_create_thread(PathNodeDjikstraPath::func_calculate_npc_pathing, path_data);

	auto it_2 = this->djikstra_path_thread_map.find(npc_key);
	if (it_2 != this->djikstra_path_thread_map.end()) {
		al_destroy_thread(it_2->second);
		this->djikstra_path_thread_map[npc_key] = NULL;
	}

	this->djikstra_path_thread_map[npc_key] = pathing_thread;

	al_start_thread(pathing_thread);

	return has_found_path;
}

void World::finish_pathing(NPC * npc)
{
	npc->set_is_processing(false);

	DjikstraPathData * path_data = this->djikstra_path_data_map[npc->get_npc_key()];

	std::pair<std::vector<std::pair<int, int>>, float> tile_path_to_primary = path_data->tile_path_to_primary; //TODO: this should come from thread data

	if (path_data->has_found_path) {
		if (path_data->should_unset_forced_destination) {
			npc->set_forced_destination(std::pair<std::string, std::pair<int, int>>("", std::pair<int, int>(-1, -1)));
		}

		for (std::pair<std::string, std::pair<int, int>> primary_dest : path_data->primary_destinations) {
			// looks backwards, but this is correct
			npc->add_primary_destination(primary_dest.second, primary_dest.first);
		}

		// TODO: second part of the pair is distance, so incorporate that if the NPC is going to care about travel time
		std::vector<std::pair<std::string, std::pair<int, int>>> secondary_dests;
		for (std::pair<int, int> secondary_dest : tile_path_to_primary.first) {
			secondary_dests.push_back(std::pair<std::string, std::pair<int, int>>
				("", std::pair<int, int>(secondary_dest.first * TILE_SIZE, secondary_dest.second * TILE_SIZE)));
		}
		npc->set_secondary_destinations(secondary_dests);
		npc->set_is_starting_path();
		//npc->reset_failed_pathing_tally();
	} else {
		npc->cancel_current_pathing(AI_PATH_BLOCKED_WAIT_TIME);
		npc->increment_failed_pathing_tally();
	}
}

void World::move_npc_to_level(NPC * npc, Level * current_level, Level * dest_level, const std::pair<int, int> position,
	const std::pair<int, int> arrive_offset, const bool animate_walking)
{
	npc->set_current_level_key(dest_level->get_filename());
	Rect * collide_rect = npc->get_rect_for_collision();
	const std::pair<int, int> offset(collide_rect->x - npc->get_x(), collide_rect->y - npc->get_y());
	npc->set_position(position.first - offset.first + arrive_offset.first, position.second - offset.second + arrive_offset.second);
	npc->cancel_current_pathing(0);
	current_level->remove_being(npc);
	dest_level->add_being(npc);
	if (animate_walking) {
		const int x_dir = std::abs(arrive_offset.first) > 0 ? std::abs(arrive_offset.first) / arrive_offset.first : 0;
		const int y_dir = std::abs(arrive_offset.second) > 0 ? std::abs(arrive_offset.second) / arrive_offset.second : 0;
		// negate because (for example) if we are coming from the left, we need to walk to the right
		npc->set_is_walking_from_level(x_dir * -1, y_dir * -1);
		npc->set_should_push_others(true);
	}
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
		npc->cancel_current_pathing(0);
		npc->clear_primary_destinations();
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
		if (npc->is_locked()) {
			DjikstraPathData * path_data = NULL;
			bool has_finished_pathing = false;
			auto it = this->djikstra_path_data_map.find(npc->get_npc_key());
			if (it != this->djikstra_path_data_map.end()) {
				path_data = this->djikstra_path_data_map[npc->get_npc_key()];
				has_finished_pathing = path_data->ready;
			}
			if (path_data && has_finished_pathing) {
				this->finish_pathing(npc);
			}
		} else {
			const std::string current_level_key = npc->get_current_level_key();
			Level * current_npc_level = this->get_level_with_key(current_level_key);
			if (current_npc_level != NULL) {
				// the level itself will not call update method for NPCs, so call it here
				npc->update(current_npc_level, time, game_mode);
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
						const std::pair<int, int> next_level_node_pos(next_level_node->get_x(), next_level_node->get_y());
						const std::pair<int, int> arrival_offset = next_level_node->get_arrival_offset();
						this->move_npc_to_level(npc, current_npc_level, npc_next_level, 
							std::pair<int, int>(next_level_node_pos.first, next_level_node_pos.second), arrival_offset, 
							next_level_node->should_animate_walking());
					}
				} else if (npc->needs_pathing_calculation()) {
					this->calculate_npc_pathing(npc, current_npc_level);
				}
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

void World::update_reload_day(Player * player, const std::string current_level_key)
{
	this->clear_all_beings();
	//TODO: make sure we do this in a way where the player doesn't jump suddenly
	// players position is updated in extract current level, so try passing the arg in there instead
	Level * current_level = this->extract_current_level(player, current_level_key);
	current_level->add_being(player);
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
		npc->clear_primary_destinations();
		npc->cancel_current_pathing(0);
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
