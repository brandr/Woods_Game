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
	this->Register("TiledImageLayers", &(this->tiled_image_layers));
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
	this->Register("TiledImageLayers", &(this->tiled_image_layers));
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
	this->Register("TiledImageLayers", &(this->tiled_image_layers));
	this->map_filename = level_filename;
	this->dungeon_filename = dungeon_filename;
	this->id = id;
	this->grid_x = grid_x;
	this->grid_y = grid_y;
	this->grid_width = grid_width;
	this->grid_height = grid_height;
	this->width = STANDARD_LEVEL_GRID_WIDTH * grid_width;
	this->height = STANDARD_LEVEL_GRID_HEIGHT * grid_height;
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
	this->Register("TiledImageLayers", &(this->tiled_image_layers));
	this->map_filename = filename;
	this->grid_x = grid_x;
	this->grid_y = grid_y;
	this->grid_width = grid_width;
	this->grid_height = grid_height;
	this->width = STANDARD_LEVEL_GRID_WIDTH * grid_width;
	this->height = STANDARD_LEVEL_GRID_HEIGHT * grid_height;
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
	this->Register("TiledImageLayers", &(this->tiled_image_layers));
	this->grid_x = grid_x;
	this->grid_y = grid_y;
	this->grid_width = grid_width;
	this->grid_height = grid_height;
	this->width = STANDARD_LEVEL_GRID_WIDTH * grid_width;
	this->height = STANDARD_LEVEL_GRID_HEIGHT * grid_height;
}

Level::~Level()
{
	//this->entity_groups.Clear();
	//this->tile_rows.Clear();
	//this->tileset->unload_content();
	//delete this->tileset;
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

void Level::load_from_map()
{
	// new XML loading
	// TODO: once this is done, use it as the "old" map loading method 
	// (in case I want to draw maps manually) and use XML loading in a different method
	this->load_tileset();
	FileManager file_manager;
	std::string filename = "resources/load/maps/" + get_filename() + ".txt";
	const std::string tile_sheet_filename_start = this->tileset->get_tile_sheet_filename();
	const std::string block_sheet_filename_start = this->tileset->get_block_tile_sheet_filename();
	const std::string entity_group_sheet_filename_start = this->tileset->get_entity_group_tile_sheet_filename();
	std::vector<std::string> layers = get_layers();
	int size = layers.size();
	for (int layer_index = 0; layer_index < size; layer_index++) {
		std::string layer_id = layers[layer_index];
		std::vector<std::vector<std::string>> attributes;
		std::vector<std::vector<std::string>> contents;
		file_manager.load_content(filename.c_str(), attributes, contents, layer_id);
		int indexY = 0;
		int size = attributes.size();
		std::string null_tile = "";
		for (int i = 0; i < size; i++) {
			std::pair<int, int> tile(0, 0);
			const int isize = attributes[i].size();
			for (int j = 0; j < isize; j++) {
				// tile loading
				if (attributes[i][j] == "NullTile") {	//consider changing to "default tile" or something
					null_tile = contents[i][j];
				}
				// entity loading
				else if (attributes[i][j] == "StartLayer") {
					const int contents_size = contents[i].size();
					if (layer_id == "tile_layer") {
						TileGroup *tile_group = new TileGroup(indexY);
						tile_rows.addItem(tile_group);
						for (int k = 0; k < contents_size; k++) {
							if (contents[i][k] != null_tile) {
								std::vector<std::string> ss_keys = FileManager::string_to_parts(contents[i][k], ",");
								int ss_col = ::atoi(ss_keys[0].c_str()), ss_row = ::atoi(ss_keys[1].c_str()), ss_type_key = ::atoi(ss_keys[2].c_str());
								Rect* offset_rect = new Rect(ss_col*TILE_SIZE, ss_row*TILE_SIZE, TILE_SIZE, TILE_SIZE);
								const int edge_priority = this->tileset->get_edge_priority(ss_type_key);
								const float speed_mod = this->tileset->get_tile_speed_mod(ss_type_key);
								std::pair<int, int> position(k*TILE_SIZE, indexY*TILE_SIZE);
								Tile *t = new Tile(tileset, k, indexY, ss_type_key, ss_col, ss_row);
								tile_group->add_tile(t);
							} else {
								Rect* offset_rect = new Rect(0, 0, TILE_SIZE, TILE_SIZE);
								std::pair<int, int> position(k*TILE_SIZE, indexY*TILE_SIZE);
								std::string tile_sheet_filename = this->tileset->get_full_tile_sheet_filename(0);
								Tile *t = Tile::null_tile(tileset, k, indexY);
								t->set_content(tile_sheet_filename, offset_rect, position);
								t->set_tile_type_index(0);
								t->set_edge_priority(0);
								t->set_speed_mod(1.0f);
								tile_group->add_tile(t);
							}
						}
					}
					// block loading
					else if (layer_id == "block_layer") {
						for (int k = 0; k < contents_size; k++) {
							if (contents[i][k] != null_tile) {	
								//TODO: allow for animated blocks if necessary
								//TODO: consider refactoring to another method
								std::vector<std::string> ss_keys = FileManager::string_to_parts(contents[i][k], ",");
								int ss_col = ::atoi(ss_keys[0].c_str()), ss_row = ::atoi(ss_keys[1].c_str()), ss_type_key = ::atoi(ss_keys[2].c_str());
								Rect* offset_rect = new Rect(ss_col*TILE_SIZE, ss_row*TILE_SIZE, TILE_SIZE, TILE_SIZE);
								std::pair<int, int> position(k*TILE_SIZE, indexY*TILE_SIZE);
								std::string block_sheet_filename = this->tileset->get_full_block_sheet_filename(ss_type_key);
								const bool solid = tileset->get_block_solid(ss_type_key);
								const bool visible = tileset->get_block_visible(ss_type_key);
								std::map<std::string, int> block_attributes = tileset->get_block_attributes(ss_type_key);
								Tile *t = this->tile_rows.getItem(indexY)->get_tile(k);
								t->initialize_block();
								Block *b = t->get_block();
								b->set_content(block_sheet_filename, offset_rect, position);
								b->set_starting_pos(ss_col*TILE_SIZE, ss_row*TILE_SIZE);
								b->set_entity_data_index(ss_type_key);
								b->set_entity_sheet_offset(ss_col, ss_row);
								b->set_bitmap(ImageLoader::get_instance().get_current_image(b));
								b->set_solid(solid); //will be serialized
								b->set_visible(visible);
								b->set_entity_attributes(block_attributes);
								b->load_entity_effects(block_sheet_filename, ss_row, std::pair<int, int>(TILE_SIZE, TILE_SIZE));	//temp. want to get a different filename eventually
								b->refresh_mask();
							}
						}
					}
					// entity group loading
					else if (layer_id == "entity_group_layer") {
						for (int k = 0; k < contents_size; k++) {
							if (contents[i][k] != null_tile) {
								//...
								std::vector<std::string> ss_keys = FileManager::string_to_parts(contents[i][k], ",");
								const int ss_col = ::atoi(ss_keys[0].c_str()), ss_row = ::atoi(ss_keys[1].c_str()), ss_type_key = ::atoi(ss_keys[2].c_str());
								const std::pair<int, int> eg_ss_pos(ss_col, ss_row);
								const std::pair<int, int> pixel_pos(k*TILE_SIZE, indexY*TILE_SIZE);
								EntityGroup * e_group = this->create_entity_group(entity_group_sheet_filename_start, ss_type_key, eg_ss_pos, pixel_pos);
								/*

								std::pair<int, int> root_off = tileset->get_entity_group_root_offset(ss_type_key);
								std::pair<int, int> center_off = tileset->get_entity_group_center_offset(ss_type_key);
								EntityGroupData* group_data = tileset->get_entity_group_data_by_index(ss_type_key);
								std::vector<EntityComponentData*> comp_data = tileset->get_entity_group_components(ss_type_key);
								std::pair<int, int> root_pos(k*TILE_SIZE, indexY*TILE_SIZE);
								std::pair<int, int> group_pos(root_pos.first - root_off.first, root_pos.second - root_off.second);
								const std::pair<int, int> entity_group_image_dimensions = this->tileset->get_entity_group_image_dimensions_by_index(ss_type_key);
								std::vector<Entity*> entity_list;
								// load the images separately for each component
								int comp_size = comp_data.size();
								for (int comp_index = 0; comp_index < comp_size; comp_index++) {
									EntityComponentData *data = comp_data[comp_index];
									std::string comp_filename = entity_group_sheet_filename_start + "_" + group_data->get_entity_group_name() +  "_" + data->name.value();
									Rect* ss_offset_rect = new Rect(
										ss_col*entity_group_image_dimensions.first,
										ss_row*entity_group_image_dimensions.second,
										entity_group_image_dimensions.first,
										entity_group_image_dimensions.second);
									Entity* e = new Entity();
									e->set_content(comp_filename, ss_offset_rect, group_pos);
									e->set_rect(group_pos.first, group_pos.second,
										entity_group_image_dimensions.first, entity_group_image_dimensions.second);
									e->set_bitmap(ImageLoader::get_instance().get_current_image(e));
									e->set_entity_attributes(data->get_attributes());
									entity_list.push_back(e);
								}
								EntityGroup *e_group = new EntityGroup();
								e_group->set_sheet_pos(ss_col, ss_row);
								e_group->set_entity_group_name(group_data->get_entity_group_name());
								e_group->set_entities(entity_list);
								e_group->set_solid(true);	//temp. consider making a set of attributes for the entire group and including solid if necessary
								e_group->set_rect(group_pos.first, group_pos.second,
									entity_group_image_dimensions.first, entity_group_image_dimensions.second);
								e_group->set_center_offset(center_off);
								e_group->set_root_pos(root_pos);
								e_group->load_mask(entity_group_sheet_filename_start + "_" + group_data->get_entity_group_name());
								add_entity(e_group);
								*/
								this->add_entity(e_group);
								this->entity_groups.addItem(e_group); //allows serialization
							}
						}
					}
					indexY++;
				}
				else if (attributes[i][j] == "EndLayer") {
					if (layer_id == "tile_layer") {
						this->initialize_tiles();
						this->load_tile_edges();
						this->draw_tile_edge_bitmaps();
						this->initialize_entity_groups();
						
					}
				}
			}
		}
	}	
}

void Level::load_from_xml()
{
	this->load_tileset();
	FileManager file_manager;
	const std::string filename = "resources/load/dungeons/" + this->dungeon_filename;
	this->initialize_tiles();	//this also intializes blocks
	this->draw_tile_edge_bitmaps();
	this->initialize_entity_groups();
	this->initialize_tiled_images();
}

void Level::intialize_dimensions()
{
	this->width = STANDARD_LEVEL_GRID_WIDTH * this->grid_width.value();
	this->height = STANDARD_LEVEL_GRID_HEIGHT * this->grid_height.value();
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
	const std::string block_sheet_filename = this->tileset->get_block_tile_sheet_filename();
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			Tile *t = this->get_tile(x, y);
			Rect *subsection = t->get_bitmap_subsection();	
			const std::string tile_filename = this->tileset->get_full_tile_sheet_filename(t->get_tile_type_index());
			std::pair<int, int> position(t->get_tile_pos_x()*TILE_SIZE, t->get_tile_pos_y()*TILE_SIZE);
			t->set_content(tile_filename, subsection, position);
			t->set_bitmap(ImageLoader::get_instance().get_current_image(t));
			Block *b = t->get_block();
			if (b == NULL) {
				continue;
			}
			std::string block_filename = this->tileset->get_full_block_sheet_filename(b->get_entity_data_index());
			Rect *block_subsection = b->get_bitmap_subsection();
			b->set_content(block_filename, block_subsection, position);
			b->set_bitmap(ImageLoader::get_instance().get_current_image(b));
			b->load_entity_effects(block_filename, b->get_entity_sheet_row(), std::pair<int, int>(TILE_SIZE, TILE_SIZE));
			b->refresh_mask();
		}
	}
}

void Level::initialize_blocks()
{

}

void Level::generate_blocks()
{
	const int rows = this->tile_rows.size();
	for (int y = 0; y < rows; y++) {
		TileGroup *row = this->tile_rows.getItem(y);
		const int cols = row->get_size();
		for (int x = 0; x < cols; x++) {
			Tile *t = row->get_tile(x);
		}
	}
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
	std::pair<int, int> ss_offset(eg->get_entity_sheet_col(), eg->get_entity_sheet_row());
	std::pair<int, int> root_off = this->tileset->get_entity_group_root_offset(ss_offset.second);
	std::pair<int, int> center_off = this->tileset->get_entity_group_center_offset(ss_offset.second);
	std::vector<EntityComponentData*> comp_data = tileset->get_entity_group_components(ss_offset.second);
	std::pair<int, int> root_pos = eg->get_root_pos();
	std::pair<int, int> group_pos(root_pos.first - root_off.first, root_pos.second - root_off.second);
	std::vector<Entity*> entity_list;
	std::string entity_group_sheet_filename = this->tileset->get_entity_group_tile_sheet_filename();
	std::string entity_group_name = eg->get_entity_group_name();
	const std::pair<int, int> entity_group_image_dimensions = this->tileset->get_entity_group_image_dimensions(entity_group_name);
	const int comp_size = comp_data.size();
	for (int comp_index = 0; comp_index < comp_size; comp_index++) {
		EntityComponentData *data = comp_data[comp_index];
		std::string comp_filename = entity_group_sheet_filename + "_" + entity_group_name + "_" + data->name.value();
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
		e->set_bitmap(ImageLoader::get_instance().get_current_image(e));
		e->set_entity_attributes(data->get_attributes());
		entity_list.push_back(e);
	}
	eg->set_entities(entity_list);
	eg->draw_bitmap_from_entities(entity_list);
	eg->set_solid(true); //temp. consider making a set of attributes for the entire group and including solid if necessary
	eg->set_rect(group_pos.first, group_pos.second,
		entity_group_image_dimensions.first, entity_group_image_dimensions.second);
	eg->set_center_offset(center_off);
	eg->load_mask(entity_group_sheet_filename + "_" + eg->get_entity_group_name());
	add_entity(eg); //allows serialization
}

void Level::initialize_tiled_images()
{
	const int layer_count = this->tiled_image_layers.size();
	for (int i = 0; i < layer_count; i++) {
		TiledImageLayer * layer = this->tiled_image_layers.getItem(i);
		layer->initialize_tiled_images(this->tileset->get_tiled_image_tile_sheet_filename());
	}
}

void Level::clear_level()
{
	this->tile_rows.Clear();
	this->entities.clear();
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
	int tile_rows = this->tile_rows.size();
	int tile_cols = this->tile_rows.getItem(0)->get_size();
	// map of sheet row/priority to groups of tiles with that priority
	std::map<std::vector<int>, std::map<int, bool> > edge_map;
	for (int y = 0; y < tile_rows; y++) {
		for (int x = 0; x < tile_cols; x++) {
			edge_map.clear();
			Tile *t = this->get_tile(x, y);
			int x_off1 = x > 0 ? -1 : 0;
			int y_off1 = y > 0 ? -1 : 0;
			int x_off2 = x < tile_cols - 1 ? 1 : 0;
			int y_off2 = y < tile_rows - 1 ? 1 : 0;
			for (int y_off = y_off1; y_off <= y_off2; y_off++) {
				for (int x_off = x_off1; x_off <= x_off2; x_off++) {
					if (y_off == 0 && x_off == 0) continue;
					Tile *check_tile = this->get_tile(x + x_off, y + y_off);
					int priority = check_tile->get_edge_priority();
					const int row = 0; //temp
					const int tile_index = check_tile->get_tile_type_index();
					std::vector<int> edge_map_key{ priority, row, tile_index};
					auto it = edge_map.find(edge_map_key);
					if (it == edge_map.end()) {
						std::map<int, bool> sub_map;
						edge_map[edge_map_key] = sub_map;
					}
					// little trick to map x_off, y_off combinations to the enum directional values in Tile.h
					int dir_val = (y_off + 1) * 3 + (x_off + 1);
					edge_map[edge_map_key][dir_val] = priority > t->get_edge_priority();
				}
			}
			// each iteration of this loop represents a different priority
			for (const auto& edge_data: edge_map) {
				std::vector<int> edge_key = edge_data.first;
				std::map<int, bool> sub_map = edge_data.second;
				int row = edge_key[1];
				int tile_index = edge_key[2];
				std::string tile_key = this->tileset->get_tile_key(tile_index);
				if (sub_map[TILE_UP] && sub_map[TILE_DOWN] && sub_map[TILE_LEFT] && sub_map[TILE_RIGHT]) add_edge_to_tile(t, row, TILE_CENTER, tile_key);
				else {
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
					std::string filename = this->tileset->get_edge_tile_sheet_filename() + "_" + edge->tile_key.c_str();
					this->draw_edge_tile_onto_bitmap(*t, filename, edge->row_index.value(), edge->direction_index.value());
				}
			}
		}
	}
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
	//this->tile_rows.Clear();
	//this->entity_groups.Clear();
	/*
	for (int i = 0; i < game_images.size(); i++) {
		if (game_images[i]) {
			//game_images[i]->unload_content();
			delete game_images[i];
		}
	}
	*/
	/*
	for (int i = 0; i < tile_rows.size(); i++) {
		for (int j = 0; j < tile_rows.getItem(i)->get_size(); j++) {
			this->get_tile(j, i)->unload_content();
		}
		tiles[i].clear();
		std::vector<Tile*>().swap(tiles[i]);
	}
	std::vector<std::vector<Tile*>>().swap(tiles);
	*/
	/*
	game_images.clear();
	std::vector<GameImage*>().swap(game_images);
	entities.clear();
	std::vector<Entity*>().swap(entities);
	beings.clear();
	std::vector<Being*>().swap(beings);
	*/
}

void Level::update(int game_mode)
{
	std::pair<int, int> dimensions = get_dimensions();
	int b_size = beings.size();
	for (int i = 0; i < b_size; i++) {
		if (beings[i]) {	// note that the player's update is called here, so we don't need to call it above.
			std::vector<Entity*> interactables = get_interactables(beings[i]);
			std::vector<Tile*> tiles = get_nearby_tiles(beings[i]);
			beings[i]->update(interactables, tiles, dimensions, game_mode);	//TEMP. need to handle gamemode some other way, maybe by accessing it from level or storing it in GameImageManager
			beings[i]->set_bitmap(ImageLoader::get_instance().get_current_image(beings[i]));
		}
		else
			std::cout << "NULL BEING" << std::endl;
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
	/*
	int e_size = entities.size();
	
	for (int i = 0; i < e_size; i++) {
		if (entities[i] && entities[i]->get_entity_attribute(E_ATTR_BROKEN) == 1) {
			//TODO: decide if we need to do anything with broken entities here
		}
	}
	*/
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
	// game images: layer 20
	std::sort(game_images.begin(), game_images.end(), game_image_center_comparison());
	int size = game_images.size();
	for (int i = 0; i < size; i++) {
		game_images[i]->draw(display, off.first, off.second);
	}
	// layers 21+
	for (int layer_index = Level::LAYER_INDEX_BEINGS + 1;
		layer_index < max_layer_index; layer_index++) {
		this->draw_tiled_images(display, offset, layer_index);
	}
	
}

void Level::draw_edge_tile_onto_bitmap(Tile &tile, std::string edge_filename, int edge_row, int dir_key)
{
	
	Rect subsection(dir_key*TILE_SIZE, edge_row*TILE_SIZE, TILE_SIZE, TILE_SIZE);
	ImageLoader::get_instance().load_image(edge_filename, subsection);
	ALLEGRO_BITMAP* edge_image = ImageLoader::get_instance().get_image(edge_filename, subsection);
	tile.draw_onto_bitmap(edge_image);
}

void Level::add_edge_to_tile(Tile * tile, int edge_row, int dir_key, std::string tile_key)
{
	tile->add_edge(edge_row, dir_key, tile_key);
}

void Level::add_entity(Entity * e)
{
	entities.push_back(e);
	game_images.push_back(e);
}

void Level::add_being(Being * b)
{
	beings.push_back(b);
	entities.push_back(b);
	game_images.push_back(b);
}

void Level::remove_player()
{
	remove_game_images(PLAYER);
	remove_entities(PLAYER);
	remove_beings(PLAYER);
}

void Level::remove_game_images(int type)
{
	int size = game_images.size();
	for (int i = size - 1; i >= 0; i--) {
		if (game_images[i]->get_type() == type) {
			game_images.erase(std::remove(game_images.begin(), game_images.end(), game_images[i]), game_images.end());
		}
	}
}

void Level::remove_entities(int type)
{
	int size = entities.size();
	for (int i = size - 1; i >= 0; i--) {
		if (entities[i]->get_type() == type) {
			entities.erase(std::remove(entities.begin(), entities.end(), entities[i]), entities.end());
		}
	}
}

void Level::remove_beings(int type)
{
	int size = beings.size();
	for (int i = size - 1; i >= 0; i--) {
		if (beings[i]->get_type() == type) {
			beings.erase(std::remove(beings.begin(), beings.end(), beings[i]), beings.end());
		}
	}
}

std::vector<Entity*> Level::get_interactables(Entity *entity)
{
	std::vector<Entity*> interactables;

	//TODO: get interactables that could potentially interact with the entity this frame, based on location and other factors.
	int size = entities.size();
	for (int i = 0; i < size; i++) {
		Entity *e = entities[i];
		if (e && e != entity) {	//TODO: make sure this != check actually works
			interactables.push_back(e);
		}
	}
	return interactables;
}

std::vector<Tile*> Level::get_nearby_tiles(Entity *entity)
{
	std::vector<Tile*> nearby_tiles;
	//TODO: get interactables that could potentially interact with the entity this frame, based on location and other factors.
	int xpos = entity->get_x(), ypos = entity->get_y(), width = entity->get_width(), height = entity->get_height();
	int tx_max = this->tile_rows.getItem(0)->get_size(), 
		ty_max = this->tile_rows.size();
	int tx1 = std::max(0, xpos / TILE_SIZE - 2), ty1 = std::max(0, ypos / TILE_SIZE - 2),
		tx2 = std::min(tx_max, (xpos + width) / TILE_SIZE + 2), ty2 = std::min(ty_max, (ypos + height) / TILE_SIZE + 2);
	for (int y = ty1; y < ty2; y++) {
		for (int x = tx1; x < tx2; x++) {
			nearby_tiles.push_back(this->get_tile(x, y));
		}
	}
	return nearby_tiles;
}

std::vector<Entity> Level::get_player_interactables()
{
	std::vector<Entity> interactables;
	//TODO: non-block interactables
	return interactables;
}

std::vector<std::string> Level::get_layers()
{
	return {"tile_layer", "block_layer", "entity_group_layer"}; //temp. figure out where to store the different lists of layers for t he level, or how to get them efficiently when reading the text file
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
	//TODO: figure out if create_entity_group() is redundant given initialize_entity_group()
	std::pair<int, int> pixel_pos(pos.first*TILE_SIZE, pos.second*TILE_SIZE);
	const std::string filename_start = this->tileset->get_entity_group_tile_sheet_filename();
	EntityGroup * eg = this->create_entity_group(filename_start, eg_index, ss_pos, pixel_pos);
	this->initialize_entity_group(eg);
	this->entity_groups.addItem(eg);
}

void Level::add_tiled_image(const int ti_index, const std::pair<int, int> ss_pos, const std::pair<int, int> pos, const int layer_index)
{
	const std::pair<int, int> pixel_pos(pos.first*TILE_SIZE, pos.second*TILE_SIZE);
	const std::string filename_start = this->tileset->get_tiled_image_tile_sheet_filename();
	TiledImage * ti = this->create_tiled_image(filename_start, ti_index, ss_pos, pixel_pos);	//TODO: need filename start?
	const std::string full_filename = this->tileset->get_full_tiled_image_sheet_filename(ti_index);
	Rect *subsection = new Rect(ss_pos.first*TILE_SIZE, ss_pos.second*TILE_SIZE, TILE_SIZE, TILE_SIZE);
	ti->set_content(full_filename, subsection, pixel_pos);
	ti->set_bitmap(ImageLoader::get_instance().get_current_image(ti));
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
	//TiledImageData* ti_data = tileset->get_tiled_image_data_by_index(index);
	TiledImage * tiled_image = new TiledImage();
	tiled_image->set_starting_pos(pos.first, pos.second);
	tiled_image->set_sheet_pos(ss_pos.first, ss_pos.second);
	tiled_image->set_tiled_image_key(index);
	const std::string name = this->tileset->get_tiled_image_name_by_index(index);
	tiled_image->set_tiled_image_name(name);
	//this->initialize_tiled_image(tiled_image);
	//TODO
	return tiled_image;
}

EntityGroup * Level::create_entity_group(std::string filename_start, int index, std::pair<int, int> ss_pos, std::pair<int, int> pos)
{
	std::pair<int, int> root_off = tileset->get_entity_group_root_offset(index);
	std::pair<int, int> center_off = tileset->get_entity_group_center_offset(index);
	EntityGroupData* group_data = tileset->get_entity_group_data_by_index(index);
	std::vector<EntityComponentData*> comp_data = tileset->get_entity_group_components(index);
	std::pair<int, int> root_pos(pos.first, pos.second);
	std::pair<int, int> group_pos(root_pos.first - root_off.first, root_pos.second - root_off.second);
	const std::pair<int, int> entity_group_image_dimensions = this->tileset->get_entity_group_image_dimensions_by_index(index);
	std::vector<Entity*> entity_list;
	// load the images separately for each component
	int comp_size = comp_data.size();
	for (int comp_index = 0; comp_index < comp_size; comp_index++) {
		EntityComponentData *data = comp_data[comp_index];
		std::string comp_filename = filename_start + "_" + group_data->get_entity_group_name() + "_" + data->name.value();
		Rect* ss_offset_rect = new Rect(
			ss_pos.first*entity_group_image_dimensions.first,
			ss_pos.second*entity_group_image_dimensions.second,
			entity_group_image_dimensions.first,
			entity_group_image_dimensions.second);
		Entity* e = new Entity();
		e->set_content(comp_filename, ss_offset_rect, group_pos);
		e->set_rect(group_pos.first, group_pos.second,
			entity_group_image_dimensions.first, entity_group_image_dimensions.second);
		e->set_bitmap(ImageLoader::get_instance().get_current_image(e));
		e->set_entity_attributes(data->get_attributes());
		entity_list.push_back(e);
	}
	EntityGroup *e_group = new EntityGroup();
	e_group->set_sheet_pos(ss_pos.first, ss_pos.second);
	e_group->set_entity_group_name(group_data->get_entity_group_name());
	e_group->set_entities(entity_list);
	e_group->draw_bitmap_from_entities(entity_list);
	e_group->set_solid(true);	//temp. 
	//consider making a set of attributes for the entire group and including solid if necessary
	e_group->set_rect(group_pos.first, group_pos.second,
		entity_group_image_dimensions.first, entity_group_image_dimensions.second);
	e_group->set_center_offset(center_off);
	e_group->set_root_pos(root_pos);
	e_group->load_mask(filename_start + "_" + group_data->get_entity_group_name());
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

Tile * Level::get_tile(int x, int y)
{
	if (y < 0 || y > this->tile_rows.size()) {
		return nullptr;
	}
	TileGroup *tile_row = this->tile_rows.getItem(y);
	if (x < 0 || x > tile_row->get_size()) {
		return nullptr;
	}
	return tile_row->get_tile(x);
}

// checks to see whether a space is passable
bool Level::passable_at(int x, int y)
{
	if (x < 0 || x > width || y < 0 || y > width) return false;
	//TODO: check for collisions (is this actually necessary?)
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
	const int size = this->entity_groups.size();
	for (int i = 0; i < size; i++) {
		EntityGroup *eg = this->entity_groups.getItem(i);
		if (eg != NULL && eg->contains_point(pos.first*TILE_SIZE, pos.second*TILE_SIZE)) {
			return eg;
		}
	}
	return NULL;
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

void Level::draw_tiles_onto_bitmap(ALLEGRO_BITMAP * bitmap)
{
	ALLEGRO_BITMAP *display = al_get_target_bitmap();
	al_set_target_bitmap(bitmap);
	const int y_size = this->tile_rows.size();
	for (int y = 0; y < y_size; y++) {
		const int x_size = this->tile_rows.getItem(y)->get_size();
		for (int x = 0; x < x_size; x++) {
			Tile * t = this->get_tile(x, y);
			ALLEGRO_BITMAP *tile_bitmap = t->get_bitmap();
			float dx = x * TILE_SIZE;
			float dy = y * TILE_SIZE;
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

void Level::draw_blocks_onto_bitmap(ALLEGRO_BITMAP * bitmap)
{
	ALLEGRO_BITMAP *display = al_get_target_bitmap();
	al_set_target_bitmap(bitmap);
	const int x_size = this->tile_rows.getItem(0)->get_size(), y_size = this->tile_rows.size();
	for (int y = 0; y < y_size; y++) {
		for (int x = 0; x < x_size; x++) {
			Tile * t = this->get_tile(x, y);
			Block * b = t->get_block();
			if (b != NULL) {
				ALLEGRO_BITMAP *tile_bitmap = b->get_bitmap();
				float dx = x * TILE_SIZE;
				float dy = y * TILE_SIZE;
				al_draw_bitmap(tile_bitmap, dx, dy, 0);	
			}
		}
	}
	al_set_target_bitmap(display);
}

void Level::draw_entity_groups_onto_bitmap(ALLEGRO_BITMAP * bitmap)
{
	ALLEGRO_BITMAP *display = al_get_target_bitmap();
	al_set_target_bitmap(bitmap);
	const int size = this->entity_groups.size();
	for (int i = 0; i < size; i++) {
		EntityGroup * eg = this->entity_groups.getItem(i);
		float dx = eg->get_x();
		float dy = eg->get_y();
		std::vector<Entity*> entities = eg->get_entities();
		for (Entity *e : entities) {
			ALLEGRO_BITMAP *entity_bitmap = e->get_bitmap();
			al_draw_bitmap(entity_bitmap, dx, dy, 0);
		}
	}
	al_set_target_bitmap(display);
}

void Level::draw_tiled_images_onto_bitmap(ALLEGRO_BITMAP * bitmap)
{
	ALLEGRO_BITMAP *display = al_get_target_bitmap();
	al_set_target_bitmap(bitmap);
	const int size = this->tiled_image_layers.size();
	for (int i = 0; i < size; i++) {
		TiledImageLayer * layer = this->tiled_image_layers.getItem(i);
		layer->draw_tiled_images_onto_bitmap(bitmap);
		//TODO
	}
	//TODO: draw tiled images from layers in order of layers
	/*
	const int size = this->buildings.size();
	for (int i = 0; i < size; i++) {
		EntityGroup * eg = this->entity_groups.getItem(i);
		float dx = eg->get_x();
		float dy = eg->get_y();
		std::vector<Entity*> entities = eg->get_entities();
		for (Entity *e : entities) {
			ALLEGRO_BITMAP *entity_bitmap = e->get_bitmap();
			al_draw_bitmap(entity_bitmap, dx, dy, 0);
		}
	}
	*/
	al_set_target_bitmap(display);
}



