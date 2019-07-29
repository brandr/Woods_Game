#ifndef DJIKSTRA_PATH_DATA_H
#define DJIKSTRA_PATH_DATA_H

#include "PathNode.h"
#include "allegro5/threads.h"
#include <vector>

class TileDjikstraPath;
class PathNodeDjikstraPath;
class DjikstraPathData {

public:

	ALLEGRO_MUTEX * mutex;
	ALLEGRO_COND  *cond;
	std::string current_level_key;
	std::string npc_key;
	std::string dest_level_key;
	std::string dest_node_id;
	std::pair<int, int> start_npc_pos;
	std::pair<int, int> npc_t_pos;
	TileDjikstraPath * tile_djikstra_path;
	PathNodeDjikstraPath * node_djikstra_path;
	std::vector<std::pair<std::string, std::pair<int, int>>> primary_destinations;
	std::pair<std::vector<std::pair<int, int>>, float> tile_path_to_primary;
	std::vector<PathNode *> current_level_path_nodes;
	std::pair<std::string, std::pair<int, int>> forced_dest;
	bool has_found_path = false;
	bool should_force = false;
	bool should_unset_forced_destination = false;
	bool ready;

	DjikstraPathData() : mutex(al_create_mutex()),
		cond(al_create_cond()),
		ready(false) {}

	~DjikstraPathData() {
		al_destroy_mutex(mutex);
		al_destroy_cond(cond);
	}
};

#endif
