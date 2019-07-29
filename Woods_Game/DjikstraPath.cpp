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

// TODO: the performance here is one of the main problems with NPC pathing.
// probably want to initailize and cache with non-moving obstacles first
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
				Tile * t = level->get_tile(tx, ty);
				TileSet * tileset = level->get_tileset();
				const float speed_mod = tileset->get_tile_speed_mod(t->get_tile_type_index());
				node->node_cost = 1.0/speed_mod;
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
						const float adj_node_cost = this->calculate_edge_cost(std::pair<int, int>(tx, ty), std::pair<int, int>(tx + dir.first, ty + dir.second));
						node->neighbors.push_back(std::pair<DjikstraNode*, float>(adj_node, adj_node_cost));
					}
				}
			}
		}
	}
}

DjikstraNode * TileDjikstraPath::get_tile_node(const int tx, const int ty)
{
	auto it1 = this->blocked_tiles.find(std::pair<int, int>(tx, ty));
	if (it1 != this->blocked_tiles.end()) {
		return NULL;
	}

	const std::string node_key = this->tile_node_map_key(tx, ty);
	auto it2 = this->node_map.find(node_key);
	if (it2 == this->node_map.end()) {
		return NULL;
	}
	return it2->second;
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

const float TileDjikstraPath::calculate_edge_cost(const std::pair<int, int> start_pos, const std::pair<int, int> dest_pos)
{
	// this method assumes the tiles are adjacent.
	DjikstraNode * start_node = this->get_tile_node(start_pos.first, start_pos.second);
	DjikstraNode * dest_node = this->get_tile_node(dest_pos.first, dest_pos.second);
	return (start_node->node_cost + dest_node->node_cost) / 2.0;
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

void TileDjikstraPath::mark_blocked_tiles(Level * level, AIBeing * being)
{
	// mark tiles blocked by moving obstacles since we only cache stationary obstacles
	this->blocked_tiles.clear();
	std::vector<Entity*> moving_entities = level->get_moving_interactables(being);
	for (Entity * e : moving_entities) {
		if (e != NULL && e != being && e->is_solid()) {
			const float x1 = e->get_x(), y1 = e->get_y();
			const float width = e->get_width(), height = e->get_height();

			const int tx1 = ((int) x1) / TILE_SIZE, ty1 = ((int) y1) / TILE_SIZE;
			const int tw = ((int) width) / TILE_SIZE + 1, th = ((int) height) / TILE_SIZE + 1;

			for (int y = ty1; y < ty1 + th; y++) {
				for (int x = tx1; x < tx1 + tw; x++) {
					this->blocked_tiles.insert(std::pair<int, int>(x, y));
				}
			}
		}
	}
}

const std::pair<std::vector<std::pair<int, int>>, float> TileDjikstraPath::calculate_shortest_tile_path(
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
		float>>(),
		0);
}

//TODO: improve performance of this method
//TODO: refactor to take better advantage of the node map we already created
			// (distances probably doesn't need to be empty every time)
const std::pair<std::vector<std::pair<int, int>>, float> TileDjikstraPath::calculate_shortest_tile_path(
	const std::pair<int, int> origin_t_pos,
	const std::pair<int, int> start_t_pos,
	const std::pair<int, int> dest_t_pos,
	std::map<std::string, std::map<std::string, float>>& distances,
	const float dist_tally)
{
	const std::string origin_t_pos_key = this->tile_node_map_key(origin_t_pos.first, origin_t_pos.second);
	const std::string start_t_pos_key = this->tile_node_map_key(start_t_pos.first, start_t_pos.second);
	const std::string dest_t_pos_key = this->tile_node_map_key(dest_t_pos.first, dest_t_pos.second);

	std::vector<std::pair<int, int>> shortest_path;
	DjikstraNode * start_node = this->get_tile_node(start_t_pos.first, start_t_pos.second);
	DjikstraNode * dest_node = this->get_tile_node(dest_t_pos.first, dest_t_pos.second);
	if (start_node == NULL || dest_node == NULL) {
		return std::pair<std::vector<std::pair<int, int>>, float>(shortest_path, -1);
	}

	//doesn't seem to fix performance issues so I commented it out
	//TODO: try limiting how far "around" we try to go
	// this will cause us to incorrectly say there's no path to some places, but it should improve performance
	//TODO: 5 is arbitrary and temporary, try different things and think carefully about what we want
	/*
	//...
	if (start_t_pos.first < std::min(origin_t_pos.first, dest_t_pos.first) - 5
		|| start_t_pos.second < std::min(origin_t_pos.second, dest_t_pos.second) - 5
		|| start_t_pos.first > std::max(origin_t_pos.first, dest_t_pos.first) + 5
		|| start_t_pos.second > std::max(origin_t_pos.second, dest_t_pos.second) + 5) {

		return std::pair<std::vector<std::pair<int, int>>, float>(shortest_path, -1);
	}
	//...
	*/ 

	std::vector<std::pair<int, int>> open_adjacent_tiles
		= this->open_adjacent_tiles(
			start_t_pos, dest_t_pos);
	if (open_adjacent_tiles.empty()) {
		return std::pair<std::vector<std::pair<int, int>>, float>(shortest_path, -1);
	}
	float adj_start_cost = 1.0;

	shortest_path.push_back(start_t_pos);
	auto it_start = distances.find(start_t_pos_key);
	if (it_start == distances.end()) {
		distances[start_t_pos_key] = std::map<std::string, float>();
		distances[start_t_pos_key][start_t_pos_key] = 0;
	}
	std::map<std::string, float> origin_dists = distances[origin_t_pos_key];
	std::map<std::string, float> start_dists = distances[start_t_pos_key];
	float shortest_dist = -1;
	for (std::pair<int, int> adj_t : open_adjacent_tiles) {
		const std::string adj_t_key = this->tile_node_map_key(adj_t.first, adj_t.second);
		auto it_adj = distances.find(adj_t_key);
		if (it_adj == distances.end()) {
			distances[adj_t_key] = std::map<std::string, float>();
			distances[adj_t_key][adj_t_key] = 0;
		}
		if (adj_t == dest_t_pos) {
			const float adj_dest_cost = this->calculate_edge_cost(adj_t, dest_t_pos);
			distances[adj_t_key][dest_t_pos_key] = 0;
			distances[start_t_pos_key][dest_t_pos_key] = adj_dest_cost;
			shortest_path.push_back(std::pair<int, int>(dest_t_pos.first, dest_t_pos.second));
			// the destination is adjacent, so just go there
			return std::pair<std::vector<std::pair<int, int>>, float>(shortest_path, adj_dest_cost);
		}

		// mark the adjacent tile with cost
		adj_start_cost = this->calculate_edge_cost(adj_t, start_t_pos);
		distances[adj_t_key][start_t_pos_key] = adj_start_cost;
		distances[start_t_pos_key][adj_t_key] = adj_start_cost;
		std::map<std::string, float> adj_dists = distances[adj_t_key];
		auto it_start_adj = start_dists.find(adj_t_key);
		auto it_origin_adj = origin_dists.find(adj_t_key);
		bool found_new_shortest = it_origin_adj == origin_dists.end();
		if (!found_new_shortest) {
			const float old_shortest = distances[origin_t_pos_key][adj_t_key];
			found_new_shortest = old_shortest < 0 || dist_tally + adj_start_cost < old_shortest;
		}
		// we found a new shortest path to the adjacent tile, so add it to the map
		if (found_new_shortest) {
			distances[origin_t_pos_key][adj_t_key] = dist_tally + adj_start_cost;
			// the vector represents the path starting from the beginning and the int represents the distance starting from this tile
			std::pair<std::vector<std::pair<int, int>>, float> adj_t_path = this->calculate_shortest_tile_path(
				origin_t_pos,
				adj_t,
				dest_t_pos,
				distances,
				dist_tally + adj_start_cost); // +1 to include the traversal to the adjacent tile
								 // an empty path means it's impossible
			if (!adj_t_path.first.empty() && adj_t_path.second >= 0) {
				const float adj_dist = adj_t_path.second;
				distances[adj_t_key][dest_t_pos_key] = adj_dist;
				const float new_shortest_full = dist_tally + adj_dist;
				auto it_origin_dest = distances[origin_t_pos_key].find(dest_t_pos_key);
				bool found_new_shortest_full = shortest_dist < 0 || it_origin_dest == distances[origin_t_pos_key].end();
				if (!found_new_shortest_full) {
					const float old_shortest_full = it_origin_dest->second;
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
		// we add cost to the shortest dist to denote that we have to travel to the adjacent tile to reach it
		return std::pair<std::vector<std::pair<int, int>>, float>(shortest_path, shortest_dist + adj_start_cost);
	}
	return std::pair<std::vector<std::pair<int, int>>, float>(shortest_path, -1);
}

DjikstraNode::DjikstraNode()
{
}

DjikstraNode::~DjikstraNode()
{
	this->neighbors.clear();
}

void DjikstraNode::add_neigbor(DjikstraNode * neighbor, const float cost)
{
	this->neighbors.push_back(std::pair<DjikstraNode *, float>(neighbor, cost));
}

const float DjikstraNode::get_neighbor_cost(DjikstraNode * neighbor)
{
	for (std::pair<DjikstraNode *, float> n : this->neighbors) {
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

void PathNodeDjikstraPath::initialize_nodes(World * world, NPC * npc)
{
	if (world != NULL && npc != NULL) {
		Level * node_level = world->find_any_level_with_node();
		if (node_level != NULL) {
			this->initialize_node(world, npc, node_level, node_level->find_any_path_node());
		}
	}
}

void PathNodeDjikstraPath::initialize_node(World * world, NPC * npc, Level * level, PathNode * start_node)
{
	const std::string start_node_key = start_node->get_node_id();
	TileDjikstraPath tile_path(level, npc, true);
	const std::pair<int, int> origin_t_pos(start_node->get_x() / TILE_SIZE, start_node->get_y() / TILE_SIZE);
	DjikstraNode * djikstra_node = new DjikstraNode();
	djikstra_node->position = std::pair<int, int>(start_node->get_x(), start_node->get_y());
	djikstra_node->level_key = level->get_filename();
	djikstra_node->node_key = start_node_key;
	djikstra_node->node_cost = 1.0;

	const std::string node_key = this->path_node_map_key(level->get_filename(), start_node_key);
	this->node_map[node_key] = djikstra_node;
	std::vector<NeighborNode *> neighbor_nodes = start_node->get_neighbor_nodes();
	for (NeighborNode * neighbor : neighbor_nodes) {
		PathNode * next_node = level->find_path_node_with_key(neighbor->node_id.value());
		const std::string next_node_key = next_node->get_node_id();
		if (next_node != NULL) {
			const std::pair<int, int> dest_t_pos(next_node->get_x() / TILE_SIZE, next_node->get_y() / TILE_SIZE);
			const std::pair<std::vector<std::pair<int, int>>, float> shortest_path
				= tile_path.calculate_shortest_tile_path(origin_t_pos, dest_t_pos);
			const float cost = shortest_path.second;
			// the next node only gets mapped as a neighbor if if it's reachable from this node
			if (cost >= 0) {
				const std::string next_node_key = this->path_node_map_key(level->get_filename(), next_node->get_node_id());
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
		const std::string next_level_key = next_level_node->level_id.value();
		Level * next_level = world->get_level_with_key(next_level_key);
		
		if (next_level != NULL) {
			PathNode * next_path_node = next_level->find_path_node_with_key(next_level_node->node_id.value());
			if (next_path_node != NULL) {
				const std::string next_node_key = this->path_node_map_key(next_level_key, next_level_node->node_id.value());
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

DjikstraNode * PathNodeDjikstraPath::get_djikstra_path_node(const std::string level_key, const std::string node_key)
{
	const std::string full_key = this->path_node_map_key(level_key, node_key);
	auto it = this->node_map.find(full_key);
	if (it == this->node_map.end()) {
		return NULL;
	}
	return it->second;
}

const std::string PathNodeDjikstraPath::path_node_map_key(const std::string level_key, const std::string node_key)
{
	const std::string prefix = PATH_NODE_PREFIX;
	return prefix + ":" + level_key + ":" + node_key;
}

// this will make it easier to lock a mutex
std::pair<std::vector<DjikstraNode*>, float> PathNodeDjikstraPath::calculate_shortest_node_path(
	const std::string origin_node_id, const std::string origin_node_level_key,
	const std::string start_node_id, const std::string start_node_level_key,
	const std::string dest_node_id, const std::string dest_node_level_key,
	std::map<std::string, std::map<std::string, float>>& distances,
	const float dist_tally)
{
	const std::string origin_node_key = this->path_node_map_key(origin_node_level_key, origin_node_id);
	const std::string start_node_key = this->path_node_map_key(start_node_level_key, start_node_id);
	const std::string dest_node_key = this->path_node_map_key(dest_node_level_key, dest_node_id);

	std::vector<DjikstraNode *> shortest_path;
	DjikstraNode * start_d_node = this->get_djikstra_path_node(start_node_level_key, start_node_id);
	DjikstraNode * dest_d_node = this->get_djikstra_path_node(dest_node_level_key, dest_node_id);

	if (start_d_node == NULL || dest_d_node == NULL) {
		return std::pair<std::vector<DjikstraNode*>, float>(shortest_path, -1);
	}

	std::vector<std::pair<DjikstraNode *, float>> adjacent_d_nodes = start_d_node->neighbors;
	if (adjacent_d_nodes.empty()) {
		return std::pair<std::vector<DjikstraNode*>, float>(shortest_path, -1);
	}

	shortest_path.push_back(start_d_node);
	
	auto it_start = distances.find(start_node_key);
	if (it_start == distances.end()) {
		distances[start_node_key] = std::map<std::string, float>();
		distances[start_node_key][start_node_key] = 0;
	}

	if (start_node_id == dest_node_id) {
		distances[start_node_key][dest_node_key] = 0;
		shortest_path.push_back(dest_d_node);
		// we're at the destination, so just stay there, so just go there
		return std::pair<std::vector<DjikstraNode*>, float>(shortest_path, 0);
	}
	std::map<std::string, float> origin_dists = distances[origin_node_key];
	std::map<std::string, float> start_dists = distances[start_node_key];
	float shortest_dist = -1;
	float shortest_adj_cost = -1;
	for (std::pair<DjikstraNode *, float> adj_d_neighbor : adjacent_d_nodes) {
		const float start_to_adj_cost = start_d_node->get_neighbor_cost(adj_d_neighbor.first);
		const std::string adj_level_key = adj_d_neighbor.first->level_key;
		const std::string adj_node_key = this->path_node_map_key(adj_level_key, adj_d_neighbor.first->node_key);
		auto it_adj = distances.find(adj_node_key);
		if (it_adj == distances.end()) {
			distances[adj_node_key] = std::map<std::string, float>();
			distances[adj_node_key][adj_node_key] = 0;
		}
		if (adj_d_neighbor.first->node_key == dest_node_id) {
			
			distances[adj_node_key][dest_node_key] = 0;
			distances[start_node_key][dest_node_key] = start_to_adj_cost;
			shortest_path.push_back(dest_d_node);
			// the destination is adjacent, so just go there
			return std::pair<std::vector<DjikstraNode*>, float>(shortest_path, start_to_adj_cost);
		}

		// mark the adjacent tile as 1 away
		distances[adj_node_key][start_node_key] = start_to_adj_cost;
		distances[start_node_key][adj_node_key] = start_to_adj_cost;
		std::map<std::string, float> adj_dists = distances[adj_node_key];
		auto it_start_adj = start_dists.find(adj_node_key);
		auto it_origin_adj = origin_dists.find(adj_node_key);
		bool found_new_shortest = it_origin_adj == origin_dists.end();
		if (!found_new_shortest) {
			const float old_shortest = distances[origin_node_key][adj_node_key];
			found_new_shortest = old_shortest < 0 || dist_tally + start_to_adj_cost < old_shortest;
		}
		// we found a new shortest path to the adjacent tile, so add it to the map
		if (found_new_shortest) {
			distances[origin_node_key][adj_node_key] = dist_tally + start_to_adj_cost;
			// the vector represents the path starting from the beginning and the int represents the distance starting from this node
			std::pair<std::vector<DjikstraNode*>, float> adj_t_path = this->calculate_shortest_node_path(
				origin_node_id, origin_node_level_key,
				adj_d_neighbor.first->node_key, adj_level_key,
				dest_node_id, dest_node_level_key,
				distances,
				dist_tally + start_to_adj_cost); 
								 // an empty path means it's impossible
			if (!adj_t_path.first.empty() && adj_t_path.second >= 0) {
				const float adj_dist = adj_t_path.second;
				distances[adj_node_key][dest_node_key] = adj_dist;
				const float new_shortest_full = dist_tally + adj_dist;
				auto it_origin_dest = distances[origin_node_key].find(dest_node_key);
				bool found_new_shortest_full = shortest_dist < 0 || it_origin_dest == distances[origin_node_key].end();
				if (!found_new_shortest_full) {
					const float old_shortest_full = it_origin_dest->second;
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
		return std::pair<std::vector<DjikstraNode*>, float>(shortest_path, shortest_dist + shortest_adj_cost);
	}
	return std::pair<std::vector<DjikstraNode*>, float>(shortest_path, -1);
}

PathNodeDjikstraPath::PathNodeDjikstraPath()
{
}

PathNodeDjikstraPath::PathNodeDjikstraPath(World * world, NPC * npc, Level * dest_level, PathNode * dest_node)
{
	this->initialize_nodes(world, npc, dest_level, dest_node);
}

PathNodeDjikstraPath::PathNodeDjikstraPath(World * world, NPC * npc)
{
	std::cout << "START initializing pathing nodes\n";
	this->initialize_nodes(world, npc);
	std::cout << "FINISH initializing pathing nodes\n";
}

PathNodeDjikstraPath::~PathNodeDjikstraPath()
{
	for (auto const &it : this->node_map) {
		delete it.second;
	}
	this->node_map.clear();
}

std::pair<std::vector<DjikstraNode*>, float> PathNodeDjikstraPath::calculate_shortest_node_path(
	const std::string origin_node_id, const std::string origin_node_level_key, const std::string dest_node_id, const std::string dest_node_level_key)
{
	return this->calculate_shortest_node_path(
		origin_node_id, origin_node_level_key,
		origin_node_id, origin_node_level_key,
		dest_node_id, dest_node_level_key,
		std::map<std::string, std::map<std::string, float>>(), 0);
}

void * PathNodeDjikstraPath::func_calculate_npc_pathing(ALLEGRO_THREAD * thr, void * arg)
{

	double start_time = al_get_time();
	double accounted_time = 0.0;

	bool enable_logging = false; //temp

	if (enable_logging) {
		std::cout << "--------------------------------------\n";
		std::cout << "START calculating npc pathing\n\n";
	}

	DjikstraPathData *data = (DjikstraPathData *)arg;
	const std::pair<std::string, std::pair<int, int>> forced_dest = data->forced_dest;
	const std::pair<int, int> forced_pos = forced_dest.second;
	TileDjikstraPath * tile_djikstra_path = data->tile_djikstra_path;

	//TODO: lock mutex whenever we interact with PathNodes. These are the only things other threads should need to see

	if (forced_pos.first >= 0 && forced_pos.second >= 0) {
		const std::pair<std::vector<std::pair<int, int>>, float> shortest_path_to_force
			= tile_djikstra_path->calculate_shortest_tile_path(data->npc_t_pos, std::pair<int, int>(forced_pos.first / TILE_SIZE, forced_pos.second / TILE_SIZE));
		if (shortest_path_to_force.second >= 0) {
			data->should_force = true;
			data->has_found_path = true;
			data->should_unset_forced_destination = true;
			data->primary_destinations.push_back(forced_dest);
		}
	}
	if (!data->should_force) {
		// we have a tile path that lets us see nodes on the level, so see which node we should walk to.
		// find the two that are closest to the npc, and from those, path to the one that is the closest to the destination.
		float shortest_current_level_node_dist = -1;
		float second_shortest_current_level_node_dist = -1;
		PathNode * closest_node = NULL;
		PathNode * second_closest_node = NULL;

		al_lock_mutex(data->mutex);
		const int level_node_count = data->current_level_path_nodes.size();
		al_unlock_mutex(data->mutex);

		for (int i = 0; i < level_node_count; i++) {
			al_lock_mutex(data->mutex);
			const std::pair<int, int> current_level_node_pos
				(data->current_level_path_nodes[i]->get_x() / TILE_SIZE, data->current_level_path_nodes[i]->get_y() / TILE_SIZE);
			al_unlock_mutex(data->mutex);

			double start_get_tile_path_time = al_get_time();
			const std::pair<std::vector<std::pair<int, int>>, float> current_level_node_shortest_path
				= tile_djikstra_path->calculate_shortest_tile_path(data->npc_t_pos, current_level_node_pos);

			if (enable_logging) {
				double end_get_tile_path_time = al_get_time();
				double delta_t2 = end_get_tile_path_time - start_get_tile_path_time;
				std::cout << "Time to calculate shortest tile path: " << std::to_string(delta_t2) << " seconds\n\n";
				accounted_time += delta_t2;
			}

			const float next_node_dist = current_level_node_shortest_path.second;

			if (next_node_dist < 0) {
				continue;
			}
			else {
				if (shortest_current_level_node_dist < 0
					|| next_node_dist < shortest_current_level_node_dist) {

					// the closest becomes the second closest
					second_shortest_current_level_node_dist = shortest_current_level_node_dist;
					second_closest_node = closest_node;
					shortest_current_level_node_dist = next_node_dist;

					al_lock_mutex(data->mutex);
					closest_node = data->current_level_path_nodes[i];
					al_unlock_mutex(data->mutex);

				}
				else if (second_shortest_current_level_node_dist < 0
					|| next_node_dist < second_shortest_current_level_node_dist) {
					second_shortest_current_level_node_dist = next_node_dist;

					al_lock_mutex(data->mutex);
					second_closest_node = data->current_level_path_nodes[i];
					al_unlock_mutex(data->mutex);
				}
			}
		}
		
		//TODO: do i need to lock mutex for other references to closest node?

		if (shortest_current_level_node_dist < 0 || closest_node == NULL) {

		} else if (second_shortest_current_level_node_dist < 0 || second_closest_node == NULL) {
			// there is only one possible destination, so we'll path there
			const std::pair<int, int> primary_dest(closest_node->get_x(), closest_node->get_y());
			data->primary_destinations.push_back(std::pair<std::string, std::pair<int, int>>(closest_node->get_node_id(), primary_dest));
			data->has_found_path = true;
		} else {
			// This is the only case where we have to use actual PathNode pathing,
			// because there is more than one relevant PathNode on the level.

			PathNodeDjikstraPath * node_djikstra_path = data->node_djikstra_path;
			double start_calculate_node_path_time = al_get_time();

			std::pair<std::vector<DjikstraNode*>, float> shortest_path_from_closest
				= node_djikstra_path->calculate_shortest_node_path(
					closest_node->get_node_id(), data->current_level_key, data->dest_node_id, data->dest_level_key);

			std::pair<std::vector<DjikstraNode*>, float> shortest_path_from_second_closest
				= node_djikstra_path->calculate_shortest_node_path(
					second_closest_node->get_node_id(), data->current_level_key, data->dest_node_id, data->dest_level_key);

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
			if (closest_node->get_node_id() == data->dest_node_id) {

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

			} else {
				// path to every node on the path that is on the current level.
				// we do not path to the next level because we can just recalculate after reaching it.
				const int path_to_execute_size = path_to_execute.size();
				for (int i = 0; i < path_to_execute_size; i++) {
					DjikstraNode * node_to_execute = path_to_execute[i];
					const std::string level_key = node_to_execute->level_key;
					if (level_key != data->current_level_key) {
						break;
					}
					if (std::abs(data->start_npc_pos.first - node_to_execute->position.first) > 0
						|| std::abs(data->start_npc_pos.second - node_to_execute->position.second) > 0) {
						// add the node as our next destination
						data->primary_destinations.push_back
						(std::pair<std::string, std::pair<int, int>>(node_to_execute->node_key, node_to_execute->position));
						data->has_found_path = true;
					}
				}
			}
		}
	}
	if (enable_logging) {
		double end_time = al_get_time();
		double delta = end_time - start_time;
		std::cout << "FINISH calculating npc pathing. Time to calculate: " + std::to_string(delta) + "\n";
		std::cout << "Unaccounted time: " << std::to_string(delta - accounted_time) << "(" << std::to_string(100.0 * (accounted_time / delta)) << "% accounted for)\n";
		std::cout << "--------------------------------------\n";
	}

	if (data->has_found_path) {
		// this dest will be in pixel pos, not tile pos
		const std::pair<int, int> first_primary = data->primary_destinations[0].second;
		data->tile_path_to_primary
			= tile_djikstra_path->calculate_shortest_tile_path
			(data->npc_t_pos, std::pair<int, int>(first_primary.first / TILE_SIZE, first_primary.second / TILE_SIZE));
	}

	al_lock_mutex(data->mutex);
	data->ready = true;
	al_broadcast_cond(data->cond);
	al_unlock_mutex(data->mutex);

	return NULL;
}
