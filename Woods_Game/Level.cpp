#include "Level.h"
#include "GameImageManager.h"

struct game_image_center_comparison
{
	inline bool operator() (GameImage* g1, GameImage* g2)
	{
		return (g1->get_center().second < g2->get_center().second);
	}
};

void Level::draw_tiled_images(ALLEGRO_DISPLAY * display, const std::pair<int, int> offset, const int layer_index)
{
	const int layer_count = this->tiled_image_layers.size();
	if (layer_count >= 0 && layer_index < layer_count) {
		TiledImageLayer * layer = this->tiled_image_layers.getItem(layer_index);
		layer->draw_tiled_images(display, offset);
	}
}

void Level::generate_paths()
{
	std::vector<LevelGenPathSystem *> path_systems = this->gen_data.get_path_systems();
	for (LevelGenPathSystem * ps : path_systems) {
		const int tile_index = ps->tile_type_index.value();
		const int node_count = ps->path_nodes.size();
		const int path_size = ps->path_size.value();
		if (node_count > 1) {
			std::vector<std::pair<int, int>> vector_coords;
			for (int i = 0; i < node_count; i++) {
				LevelGenPathNode * n = ps->path_nodes.getItem(i);
				vector_coords.push_back(std::pair<int, int>(n->x_pos.value(), n->y_pos.value()));
			}
			srand(std::time(NULL) + node_count);
			int i1 = rand() % node_count;
			const std::pair<int, int> pos1 = vector_coords[i1];
			vector_coords.erase(vector_coords.begin() + i1);
			int i2 = rand() % vector_coords.size();
			const std::pair<int, int> pos2 = vector_coords[i2];
			vector_coords.erase(vector_coords.begin() + i2);
			std::vector<std::pair<int, int>> visited_coords;
			std::cout << "\nGenerating coords\n";
			const std::vector<std::pair<int, int>> coordinates = this->connect_path_nodes(tile_index, pos1, pos2, visited_coords, path_size);
			for (std::pair<int, int> c : coordinates) {
				if (!(std::find(visited_coords.begin(), visited_coords.end(), c) != visited_coords.end())) {
					visited_coords.push_back(c);
				}
			}
			while (vector_coords.size() > 0) {
				// choose a random node from those remaining and start pathing from there
				srand(std::time(NULL) + vector_coords.size());
				const int iv = rand() % visited_coords.size();
				// choose a random destination from those visited and path to there
				const std::pair<int, int> target_coords = visited_coords[iv];
				srand(std::time(NULL) + vector_coords.size() + 1);
				const int index = rand() % vector_coords.size();
				const std::pair<int, int> start_coords = vector_coords[index];
				vector_coords.erase(vector_coords.begin() + index);
				std::cout << "\nGenerating coords\n";
				const std::vector<std::pair<int, int>> path_coords = this->connect_path_nodes(tile_index, start_coords, target_coords, visited_coords, path_size);
				for (std::pair<int, int> c : path_coords) {
					if (!(std::find(visited_coords.begin(), visited_coords.end(), c) != visited_coords.end())) {
						visited_coords.push_back(c);
					}
				}
			}
		}
	}
}

void Level::generate_tiles()
{
	const int base_tile_index = this->gen_data.get_base_tile_type_index();
	const int width = this->tile_rows.getItem(0)->get_size(), height = this->tile_rows.size();
	const int base_tile_num_sheet_cols = tileset->get_tile_sheet_image_cols_by_index(base_tile_index);
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			Tile *t = this->get_tile(x, y);
			srand(std::time(NULL) + x + y);
			const int sheet_col = rand() % base_tile_num_sheet_cols;
			t->reset(this->tileset, base_tile_index);
			t->set_tile_sheet_col(sheet_col);
		}
	}
	const int tile_width = this->width / TILE_SIZE, tile_height = this->height / TILE_SIZE;
	const std::vector<TileGenRule *> gen_rules = this->gen_data.get_tile_gen_rules();
	for (TileGenRule * rule : gen_rules) {
		const int tile_index = rule->tile_type_index.value();
		const int tile_num_sheet_cols = tileset->get_tile_sheet_image_cols_by_index(tile_index);
		const int min_patches = rule->min_patches.value(), max_patches = rule->max_patches.value(),
			min_size = rule->min_patch_size.value(), max_size = rule->max_patch_size.value();
		const int num_patches = min_patches + rand() % (max_patches - min_patches);
		for (int i = 0; i < num_patches; i++) {
			srand(std::time(NULL) + tile_index + i);
			const int patch_size = std::max(1, min_size + rand() % (max_size - min_size));
			const int min_rect_area = std::max(16, patch_size * 3), max_rect_area = std::max(min_rect_area + 1, patch_size * 4);
			const int min_rect_width = std::max(4, (int) (std::pow((double)min_rect_area, 0.5) * 0.6));
			const int max_rect_width = std::min(tile_width/2, (int)(std::pow((double)min_rect_area, 0.5) / 0.6));
			const int rect_width = min_rect_width == max_rect_width ?
				min_rect_width : min_rect_width + rand() % (max_rect_width - min_rect_width);
			const int min_rect_height = std::max(4, min_rect_area / rect_width);
			const int max_rect_height = std::min(tile_width / 2, max_rect_area / rect_width);
			const int rect_height = min_rect_height == max_rect_height ?
				min_rect_height : min_rect_height + rand() % (max_rect_height - min_rect_height);
			const int min_rect_x = 2, min_rect_y = 2, max_rect_x = tile_width - 3 - rect_width, max_rect_y = tile_height - 3 - rect_height;
			const int rect_x = min_rect_x + rand() % (max_rect_x - min_rect_x), rect_y = min_rect_y + rand() % (max_rect_y - min_rect_y);
			const int center_x = rect_x + rect_width / 2, center_y = rect_y + rect_height / 2;
			Tile * center_tile = this->get_tile(center_x, center_y);
			const int sc = rand() % tile_num_sheet_cols;
			center_tile->set_tile_sheet_col(sc);
			center_tile->reset(this->tileset, tile_index);
			int remaining_tiles = patch_size - 1;
			std::vector<std::pair<int, int>> visited;
			int cx = center_x, cy = center_y;
			while (remaining_tiles > 0) {
				srand(std::time(NULL) + remaining_tiles);
				std::vector<std::pair<int, int>> candidates;
				//TODO: figure out if patches spreading diagonally will look good or not (try both ways)
				std::vector<Tile *> adjacent_tiles = this->get_tiles_in_range(cx, cy, 1, 1, 1);
				for (Tile * at : adjacent_tiles) {
					const int tx = at->get_tile_pos_x(), ty = at->get_tile_pos_y();
					if ((tx == cx && ty == cy) || tx < rect_x || ty < rect_y || tx >= rect_x + rect_width || ty >= rect_y + rect_height
						|| std::find(visited.begin(), visited.end(), std::pair<int, int>(tx, ty)) != visited.end()) {
						continue;
					}
					candidates.push_back(std::pair<int, int>(tx, ty));
				}
				if (candidates.size() > 0) {
					const int ci = rand() % candidates.size();
					const std::pair<int, int> c_pos = candidates[ci];
					Tile * cand_tile = this->get_tile(c_pos.first, c_pos.second);
					const int c_sc = rand() % tile_num_sheet_cols;
					cand_tile->set_tile_sheet_col(c_sc);
					cand_tile->reset(this->tileset, tile_index);
					cx = c_pos.first, cy = c_pos.second;
					visited.push_back(c_pos);
					remaining_tiles--;
				} else {
					break; //TODO: this is probably bad
				}
			}
		}
	}
}

void Level::generate_entity_groups()
{
	const std::vector<EntityGroupGenRule *> gen_rules = this->gen_data.get_entity_group_gen_rules();
	for (EntityGroupGenRule * rule : gen_rules) {
		const int eg_index = rule->entity_group_index.value();
		const int eg_num_sheet_cols = this->tileset->get_entity_group_sheet_image_cols_by_index(eg_index);
		const int min_spawn_tx = rule->min_spawn_dist_tx.value(), min_spawn_ty = rule->min_spawn_dist_ty.value();
		const std::pair<int, int> root_offset = this->tileset->get_entity_group_root_offset(eg_index);
		//const std::pair<int, int> collide_offset = this->tileset->get_entity_group_collide_offset(eg_index);
		//const std::pair<int, int> collide_dimensions = this->tileset->get_entity_group_collide_dimensions(eg_index);
		const std::pair<int, int> eg_dim = this->tileset->get_entity_group_image_dimensions_by_index(eg_index);
		const std::vector<int> allowed_tiles = rule->get_allowed_spawn_tile_indeces();
		std::vector<std::pair<int, int>> candidate_tiles;
		// constrain allowed spawn locations based on level edges and eg dimensions
		const int tx_min = 1;
		const int ty_min = 1;
		const int tx_max = std::min(this->width/TILE_SIZE - 2, this->width/TILE_SIZE - ((eg_dim.first - root_offset.first) / TILE_SIZE) - 1);
		const int ty_max = std::min(this->height / TILE_SIZE - 2, this->height / TILE_SIZE - ((eg_dim.second - root_offset.second) / TILE_SIZE) - 1);
		const int check_w = std::max(1, std::min((eg_dim.first / TILE_SIZE) / 2, (eg_dim.first - root_offset.first) / TILE_SIZE));
		const int check_h = std::max(1, std::min((eg_dim.second / TILE_SIZE) / 2, (eg_dim.second - root_offset.second) / TILE_SIZE));
		// TODO: refactor check tile method into utility method since it's identical to that for genning blocks
		for (int ty = ty_min; ty < ty_max; ty++) {
			for (int tx = tx_min; tx < tx_max; tx++) {
				std::pair<int, int> t_pos(tx, ty);
				Tile * ct = this->get_tile(tx, ty);
				const int cx1 = std::max(tx_min, tx - check_w);
				const int cy1 = std::max(ty_min, ty - check_h);
				const int cx2 = std::min(tx_max, tx + check_w);
				const int cy2 = std::min(ty_max, ty + check_h);
				bool may_spawn = true;
				for (int cy = cy1; cy < cy2; cy++) {
					for (int cx = cx1; cx < cx2; cx++) {
						if (may_spawn) {
							std::pair<int, int> ct_pos(cx, cy);
							Tile * check_t = this->get_tile(cx, cy);
							const int cti = check_t->get_tile_type_index();
							// check tile is occupied by a block/eg or we are not allowed to spawn an eg on that tile
							if ((check_t->get_block() != NULL && !check_t->get_block()->is_empty())
								|| this->entity_group_at_tile_pos(ct_pos, true) != NULL
								|| std::find(allowed_tiles.begin(), allowed_tiles.end(), cti) == allowed_tiles.end()) {
								may_spawn = false;
							}
						}
					}
				}
				if (may_spawn) {
					candidate_tiles.push_back(t_pos);
				}
			}
		}
		const int num_candidates = candidate_tiles.size();
		const int min_egs = std::min(num_candidates, rule->min_groups.value());
		const int max_egs = std::min(num_candidates, rule->max_groups.value());
		srand(std::time(NULL) + eg_index + num_candidates);
		const int num_egs = min_egs < max_egs ? min_egs + rand() % (max_egs - min_egs) : min_egs;
		int remaining_egs = num_egs;
		while (candidate_tiles.size() > 0 && remaining_egs > 0) {
			srand(std::time(NULL) + eg_index + num_candidates + remaining_egs);
			const int cand_index = rand() % candidate_tiles.size();
			const std::pair<int, int> spawn_pos = candidate_tiles[cand_index];
			const int eg_sc = rand() % eg_num_sheet_cols;
			this->add_entity_group(eg_index, std::pair<int, int>(eg_sc, 0), spawn_pos);
			remaining_egs--;
			// remove candidate tiles that would be too close to this eg
			const int bx1 = std::max(tx_min, spawn_pos.first - min_spawn_tx);
			const int by1 = std::max(ty_min, spawn_pos.second - min_spawn_ty);
			const int bx2 = std::min(tx_max, spawn_pos.first + min_spawn_tx);
			const int by2 = std::min(ty_max, spawn_pos.second + min_spawn_ty);
			for (int by = by1; by < by2; by++) {
				for (int bx = bx1; bx < bx2; bx++) {
					const std::pair<int, int> b_pos(bx, by);
					if (std::find(candidate_tiles.begin(), candidate_tiles.end(), b_pos) != candidate_tiles.end()) {
						candidate_tiles.erase(std::remove(candidate_tiles.begin(), candidate_tiles.end(), b_pos), candidate_tiles.end());
					}
				}
			}
		}
	}
}

void Level::generate_blocks()
{
	const std::vector<BlockGenRule *> gen_rules = this->gen_data.get_block_gen_rules();
	for (BlockGenRule * rule : gen_rules) {
		const int block_index = rule->block_index.value();
		const int num_sheet_cols = this->tileset->get_block_sheet_image_cols_by_index(block_index);
		const int min_spawn_tx = rule->min_spawn_dist_tx.value(), min_spawn_ty = rule->min_spawn_dist_ty.value();
		const std::vector<int> allowed_tiles = rule->get_allowed_spawn_tile_indeces();
		std::vector<std::pair<int, int>> candidate_tiles;
		// constrain allowed spawn locations based on level edges and eg dimensions
		const int tx_min = 1;
		const int ty_min = 1;
		const int tx_max = this->width / TILE_SIZE - 2;
		const int ty_max = this->height / TILE_SIZE - 2;
		const int check_w = 1;
		const int check_h = 1;
		// TODO: refactor check tile method into utility method since it's identical to that for genning egs
		for (int ty = ty_min; ty < ty_max; ty++) {
			for (int tx = tx_min; tx < tx_max; tx++) {
				std::pair<int, int> t_pos(tx, ty);
				Tile * ct = this->get_tile(tx, ty);
				const int cx1 = std::max(tx_min, tx - check_w);
				const int cy1 = std::max(ty_min, ty - check_h);
				const int cx2 = std::min(tx_max, tx + check_w);
				const int cy2 = std::min(ty_max, ty + check_h);
				bool may_spawn = true;
				for (int cy = cy1; cy < cy2; cy++) {
					for (int cx = cx1; cx < cx2; cx++) {
						if (may_spawn) {
							std::pair<int, int> ct_pos(cx, cy);
							Tile * check_t = this->get_tile(cx, cy);
							const int cti = check_t->get_tile_type_index();
							// check tile is occupied by a block/eg or we are not allowed to spawn an eg on that tile
							if ((check_t->get_block() != NULL && !check_t->get_block()->is_empty())
								|| this->entity_group_at_tile_pos(ct_pos, true) != NULL
								|| std::find(allowed_tiles.begin(), allowed_tiles.end(), cti) == allowed_tiles.end()) {
								may_spawn = false;
							}
						}
					}
				}
				if (may_spawn) {
					candidate_tiles.push_back(t_pos);
				}
			}
		}

		const int num_candidates = candidate_tiles.size();
		const int min_blocks = std::min(num_candidates, rule->min_blocks.value());
		const int max_blocks = std::min(num_candidates, rule->max_blocks.value());
		srand(std::time(NULL) + block_index + num_candidates);
		const int num_blocks = min_blocks < max_blocks ? min_blocks + rand() % (max_blocks - min_blocks) : min_blocks;
		int remaining_blocks = num_blocks;
		while (candidate_tiles.size() > 0 && remaining_blocks > 0) {
			srand(std::time(NULL) + block_index + num_candidates + remaining_blocks);
			const int cand_index = rand() % candidate_tiles.size();
			const std::pair<int, int> spawn_pos = candidate_tiles[cand_index];
			const int b_sc = rand() % num_sheet_cols;
			this->replace_block(block_index, std::pair<int, int>(b_sc, 0), spawn_pos);
			remaining_blocks--;
			// remove candidate tiles that would be too close to this eg
			const int bx1 = std::max(tx_min, spawn_pos.first - min_spawn_tx);
			const int by1 = std::max(ty_min, spawn_pos.second - min_spawn_ty);
			const int bx2 = std::min(tx_max, spawn_pos.first + min_spawn_tx);
			const int by2 = std::min(ty_max, spawn_pos.second + min_spawn_ty);
			for (int by = by1; by < by2; by++) {
				for (int bx = bx1; bx < bx2; bx++) {
					const std::pair<int, int> b_pos(bx, by);
					if (std::find(candidate_tiles.begin(), candidate_tiles.end(), b_pos) != candidate_tiles.end()) {
						candidate_tiles.erase(std::remove(candidate_tiles.begin(), candidate_tiles.end(), b_pos), candidate_tiles.end());
					}
				}
			}
		}
	}
}

const std::vector<std::pair<int, int>> Level::connect_path_nodes(const int tile_index, const std::pair<int, int> pos1, const std::pair<int, int> pos2, 
	const std::vector<std::pair<int, int>> visited, const int path_size)
{
	const int x1 = pos1.first, y1 = pos1.second, x2 = pos2.first, y2 = pos2.second;
	Tile * t1 = this->get_tile(x1, y1);
	Tile * t2 = this->get_tile(x2, y2);
	t1->reset(this->tileset, tile_index);
	t2->reset(this->tileset, tile_index);
	std::vector<std::pair<int, int>> path_coords;
	if (x1 == x2 && y1 == y2) {
		// the two tiles are actually the same, so there is no need to connect them
		path_coords.push_back(std::pair<int, int>(x1, y1));
		std::cout << "Not connecting tiles with coords (" + std::to_string(x1) + ", " + std::to_string(y1) + ") because same tile";
		return path_coords;
	}
	if (std::abs(x1 - x2) == 1 
		&& std::abs(y1 - y2) == 1) {
		std::cout << "Not connecting tiles with coords (" 
			+ std::to_string(x1) + ", " + std::to_string(y1) + ") and " 
			+ std::to_string(x2) + ", " + std::to_string(y2) + " because they are adjacent";
		// the two tiles are adjacent and therefore already connected
		path_coords.push_back(std::pair<int, int>(x1, y1));
		path_coords.push_back(std::pair<int, int>(x2, y2));
		return path_coords;
	}
	return this->generate_weaving_paths(tile_index, pos1, pos2, visited, path_size);
}

//TODO: avoid obstacles
const std::vector<std::pair<int, int>> Level::generate_weaving_paths(const int tile_index, const std::pair<int, int> pos1, const std::pair<int, int> pos2, 
	const std::vector<std::pair<int, int>> visited, const int path_size)
{
	const int x1 = pos1.first, y1 = pos1.second, x2 = pos2.first, y2 = pos2.second;
	std::vector<std::pair<int, int>> coordinates;
	const std::vector<std::pair<int, int>> rolled_coordinates = this->generate_weaving_coordinates(x1, y1, x2, y2, 1);
	for (std::pair<int, int> coords : rolled_coordinates) {
		coordinates.push_back(coords);
	}
	std::vector<std::pair<int, int>> path_coords;
	const int size = coordinates.size();
	for (int i = 0; i < size - 1; i++) {
		const std::pair<int, int> pos1(coordinates[i].first, coordinates[i].second);
		const std::pair<int, int> pos2(coordinates[i + 1].first, coordinates[i + 1].second);
		const std::vector<std::pair<int, int>> line_coords 
			= this->connect_path_nodes_straight_line(tile_index, pos1, pos2, path_size);
		std::cout << "Connected path nodes: (" 
			+ std::to_string(pos1.first) + "," + std::to_string(pos1.second) + ") and " 
			+ std::to_string(pos2.first) + "," + std::to_string(pos2.second) + ")\n";
		for (std::pair<int, int> lc : line_coords) {
			if (!(std::find(path_coords.begin(), path_coords.end(), lc) != path_coords.end())) {
				path_coords.push_back(lc);
				if (std::find(visited.begin(), visited.end(), lc) != visited.end()) {
					// return early if we connect to a path we already visited
					return path_coords;
				}
			}
		}
	}
	return path_coords; 
}

const std::vector<std::pair<int, int>> Level::generate_weaving_coordinates(const int x1, const int y1, const int x2, const int y2, const int seed)
{
	std::vector<std::pair<int, int>> coordinates;
	coordinates.push_back(std::pair<int, int>(x1, y1));
	const int x_dist = std::abs(x1 - x2), y_dist = std::abs(y1 - y2);
	// base case is that they are so close together we don't need to weave
	if (x_dist > 4 || y_dist > 4) {
		const int line_dist = std::pow(std::pow((float)x_dist, 2.0) + std::pow((float)y_dist, 2.0), 0.5);
		const int num_segments = std::max(1, line_dist / 6); //TODO: experiment with different formulas. might want to roll intsead of just using 6
		const int x_incr = (x2 - x1)/num_segments;
		const int y_incr = (y2 - y1)/num_segments;
		const int x_mid = x1 + x_incr, y_mid = y1 + y_incr;
		const int x_range = std::max(2, (int) ((float)(std::abs(x_mid - x1))/ 1.05f)), 
			y_range = std::max(2, ((int)(((float) (std::abs(y_mid - y1))/ 1.05f)))); //TODO: experiment with different values here
		srand(std::time(NULL) + seed);
		const int x_roll = (rand() % x_range) - x_range;
		srand(std::time(NULL) + seed);
		const int y_roll = (rand() % y_range) - y_range;
		const int adj_x = std::max(1, std::min(this->width / TILE_SIZE, x_mid + x_roll));
		const int adj_y = std::max(1, std::min(this->height / TILE_SIZE, y_mid + y_roll));
		const std::vector<std::pair<int, int>> rolled_coordinates = this->generate_weaving_coordinates(adj_x, adj_y, x2, y2, seed + 1);
		for (std::pair<int, int> coords : rolled_coordinates) {
			coordinates.push_back(coords);
		}
	} else {
		coordinates.push_back(std::pair<int, int>(x2, y2));
	}
	return coordinates;
}

const std::vector<std::pair<int, int>> Level::connect_path_nodes_straight_line(const int tile_index, const std::pair<int, int> pos1, const std::pair<int, int> pos2, const int path_size)
{
	std::vector<std::pair<int, int>> coordinates;
	const int x1 = pos1.first, y1 = pos1.second, x2 = pos2.first, y2 = pos2.second;
	std::vector<Tile *> tiles = this->get_tiles_in_line(x1, y1, x2, y2, path_size);
	for (Tile * t : tiles) {
		t->reset(this->tileset, tile_index);
		std::pair<int, int> tile_coords(t->get_tile_pos_x(), t->get_tile_pos_y());
		coordinates.push_back(tile_coords);
	}
	return coordinates;
}

inline const int iround(const int x) {
	return (int)(std::round(x) - 0.5) + (x > 0);
};

std::vector<Tile*> Level::get_tiles_in_line(const int x1, const int y1, const int x2, const int y2)
{
	return this->get_tiles_in_line(x1, y1, x2, y2, 1);
}

std::vector<Tile*> Level::get_tiles_in_line(const int x1, const int y1, const int x2, const int y2, const int line_size)
{
	std::vector<Tile*> line_tiles;
	const int x_dist = x2 - x1, y_dist = y2 - y1;
	if (std::abs(x_dist) > std::abs(y_dist)) {
		const float slope = (float)y_dist / (float)x_dist;
		const int increment = x1 <= x2 ? 1 : -1;
		int x = x1 + increment, y = y1;
		while ((increment > 0 && x <= x2) || (increment < 0 && x >= x2)) {
			const int x_off = x - x1;
			const int y_adj = iround(((float)x_off) * slope);
			const int prev_y = y;
			y = y_adj + y1;
			if (prev_y != y) {
				line_tiles.push_back(this->get_tile(x, prev_y));
			}
			line_tiles.push_back(this->get_tile(x, y));
			if (line_size > 1) {
				int y_up = std::min(y, prev_y) - 1;
				int y_down = std::max(y, prev_y + 1);
				int remaining_ls = line_size - 1;
				int index = 0;
				while (remaining_ls > 0) {
					const int above = y_up - 1;
					const int below = this->height / TILE_SIZE - y_down;
					if (above <= 0 && below <= 0) {
						break;
					}
					const bool up = above > 0 && (below <= 0 || index % 2 == 1);
					if (up) {
						line_tiles.push_back(this->get_tile(x, y_up));
						y_up--;
					} else {
						line_tiles.push_back(this->get_tile(x, y_down));
						y_down++;
					}
					remaining_ls--;
					index++;
				}
			}
			x += increment;
		}
	} else {
		const float slope = (float)x_dist / (float)y_dist;
		const int increment = y1 <= y2 ? 1 : -1;
		int y = y1 + increment, x = x1;
		while ((increment > 0 && y <= y2) || (increment < 0 && y >= y2)) {
			const int y_off = y - y1;
			const int x_adj = iround(((float)y_off) * slope);
			const int prev_x = x;
			x = x_adj + x1;
			if (prev_x != x) {
				line_tiles.push_back(this->get_tile(prev_x, y));
			}
			line_tiles.push_back(this->get_tile(x, y));
			if (line_size > 1) {
				int remaining_ls = line_size - 1;
				int index = 0;
				int x_left = std::min(x, prev_x) - 1;
				int x_right = std::max(x, prev_x + 1);
				while (remaining_ls > 0) {
					const int left = x_left - 1;
					const int right = this->width / TILE_SIZE - x_right;
					if (left <= 0 && right <= 0) {
						break;
					}
					const bool go_left = left > 0 && (right <= 0 || index % 2 == 1);
					if (go_left) {
						line_tiles.push_back(this->get_tile(x_left, y));
						x_left--;
					} else {
						line_tiles.push_back(this->get_tile(x_right, y));
						x_right++;
					}
					remaining_ls--;
					index++;
				}
			}
			y += increment;
		}
	}
	return line_tiles;
}

Level::Level()
{
	this->setClassName("Level");
	this->Register("TilesetKey", &(this->tileset_key));
	this->Register("GridX", &(this->grid_x));
	this->Register("GridY", &(this->grid_y));
	this->Register("GridWidth", &(this->grid_width));
	this->Register("GridHeight", &(this->grid_height));
	this->Register("TileRows", &(this->tile_rows));
	this->Register("EntityGroups", &(this->entity_groups));
	this->Register("Spawners", &(this->spawners));
	this->Register("PathNodes", &(this->path_nodes));
	this->Register("TiledImageLayers", &(this->tiled_image_layers));
	this->Register("LevelGenData", &(this->gen_data));
}

Level::Level(std::string level_filename, std::string dungeon_filename, std::string id)
{
	this->setClassName("Level");
	this->Register("TilesetKey", &(this->tileset_key));
	this->Register("GridX", &(this->grid_x));
	this->Register("GridY", &(this->grid_y));
	this->Register("GridWidth", &(this->grid_width));
	this->Register("GridHeight", &(this->grid_height));
	this->Register("TileRows", &(this->tile_rows));
	this->Register("EntityGroups", &(this->entity_groups));
	this->Register("Spawners", &(this->spawners));
	this->Register("PathNodes", &(this->path_nodes));
	this->Register("TiledImageLayers", &(this->tiled_image_layers));
	this->Register("LevelGenData", &(this->gen_data));
	this->map_filename = level_filename;
	this->dungeon_filename = dungeon_filename;
	this->id = id;
}

Level::Level(std::string level_filename, std::string dungeon_filename,  std::string id, int grid_x, int grid_y, int grid_width, int grid_height)
{
	this->setClassName("Level");
	this->Register("TilesetKey", &(this->tileset_key));
	this->Register("GridX", &(this->grid_x));
	this->Register("GridY", &(this->grid_y));
	this->Register("GridWidth", &(this->grid_width));
	this->Register("GridHeight", &(this->grid_height));
	this->Register("TileRows", &(this->tile_rows));
	this->Register("EntityGroups", &(this->entity_groups));
	this->Register("Spawners", &(this->spawners));
	this->Register("PathNodes", &(this->path_nodes));
	this->Register("TiledImageLayers", &(this->tiled_image_layers));
	this->Register("LevelGenData", &(this->gen_data));
	this->map_filename = level_filename;
	this->dungeon_filename = dungeon_filename;
	this->id = id;
	this->grid_x = grid_x;
	this->grid_y = grid_y;
	this->grid_width = grid_width;
	this->grid_height = grid_height;
	const int standard_width = STANDARD_LEVEL_GRID_WIDTH;
	const int standard_height = STANDARD_LEVEL_GRID_HEIGHT;
	this->width = standard_width * grid_width;
	this->height = standard_height * grid_height;
}

Level::Level(std::string filename, int grid_x, int grid_y, int grid_width, int grid_height)
{
	this->setClassName("Level");
	this->Register("TilesetKey", &(this->tileset_key));
	this->Register("GridX", &(this->grid_x));
	this->Register("GridY", &(this->grid_y));
	this->Register("GridWidth", &(this->grid_width));
	this->Register("GridHeight", &(this->grid_height));
	this->Register("TileRows", &(this->tile_rows));
	this->Register("EntityGroups", &(this->entity_groups));
	this->Register("Spawners", &(this->spawners));
	this->Register("PathNodes", &(this->path_nodes));
	this->Register("TiledImageLayers", &(this->tiled_image_layers));
	this->Register("LevelGenData", &(this->gen_data));
	this->map_filename = filename;
	this->grid_x = grid_x;
	this->grid_y = grid_y;
	this->grid_width = grid_width;
	this->grid_height = grid_height;
	const int standard_width = STANDARD_LEVEL_GRID_WIDTH;
	const int standard_height = STANDARD_LEVEL_GRID_HEIGHT;
	this->width = standard_width * grid_width;
	this->height = standard_height * grid_height;
}

Level::Level(int grid_x, int grid_y, int grid_width, int grid_height)		
{
	this->setClassName("Level");
	this->Register("TilesetKey", &(this->tileset_key));
	this->Register("GridX", &(this->grid_x));
	this->Register("GridY", &(this->grid_y));
	this->Register("GridWidth", &(this->grid_width));
	this->Register("GridHeight", &(this->grid_height));
	this->Register("TileRows", &(this->tile_rows));
	this->Register("EntityGroups", &(this->entity_groups));
	this->Register("Spawners", &(this->spawners));
	this->Register("PathNodes", &(this->path_nodes));
	this->Register("TiledImageLayers", &(this->tiled_image_layers));
	this->Register("LevelGenData", &(this->gen_data));
	this->grid_x = grid_x;
	this->grid_y = grid_y;
	this->grid_width = grid_width;
	this->grid_height = grid_height;
	const int standard_width = STANDARD_LEVEL_GRID_WIDTH;
	const int standard_height = STANDARD_LEVEL_GRID_HEIGHT;
	this->width = standard_width * grid_width;
	this->height = standard_height * grid_height;
}

Level::~Level()
{
}

void Level::load_tileset()
{
	this->tileset = new TileSet();
	FileManager filemanager;
	std::string filename = "resources/load/tilesets";
	filemanager.load_xml_content(this->tileset, filename, "SerializableClass", "TilesetKey", this->tileset_key.value());
	this->tileset->set_tileset_key(this->tileset_key.value());
	this->tileset->load_sheet_images();
}

void Level::load_from_xml()
{
	this->load_tileset();
	this->initialize_tiles();	//this also intializes blocks
	this->draw_tile_edge_bitmaps();
	this->initialize_entity_groups();
	this->initialize_tiled_images();
	this->initialize_spawners();
	this->initialize_path_nodes();
}

void Level::reload_from_xml(Level &copy_level)
{
	this->reload_tiles(copy_level);
}

void Level::reset_for_reload()
{
	for (int i = 0; i < tile_rows.size(); i++) {
		for (int j = 0; j < tile_rows.getItem(i)->get_size(); j++) {
			this->get_tile(j, i)->reset_for_reload();
		}
	}
}

void Level::reload_tiles(Level &copy_level)
{
	const int width = this->tile_rows.getItem(0)->get_size(), height = this->tile_rows.size();
	const std::string block_sheet_filename = this->tileset->get_tile_sheet_filename();
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			Tile *t = this->get_tile(x, y);
			Tile *copy_tile = copy_level.get_tile(x, y);
			std::pair<int, int> position(t->get_tile_pos_x()*TILE_SIZE, t->get_tile_pos_y()*TILE_SIZE);
			Block *b = t->get_block();
			Block *copy_block = copy_tile->get_block();
			if (b == NULL) {
				if (copy_block != NULL) {
					const int block_index = copy_block->get_entity_data_index();
					const std::pair<int, int> ss_pos(copy_block->get_entity_sheet_col(), copy_block->get_entity_sheet_row());
					const std::pair<int, int> pos(t->get_tile_pos_x(), t->get_tile_pos_y());
					t->replace_block(this->tileset, block_index, ss_pos, pos);
					//const std::string filename = tileset->get_full_block_sheet_filename(block_index);
					//refresh_mask();
				}
			} else if (copy_block == NULL) {
				t->remove_block();
			} else {
				b->copy_entity_attributes(copy_block);
				b->refresh_mask();
			}
		}
	}
}

void Level::update_new_day(Player * player)
{
	this->update_tiles_new_day(player);
	this->update_npcs_new_day();
	//TODO: what other object types need to be updated?
}

void Level::update_tiles_new_day(Player * player)
{
	const int width = this->tile_rows.getItem(0)->get_size(), height = this->tile_rows.size();
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			Tile *t = this->get_tile(x, y);
			std::pair<int, int> position(t->get_tile_pos_x()*TILE_SIZE, t->get_tile_pos_y()*TILE_SIZE);
			Block *b = t->get_block();
			if (b == NULL || b->is_empty()) {
				continue;
			}
			if (b->update_new_day(player)) {
				if (b->needs_plant_day_update()) {
					this->plant_day_update(b, x, y);
				}
				b->refresh_mask();
			}
		}
	}
}

void Level::update_npcs_new_day()
{

	//TODO: delete this if it doesn't make sense to do it in level
	//TODO: send NPCs to the correct starting locations
}

void Level::plant_day_update(Entity * plant, const int plant_tx, const int plant_ty)
{
	const int range = plant->get_plant_growth_spread_range();
	if (range > 0) {
		// TODO: what other objects do we need to avoid besides blocks and entity groups?
			if (plant->has_entity_attribute(GameImage::E_ATTR_PLANT_GROWTH_CURRENT_AGE)
				&& plant->has_entity_attribute(GameImage::E_ATTR_PLANT_GROWTH_MATURE_AGE)) {
				int current_age = plant->get_entity_attribute(GameImage::E_ATTR_PLANT_GROWTH_CURRENT_AGE);
				const int mature_age = plant->get_entity_attribute(GameImage::E_ATTR_PLANT_GROWTH_MATURE_AGE);
				const int spread_rate = plant->get_entity_attribute(GameImage::E_ATTR_PLANT_GROWTH_SPREAD_RATE);
				const int spread_range = plant->get_entity_attribute(GameImage::E_ATTR_PLANT_GROWTH_SPREAD_RANGE);
				const int crowd_aversion = plant->get_entity_attribute(GameImage::E_ATTR_PLANT_GROWTH_SPREAD_CROWD_AVERSION);
				const bool started_mature = current_age >= mature_age;
				const std::vector<int> allowed_tile_types = plant->get_allowed_spawn_tile_types();
				const int block_index = plant->get_entity_data_index();
				current_age++;
				// increase age by 1 day
				plant->set_entity_attribute(GameImage::E_ATTR_PLANT_GROWTH_CURRENT_AGE, current_age); 
				if (!started_mature) {
					// make sure we update the sprite as necessary after aging
					const std::string filename = tileset->get_full_block_sheet_filename(block_index);
					ImageLoader::get_instance().load_image(
						filename + plant->image_filename_suffix(), *(plant->get_image_subsection()));
				}
				if (started_mature && spread_rate > 0 && spread_range > 0) {
					std::vector<int> open_tile_indeces;
					int open_tiles = 0;
					int blocked_tiles = 0;
					std::vector<Tile*> tiles_in_range = this->get_tiles_in_range(plant, range);
					const int size = tiles_in_range.size();
					for (int i = 0; i < size; i++) {
						Tile * t = tiles_in_range[i];
						if (!(t->get_tile_pos_x() == plant_tx 
							&& t->get_tile_pos_y() == plant_ty)
							&& std::find(allowed_tile_types.begin(), allowed_tile_types.end(), t->get_tile_type_index()) != allowed_tile_types.end()) {
							if (this->entity_group_at_tile_pos(std::pair<int,int>(plant_tx, plant_ty), true) == NULL 
								&& t->get_can_grow_plants()) {
								open_tiles++;
								open_tile_indeces.push_back(i);
							} else {
								blocked_tiles++;
							}
						}
					}
					if (open_tiles > 0) {
						srand(std::time(NULL) + open_tiles);
						const float c = -0.024 + (7.524 / (1.0 + pow(pow((((double)crowd_aversion)/6.39), 15.75), 0.123)));
						const float r = ((float) blocked_tiles / (float) open_tiles);
						const float crowd_grow_ratio = -1.0*c + ((1.0 + c)/(1.0 + (r/c)));
						const float base_grow_ratio = ((float)spread_rate) / 100.0;
						const float grow_ratio = crowd_grow_ratio * base_grow_ratio;
						const int roll = rand() % 100;
						// maximum of one plant spread per day. chance of spread comes from crowd aversion, surronding blocked tiles, and base growth rate.
						// example crowd aversion values:
						// 1 - minimal aversion. Ratio of unblocked to blocked translates linearly to crowd ratio, so if 1/2 of tiles are blocked, chance of spread is 1/2 * growth rate.
						// 50 - medium aversion. If 1/2 of tiles are blocked, chance of spread is 1/10 * growth rate.
						// 100 - high aversion. Will generally never spread if even one tile nearby is blocked.
						if (grow_ratio > 0.02f && roll < grow_ratio * 100.0f) {
							const int check_index = rand() % open_tiles;
							const int open_tile_i = open_tile_indeces[check_index];
							Tile * t = tiles_in_range[open_tile_i];
							const int num_sheet_cols = tileset->get_block_sheet_image_cols_by_index(block_index);
							const int sheet_col = rand() % num_sheet_cols;
							// TODO: do we want to randomize sheet row as well?
							const std::pair<int, int> ss_pos(sheet_col, plant->get_entity_sheet_row()); 
							const std::pair<int, int> pos(t->get_tile_pos_x(), t->get_tile_pos_y());
							t->replace_block(this->tileset, block_index, ss_pos, pos);
							Block *b = t->get_block();
							b->set_entity_attribute(GameImage::E_ATTR_BROKEN, 0);
							b->set_entity_attribute(GameImage::E_ATTR_CURRENT_DURABILITY, b->get_entity_attribute(GameImage::E_ATTR_DURABILITY));
							const std::string filename = tileset->get_full_block_sheet_filename(block_index);
							b->set_entity_attribute(GameImage::E_ATTR_PLANT_GROWTH_CURRENT_AGE, 0);
							ImageLoader::get_instance().load_image(filename + b->image_filename_suffix(), *(b->get_image_subsection()));
							b->refresh_mask();
							open_tile_indeces.erase(std::remove(open_tile_indeces.begin(), open_tile_indeces.end(), open_tile_i), open_tile_indeces.end());
						}
					}
				}
			}
	}
	plant->unmark_needs_plant_day_update();
}

void Level::intialize_dimensions()
{
	const int standard_width = STANDARD_LEVEL_GRID_WIDTH;
	const int standard_height = STANDARD_LEVEL_GRID_HEIGHT;
	this->width = standard_width * this->grid_width.value();
	this->height = standard_height * this->grid_height.value();
}

void Level::initialize_empty()
{
	const int tile_width = this->width / TILE_SIZE, tile_height = this->height / TILE_SIZE;
	for (int y = 0; y < tile_height; y++) {
		TileGroup *group = new TileGroup();
		for (int x = 0; x < tile_width; x++) {
			group->add_tile(Tile::null_tile(this->tileset, x, y));
		}
		this->tile_rows.addItem(group);
	}
}

void Level::initialize_tiles()
{
	const int width = this->tile_rows.getItem(0)->get_size(), height = this->tile_rows.size();
	const std::string block_sheet_filename = this->tileset->get_tile_sheet_filename();
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			Tile *t = this->get_tile(x, y);
			Rect *subsection = t->get_bitmap_subsection();	
			const std::string tile_filename = this->tileset->get_full_tile_sheet_filename(t->get_tile_type_index());
			std::pair<int, int> position(t->get_tile_pos_x()*TILE_SIZE, t->get_tile_pos_y()*TILE_SIZE);
			t->set_content(tile_filename, subsection, position);
			Block *b = t->get_block();
			if (b == NULL) {
				continue;
			}
			const std::string block_filename = this->tileset->get_full_block_sheet_filename(b->get_entity_data_index());
			Rect *block_subsection = b->get_bitmap_subsection();
			b->set_content(block_filename, block_subsection, position);
			const std::string block_key = this->tileset->get_block_key(b->get_entity_data_index());
			b->load_entity_effects(this->tileset->get_tile_sheet_filename(), block_key, b->get_entity_sheet_row(), std::pair<int, int>(TILE_SIZE, TILE_SIZE));
			b->refresh_mask();
		}
	}
}

void Level::initialize_blocks()
{

}

void Level::generate_level()
{
	if (!(this->gen_data.get_should_generate())) {
		return;
	}
	this->generate_tiles();
	this->generate_paths();
	this->generate_entity_groups();
	this->generate_blocks();
	
	// reload things as necessary
	this->initialize_tiles();	
	this->load_tile_edges();
	this->draw_tile_edge_bitmaps();
}

void Level::initialize_entity_groups()
{
	const int size = this->entity_groups.size();
	for (int i = 0; i < size; i++) {
		EntityGroup *eg = this->entity_groups.getItem(i);
		this->initialize_entity_group(eg);
	}
}

void Level::initialize_entity_group(EntityGroup *eg)
{
	EntityGroupData *egd = this->tileset->get_entity_group_data(eg->get_entity_group_name());
	const int eg_index = egd->get_entity_group_index();
	const std::pair<int, int> ss_offset(eg->get_entity_sheet_col(), eg->get_entity_sheet_row());
	const std::pair<int, int> root_off = this->tileset->get_entity_group_root_offset(eg_index);
	const std::pair<int, int> collide_off = this->tileset->get_entity_group_collide_offset(eg_index);
	const std::pair<int, int> collide_dim = this->tileset->get_entity_group_collide_dimensions(eg_index);
	const std::pair<int, int> center_off = this->tileset->get_entity_group_center_offset(eg_index);
	const std::vector<EntityComponentData*> comp_data = tileset->get_entity_group_components(eg_index);
	const std::pair<int, int> root_pos = eg->get_root_pos();
	const std::pair<int, int> group_pos(root_pos.first - root_off.first, root_pos.second - root_off.second);
	const std::string sheet_filename = this->tileset->get_tile_sheet_filename();
	const std::string entity_group_name = eg->get_entity_group_name();
	const std::pair<int, int> entity_group_image_dimensions = this->tileset->get_entity_group_image_dimensions(entity_group_name);
	std::vector<Entity*> entity_list;
	const int comp_size = comp_data.size();
	for (int comp_index = 0; comp_index < comp_size; comp_index++) {
		EntityComponentData *data = comp_data[comp_index];
		const std::string comp_filename = sheet_filename + "/entity_groups/" + entity_group_name + "_" + data->name.value();
		Rect* ss_offset_rect = new Rect(
			ss_offset.first*entity_group_image_dimensions.first,
			ss_offset.second*entity_group_image_dimensions.second,
			entity_group_image_dimensions.first,
			entity_group_image_dimensions.second);

		ImageLoader::get_instance().load_image(comp_filename, *ss_offset_rect);
		Entity* e = new Entity();
		e->set_content(comp_filename, ss_offset_rect, group_pos);
		e->set_rect(group_pos.first, group_pos.second,
			entity_group_image_dimensions.first, entity_group_image_dimensions.second);
		e->set_entity_attributes(data->get_attributes());
		entity_list.push_back(e);
	}
	eg->set_entities(entity_list);
	eg->set_solid(true); //temp. consider making a set of attributes for the entire group and including solid if necessary
	eg->set_rect(group_pos.first, group_pos.second,
		entity_group_image_dimensions.first, entity_group_image_dimensions.second);
	eg->set_center_offset(center_off);
	eg->set_collide_offset(collide_off);
	eg->set_collide_dimensions(collide_dim);
	eg->load_mask(sheet_filename + "/entity_groups/" + eg->get_entity_group_name());
}

void Level::initialize_tiled_images()
{
	const int layer_count = this->tiled_image_layers.size();
	for (int i = 0; i < layer_count; i++) {
		TiledImageLayer * layer = this->tiled_image_layers.getItem(i);
		layer->initialize_tiled_images(this->tileset->get_tile_sheet_filename());
	}
}

void Level::initialize_spawners()
{
	const int size = this->spawners.size();
	for (int i = 0; i < size; i++) {
		Spawner *s = this->spawners.getItem(i);
		Rect *subsection = s->get_bitmap_subsection();
		const std::string filename = this->tileset->get_full_spawner_sheet_filename(s->get_entity_data_index());
		std::pair<int, int> position(s->get_entity_starting_pos_x(), s->get_entity_starting_pos_y());
		s->set_content(filename, subsection, position);
		s->set_rect(position.first, position.second,
			TILE_SIZE, TILE_SIZE);
	}
}

void Level::initialize_path_nodes()
{
	const int size = this->path_nodes.size();
	for (int i = 0; i < size; i++) {
		PathNode *n = this->path_nodes.getItem(i);
		Rect *subsection = n->get_bitmap_subsection();
		const std::string filename = this->tileset->get_full_path_node_sheet_filename(n->get_entity_data_index());
		std::pair<int, int> position(n->get_entity_starting_pos_x(), n->get_entity_starting_pos_y());
		n->set_content(filename, subsection, position);
		n->set_rect(position.first, position.second,
			TILE_SIZE, TILE_SIZE);
	}
}

void Level::clear_level()
{
	this->tile_rows.Clear();
	this->beings.clear();
	this->entity_groups.Clear();
	this->tiled_image_layers.Clear();
	this->initialize_empty();
}

void Level::remove_tile_edges()
{
	int tile_rows = this->tile_rows.size();
	int tile_cols = this->tile_rows.getItem(0)->get_size();
	for (int y = 0; y < tile_rows; y++) {
		for (int x = 0; x < tile_cols; x++) {
			Tile *t = this->get_tile(x, y);
			//remove any edges we already have
			t->remove_edges();
		}
	}
}

void Level::load_tile_edges()
{
	const int tile_rows = this->tile_rows.size();
	const int tile_cols = this->tile_rows.getItem(0)->get_size();
	// map of sheet row/priority to groups of tiles with that priority
	std::map<std::vector<int>, std::map<int, bool> > edge_map;
	for (int y = 0; y < tile_rows; y++) {
		for (int x = 0; x < tile_cols; x++) {
			edge_map.clear();
			Tile *t = this->get_tile(x, y);
			const int x_off1 = x > 0 ? -1 : 0;
			const int y_off1 = y > 0 ? -1 : 0;
			const int x_off2 = x < tile_cols - 1 ? 1 : 0;
			const int y_off2 = y < tile_rows - 1 ? 1 : 0;
			for (int y_off = y_off1; y_off <= y_off2; y_off++) {
				for (int x_off = x_off1; x_off <= x_off2; x_off++) {
					if (y_off == 0 && x_off == 0) continue;
					Tile *check_tile = this->get_tile(x + x_off, y + y_off);
					const int priority = tileset->get_edge_priority(check_tile->get_tile_type_index());
					const int row = 0; //temp
					const int tile_index = check_tile->get_tile_type_index();
					std::vector<int> edge_map_key{ priority, row, tile_index};
					auto it = edge_map.find(edge_map_key);
					if (it == edge_map.end()) {
						std::map<int, bool> sub_map;
						edge_map[edge_map_key] = sub_map;
					}
					// little trick to map x_off, y_off combinations to the enum directional values in Tile.h
					const int dir_val = (y_off + 1) * 3 + (x_off + 1);
					edge_map[edge_map_key][dir_val] = priority > tileset->get_edge_priority(t->get_tile_type_index());// t->get_edge_priority();
				}
			}
			// each iteration of this loop represents a different priority
			for (const auto& edge_data: edge_map) {
				std::vector<int> edge_key = edge_data.first;
				std::map<int, bool> sub_map = edge_data.second;
				const int row = edge_key[1];
				const int tile_index = edge_key[2];
				std::string tile_key = this->tileset->get_tile_key(tile_index);
				if (sub_map[TILE_UP]
					&& sub_map[TILE_DOWN]
					&& sub_map[TILE_LEFT]
					&& sub_map[TILE_RIGHT]) {
					add_edge_to_tile(t, row, TILE_CENTER, tile_key);
				} else {
					if (sub_map[TILE_UP]) add_edge_to_tile(t, row, TILE_UP, tile_key);
					if (sub_map[TILE_DOWN]) add_edge_to_tile(t, row, TILE_DOWN, tile_key);
					if (sub_map[TILE_LEFT]) add_edge_to_tile(t, row, TILE_LEFT, tile_key);
					if (sub_map[TILE_RIGHT]) add_edge_to_tile(t, row, TILE_RIGHT, tile_key);
					if (sub_map[TILE_UP_LEFT] && !sub_map[TILE_UP] && !sub_map[TILE_LEFT]) add_edge_to_tile(t, row, TILE_UP_LEFT, tile_key);
					if (sub_map[TILE_UP_RIGHT] && !sub_map[TILE_UP] && !sub_map[TILE_RIGHT]) add_edge_to_tile(t, row, TILE_UP_RIGHT, tile_key);
					if (sub_map[TILE_DOWN_LEFT] && !sub_map[TILE_DOWN] && !sub_map[TILE_LEFT]) add_edge_to_tile(t, row, TILE_DOWN_LEFT, tile_key);
					if (sub_map[TILE_DOWN_RIGHT] && !sub_map[TILE_DOWN] && !sub_map[TILE_RIGHT]) add_edge_to_tile(t, row, TILE_DOWN_RIGHT, tile_key);
				}
			}
		}
	}
}

void Level::draw_tile_edge_bitmaps()
{
	const int width = this->tile_rows.getItem(0)->get_size(), height = this->tile_rows.size();
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			Tile *t = this->get_tile(x, y);
			t->reset(this->tileset, t);
			if (t->has_edges()) {
				std::vector<TileEdge*> tile_edges = t->get_tile_edges();
				for (TileEdge *edge : tile_edges) {
					const std::string filename = this->tileset->get_tile_sheet_filename() + "/tiles/edges/" + edge->tile_key.c_str();
					this->draw_edge_tile_onto_bitmap(*t, filename, edge->row_index.value(), edge->direction_index.value());
				}
			}
		}
	}
}

void Level::clear_all_beings()
{
	this->beings.clear();
}

void Level::save_to_xml()
{
	FileManager file_manager;
	const std::string filename = "resources/load/dungeons/" + this->dungeon_filename;
	file_manager.replace_xml_content(filename, "SerializableClass", "LevelKey", this->id, this->toXML());
}

//TODO: how to unload level to avoid memory leak?
void Level::unload_content()
{
	//FIXME: Figure out why there's a memory leak here
	for (int i = 0; i < tile_rows.size(); i++) {
		for (int j = 0; j < tile_rows.getItem(i)->get_size(); j++) {
			this->get_tile(j, i)->unload_content();
		}
	}
	this->tile_rows.Clear();
	this->entity_groups.Clear();
	beings.clear();
}

void Level::update(GlobalTime * time, const int game_mode)
{
	std::pair<int, int> dimensions = get_dimensions();
	const int b_size = beings.size();
	for (int i = 0; i < b_size; i++) {
		if (beings[i]) {	// note that the player's update is called here, so we don't need to call it above.
			std::vector<Entity*> interactables = get_interactables(beings[i]);
			std::vector<Tile*> tiles = get_nearby_tiles(beings[i]);
			beings[i]->update(this, time, game_mode);
		}
		else {
			// TODO: better error handling
			std::cout << "NULL BEING" << std::endl;
		}
	}
	const int height = tile_rows.size();
	for (int y = 0; y < height; y++) {
		const int width = tile_rows.getItem(y)->get_size();
		for (int x = 0; x < width; x++) {
			Block *b = this->get_tile(x, y)->get_block();
			if (b) {
				b->update();
			}
		}
	}
}

void Level::draw(ALLEGRO_DISPLAY * display, std::pair<int, int> offset)
{
	const int max_layer_index = this->tiled_image_layers.size();
	std::pair<int, int> off = offset;
	const int start_x = std::max(0, (-1 * off.first) / TILE_SIZE - 1);
	const int start_y = std::max(0, (-1 * off.second) / TILE_SIZE - 1);
	const int x_size = this->tile_rows.getItem(0)->get_size(), y_size = this->tile_rows.size();
	const int end_x = std::min(x_size, start_x + al_get_display_width(display) / TILE_SIZE + 3);
	const int end_y = std::min(y_size, start_y + al_get_display_height(display) / TILE_SIZE + 3);

	// tiles: layer 0
	for (int y = start_y; y < end_y; y++) {
		for (int x = start_x; x < end_x; x++) {
			this->get_tile(x, y)->draw(display, off.first, off.second);
		}
	}

	// layers 1-9
	for (int layer_index = Level::LAYER_INDEX_TILES + 1; 
		layer_index < std::min(Level::LAYER_INDEX_BLOCKS, max_layer_index); layer_index++) {
		this->draw_tiled_images(display, offset, layer_index);
	}

	// blocks: layer 10
	for (int y = start_y; y < end_y; y++) {
		for (int x = start_x; x < end_x; x++) {
			this->get_tile(x, y)->draw_block(display, off.first, off.second);
		}
	}

	// layers 11-19
	for (int layer_index = Level::LAYER_INDEX_BLOCKS + 1;
		layer_index < std::min(Level::LAYER_INDEX_BEINGS, max_layer_index); layer_index++) {
		this->draw_tiled_images(display, offset, layer_index);
	}
	std::vector<Entity *> draw_entities;
	const int eg_size = this->entity_groups.size();
	for (int i = 0; i < eg_size; i++) {
		draw_entities.push_back(this->entity_groups.getItem(i));
	}

	// beings includes the player and NPCs
	for (Being * b : this->beings) {
		draw_entities.push_back(b);
	}
	// miscellaneous entities: layer 20
	std::sort(draw_entities.begin(), draw_entities.end(), game_image_center_comparison());
	int size = draw_entities.size();
	for (int i = 0; i < size; i++) {
		draw_entities[i]->draw(display, off.first, off.second);
	}
	// layers 21+
	for (int layer_index = Level::LAYER_INDEX_BEINGS + 1;
		layer_index < max_layer_index; layer_index++) {
		this->draw_tiled_images(display, offset, layer_index);
	}
}

void Level::draw_edge_tile_onto_bitmap(Tile &tile, const std::string edge_filename, const int edge_row, const int dir_key)
{
	
	Rect subsection(dir_key*TILE_SIZE, edge_row*TILE_SIZE, TILE_SIZE, TILE_SIZE);
	ImageLoader::get_instance().load_image(edge_filename, subsection);
	tile.add_additional_image_layer(edge_filename, subsection);
}

void Level::add_edge_to_tile(Tile * tile, const int edge_row, const int dir_key, const std::string tile_key)
{
	tile->add_edge(edge_row, dir_key, tile_key);
}

void Level::add_being(Being * b)
{
	beings.push_back(b);
}

void Level::remove_player()
{
	remove_beings(PLAYER);
}

void Level::remove_being(Being * b)
{
	const int size = beings.size();
	for (int i = size - 1; i >= 0; i--) {
		if (beings[i] == b) {
			beings.erase(std::remove(beings.begin(), beings.end(), beings[i]), beings.end());
			return;
		}
	}
}

void Level::remove_beings(const int type)
{
	const int size = beings.size();
	for (int i = size - 1; i >= 0; i--) {
		if (beings[i]->get_type() == type) {
			beings.erase(std::remove(beings.begin(), beings.end(), beings[i]), beings.end());
		}
	}
}

Being * Level::get_player()
{
	const int size = beings.size();
	for (int i = 0; i < size; i++) {
		if (beings[i]->get_type() == PLAYER) {
			return beings[i];
		}
	}
	return NULL;
}

std::vector<Entity*> Level::get_interactables(Entity *entity)
{
	return this->get_interactables(entity, false);
}

std::vector<Entity*> Level::get_interactables(Entity * entity, const bool ignore_moving_obstacles)
{
	std::vector<Entity*> interactables;
	int size = this->entity_groups.size();
	for (int i = 0; i < size; i++) {
		Entity *e = entity_groups.getItem(i);
		if (e && e != entity) {
			interactables.push_back(e);
		}
	}
	if (!ignore_moving_obstacles) {
		if (entity->get_type() != PLAYER) {
			interactables.push_back(this->get_player());
		}
		for (Being * b : this->beings) {
			if (b && b != entity) {
				interactables.push_back(b);
			}
		}
	}
	return interactables;
}

std::vector<Entity*> Level::get_moving_interactables(Entity * entity)
{
	std::vector<Entity*> interactables;
	if (entity->get_type() != PLAYER) {
		interactables.push_back(this->get_player());
	}
	for (Being * b : this->beings) {
		if (b && b != entity) {
			interactables.push_back(b);
		}
	}
	return interactables;
}

std::vector<Tile*> Level::get_nearby_tiles(Entity *entity)
{
	return this->get_tiles_in_range(entity, 2);
}

std::vector<Tile*> Level::get_tiles_in_range(Entity * entity, const int range)
{
	std::vector<Tile*> nearby_tiles;
	const int xpos = entity->get_x(), ypos = entity->get_y(), width = entity->get_width(), height = entity->get_height();
	return get_tiles_in_range(xpos/TILE_SIZE, ypos/TILE_SIZE, width / TILE_SIZE, height / TILE_SIZE, range);
}

std::vector<Tile*> Level::get_tiles_in_range(const int tx, const int ty, const int t_width, const int t_height, const int range)
{
	std::vector<Tile*> nearby_tiles;
	int tx_max = this->tile_rows.getItem(0)->get_size(),
		ty_max = this->tile_rows.size();
	int tx1 = std::max(0, tx - range), ty1 = std::max(0, ty - range),
		tx2 = std::min(tx_max, tx + t_width + range), ty2 = std::min(ty_max, ty + t_height + range);
	for (int y = ty1; y < ty2; y++) {
		for (int x = tx1; x < tx2; x++) {
			nearby_tiles.push_back(this->get_tile(x, y));
		}
	}
	return nearby_tiles;
}

void Level::remove_tile(std::pair<int, int> pos)
{
	Tile * replacing_tile = Tile::null_tile(this->tileset, pos.first, pos.second);
	this->set_tile(replacing_tile, pos);
}

void Level::remove_block(std::pair<int, int> pos)
{
	Tile * tile = this->get_tile(pos.first, pos.second);
	if (tile != NULL) {
		tile->remove_block();
	}
}

void Level::remove_entity_group(std::pair<int, int> pos)
{
	const int size = this->entity_groups.size();
	for (int i = 0; i < size; i++) {
		EntityGroup *eg = this->entity_groups.getItem(i);
		if (eg != NULL && eg->contains_point(pos.first*TILE_SIZE, pos.second*TILE_SIZE)) {
			eg->unload_content();
			this->entity_groups.removeItem(i);
			break;
		}
	}
}

bool Level::remove_tiled_image(const std::pair<int, int> pos, const int layer_index)
{
	bool did_remove = false;
	const int layer_count = this->tiled_image_layers.size();
	if (layer_index >= 0 && layer_index < layer_count) {
		TiledImageLayer * layer = this->tiled_image_layers.getItem(layer_index);
		did_remove = layer->remove_tiled_image(pos);
	}
	return did_remove;
}

void Level::remove_spawner(const std::pair<int, int> pos)
{
	const int size = this->spawners.size();
	for (int i = 0; i < size; i++) {
		Spawner *s = this->spawners.getItem(i);
		if (s != NULL && s->contains_point(pos.first*TILE_SIZE, pos.second*TILE_SIZE)) {
			this->spawners.removeItem(i);
			s->unload_content();
			break;
		}
	}
}

void Level::remove_path_node(const std::pair<int, int> pos)
{
	const int size = this->path_nodes.size();
	for (int i = 0; i < size; i++) {
		PathNode *n = this->path_nodes.getItem(i);
		if (n != NULL && n->contains_point(pos.first*TILE_SIZE, pos.second*TILE_SIZE)) {
			this->path_nodes.removeItem(i);
			n->unload_content();
			break;
		}
	}
}

void Level::replace_tile(int tile_index, std::pair<int, int> ss_pos, std::pair<int, int> pos)
{
	Tile * replacing_tile = new Tile(this->tileset, pos.first, pos.second, 
		tile_index, ss_pos.first, ss_pos.second);
	this->set_tile(replacing_tile, pos);
}

void Level::replace_block(int block_index, std::pair<int, int> ss_pos, std::pair<int, int> pos)
{
	Tile * tile = this->get_tile(pos.first, pos.second);
	if (tile != NULL) {
		tile->replace_block(this->tileset, block_index, ss_pos, pos);
	}
}

void Level::add_entity_group(int eg_index, std::pair<int, int> ss_pos, std::pair<int, int> pos)
{
	std::pair<int, int> pixel_pos(pos.first*TILE_SIZE, pos.second*TILE_SIZE);
	const std::string filename_start = this->tileset->get_tile_sheet_filename();
	EntityGroup * eg = this->create_entity_group(filename_start, eg_index, ss_pos, pixel_pos);
	this->initialize_entity_group(eg);
	this->entity_groups.addItem(eg);
}

void Level::add_tiled_image(const int ti_index, const std::pair<int, int> ss_pos, const std::pair<int, int> pos, const int layer_index)
{
	const std::pair<int, int> pixel_pos(pos.first*TILE_SIZE, pos.second*TILE_SIZE);
	const std::string filename_start = this->tileset->get_tile_sheet_filename();
	TiledImage * ti = this->create_tiled_image(filename_start, ti_index, ss_pos, pixel_pos);
	const std::string full_filename = this->tileset->get_full_tiled_image_sheet_filename(ti_index);
	Rect *subsection = new Rect(ss_pos.first*TILE_SIZE, ss_pos.second*TILE_SIZE, TILE_SIZE, TILE_SIZE);
	ti->set_content(full_filename, subsection, pixel_pos);
	ti->set_not_empty();
	int layer_count = this->tiled_image_layers.size();
	while (layer_count <= layer_index) {
		this->tiled_image_layers.addItem(new TiledImageLayer());
		layer_count++;
	}
	TiledImageLayer * layer = this->tiled_image_layers.getItem(layer_index);
	layer->add_tiled_image(ti);
}

TiledImage * Level::create_tiled_image(std::string filename_start, int index, std::pair<int, int> ss_pos, std::pair<int, int> pos)
{
	TiledImage * tiled_image = new TiledImage();
	tiled_image->set_starting_pos(pos.first, pos.second);
	tiled_image->set_sheet_pos(ss_pos.first, ss_pos.second);
	tiled_image->set_tiled_image_key(index);
	const std::string name = this->tileset->get_tiled_image_name_by_index(index);
	tiled_image->set_tiled_image_name(name);
	return tiled_image;
}

void Level::add_spawner(const int spawner_index, const std::pair<int, int> ss_pos, const std::pair<int, int> pos)
{
	std::pair<int, int> pixel_pos(pos.first*TILE_SIZE, pos.second*TILE_SIZE);
	const std::string filename_start = this->tileset->get_tile_sheet_filename();
	Spawner * s = this->create_spawner(filename_start, spawner_index, ss_pos, pixel_pos);
	this->spawners.addItem(s);
}

Spawner * Level::create_spawner(std::string filename_start, int index, std::pair<int, int> ss_pos, std::pair<int, int> pos)
{
	EntityData* spawner_data = tileset->get_spawner_data_by_index(index);
	const std::string filename = filename_start + "/spawners/" + spawner_data->get_entity_data_key();
	Spawner * spawner = new Spawner();
	spawner->set_entity_data_index(index);
	Rect * ss_offset_rect = new Rect(TILE_SIZE*ss_pos.first, 
		TILE_SIZE*ss_pos.second, 
		TILE_SIZE*(ss_pos.first + 1), 
		TILE_SIZE*(ss_pos.second + 1));
	spawner->set_content(filename, ss_offset_rect, pos);
	spawner->set_rect(pos.first, pos.second,
		TILE_SIZE, TILE_SIZE);
	spawner->set_entity_sheet_offset(ss_pos.first, ss_pos.second);
	return spawner;
}

void Level::add_path_node(const int path_node_index, const std::pair<int, int> ss_pos, const std::pair<int, int> pos)
{
	std::pair<int, int> pixel_pos(pos.first*TILE_SIZE, pos.second*TILE_SIZE);
	const std::string filename_start = this->tileset->get_tile_sheet_filename();
	PathNode * n = this->create_path_node(filename_start, path_node_index, ss_pos, pixel_pos);
	this->path_nodes.addItem(n);
}

PathNode * Level::create_path_node(const std::string filename_start, const int index, const std::pair<int, int> ss_pos, const std::pair<int, int> pos)
{
	EntityData* node_data = tileset->get_path_node_data_by_index(index);
	const std::string filename = filename_start + "/path_nodes/" + node_data->get_entity_data_key();
	PathNode * node = new PathNode();
	node->set_entity_data_index(index);
	Rect * ss_offset_rect = new Rect(TILE_SIZE*ss_pos.first,
		TILE_SIZE*ss_pos.second,
		TILE_SIZE*(ss_pos.first + 1),
		TILE_SIZE*(ss_pos.second + 1));
	node->set_content(filename, ss_offset_rect, pos);
	node->set_rect(pos.first, pos.second,
		TILE_SIZE, TILE_SIZE);
	node->set_entity_sheet_offset(ss_pos.first, ss_pos.second);
	return node;
}

const bool Level::has_path_node_for_key(const std::string node_key)
{
	return this->find_path_node_with_key(node_key) != NULL;
}

PathNode * Level::find_path_node_with_key(const std::string node_key)
{
	const int size = this->path_nodes.size();
	for (int i = 0; i < size; i++) {
		PathNode * n = this->path_nodes.getItem(i);
		if (n && n->get_node_id() == node_key) {
			return n;
		}
	}
	return NULL;
}

const bool Level::has_any_path_node()
{
	return this->find_any_path_node() != NULL;
}

PathNode * Level::find_any_path_node()
{
	const int size = this->path_nodes.size();
	if (size > 0) {
		return this->path_nodes.getItem(0);
	}
	return NULL;
}

const bool Level::has_spawner_for_key(const std::string spawn_key)
{
	return this->spawner_for_key(spawn_key) != NULL;
}

Spawner * Level::spawner_for_key(const std::string spawn_key)
{
	const int size = this->spawners.size();
	for (int i = 0; i < size; i++) {
		Spawner * s = this->spawners.getItem(i);
		if (s && s->get_spawner_id() == spawn_key) {
			return s;
		}
	}
	return NULL;
}

void Level::add_npc_at_spawner(NPC * npc, const std::string spawn_key)
{
	Spawner * spawner = this->spawner_for_key(spawn_key);
	if (spawner) {
		npc->set_starting_pos(spawner->get_x(), spawner->get_y());
		npc->load_content_from_attributes();
		npc->set_current_level_key(this->get_filename());
		npc->cancel_current_pathing();
		this->add_being(npc);
	}
}

EntityGroup * Level::create_entity_group(std::string filename_start, int index, std::pair<int, int> ss_pos, std::pair<int, int> pos)
{
	std::pair<int, int> root_off = tileset->get_entity_group_root_offset(index);
	std::pair<int, int> collide_off = tileset->get_entity_group_collide_offset(index);
	std::pair<int, int> collide_dim = tileset->get_entity_group_collide_dimensions(index);
	std::pair<int, int> center_off = tileset->get_entity_group_center_offset(index);
	EntityGroupData* group_data = tileset->get_entity_group_data_by_index(index);
	std::vector<EntityComponentData*> comp_data = tileset->get_entity_group_components(index);
	const std::pair<int, int> root_pos(pos.first, pos.second);
	const std::pair<int, int> group_pos(root_pos.first - root_off.first, root_pos.second - root_off.second);
	const std::pair<int, int> entity_group_image_dimensions = this->tileset->get_entity_group_image_dimensions_by_index(index);
	std::vector<Entity*> entity_list;
	// load the images separately for each component
	int comp_size = comp_data.size();
	for (int comp_index = 0; comp_index < comp_size; comp_index++) {
		EntityComponentData *data = comp_data[comp_index];
		std::string comp_filename = filename_start + "/entity_groups/" + group_data->get_entity_group_name() + "_" + data->name.value();
		Rect* ss_offset_rect = new Rect(
			ss_pos.first*entity_group_image_dimensions.first,
			ss_pos.second*entity_group_image_dimensions.second,
			entity_group_image_dimensions.first,
			entity_group_image_dimensions.second);
		Entity* e = new Entity();
		e->set_content(comp_filename, ss_offset_rect, group_pos);
		e->set_rect(group_pos.first, group_pos.second,
			entity_group_image_dimensions.first, entity_group_image_dimensions.second);
		e->set_entity_attributes(data->get_attributes());
		entity_list.push_back(e);
	}
	EntityGroup *e_group = new EntityGroup();
	e_group->set_sheet_pos(ss_pos.first, ss_pos.second);
	e_group->set_entity_group_name(group_data->get_entity_group_name());
	e_group->set_entities(entity_list);
	e_group->set_solid(true);
	e_group->set_rect(group_pos.first, group_pos.second,
		entity_group_image_dimensions.first, entity_group_image_dimensions.second);
	e_group->set_center_offset(center_off);
	e_group->set_root_pos(root_pos);
	e_group->set_collide_offset(collide_off);
	e_group->set_collide_dimensions(collide_dim);
	e_group->load_mask(filename_start + "/entity_groups/" + group_data->get_entity_group_name());
	return e_group;
}

void Level::set_tile(Tile * tile, std::pair<int, int> pos)
{

	while (pos.second >= this->tile_rows.size()) {
		this->tile_rows.addItem(new TileGroup());
	}
		TileGroup * tg = this->tile_rows.getItem(pos.second);
		if (tg != NULL) {
			tg->set_tile(this->tileset, tile, pos.first);
		}
	
}

Tile * Level::get_tile(const int x, const int y)
{
	if (y < 0 
		|| y > this->tile_rows.size()) {
		return NULL;
	}
	TileGroup *tile_row = this->tile_rows.getItem(y);
	if (x < 0 
		|| x > tile_row->get_size()) {
		return NULL;
	}
	return tile_row->get_tile(x);
}

// checks to see whether a space is passable
bool Level::passable_at(const int x, const int y)
{
	if (x < 0 || x > width || y < 0 || y > width) {
		return false;
	}
	return true;
}

TileSet * Level::get_tileset()
{
	return this->tileset;
}

void Level::set_tileset_key(std::string value)
{
	this->tileset_key = value.c_str();
}

const std::string Level::get_tileset_key()
{
	return this->tileset_key.value();
}

EntityGroup * Level::entity_group_at_tile_pos(const std::pair<int, int> pos)
{
	return this->entity_group_at_tile_pos(pos, false);
}

EntityGroup * Level::entity_group_at_tile_pos(const std::pair<int, int> tpos,  const bool root_only)
{
	const int size = this->entity_groups.size();
	const std::pair<int, int> pos(tpos.first*TILE_SIZE, tpos.second*TILE_SIZE);
	for (int i = 0; i < size; i++) {
		EntityGroup *eg = this->entity_groups.getItem(i);
		if (eg != NULL) {
			if (root_only) {
				const std::pair<int, int> eg_pos(eg->get_x(), eg->get_y());
				const std::pair<int, int> root_pos = eg->get_root_pos();
				const std::pair<int, int> eg_dim(eg->get_width(), eg->get_height());
				if (pos.first >= root_pos.first
					&& pos.second >= root_pos.second
					&& pos.first < eg_pos.first + eg_dim.first
					&& pos.second < eg_pos.second + eg_dim.second) {
					return eg;
				}
			} else if (eg != NULL && eg->contains_point(pos.first, pos.second)) {
				return eg;
			}
		}
	}
	return NULL;
}

Spawner * Level::spawner_at_tile_pos(const std::pair<int, int> pos)
{
	const int size = this->spawners.size();
	for (int i = 0; i < size; i++) {
		Spawner *s = this->spawners.getItem(i);
		if (s != NULL && s->contains_point(pos.first*TILE_SIZE, pos.second*TILE_SIZE)) {
			return s;
		}
	}
	return NULL;
}
PathNode * Level::path_node_at_tile_pos(const std::pair<int, int> pos)
{
	const int size = this->path_nodes.size();
	for (int i = 0; i < size; i++) {
		PathNode *n = this->path_nodes.getItem(i);
		if (n != NULL && n->contains_point(pos.first*TILE_SIZE, pos.second*TILE_SIZE)) {
			return n;
		}
	}
	return NULL;
}

std::vector<PathNode*> Level::get_path_nodes()
{
	std::vector<PathNode*> nodes;
	const int size = this->path_nodes.size();
	for (int i = 0; i < size; i++) {
		PathNode * node = this->path_nodes.getItem(i);
		if (node) {
			nodes.push_back(node);
		}
	}
	return nodes;
}

std::string Level::get_dungeon_filename()
{
	return this->dungeon_filename;
}

void Level::set_dungeon_filename(std::string value)
{
	this->dungeon_filename = value;
}

std::string Level::get_filename()
{
	return this->map_filename;
}

void Level::set_filename(std::string value)
{
	this->map_filename = value;
}

std::string Level::get_id()
{
	return id;
}

std::pair<int, int> Level::get_dimensions()
{
	return std::make_pair(width, height);
}

int Level::get_width()
{
	return width;
}

int Level::get_height()
{
	return height;
}

void Level::set_grid_x(const int value)
{
	this->grid_x = value;
}

int Level::get_grid_x()
{
	return this->grid_x.value();
}

void Level::set_grid_y(const int value)
{
	this->grid_y = value;
}

int Level::get_grid_y()
{
	return this->grid_y.value();
}

void Level::set_grid_width(const int value)
{
	this->grid_width = value;
	this->width = value * STANDARD_LEVEL_GRID_WIDTH;
}

int Level::get_grid_width()
{
	return width/STANDARD_LEVEL_GRID_WIDTH;
}

void Level::set_grid_height(const int value)
{
	this->grid_height = value;
	this->height = value * STANDARD_LEVEL_GRID_HEIGHT;
}

int Level::get_grid_height()
{
	return height/STANDARD_LEVEL_GRID_HEIGHT;
}

// level editor methods
void Level::draw_tiles_onto_bitmap(ALLEGRO_BITMAP * bitmap, Rect &subsection)
{
	ALLEGRO_BITMAP *display = al_get_target_bitmap();
	al_set_target_bitmap(bitmap);
	const int x1 = subsection.x / TILE_SIZE, y1 = subsection.y / TILE_SIZE;
	const int y_size = this->tile_rows.size();
	for (int y = y1; y < y_size; y++) {
		const int x_size = this->tile_rows.getItem(y)->get_size();
		for (int x = x1; x < x_size; x++) {
			Tile * t = this->get_tile(x, y);
			ALLEGRO_BITMAP *tile_bitmap = ImageLoader::get_instance().get_current_image(t);
			float dx = x * TILE_SIZE - subsection.x;
			float dy = y * TILE_SIZE - subsection.y;
			al_draw_bitmap(tile_bitmap, dx, dy, 0);
			// draw tile edges
			std::vector<ALLEGRO_BITMAP*> additional_image_layers = t->get_additional_image_layers();
			int size = additional_image_layers.size();
			for (int i = 0; i < size; i++) {
				al_draw_bitmap(additional_image_layers[i], dx, dy, 0);
			}
		}
	}
	al_set_target_bitmap(display);
}

void Level::draw_blocks_onto_bitmap(ALLEGRO_BITMAP * bitmap, Rect& subsection)
{
	ALLEGRO_BITMAP *display = al_get_target_bitmap();
	al_set_target_bitmap(bitmap);
	const int x1 = subsection.x / TILE_SIZE, y1 = subsection.y / TILE_SIZE;
	const int x_size = this->tile_rows.getItem(0)->get_size(), y_size = this->tile_rows.size();
	for (int y = y1; y < y_size; y++) {
		for (int x = x1; x < x_size; x++) {
			Tile * t = this->get_tile(x, y);
			Block * b = t->get_block();
			if (b != NULL) {
				ALLEGRO_BITMAP *tile_bitmap = ImageLoader::get_instance().get_current_image(b);
				float dx = x * TILE_SIZE - subsection.x;
				float dy = y * TILE_SIZE - subsection.y;
				al_draw_bitmap(tile_bitmap, dx, dy, 0);	
			}
		}
	}
	al_set_target_bitmap(display);
}

void Level::draw_entity_groups_onto_bitmap(ALLEGRO_BITMAP * bitmap, Rect &subsection)
{
	ALLEGRO_BITMAP *display = al_get_target_bitmap();
	al_set_target_bitmap(bitmap);
	const int size = this->entity_groups.size();
	for (int i = 0; i < size; i++) {
		EntityGroup * eg = this->entity_groups.getItem(i);
		float dx = eg->get_x() - subsection.x;
		float dy = eg->get_y() - subsection.y;
		if (dx + eg->get_width() >= 0 && dy + eg->get_height() >= 0
			&& dx < subsection.width
			&& dy < subsection.height) {
			std::vector<Entity*> entities = eg->get_entities();
			for (Entity *e : entities) {
				ALLEGRO_BITMAP *entity_bitmap = ImageLoader::get_instance().get_current_image(e);//e->get_bitmap();
				al_draw_bitmap(entity_bitmap, dx, dy, 0);
			}
		}
	}
	al_set_target_bitmap(display);
}

void Level::draw_tiled_images_onto_bitmap(ALLEGRO_BITMAP * bitmap, Rect &subsection)
{
	ALLEGRO_BITMAP *display = al_get_target_bitmap();
	al_set_target_bitmap(bitmap);
	const int size = this->tiled_image_layers.size();
	for (int i = 0; i < size; i++) {
		TiledImageLayer * layer = this->tiled_image_layers.getItem(i);
		layer->draw_tiled_images_onto_bitmap(bitmap, subsection);
	}
	al_set_target_bitmap(display);
}

void Level::draw_spawners_onto_bitmap(ALLEGRO_BITMAP * bitmap, Rect &subsection)
{
	ALLEGRO_BITMAP *display = al_get_target_bitmap();
	al_set_target_bitmap(bitmap);
	const int size = this->spawners.size();
	for (int i = 0; i < size; i++) {
		Spawner * s = this->spawners.getItem(i);
		float dx = s->get_x() - subsection.x;
		float dy = s->get_y() - subsection.y;
		if (dx >= 0 && dy >= 0
			&& dx < subsection.width
			&& dy < subsection.height) {
			ALLEGRO_BITMAP *spawner_bitmap = ImageLoader::get_instance().get_current_image(s);
			al_draw_bitmap(spawner_bitmap, dx, dy, 0);
		}
	}
	al_set_target_bitmap(display);
}

void Level::draw_path_nodes_onto_bitmap(ALLEGRO_BITMAP * bitmap, Rect & subsection)
{
	ALLEGRO_BITMAP *display = al_get_target_bitmap();
	al_set_target_bitmap(bitmap);
	const int size = this->path_nodes.size();
	for (int i = 0; i < size; i++) {
		PathNode * n = this->path_nodes.getItem(i);
		float dx = n->get_x() - subsection.x;
		float dy = n->get_y() - subsection.y;
		if (dx >= 0 && dy >= 0
			&& dx < subsection.width
			&& dy < subsection.height) {
			ALLEGRO_BITMAP *node_bitmap = ImageLoader::get_instance().get_current_image(n);
			al_draw_bitmap(node_bitmap, dx, dy, 0);
		}
	}
	al_set_target_bitmap(display);
}
