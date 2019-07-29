#ifndef DJIKSTRA_PATH_H
#define DJIKSTRA_PATH_H

#define TILE_NODE_PREFIX "tile_node"
#define PATH_NODE_PREFIX "path_node"

#include <vector>
#include <set>
#include <map>
#include "DjiktraPathData.h"
#include "PathNode.h"
#include "allegro5/threads.h"

class Level;
class NPC;
class World;
class AIBeing;

struct DjikstraNode {
	std::string level_key = "";
	std::string node_key = "";
	std::pair<int, int> position;
	float node_cost;
	// neighbors with costs
	std::vector<std::pair<DjikstraNode *, float>> neighbors;
	DjikstraNode();
	~DjikstraNode();
	void add_neigbor(DjikstraNode * neighbor, const float cost);
	const float get_neighbor_cost(DjikstraNode * neighbor);
};

// base path method
class DjikstraPath
{
protected:
	std::map<std::string, DjikstraNode *> node_map;
	// TODO: probably want start and end node pointers here, and full node maps in other methods
	// TODO: sholud this be void?
	virtual void calculate_shortest_path();
public:
	DjikstraPath();
	~DjikstraPath();
};

class TileDjikstraPath : public DjikstraPath {
private:
	void initialize_nodes(Level * level, AIBeing * being);
	void initialize_nodes(Level * level, AIBeing * being, const bool ignore_moving_obstacles);
	DjikstraNode * get_tile_node(const int tx, const int ty);
	const std::vector<std::pair<int, int>> open_adjacent_tiles(const std::pair<int, int> start_t_pos, const std::pair<int, int> dest_t_pos);
	const std::string tile_node_map_key(const int tx, const int ty);
	const float calculate_edge_cost(const std::pair<int, int> start_pos, const std::pair<int, int> dest_pos);
	std::set<std::pair<int, int>> blocked_tiles;
protected:
	virtual void calculate_shortest_path();
public:
	TileDjikstraPath();
	TileDjikstraPath(Level * level, AIBeing * being);
	TileDjikstraPath(Level * level, AIBeing * being, const bool ignore_moving_obstacles);
	~TileDjikstraPath();
	void mark_blocked_tiles(Level * level, AIBeing * being);
	const std::pair<std::vector<std::pair<int, int>>, float> calculate_shortest_tile_path(
		const std::pair<int, int> origin_t_pos, const std::pair<int, int> dest_t_pos);
	const std::pair<std::vector<std::pair<int, int>>, float> calculate_shortest_tile_path(
		const std::pair<int, int> origin_t_pos,
		const std::pair<int, int> start_t_pos,
		const std::pair<int, int> dest_t_pos,
		std::map<std::string, std::map<std::string, float>>& distances,
		const float dist_tally);
};

class PathNodeDjikstraPath : DjikstraPath {
private:
	void initialize_nodes(World * world, NPC * npc, Level * level, PathNode * dest_node);
	void initialize_nodes(World * world, NPC * npc);
	void initialize_node(World * world, NPC * npc, Level * level, PathNode * node);
	DjikstraNode * get_djikstra_path_node(const std::string level_key, const std::string node_key);
	const std::string path_node_map_key(const std::string level_key, const std::string node_key);
	std::pair<std::vector<DjikstraNode*>, float> calculate_shortest_node_path(
		const std::string origin_node_id, const std::string origin_node_level_key,
		const std::string start_node_id, const std::string start_node_level_key,
		const std::string dest_node_id, const std::string dest_node_level_key,
		std::map<std::string, std::map<std::string, float>>& distances, 
		const float dist_tally);
public :
	PathNodeDjikstraPath();
	PathNodeDjikstraPath(World * world, NPC * npc, Level * level, PathNode * dest_node);
	PathNodeDjikstraPath(World * world, NPC * npc);
	~PathNodeDjikstraPath();
	std::pair<std::vector<DjikstraNode*>, float> calculate_shortest_node_path(
		 const std::string origin_node_id, const std::string origin_node_level_key, const std::string dest_node_id, const std::string dest_node_level_key);
	// async functions
	static void *func_calculate_npc_pathing(ALLEGRO_THREAD *thr, void *arg);
};

#endif
