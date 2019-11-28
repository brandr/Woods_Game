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

Level * World::level_containing_location_marker(LocationMarker * marker)
{
	if (marker != NULL) {
		const std::string level_key = marker->get_containing_level_key();
		if (level_key != "") {
			return this->get_level_with_key(level_key);
		}
	}
	return NULL;
}

LocationMarker * World::location_marker_matching_level(Level * level)
{
	if (level != NULL) {
		const int size = this->dungeons.size();
		for (int i = 0; i < size; i++) {
			Dungeon *d = this->dungeons[i].get();
			LocationMarker * lm = d->location_marker_matching_level(level);
			if (lm) {
				return lm;
			}
		}
	}
	return NULL;
}

const std::pair<int, int> World::map_location_for_level(Level * level)
{
	if (level != NULL) {
		LocationMarker * lm = this->location_marker_matching_level(level);
		if (lm != NULL) {
			Level * containing_level = this->level_containing_location_marker(lm);
			const int grid_x = containing_level->get_grid_x(), grid_y = containing_level->get_grid_y();
			const int standard_w = this->get_default_level_width(), standard_h = this->get_default_level_height();
			const int x_off = lm->get_x(), y_off = lm->get_y();
			return std::pair<int, int>(grid_x * standard_w + x_off, grid_y * standard_h + y_off);
		}
	}
	return std::pair<int, int>(-1, -1);
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
	Register("default_level_width", &default_level_width);
	Register("default_level_height", &default_level_height);
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
	this->load_dungeons("resources/load/dungeons/");
}

void World::load_dungeons(const std::string filepath)
{
	const int size = this->dungeon_data.size();
	for (int i = 0; i < size; i++) {
		const std::string dungeon_key = this->dungeon_data.getItem(i)->get_dungeon_key();
		this->add_dungeon(new Dungeon(dungeon_key, filepath, this->default_level_width.value(), this->default_level_height.value()));
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

void World::reload_world_state(const std::string world_state_path)
{
	FileManager filemanager;
	this->world_state.reset();
	filemanager.load_xml_content(&(this->world_state), world_state_path, "SerializableClass", "WorldKey", this->get_world_key());
}

void World::reload_quest_data(const std::string filepath)
{
	FileManager filemanager;
	this->quest_data.reset();
	filemanager.load_xml_content(&(this->quest_data), filepath, "SerializableClass", "WorldKey", this->get_world_key());
	this->quest_data.load_content_from_attributes();
}

void World::reload_encyclopedia(const std::string filepath)
{
	FileManager filemanager;
	this->encyclopedia.reset();
	filemanager.load_xml_content(&(this->encyclopedia), filepath, "SerializableClass", "WorldKey", this->get_world_key());
	this->encyclopedia.load_content_from_attributes();
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
		npc->stand_still();
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

void World::generate_map_images()
{
	const std::string path_prefix = "resources/load/saves/";
	const std::string world_path = this->world_key.value();
	const std::string filepath = world_path + "/maps/";
	const std::string full_filepath = path_prefix + filepath;
	if (!boost::filesystem::is_directory(full_filepath)) {
		boost::filesystem::create_directory(full_filepath);
	}
	const int size = dungeons.size();
	for (int i = 0; i < size; i++) {
		if (dungeons[i]) {
			const std::string filename = filepath + dungeons[i]->get_dungeon_name();
			const std::string full_filename = path_prefix + filename;
			ALLEGRO_BITMAP * dungeon_bitmap = 
				dungeons[i]->generate_map_image(this->default_level_width.value(), this->default_level_height.value());
			al_save_bitmap((full_filename + ".png").c_str(), dungeon_bitmap);
			al_destroy_bitmap(dungeon_bitmap);
			ImageLoader::get_instance().load_image(filename, "", false, path_prefix);
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
	std::string day_save_file_dir = "resources/load/saves/" + save_file_name;
	if (!boost::filesystem::is_directory(day_save_file_dir)) {
		boost::filesystem::create_directory(day_save_file_dir);
	}
	day_save_file_dir += "/" + day_str;
	if (!boost::filesystem::is_directory(day_save_file_dir)) {
		boost::filesystem::create_directory(day_save_file_dir);
	}
	FileManager filemanager;
	const std::string world_file_path = "resources/load/saves/" + save_file_name + "/world";
	std::map<std::string, std::string> attributes;
	attributes["Type"] = "World";
	attributes["Version"] = "1";
	attributes["WorldKey"] = save_file_name;
	// TODO: this saves anything that should be global. need to draw distinctions between this and per-day saves
	file_manager.create_xml_file(world_file_path);
	file_manager.save_xml_content(world_file_path, "SerializableClass", attributes);
	const std::string world_xml = world->toXML();
	file_manager.replace_xml_content(world_file_path, "SerializableClass", "WorldKey", save_file_name, world_xml);
	const std::string dungeon_dir = day_save_file_dir + "/dungeons";
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
	// world state
	const std::string world_state_xml = world->get_world_state()->toXML();
	const std::string day_save_file_path = day_save_file_dir + "/" + "world_state";
	std::map<std::string, std::string> ws_attributes;
	ws_attributes["Type"] = "WorldState";
	ws_attributes["Version"] = "1";
	ws_attributes["WorldKey"] = save_file_name;
	file_manager.create_xml_file(day_save_file_path);
	file_manager.save_xml_content(day_save_file_path, "SerializableClass", ws_attributes);
	file_manager.replace_xml_content(day_save_file_path, "SerializableClass", "WorldKey", save_file_name, world_state_xml);
	// quest data
	const std::string quest_data_xml = world->get_quest_data()->toXML();
	const std::string quest_data_file_path = "resources/load/saves/" + save_file_name + "/" + "quest_data";
	std::map<std::string, std::string> qd_attributes;
	qd_attributes["Type"] = "QuestData";
	qd_attributes["Version"] = "1";
	qd_attributes["WorldKey"] = save_file_name;
	file_manager.create_xml_file(quest_data_file_path);
	file_manager.save_xml_content(quest_data_file_path, "SerializableClass", qd_attributes);
	file_manager.replace_xml_content(quest_data_file_path, "SerializableClass", "WorldKey", save_file_name, quest_data_xml);
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
				npc->update(this, current_npc_level, time, game_mode);
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

void World::update_new_day(GlobalTime * time, Player * player, const std::string current_level_key)
{
	this->clear_all_beings();
	//TODO: make sure we do this in a way where the player doesn't jump suddenly
	// players position is updated in extract current level, so try passing the arg in there instead
	Level * current_level = this->extract_current_level(player, current_level_key);
	current_level->add_being(player);
	const int size = dungeons.size();
	for (int i = 0; i < size; i++) {
		if (dungeons[i]) {
			dungeons[i]->update_new_day(this, player);
		}
	}
	this->update_npcs_new_day();
	this->spawn_critters(time);
}

void World::update_reload_day(GlobalTime * time, Player * player, const std::string current_level_key)
{
	this->clear_all_beings();
	//TODO: make sure we do this in a way where the player doesn't jump suddenly
	// players position is updated in extract current level, so try passing the arg in there instead
	Level * current_level = this->extract_current_level(player, current_level_key);
	current_level->add_being(player);
	this->update_npcs_new_day();
	this->spawn_critters(time);
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

void World::spawn_critters(GlobalTime * time)
{
	const int size = dungeons.size();
	for (int i = 0; i < size; i++) {
		if (dungeons[i]) {
			dungeons[i]->spawn_critters(this, time);
		}
	}
}

void World::player_visible_entities_update(Level * level)
{
	std::vector<Entity *> visible_entities = level->player_visible_entities();
	for (Entity * e : visible_entities) {
		if (e->has_visibility_actions(level)) {
			e->update_visibility(this, level);
		}
	}
}

void World::add_dungeon(Dungeon *dungeon)
{
	current_dungeon = dungeon;
	dungeons.push_back(std::move(std::shared_ptr<Dungeon>(dungeon)));
}

Player * World::get_player()
{
	return this->world_state.get_player();
}

NPC * World::get_npc(const std::string npc_key)
{
	const int size = this->npcs.size();
	for (int i = 0; i < size; i++) {
		NPC * npc = this->npcs.getItem(i);
		if (npc != NULL && npc_key == npc->get_npc_key()) {
			return npc;
		}
	}
	return NULL;
}

const bool World::npc_is_on_node(const std::string npc_key, const std::string node_key)
{
	NPC * npc = this->get_npc(npc_key);
	PathNode * node = this->find_path_node_with_key(node_key);
	if (npc != NULL && node != NULL) {
		Level * node_level = this->find_level_with_path_node_key(node_key);
		if (node_level != NULL) {
			const std::string npc_level_key = npc->get_current_level_key();
			if (npc_level_key == node_level->get_filename()) {
				Rect * npc_rect = npc->get_rect_for_collision();
				Rect * node_rect = node->get_rect_for_collision();
				const int x_dist = std::abs(npc_rect->x - node_rect->x), y_dist = std::abs(npc_rect->y - node_rect->y);
				return x_dist < 2 && y_dist < 2;
			}
		}
	}
	return false;
}

void World::set_has_met_npc(const std::string npc_key)
{
	this->world_state.set_has_met_npc(npc_key);
}

void World::copy_trigger_status(TriggerStatus * status)
{
	this->world_state.copy_trigger_status(status);
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

WorldState * World::get_world_state()
{
	return &world_state;
}

void World::update_quests()
{
	Level * level = this->get_current_level();
	std::vector<Quest *> all_quests = this->world_state.get_quests();
	for (Quest * q : all_quests) {
		q->update(this, level);
	}
}

QuestData * World::get_quest_data()
{
	return &(this->quest_data);
}

TriggerStatus * World::matching_trigger_status(TriggerStatus * status)
{
	return this->world_state.matching_trigger_status(status);
}

const int World::get_default_level_width()
{
	return this->default_level_width.value();
}

const int World::get_default_level_height()
{
	return this->default_level_height.value();
}

void World::mark_grid_explored(const int grid_x, const int grid_y)
{
	this->world_state.mark_grid_explored(grid_x, grid_y);
}

void World::mark_cutscene_viewed(const std::string cutscene_key)
{
	this->world_state.mark_cutscene_viewed(cutscene_key);
}

const bool World::has_viewed_cutscene(const std::string cutscene_key)
{
	return this->world_state.has_viewed_cutscene(cutscene_key);
}

const std::set<std::pair<int, int>> World::explored_map()
{
	//TODO: does this need to take current dungeon into account? or do we never switch dungeons within a single world?
	return this->world_state.explored_map();
}

Inventory * World::get_inventory_for_key(const std::string inv_key, const bool create_if_missing)
{
	Inventory * inv = NULL;
	if (!inv_key.empty()) {
		return this->world_state.get_inventory_for_key(inv_key, create_if_missing);
	}
	return inv;
}

void World::process_quest_update(QuestUpdate * quest_update)
{
	this->world_state.process_quest_update(quest_update);
}

std::vector<Quest*> World::get_active_quests()
{
	return this->world_state.get_active_quests();
}

std::vector<Quest*> World::get_failed_quests()
{
	return this->world_state.get_failed_quests();
}

std::vector<Quest*> World::get_completed_quests()
{
	return this->world_state.get_completed_quests();
}

Quest * World::quest_for_key(const std::string quest_key)
{
	return this->world_state.quest_with_key(quest_key);
}

void World::set_has_quest_item(const std::string item_key, const bool has_item)
{
	this->quest_data.set_has_quest_item(item_key, has_item);
}

Encyclopedia * World::get_encyclopedia()
{
	return &(this->encyclopedia);
}

void World::update_encyclopedia_state(const std::string category_name, const std::string entry_name, const int entry_state)
{
	if (category_name != "" && entry_name != "") {
		this->encyclopedia.update_entry_state(category_name, entry_name, entry_state);
	}
}

void World::update_encyclopedia_for_critter(Critter * critter, const int entry_state)
{
	const std::string critter_key = critter->get_critter_key();
	
	const std::string category_name = CritterManager::get_instance().get_encyclopedia_category_name(critter_key);
	const std::string entry_name = CritterManager::get_instance().get_encyclopedia_entry_name(critter_key);
	if (category_name != "" && entry_name != "") {
		this->encyclopedia.update_entry_state(category_name, entry_name, entry_state);
	}
	
	//TODO: update number caught
	//TODO: more information the more we catch?
	//TODO: probably want this to be agnostic to seeing large animals vs catch small ones, so check whether the critter is catchable
	//			(I think I need to add a new serialized binding for this)
}

const int World::encyclopedia_state_for_critter(Critter * critter)
{
	const std::string critter_key = critter->get_critter_key();

	const std::string category_name = CritterManager::get_instance().get_encyclopedia_category_name(critter_key);
	const std::string entry_name = CritterManager::get_instance().get_encyclopedia_entry_name(critter_key);
	if (category_name != "" && entry_name != "") {
		return this->encyclopedia.get_entry_state(category_name, entry_name);
	}
	return 0;
}

const int World::get_encyclopedia_state(const std::string category_name, const std::string entry_name)
{
	if (category_name != "" && entry_name != "") {
		return this->encyclopedia.get_entry_state(category_name, entry_name);
	}
	return 0;
}
