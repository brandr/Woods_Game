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
	this->initialize_nodes(level, being, true);
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
	//temp
	bool enable_logging = false;
	if (enable_logging) {
		std::cout << "----------------------------------------\n";
		for (int ty = 0; ty < t_height; ty++) {
			for (int tx = 0; tx < t_width; tx++) {
				DjikstraNode * t_node = this->get_tile_node(tx, ty);
				std::cout << (t_node != NULL ? "0" : "X");
			}
			std::cout << "\n";
		}
		std::cout << "----------------------------------------\n";
	}
	//temp
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

const std::vector<std::pair<int, int>> TileDjikstraPath::open_adjacent_tiles(const std::pair<int, int> start_t_pos, const std::pair<int, int> dest_t_pos, std::set<std::pair<int, int>> visited)
{
	const int x_dist = dest_t_pos.first - start_t_pos.first;
	const int y_dist = dest_t_pos.second - start_t_pos.second;
	const int x_dir = x_dist != 0 && std::abs(x_dist) > std::abs(y_dist) ? (x_dist / std::abs(x_dist)) : 0;
	const int y_dir = x_dir == 0 && y_dist != 0 ? (y_dist / std::abs(y_dist)) : 0;

	std::vector<std::pair<int, int>> open_tiles;
	// we sort the tiles from closest to furthest from destination
	const int x1 = start_t_pos.first, y1 = start_t_pos.second;
	for (int i = 0; i < 4; i++) {
		int c_x_dir = x_dir, c_y_dir = y_dir;
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
		
		if (visited.find(std::pair<int, int>(start_t_pos.first + c_x_dir, start_t_pos.second + c_y_dir)) == visited.end()) {
			DjikstraNode * adj_node = this->get_tile_node(start_t_pos.first + c_x_dir, start_t_pos.second + c_y_dir);
			if (adj_node != NULL) {
				const std::pair<int, int> pos(x1 + c_x_dir, y1 + c_y_dir);
				//if (std::find(open_tiles.begin(), open_tiles.end(), pos) == open_tiles.end()) {
				open_tiles.push_back(pos);
				//}
			}
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
	return start_node != NULL && dest_node != NULL ? (start_node->node_cost + dest_node->node_cost) / 2.0 : 1.0;
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

void TileDjikstraPath::mark_blocked_tiles(Level * level, AIBeing * being, const bool ignore_moving_obstacles)
{
	// mark tiles blocked by moving obstacles since we only cache stationary obstacles
	this->blocked_tiles.clear();
	if (ignore_moving_obstacles) {
		return;
	}
	std::vector<Entity*> moving_entities = level->get_moving_interactables(being);
	Rect * being_rect = being->get_rect_for_collision();
	const int otx = being_rect->x / TILE_SIZE, oty = being_rect->y / TILE_SIZE;
	for (Entity * e : moving_entities) {
		if (e != NULL && e != being && e->is_solid()) {
			Rect * collide_rect = e->get_rect_for_collision();
			const float x1 = collide_rect->x, y1 = collide_rect->y;
			const float width = collide_rect->width, height = collide_rect->height;

			const int tx1 = ((int) x1) / TILE_SIZE, ty1 = ((int) y1) / TILE_SIZE;
			const int tw = ((int) width) / TILE_SIZE + 1, th = ((int) height) / TILE_SIZE + 1;

			for (int y = ty1; y < ty1 + th; y++) {
				for (int x = tx1; x < tx1 + tw; x++) {
					this->blocked_tiles.insert(std::pair<int, int>(x, y));
				}
			}
		}
	}

	//TODO: may want an option to do more precise checks

	// if there is something blocking the being's current square, don't try to path
	const bool block_all = being->pathing_blocked_by_moving_at(being_rect->x, being_rect->y, level);

	if (block_all || being->pathing_blocked_by_moving_at(being_rect->x, being_rect->y - TILE_SIZE, level)) {
		this->blocked_tiles.insert(std::pair<int, int>(otx, oty - 1 ));
	}
	if (block_all || being->pathing_blocked_by_moving_at(being_rect->x, being_rect->y + TILE_SIZE, level)) {
		this->blocked_tiles.insert(std::pair<int, int>(otx, oty + 1));
	}
	if (block_all || being->pathing_blocked_by_moving_at(being_rect->x - TILE_SIZE, being_rect->y, level)) {
		this->blocked_tiles.insert(std::pair<int, int>(otx - 1, oty));
	}
	if (block_all || being->pathing_blocked_by_moving_at(being_rect->x + TILE_SIZE, being_rect->y, level)) {
		this->blocked_tiles.insert(std::pair<int, int>(otx + 1, oty));
	}
}

const bool TileDjikstraPath::tile_is_blocked(const int tx, const int ty)
{
	return this->blocked_tiles.find(std::pair<int, int>(tx, ty)) != this->blocked_tiles.end();
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
		0,
		std::set<std::pair<int, int>>());
}

//TODO: improve performance of this method
//TODO: refactor to take better advantage of the node map we already created
			// (distances probably doesn't need to be empty every time)
const std::pair<std::vector<std::pair<int, int>>, float> TileDjikstraPath::calculate_shortest_tile_path(
	const std::pair<int, int> origin_t_pos,
	const std::pair<int, int> start_t_pos,
	const std::pair<int, int> dest_t_pos,
	std::map<std::string, std::map<std::string, float>>& distances,
	const float dist_tally,
	std::set<std::pair<int, int>> visited)
{
	const std::string origin_t_pos_key = this->tile_node_map_key(origin_t_pos.first, origin_t_pos.second);
	const std::string start_t_pos_key = this->tile_node_map_key(start_t_pos.first, start_t_pos.second);
	const std::string dest_t_pos_key = this->tile_node_map_key(dest_t_pos.first, dest_t_pos.second);

	std::vector<std::pair<int, int>> shortest_path;
	visited.insert(start_t_pos);
	DjikstraNode * start_node = this->get_tile_node(start_t_pos.first, start_t_pos.second);
	DjikstraNode * dest_node = this->get_tile_node(dest_t_pos.first, dest_t_pos.second);
	if (start_node == NULL || dest_node == NULL) {
		return std::pair<std::vector<std::pair<int, int>>, float>(shortest_path, -1);
	}

	std::vector<std::pair<int, int>> open_adjacent_tiles
		= this->open_adjacent_tiles(
			start_t_pos, dest_t_pos, visited);
	if (open_adjacent_tiles.empty()) {
		return std::pair<std::vector<std::pair<int, int>>, float>(shortest_path, -1);
	}
	float adj_start_cost = 1.0;

	shortest_path.push_back(start_t_pos);
	if (start_t_pos == dest_t_pos) {
		return std::pair<std::vector<std::pair<int, int>>, float>(shortest_path, 0);
	}
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
			//distances[origin_t_pos_key][dest_t_pos_key] = dist_tally + adj_dest_cost;
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

		auto it_origin_dest_1 = distances[origin_t_pos_key].find(dest_t_pos_key);
		bool has_full_shortest = false;
		if (it_origin_dest_1 != distances[origin_t_pos_key].end()) {
			has_full_shortest = dist_tally + adj_start_cost >= it_origin_dest_1->second;
		}

		// this path is already too long, so just return here
		if (has_full_shortest) {
			continue;
		}

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
				dist_tally + adj_start_cost,
				visited);
								 // an empty path means it's impossible
			if (!adj_t_path.first.empty() && adj_t_path.second >= 0) {
				const float adj_dist = adj_t_path.second;
				distances[adj_t_key][dest_t_pos_key] = adj_dist;
				const float new_shortest_full = dist_tally + adj_dist;
				auto it_origin_dest = distances[origin_t_pos_key].find(dest_t_pos_key);
				bool found_new_shortest_full = it_origin_dest == distances[origin_t_pos_key].end();
				// shortest_dist < 0 || it_origin_dest == distances[origin_t_pos_key].end();
				//TODO: doing new_shortest_full <= old_shortest_full might be causing slowdown, but changing it to <
				//		means we never path because the shortest dist that we actually mean to calculate gets stored in distances
				if (!found_new_shortest_full) {
					const float old_shortest_full = it_origin_dest->second;
					found_new_shortest_full = new_shortest_full > 0 && (old_shortest_full < 0
						|| (adj_dist >= 0 && new_shortest_full <= old_shortest_full));
				}
				// if we have a possible path shorter than those starting from the other adjacent tiles, that's our new shortest path
				if (found_new_shortest_full) {
					distances[origin_t_pos_key][dest_t_pos_key] = new_shortest_full;
					// erase the previous shortest path and replace it with the new path
					shortest_path.clear();
					shortest_path.push_back(start_t_pos);
					shortest_path.insert(shortest_path.end(), adj_t_path.first.begin(), adj_t_path.first.end());
					shortest_dist = adj_dist;
					const int x_dist = std::abs(origin_t_pos.first - dest_t_pos.first),
						y_dist = std::abs(origin_t_pos.second - dest_t_pos.second);
					// definitely the shortest possible
					if (dist_tally + shortest_dist + adj_start_cost <= x_dist + y_dist) {
						return std::pair<std::vector<std::pair<int, int>>, float>(shortest_path, shortest_dist + adj_start_cost);
					}
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

void DjikstraNode::add_neighbor(DjikstraNode * neighbor, const float cost, const std::vector<std::pair<int, int>> tile_path)
{
	this->neighbors.push_back(std::pair<DjikstraNode *, float>(neighbor, cost));
	this->neighbor_paths[neighbor->node_key] = std::pair<std::vector<std::pair<int, int>>, float>(tile_path, cost);
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
			const std::vector<std::pair<int, int>> tile_path = shortest_path.first;
			const float cost = shortest_path.second;
			// the next node only gets mapped as a neighbor if if it's reachable from this node
			if (cost >= 0) {
				const std::string next_node_key = this->path_node_map_key(level->get_filename(), next_node->get_node_id());
				auto it = this->node_map.find(next_node_key);
				if (it == this->node_map.end()) {
					this->initialize_node(world, npc, level, next_node);
				}
				DjikstraNode * next_djikstra_node = this->node_map[next_node_key];
				djikstra_node->add_neighbor(next_djikstra_node, cost, tile_path);
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
				//TODO: might not be able to use an empty vector for tile path, and/or may need to refactor
				djikstra_node->add_neighbor(next_djikstra_node, 1.0, std::vector<std::pair<int, int>>());
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

const bool PathNodeDjikstraPath::has_node(const std::string level_key, const std::string node_key)
{
	const std::string map_key = this->path_node_map_key(level_key, node_key);
	return this->node_map.find(map_key) != this->node_map.end();
}

const std::pair<std::vector<std::pair<int, int>>, float> PathNodeDjikstraPath::get_shortest_tile_path(const std::string level_key, const std::string start_key, const std::string dest_node_key)
{
	const std::string start_node_key = this->path_node_map_key(level_key, start_key);
	if (this->node_map.find(start_node_key) != this->node_map.end()) {
		DjikstraNode * start_node = this->node_map[start_node_key];
		if (start_node->neighbor_paths.find(dest_node_key) != start_node->neighbor_paths.end()) {
			return start_node->neighbor_paths[dest_node_key];
		}
	}
	return std::pair<std::vector<std::pair<int, int>>, float>(std::vector<std::pair<int, int>>(), -1.0);
}

void * PathNodeDjikstraPath::func_calculate_npc_pathing(ALLEGRO_THREAD * thr, void * arg)
{

	double start_time = al_get_time();
	double accounted_time = 0.0;

	bool enable_logging = true; //temp

	if (enable_logging) {
		std::cout << "--------------------------------------\n";
		std::cout << "START calculating npc pathing\n\n";
	}

	DjikstraPathData *data = (DjikstraPathData *)arg;
	const std::pair<std::string, std::pair<int, int>> forced_dest = data->forced_dest;
	const bool path_around_moving = data->should_path_around_moving;
	// TODO: work backwards from first primary tile pos to last open tile if path_around_moving is true
	const std::pair<int, int> forced_pos = forced_dest.second;
	TileDjikstraPath * tile_djikstra_path = data->tile_djikstra_path;
	PathNodeDjikstraPath * node_djikstra_path = data->node_djikstra_path;

	if (forced_pos.first >= 0 && forced_pos.second >= 0) {
		data->should_force = true;
		data->has_found_path = true;
		data->should_unset_forced_destination = true;
		data->primary_destinations.push_back(forced_dest);
	} 
	if (!path_around_moving && data->starting_node_key != ""
		&& data->node_djikstra_path->has_node(data->current_level_key, data->starting_node_key)) {
		std::pair<std::vector<DjikstraNode*>, float> shortest_path_from_start
			= node_djikstra_path->calculate_shortest_node_path(
				data->starting_node_key, data->current_level_key, data->dest_node_id, data->dest_level_key);
		const int path_to_execute_size = shortest_path_from_start.first.size();
		for (int i = 0; i < path_to_execute_size; i++) {
			DjikstraNode * node_to_execute = shortest_path_from_start.first[i];
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
		//TODO: the entire point of this else block is to select a next primary destintation to path to.
		// should refactor it since there should be a much easier way to do this.
	} else if (!data->should_force) {
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
			const std::pair<int, int> current_level_node_t_pos
			(data->current_level_path_nodes[i]->get_x() / TILE_SIZE, data->current_level_path_nodes[i]->get_y() / TILE_SIZE);
			al_unlock_mutex(data->mutex);

			if (path_around_moving && data->tile_djikstra_path->tile_is_blocked(current_level_node_t_pos.first, current_level_node_t_pos.second)) {
				continue;
			}

			double start_get_tile_path_time = al_get_time();
			float current_level_node_shortest_dist;
			std::pair<std::vector<std::pair<int, int>>, float> current_level_node_shortest_path;

			std::string start_node_key = data->starting_node_key;
			if (data->starting_node_key == "" || !data->node_djikstra_path->has_node(data->current_level_key, data->starting_node_key)) {
				start_node_key = data->last_primary_dest.first;
			}
			if (start_node_key != "" && data->node_djikstra_path->has_node(data->current_level_key, start_node_key)) {
				// the NPC is starting on a known PathNode, so we can use the full stored path
				// we won't actually step on these tiles, so we don't care about pathing around moving, just a distance estimate
				current_level_node_shortest_path
					= data->node_djikstra_path->get_shortest_tile_path(data->current_level_key, data->starting_node_key, data->current_level_path_nodes[i]->get_node_id());
				current_level_node_shortest_dist = current_level_node_shortest_path.second;
			} else {
				// this is a rough estimate
				current_level_node_shortest_dist = std::abs(data->current_level_path_nodes[i]->get_x() / TILE_SIZE - data->npc_t_pos.first)
					+ std::abs(data->current_level_path_nodes[i]->get_y() / TILE_SIZE - data->npc_t_pos.second);
				//current_level_node_shortest_path
				//	= tile_djikstra_path->calculate_shortest_tile_path(data->npc_t_pos, current_level_node_t_pos);
			}

			if (enable_logging) {
				double end_get_tile_path_time = al_get_time();
				double delta_t2 = end_get_tile_path_time - start_get_tile_path_time;
				std::cout << "Time to calculate shortest tile path: " << std::to_string(delta_t2) << " seconds\n\n";
				std::cout << "~~~~~~~~~~ \n"; 
				std::cout << "Shortest tile path: \n";
				for (std::pair<int, int> log_tile_pos : current_level_node_shortest_path.first) {
					std::cout << "(" << log_tile_pos.first << ", " << log_tile_pos.second << ") ";
				}
				std::cout << "\n";
				std::cout << "~~~~~~~~~~ \n";
				accounted_time += delta_t2;
			}

			const float next_node_dist = current_level_node_shortest_path.second;

			if (next_node_dist < 0) {
				continue;
			} else {
				if (shortest_current_level_node_dist < 0
					|| next_node_dist < shortest_current_level_node_dist) {

					// the closest becomes the second closest
					second_shortest_current_level_node_dist = shortest_current_level_node_dist;
					second_closest_node = closest_node;
					shortest_current_level_node_dist = next_node_dist;

					al_lock_mutex(data->mutex);
					closest_node = data->current_level_path_nodes[i];
					al_unlock_mutex(data->mutex);

				} else if (second_shortest_current_level_node_dist < 0
					|| next_node_dist < second_shortest_current_level_node_dist) {
					second_shortest_current_level_node_dist = next_node_dist;

					al_lock_mutex(data->mutex);
					second_closest_node = data->current_level_path_nodes[i];
					al_unlock_mutex(data->mutex);
				}
			}
		}

		if (shortest_current_level_node_dist < 0 || closest_node == NULL) {

		} else if (second_shortest_current_level_node_dist < 0 || second_closest_node == NULL) {
			// there is only one possible destination, so we'll path there
			const std::pair<int, int> primary_dest(closest_node->get_x(), closest_node->get_y());
			data->primary_destinations.push_back(std::pair<std::string, std::pair<int, int>>(closest_node->get_node_id(), primary_dest));
			data->has_found_path = true;
		} else {
			// This is the only case where we have to use actual PathNode pathing,
			// because there is more than one relevant PathNode on the level.

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
			//TODO: choosing the path to execute might work better if we look at the cached node path and work backwards from the destination, similar to tile pathing
			// we can just go straight to the destination node
			if (closest_node->get_node_id() == data->dest_node_id) {
				path_to_execute = node_list_from_closest; //temp. was this removed for a reason?
			} else if (!can_path_from_closest && !can_path_from_second_closest) {
				path_to_execute = node_list_from_closest;
			} else if (can_path_from_closest && !can_path_from_second_closest) {
				path_to_execute = node_list_from_closest;
			} else if (!can_path_from_closest && can_path_from_second_closest) {
				path_to_execute = node_list_from_second_closest;
			} else {
				std::vector<DjikstraNode*> unblocked_first_path;
				const int closest_node_list_count = node_list_from_closest.size();
				// first, look for a blocked primary node and go to the next node after it
				for (int cli = closest_node_list_count - 1; cli >= 0; cli--) {
					DjikstraNode * cli_node = node_list_from_closest[cli];
					if (path_around_moving && data->tile_djikstra_path
						->tile_is_blocked(
							cli_node->position.first / TILE_SIZE, cli_node->position.second / TILE_SIZE)) {
						break;
					}
					unblocked_first_path.insert(unblocked_first_path.begin(), cli_node);
				}

				std::vector<DjikstraNode*> unblocked_second_path;
				const int second_closest_node_list_count = node_list_from_second_closest.size();
				// first, look for a blocked primary node and go to the next node after it
				for (int cli = second_closest_node_list_count - 1; cli >= 0; cli--) {
					DjikstraNode * cli_node = node_list_from_second_closest[cli];
					if (path_around_moving && data->tile_djikstra_path
						->tile_is_blocked(
							cli_node->position.first / TILE_SIZE, cli_node->position.second / TILE_SIZE)) {
						break;
					}
					unblocked_second_path.insert(unblocked_second_path.begin(), cli_node);
				}

				bool found_shared_node = false;
				for (DjikstraNode * close_1 : unblocked_first_path) {
					if (found_shared_node) {
						break;
					}
					for (DjikstraNode * close_2 : unblocked_second_path) {
						if (close_1->node_key == close_2->node_key) {
							found_shared_node = true;
							path_to_execute.push_back(close_1);
							break;
						}
					}
				}
				if (!found_shared_node) {
					if (shortest_from_closest_cost <= shortest_from_second_closest_cost) {
						path_to_execute = node_list_from_closest;
					} else {
						path_to_execute = node_list_from_second_closest;
					}
				}
			}
			if (path_to_execute.empty()) {
				data->has_found_path = false;
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

	if (data->has_found_path) {
		// this dest will be in pixel pos, not tile pos
		const std::string first_dest_key = data->primary_destinations[0].first;
		const std::pair<int, int> first_primary = data->primary_destinations[0].second;
		std::pair<std::string, std::pair<int, int>> last_primary_dest = data->last_primary_dest;
		std::pair<std::vector<std::pair<int, int>>, float> primary_tile_path;
		std::pair<std::vector<std::pair<int, int>>, float> stored_tile_path;
		

		bool use_full_stored_path = false;
		bool recalculate_from_scratch = true;
		bool has_failed = false;

		std::string start_node_key = "";
		if (data->starting_node_key != "" && data->node_djikstra_path->has_node(data->current_level_key, data->starting_node_key)) {
			// the NPC is starting on a known PathNode, so we might be able to use the full stored path
			start_node_key = data->starting_node_key;
			use_full_stored_path = !path_around_moving;
			stored_tile_path
				= data->node_djikstra_path->get_shortest_tile_path(data->current_level_key, start_node_key, first_dest_key);
			recalculate_from_scratch = false;
		} else {
			// the NPC is not standing on a known PathNode
			start_node_key = data->last_primary_dest.first;
			if (start_node_key != "" && data->node_djikstra_path->has_node(data->current_level_key, start_node_key)) {
				stored_tile_path
					= data->node_djikstra_path->get_shortest_tile_path(data->current_level_key, start_node_key, first_dest_key);
				recalculate_from_scratch = false;
			}
		}
		
		if (use_full_stored_path) {
			primary_tile_path = stored_tile_path;
		} else {
			std::pair<int, int> final_tile_path_dest;
			if (recalculate_from_scratch) {
				final_tile_path_dest = std::pair<int, int>(first_primary.first / TILE_SIZE, first_primary.second / TILE_SIZE);
				primary_tile_path = tile_djikstra_path->calculate_shortest_tile_path
					(data->npc_t_pos, final_tile_path_dest);
				has_failed = primary_tile_path.second < 0;
			} else {
				std::vector<std::pair<int, int>> known_primary_tiles;
				float known_primary_dist = 0.0;
				int shortest_dist_to_known = -1;
				std::pair<int, int> closest_known(-1, -1);
				int closest_index = -1;

				const int size = stored_tile_path.first.size();
				for (int ti = size - 1; ti >= 0; ti--) {
					const std::pair<int, int> ti_pos = stored_tile_path.first[ti];
					if (path_around_moving && data->tile_djikstra_path->tile_is_blocked(ti_pos.first, ti_pos.second)) {
						break;
					}
					// if we're not trying to path around something that's moving, just keep pushing back tiles 
					//		until we get to the shortest distance
					const int dist_to_known = std::abs(ti_pos.first - data->npc_t_pos.first) 
						+ std::abs(ti_pos.second - data->npc_t_pos.second);
					if (shortest_dist_to_known < 0 || dist_to_known < shortest_dist_to_known) {
						shortest_dist_to_known = dist_to_known;
						closest_known = ti_pos;
						closest_index = ti;
					}
					known_primary_tiles.insert(known_primary_tiles.begin(), ti_pos);
				}
				if (!has_failed) {
					primary_tile_path = tile_djikstra_path->calculate_shortest_tile_path
						(data->npc_t_pos, closest_known);
					const int known_p_size = known_primary_tiles.size();
					const int starting_index = known_p_size - (stored_tile_path.first.size() - closest_index);
					for (int tki = closest_index; tki < known_p_size; tki++) {
						primary_tile_path.first.push_back(known_primary_tiles[tki]);
						// we don't have a distance for this partial path, so just add the edge cost so we get the right total
						if (tki > 0) {
							known_primary_dist 
								+= data->tile_djikstra_path->calculate_edge_cost(known_primary_tiles[tki] , known_primary_tiles[tki - 1]);
						}
					}
					primary_tile_path.second += known_primary_dist;
				}
			}
		}
		data->has_found_path = !has_failed;
		if (data->has_found_path) {
			data->tile_path_to_primary = primary_tile_path;
		}
	}

	if (enable_logging) {
		double end_time = al_get_time();
		double delta = end_time - start_time;
		std::cout << "FINISH calculating npc pathing. Time to calculate: " + std::to_string(delta) + "\n";
		std::cout << "Unaccounted time: " << std::to_string(delta - accounted_time) << "(" << std::to_string(100.0 * (accounted_time / delta)) << "% accounted for)\n";
		std::cout << "--------------------------------------\n";
	}

	al_lock_mutex(data->mutex);
	data->ready = true;
	al_broadcast_cond(data->cond);
	al_unlock_mutex(data->mutex);

	return NULL;
}
