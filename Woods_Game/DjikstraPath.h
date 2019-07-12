#ifndef DJIKSTRA_PATH_H
#define DJIKSTRA_PATH_H

#define TILE_NODE_PREFIX "tile_node"
#define PATH_NODE_PREFIX "path_node"

#include <vector>
#include <map>
#include "PathNode.h"

class Level;
class NPC;
class World;
class AIBeing;

struct DjikstraNode {
	std::string level_key = "";
	std::string node_key = "";
	std::pair<int, int> position;
	// neighbors with costs
	std::vector<std::pair<DjikstraNode *, int>> neighbors;
	DjikstraNode();
	~DjikstraNode();
	void add_neigbor(DjikstraNode * neighbor, const int cost);
	const int get_neighbor_cost(DjikstraNode * neighbor);
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
protected:
	virtual void calculate_shortest_path();
public:
	TileDjikstraPath();
	TileDjikstraPath(Level * level, AIBeing * being);
	TileDjikstraPath(Level * level, AIBeing * being, const bool ignore_moving_obstacles);
	~TileDjikstraPath();
	const std::pair<std::vector<std::pair<int, int>>, int> calculate_shortest_tile_path(
		const std::pair<int, int> origin_t_pos, const std::pair<int, int> dest_t_pos);
	const std::pair<std::vector<std::pair<int, int>>, int> calculate_shortest_tile_path(
		const std::pair<int, int> origin_t_pos,
		const std::pair<int, int> start_t_pos,
		const std::pair<int, int> dest_t_pos,
		std::map<std::string, std::map<std::string, int>>& distances,
		std::vector<std::pair<int, int>> ongoing_path, 
		const int dist_tally);
};

class PathNodeDjikstraPath : DjikstraPath {
private:
	void initialize_nodes(World * world, NPC * npc, Level * level, PathNode * dest_node);
	void initialize_node(World * world, NPC * npc, Level * level, PathNode * node);
	DjikstraNode * get_djikstra_path_node(Level * level, PathNode * node);
	const std::string path_node_map_key(Level * level, PathNode * node);
	std::pair<std::vector<DjikstraNode*>, int> calculate_shortest_node_path(
		World * world,
		PathNode * origin_node, const std::string origin_node_level_key,
		PathNode * start_node, const std::string start_node_level_key,
		PathNode * dest_node, const std::string dest_node_level_key,
		std::map<std::string, std::map<std::string, int>>& distances, std::vector<PathNode*> ongoing_path, const int dist_tally);
public :
	PathNodeDjikstraPath();
	PathNodeDjikstraPath(World * world, NPC * npc, Level * level, PathNode * dest_node);
	~PathNodeDjikstraPath();
	std::pair<std::vector<DjikstraNode*>, int> calculate_shortest_node_path(
		World * world, PathNode * origin_node, const std::string origin_node_level_key, PathNode * dest_node, const std::string dest_node_level_key);
};

#endif
