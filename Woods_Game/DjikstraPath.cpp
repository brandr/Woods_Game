#include "DjikstraPath.h"
#include "Level.h"
#include "World.h"
#include "AIBeing.h"
#include "NPC.h"

void DjikstraPath::calculate_shortest_path()
{
	//TODO
}

DjikstraPath::DjikstraPath()
{

}

DjikstraPath::~DjikstraPath()
{
	for (auto const &it : this->node_map) {
		delete it.second;
	}
	this->node_map.clear();
}

void TileDjikstraPath::initialize_nodes(Level * level, AIBeing * being)
{
	this->initialize_nodes(level, being, false);
}

void TileDjikstraPath::initialize_nodes(Level * level, AIBeing * being, const bool ignore_moving_obstacles)
{
	const int t_width = level->get_width() / TILE_SIZE;
	const int t_height = level->get_height() / TILE_SIZE;
	// create nodes
	for (int ty = 0; ty < t_height; ty++) {
		for (int tx = 0; tx < t_width; tx++) {
			DjikstraNode * node = NULL;
			const bool blocked = being->pathing_blocked_at(tx * TILE_SIZE, ty * TILE_SIZE, level, ignore_moving_obstacles);
			if (!blocked) {
				node = new DjikstraNode();
				node->level_key = level->get_filename();
				node->position = std::pair<int, int>(tx*TILE_SIZE, ty*TILE_SIZE);
			}
			const std::string node_key = this->tile_node_map_key(tx, ty);
			this->node_map[node_key] = node;
		}
	}
	// initialize adjacent paths
	for (int ty = 0; ty < t_height; ty++) {
		for (int tx = 0; tx < t_width; tx++) {
			DjikstraNode * node = this->get_tile_node(tx, ty);
			if (node != NULL) {
				const std::pair<int, int> up(0, -1), down(0, 1), left(-1, 0), right(1, 0);
				const std::vector<std::pair<int, int>> directions{ up, down, left, right };
				for (std::pair<int, int> dir : directions) {
					DjikstraNode * adj_node = this->get_tile_node(tx + dir.first, ty + dir.second);
					if (adj_node != NULL) {
						node->neighbors.push_back(std::pair<DjikstraNode*, int>(adj_node, 1));
					}
				}
			}
		}
	}
}

DjikstraNode * TileDjikstraPath::get_tile_node(const int tx, const int ty)
{
	const std::string node_key = this->tile_node_map_key(tx, ty);
	auto it = this->node_map.find(node_key);
	if (it == this->node_map.end()) {
		return NULL;
	}
	return it->second;
}

const std::vector<std::pair<int, int>> TileDjikstraPath::open_adjacent_tiles(const std::pair<int, int> start_t_pos, const std::pair<int, int> dest_t_pos)
{
	const int x_dist = dest_t_pos.first - start_t_pos.first;
	const int y_dist = dest_t_pos.second - start_t_pos.second;
	const int x_dir = x_dist != 0 ? (x_dist / std::abs(x_dist)) : 0;
	const int y_dir = y_dist != 0 ? (y_dist / std::abs(y_dist)) : 0;

	std::vector<std::pair<int, int>> open_tiles;
	// we sort the tiles from closest to furthest from destination
	const int x1 = start_t_pos.first, y1 = start_t_pos.second;
	for (int i = 0; i < 4; i++) {
		int c_x_dir = x_dir, c_y_dir = y_dir;
		// 0 -> 0, 1 -> -1, 2 -> 1, 3 -> 0
		if (c_x_dir == 0) {
			switch (i) {
			case 1:
				c_x_dir = -1;
				if (c_y_dir != 0) {
					c_y_dir = 0;
				}
				break;
			case 2:
				c_x_dir = 1;
				if (c_y_dir != 0) {
					c_y_dir = 0;
				}
				break;
			case 3:
				c_y_dir *= -1;
			default:
				break;
			}
		}
		else if (c_y_dir == 0) {
			switch (i) {
			case 1:
				c_y_dir = -1;
				if (c_x_dir != 0) {
					c_x_dir = 0;
				}
				break;
			case 2:
				c_y_dir = 1;
				if (c_x_dir != 0) {
					c_x_dir = 0;
				}
				break;
			case 3:
				c_x_dir *= -1;
			default:
				break;
			}
		}
		// don't travel diagonally
		if (c_x_dir != 0 && c_y_dir != 0) {
			if (i % 2 == 0) {
				c_x_dir = 0;
			}
			else {
				c_y_dir = 0;
			}
		}
		DjikstraNode * adj_node = this->get_tile_node(start_t_pos.first + c_x_dir, start_t_pos.second + c_y_dir);
		if (adj_node != NULL) {
			const std::pair<int, int> pos(x1 + c_x_dir, y1 + c_y_dir);
			open_tiles.push_back(pos);
		}
	}
	return open_tiles;
}

const std::string TileDjikstraPath::tile_node_map_key(const int tx, const int ty)
{
	const std::string node_key_prefix = TILE_NODE_PREFIX;
	return node_key_prefix + ":" + std::to_string(tx) + "," + std::to_string(ty);
}

void TileDjikstraPath::calculate_shortest_path()
{
}

TileDjikstraPath::TileDjikstraPath()
{
}

TileDjikstraPath::TileDjikstraPath(Level * level, AIBeing * being)
{
	this->initialize_nodes(level, being);
}

TileDjikstraPath::TileDjikstraPath(Level * level, AIBeing * being, const bool ignore_moving_obstacles)
{
	this->initialize_nodes(level, being, ignore_moving_obstacles);
}

TileDjikstraPath::~TileDjikstraPath()
{
	for (auto const &it : this->node_map) {
		delete it.second;
	}
	this->node_map.clear();
}

const std::pair<std::vector<std::pair<int, int>>, int> TileDjikstraPath::calculate_shortest_tile_path(
	const std::pair<int, int> origin_t_pos, const std::pair<int, int> dest_t_pos)
{
	
	return this->calculate_shortest_tile_path(
		origin_t_pos,
		origin_t_pos,
		dest_t_pos,
		std::map<
		std::string,
		std::map<
		std::string,
		int>>(),
		std::vector<std::pair<int, int>>(),
		0);
}

//TODO: refactor as much as possible to a general DjikstraPath method (node neighbors should work the same way, just need to check costs)
//TODO: replace "1" distances with costs (might be different per occurence)
//TODO: consider making neighbor tile pointers also hold mapped multi-node paths w/ costs
//TODO: consider making unique keys for all node types (for tile nodes, level name + coords)
const std::pair<std::vector<std::pair<int, int>>, int> TileDjikstraPath::calculate_shortest_tile_path(
	const std::pair<int, int> origin_t_pos,
	const std::pair<int, int> start_t_pos,
	const std::pair<int, int> dest_t_pos,
	std::map<std::string, std::map<std::string, int>>& distances,
	std::vector<std::pair<int, int>> ongoing_path, const int dist_tally)
{
	//TODO: refactor to take advantage of the node map we already created
	const std::string origin_t_pos_key = this->tile_node_map_key(origin_t_pos.first, origin_t_pos.second);
	const std::string start_t_pos_key = this->tile_node_map_key(start_t_pos.first, start_t_pos.second);
	const std::string dest_t_pos_key = this->tile_node_map_key(dest_t_pos.first, dest_t_pos.second);
	const int cost = 1; //TODO: might need to generalize this more based on nodes
	std::vector<std::pair<int, int>> shortest_path;
	DjikstraNode * start_node = this->get_tile_node(start_t_pos.first, start_t_pos.second);
	DjikstraNode * dest_node = this->get_tile_node(dest_t_pos.first, dest_t_pos.second);
	if (start_node == NULL || dest_node == NULL) {
		return std::pair<std::vector<std::pair<int, int>>, int>(shortest_path, -1);
	}

	//TODO: might want adjacent Djikstra nodes instead to be more general, but also need to make sure we sort properly
	std::vector<std::pair<int, int>> open_adjacent_tiles
		= this->open_adjacent_tiles(
			start_t_pos, dest_t_pos);
	if (open_adjacent_tiles.empty()) {
		return std::pair<std::vector<std::pair<int, int>>, int>(shortest_path, -1);
	}
	ongoing_path.push_back(start_t_pos);
	shortest_path.push_back(start_t_pos);
	auto it_start = distances.find(start_t_pos_key);
	if (it_start == distances.end()) {
		distances[start_t_pos_key] = std::map<std::string, int>();
		distances[start_t_pos_key][start_t_pos_key] = 0;
	}
	std::map<std::string, int> origin_dists = distances[origin_t_pos_key];
	std::map<std::string, int> start_dists = distances[start_t_pos_key];
	int shortest_dist = -1;
	for (std::pair<int, int> adj_t : open_adjacent_tiles) {
		const std::string adj_t_key = this->tile_node_map_key(adj_t.first, adj_t.second);
		auto it_adj = distances.find(adj_t_key);
		if (it_adj == distances.end()) {
			distances[adj_t_key] = std::map<std::string, int>();
			distances[adj_t_key][adj_t_key] = 0;
		}
		if (adj_t == dest_t_pos) {
			distances[adj_t_key][dest_t_pos_key] = 0;
			distances[start_t_pos_key][dest_t_pos_key] = cost;
			shortest_path.push_back(std::pair<int, int>(dest_t_pos.first, dest_t_pos.second));
			// the destination is adjacent, so just go there
			return std::pair<std::vector<std::pair<int, int>>, int>(shortest_path, cost);
		}

		// mark the adjacent tile as 1 away
		distances[adj_t_key][start_t_pos_key] = cost;
		distances[start_t_pos_key][adj_t_key] = cost;
		std::map<std::string, int> adj_dists = distances[adj_t_key];
		auto it_start_adj = start_dists.find(adj_t_key);
		auto it_origin_adj = origin_dists.find(adj_t_key);
		bool found_new_shortest = it_origin_adj == origin_dists.end();
		if (!found_new_shortest) {
			const int old_shortest = distances[origin_t_pos_key][adj_t_key];
			found_new_shortest = old_shortest < 0 || dist_tally + cost < old_shortest;
		}
		// we found a new shortest path to the adjacent tile, so add it to the map
		if (found_new_shortest) {
			distances[origin_t_pos_key][adj_t_key] = dist_tally + cost;
			std::vector<std::pair<int, int>> ongoing_adjacent_path = ongoing_path;
			// the vector represents the path starting from the beginning and the int represents the distance starting from this tile
			std::pair<std::vector<std::pair<int, int>>, int> adj_t_path = this->calculate_shortest_tile_path(
				origin_t_pos,
				adj_t,
				dest_t_pos,
				distances,
				ongoing_path,
				dist_tally + 1); // +1 to include the traversal to the adjacent tile
								 // an empty path means it's impossible
			if (!adj_t_path.first.empty() && adj_t_path.second >= 0) {
				const int adj_dist = adj_t_path.second;
				distances[adj_t_key][dest_t_pos_key] = adj_dist;
				const int new_shortest_full = dist_tally + adj_dist;
				auto it_origin_dest = distances[origin_t_pos_key].find(dest_t_pos_key);
				bool found_new_shortest_full = shortest_dist < 0 || it_origin_dest == distances[origin_t_pos_key].end();
				if (!found_new_shortest_full) {
					const int old_shortest_full = it_origin_dest->second;
					found_new_shortest_full = new_shortest_full > 0 && (old_shortest_full < 0
						|| (adj_dist >= 0 && new_shortest_full < old_shortest_full));
				}
				// if we have a possible path shorter than those starting from the other adjacent tiles, that's our new shortest path
				if (found_new_shortest_full) {
					distances[origin_t_pos_key][dest_t_pos_key] = new_shortest_full;
					// erase the previous shortest path and replace it with the new path
					shortest_path.clear();
					shortest_path.push_back(start_t_pos);
					shortest_path.insert(shortest_path.end(), adj_t_path.first.begin(), adj_t_path.first.end());
					shortest_dist = adj_dist;
				}
			}
		}
	}
	if (shortest_dist > 0 && !shortest_path.empty()) {
		// we add 1 to the shortest dist to denote that we have to travel to the adjacent tile to reach it
		return std::pair<std::vector<std::pair<int, int>>, int>(shortest_path, shortest_dist + cost);
	}
	return std::pair<std::vector<std::pair<int, int>>, int>(shortest_path, -1);
}

DjikstraNode::DjikstraNode()
{
}

DjikstraNode::~DjikstraNode()
{
	this->neighbors.clear();
}

void DjikstraNode::add_neigbor(DjikstraNode * neighbor, const int cost)
{
	this->neighbors.push_back(std::pair<DjikstraNode *, int>(neighbor, cost));
}

const int DjikstraNode::get_neighbor_cost(DjikstraNode * neighbor)
{
	for (std::pair<DjikstraNode *, int> n : this->neighbors) {
		if (neighbor->node_key == n.first->node_key) {
			return n.second;
		}
	}
	return -1;
}

void PathNodeDjikstraPath::initialize_nodes(World * world, NPC * npc, Level * dest_level, PathNode * dest_node)
{
	if (world != NULL && npc != NULL && dest_level && dest_node != NULL) {
		this->initialize_node(world, npc, dest_level, dest_node);
	}
}

void PathNodeDjikstraPath::initialize_node(World * world, NPC * npc, Level * level, PathNode * start_node)
{
	TileDjikstraPath tile_path(level, npc, true);
	const std::pair<int, int> origin_t_pos(start_node->get_x() / TILE_SIZE, start_node->get_y() / TILE_SIZE);
	DjikstraNode * djikstra_node = new DjikstraNode();
	djikstra_node->position = std::pair<int, int>(start_node->get_x(), start_node->get_y());
	djikstra_node->level_key = level->get_filename();
	djikstra_node->node_key = start_node->get_node_id();
	const std::string node_key = this->path_node_map_key(level, start_node);
	this->node_map[node_key] = djikstra_node;
	std::vector<NeighborNode *> neighbor_nodes = start_node->get_neighbor_nodes();
	for (NeighborNode * neighbor : neighbor_nodes) {
		PathNode * next_node = level->find_path_node_with_key(neighbor->node_id.value());
		if (next_node != NULL) {
			const std::pair<int, int> dest_t_pos(next_node->get_x() / TILE_SIZE, next_node->get_y() / TILE_SIZE);
			const std::pair<std::vector<std::pair<int, int>>, int> shortest_path
				= tile_path.calculate_shortest_tile_path(origin_t_pos, dest_t_pos);
			const int cost = shortest_path.second;
			// the next node only gets mapped as a neighbor if if it's reachable from this node
			if (cost >= 0) {
				const std::string next_node_key = this->path_node_map_key(level, next_node);
				auto it = this->node_map.find(next_node_key);
				if (it == this->node_map.end()) {
					this->initialize_node(world, npc, level, next_node);
				}
				DjikstraNode * next_djikstra_node = this->node_map[next_node_key];
				djikstra_node->add_neigbor(next_djikstra_node, cost);
			}
		}
	}
	// initialize nodes that path between levels
	std::vector<NextLevelNode *> next_level_nodes = start_node->get_next_level_nodes();
	for (NextLevelNode * next_level_node : next_level_nodes) {
		Level * next_level = world->get_level_with_key(next_level_node->level_id.value());
		if (next_level != NULL) {
			PathNode * next_path_node = next_level->find_path_node_with_key(next_level_node->node_id.value());
			if (next_path_node != NULL) {
				const std::string next_node_key = this->path_node_map_key(next_level, next_path_node);
				auto it = this->node_map.find(next_node_key);
				if (it == this->node_map.end()) {
					this->initialize_node(world, npc, next_level, next_path_node);
				}
				DjikstraNode * next_djikstra_node = this->node_map[next_node_key];
				djikstra_node->add_neigbor(next_djikstra_node, 1);
			}
		}
	}
}

DjikstraNode * PathNodeDjikstraPath::get_djikstra_path_node(Level * level, PathNode * node)
{
	const std::string node_key = this->path_node_map_key(level, node);
	auto it = this->node_map.find(node_key);
	if (it == this->node_map.end()) {
		return NULL;
	}
	return it->second;
}

const std::string PathNodeDjikstraPath::path_node_map_key(Level * level, PathNode * node)
{
	const std::string prefix = PATH_NODE_PREFIX;
	return prefix + ":" + level->get_filename() + ":" + node->get_node_id();
}

std::pair<std::vector<DjikstraNode*>, int> PathNodeDjikstraPath::calculate_shortest_node_path(
	World * world,
	PathNode * origin_node, const std::string origin_node_level_key,
	PathNode * start_node, const std::string start_node_level_key,
	PathNode * dest_node, const std::string dest_node_level_key,
	std::map<std::string, std::map<std::string, int>>& distances, std::vector<PathNode*> ongoing_path, const int dist_tally)
{
	Level * origin_level = world->get_level_with_key(origin_node_level_key);
	Level * start_level = world->get_level_with_key(start_node_level_key);
	Level * dest_level = world->get_level_with_key(dest_node_level_key);
	const std::string origin_node_key = this->path_node_map_key(origin_level, origin_node);
	const std::string start_node_key = this->path_node_map_key(start_level, start_node);
	const std::string dest_node_key = this->path_node_map_key(dest_level, dest_node);

	std::vector<DjikstraNode *> shortest_path;
	DjikstraNode * start_d_node = this->get_djikstra_path_node(start_level, start_node);
	DjikstraNode * dest_d_node = this->get_djikstra_path_node(dest_level, dest_node);
	if (start_d_node == NULL || dest_d_node == NULL) {
		return std::pair<std::vector<DjikstraNode*>, int>(shortest_path, -1);
	}

	std::vector<std::pair<DjikstraNode *, int>> adjacent_d_nodes = start_d_node->neighbors;
	if (adjacent_d_nodes.empty()) {
		return std::pair<std::vector<DjikstraNode*>, int>(shortest_path, -1);
	}
	ongoing_path.push_back(start_node);
	shortest_path.push_back(start_d_node);
	
	auto it_start = distances.find(start_node_key);
	if (it_start == distances.end()) {
		distances[start_node_key] = std::map<std::string, int>();
		distances[start_node_key][start_node_key] = 0;
	}

	if (start_node->get_node_id() == dest_node->get_node_id()) {
		distances[start_node_key][dest_node_key] = 0;
		shortest_path.push_back(dest_d_node);
		// we're at the destination, so just stay there, so just go there
		return std::pair<std::vector<DjikstraNode*>, int>(shortest_path, 0);
	}
	std::map<std::string, int> origin_dists = distances[origin_node_key];
	std::map<std::string, int> start_dists = distances[start_node_key];
	int shortest_dist = -1;
	int shortest_adj_cost = -1;
	for (std::pair<DjikstraNode *, int> adj_d_neighbor : adjacent_d_nodes) {
		const int start_to_adj_cost = start_d_node->get_neighbor_cost(adj_d_neighbor.first);
		const std::string adj_level_key = adj_d_neighbor.first->level_key;
		Level * adj_node_level = world->get_level_with_key(adj_level_key);
		PathNode * adj_neighbor = adj_node_level->find_path_node_with_key(adj_d_neighbor.first->node_key);
		const std::string adj_node_key = this->path_node_map_key(adj_node_level, adj_neighbor);
		auto it_adj = distances.find(adj_node_key);
		if (it_adj == distances.end()) {
			distances[adj_node_key] = std::map<std::string, int>();
			distances[adj_node_key][adj_node_key] = 0;
		}
		if (adj_neighbor->get_node_id() == dest_node->get_node_id()) {
			
			distances[adj_node_key][dest_node_key] = 0;
			distances[start_node_key][dest_node_key] = start_to_adj_cost;
			shortest_path.push_back(dest_d_node);
			// the destination is adjacent, so just go there
			return std::pair<std::vector<DjikstraNode*>, int>(shortest_path, start_to_adj_cost);
		}

		// mark the adjacent tile as 1 away
		distances[adj_node_key][start_node_key] = start_to_adj_cost;
		distances[start_node_key][adj_node_key] = start_to_adj_cost;
		std::map<std::string, int> adj_dists = distances[adj_node_key];
		auto it_start_adj = start_dists.find(adj_node_key);
		auto it_origin_adj = origin_dists.find(adj_node_key);
		bool found_new_shortest = it_origin_adj == origin_dists.end();
		if (!found_new_shortest) {
			const int old_shortest = distances[origin_node_key][adj_node_key];
			found_new_shortest = old_shortest < 0 || dist_tally + start_to_adj_cost < old_shortest;
		}
		// we found a new shortest path to the adjacent tile, so add it to the map
		if (found_new_shortest) {
			distances[origin_node_key][adj_node_key] = dist_tally + start_to_adj_cost;
			std::vector<PathNode*> ongoing_adjacent_path = ongoing_path;
			// the vector represents the path starting from the beginning and the int represents the distance starting from this node
			std::pair<std::vector<DjikstraNode*>, int> adj_t_path = this->calculate_shortest_node_path(
				world,
				origin_node, origin_node_level_key,
				adj_neighbor, adj_level_key,
				dest_node, dest_node_level_key,
				distances,
				ongoing_path,
				dist_tally + start_to_adj_cost); 
								 // an empty path means it's impossible
			if (!adj_t_path.first.empty() && adj_t_path.second >= 0) {
				const int adj_dist = adj_t_path.second;
				distances[adj_node_key][dest_node_key] = adj_dist;
				const int new_shortest_full = dist_tally + adj_dist;
				auto it_origin_dest = distances[origin_node_key].find(dest_node_key);
				bool found_new_shortest_full = shortest_dist < 0 || it_origin_dest == distances[origin_node_key].end();
				if (!found_new_shortest_full) {
					const int old_shortest_full = it_origin_dest->second;
					found_new_shortest_full = new_shortest_full > 0 && (old_shortest_full < 0
						|| (adj_dist >= 0 && new_shortest_full < old_shortest_full));
				}
				// if we have a possible path shorter than those starting from the other adjacent tiles, that's our new shortest path
				if (found_new_shortest_full) {
					distances[origin_node_key][dest_node_key] = new_shortest_full;
					// erase the previous shortest path and replace it with the new path
					shortest_path.clear();
					shortest_path.push_back(start_d_node);
					shortest_path.insert(shortest_path.end(), adj_t_path.first.begin(), adj_t_path.first.end());
					shortest_dist = adj_dist;
					shortest_adj_cost = start_to_adj_cost;
				}
			}
		}
	}
	if (shortest_dist > 0 && !shortest_path.empty()) {
		// we add the adj cost to the shortest dist to denote that we have to travel to the adjacent node to reach it
		return std::pair<std::vector<DjikstraNode*>, int>(shortest_path, shortest_dist + shortest_adj_cost);
	}
	return std::pair<std::vector<DjikstraNode*>, int>(shortest_path, -1);
}

PathNodeDjikstraPath::PathNodeDjikstraPath()
{
}

PathNodeDjikstraPath::PathNodeDjikstraPath(World * world, NPC * npc, Level * dest_level, PathNode * dest_node)
{
	this->initialize_nodes(world, npc, dest_level, dest_node);
}

PathNodeDjikstraPath::~PathNodeDjikstraPath()
{
	for (auto const &it : this->node_map) {
		delete it.second;
	}
	this->node_map.clear();
}

std::pair<std::vector<DjikstraNode*>, int> PathNodeDjikstraPath::calculate_shortest_node_path(
	World * world, PathNode * origin_node, const std::string origin_node_level_key, PathNode * dest_node, const std::string dest_node_level_key)
{
	return this->calculate_shortest_node_path(
		world,
		origin_node, origin_node_level_key,
		origin_node, origin_node_level_key,
		dest_node, dest_node_level_key,
		std::map<std::string, std::map<std::string, int>>(), 
		std::vector<PathNode*>(), 0);
}
