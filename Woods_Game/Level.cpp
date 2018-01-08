#include "Level.h"
#include "GameImageManager.h"

struct game_image_center_comparison
{
	inline bool operator() (GameImage* g1, GameImage* g2)
	{
		return (g1->get_center().second < g2->get_center().second);
	}
};

Level::Level()
{
	width = 800;
	height = 600;
}

Level::Level(std::string filename, std::string id,  int grid_x, int grid_y, int grid_width, int grid_height)
{
	this->filename = filename;
	this->id = id;
	this->grid_x = grid_x;
	this->grid_y = grid_y;
	this->width = STANDARD_LEVEL_GRID_WIDTH * grid_width;
	this->height = STANDARD_LEVEL_GRID_HEIGHT * grid_height;
}

Level::Level(std::string filename, int grid_x, int grid_y, int grid_width, int grid_height)
{
	this->filename = filename;
	this->grid_x = grid_x;
	this->grid_y = grid_y;
	this->width = STANDARD_LEVEL_GRID_WIDTH * grid_width;
	this->height = STANDARD_LEVEL_GRID_HEIGHT * grid_height;
}

Level::Level(int grid_x, int grid_y, int grid_width, int grid_height)		
{
	this->grid_x = grid_x;
	this->grid_y = grid_y;
	this->width = STANDARD_LEVEL_GRID_WIDTH * grid_width;
	this->height = STANDARD_LEVEL_GRID_HEIGHT * grid_height;
}


Level::~Level()
{

}

void Level::load_tileset()
{
	this->tileset = new TileSet();
	FileManager filemanager;
	std::string filename = "resources/load/tilesets";
	const std::string tileset_key = "tileset_1"; //TODO: need a getter at some point
	filemanager.load_xml_content(this->tileset, filename, "SerializableClass", "TilesetKey", tileset_key);
}

void Level::load_from_map()
{
	// new XML loading
	this->load_tileset();
	FileManager file_manager;
	std::string filename = "resources/load/maps/" + get_filename() + ".txt";
	const std::string tile_sheet_filename = this->tileset->get_tile_sheet_filename();
	const std::string block_sheet_filename = this->tileset->get_block_tile_sheet_filename();
	const std::string entity_group_sheet_filename = this->tileset->get_entity_group_tile_sheet_filename();
	const std::pair<int, int> entity_group_image_dimensions = this->tileset->get_entity_group_image_dimensions();
	std::vector<std::string> layers = get_layers();
	int size = layers.size();
	for (int layer_index = 0; layer_index < size; layer_index++) {
		//std::string tile_sheet_filename = "";
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
						tiles.push_back(std::vector<Tile*>());
						for (int k = 0; k < contents_size; k++) {
							if (contents[i][k] != null_tile) {
								std::pair<int, int> tile_offset = FileManager::string_to_pair(contents[i][k]);
								Rect* offset_rect = new Rect(tile_offset.first*TILE_SIZE, tile_offset.second*TILE_SIZE, TILE_SIZE, TILE_SIZE);
								const int edge_priority = this->tileset->get_edge_priority(tile_offset.second);
								const float speed_mod = this->tileset->get_tile_speed_mod(tile_offset.second);
								ImageLoader::get_instance().load_image(tile_sheet_filename, *offset_rect);
								std::pair<int, int> position(k*TILE_SIZE, indexY*TILE_SIZE);
								Tile *t = new Tile();
								t->set_content(tile_sheet_filename, offset_rect, position);
								t->set_sheet_row(tile_offset.second);
								t->set_edge_priority(edge_priority);
								t->set_bitmap(ImageLoader::get_instance().get_current_image(t));
								t->set_speed_mod(speed_mod);
								tiles[indexY].push_back(t);
							} else {
								Rect* offset_rect = new Rect(0, 0, TILE_SIZE, TILE_SIZE);
								ImageLoader::get_instance().load_image(tile_sheet_filename, *offset_rect);
								std::pair<int, int> position(k*TILE_SIZE, indexY*TILE_SIZE);
								Tile *t = new Tile();
								t->set_content(tile_sheet_filename, offset_rect, position);
								t->set_edge_priority(0);
								t->set_speed_mod(1.0f);
								t->set_bitmap(ImageLoader::get_instance().get_current_image(t));
								//NOTE: currently all default tiles have a speed mod of 1.0 and an edge priority of 0.
								tiles[indexY].push_back(t);
							}
						}
					}
					// block loading
					else if (layer_id == "block_layer") {
						for (int k = 0; k < contents_size; k++) {
							if (contents[i][k] != null_tile) {	//TODO: allow for animated blocks if necessary
								std::pair<int, int>tile_offset = FileManager::string_to_pair(contents[i][k]);
								Rect* offset_rect = new Rect(tile_offset.first*TILE_SIZE, tile_offset.second*TILE_SIZE, TILE_SIZE, TILE_SIZE);
								ImageLoader::get_instance().load_image(block_sheet_filename, *offset_rect);
								std::pair<int, int> position(k*TILE_SIZE, indexY*TILE_SIZE);
								const bool solid = tileset->get_block_solid(tile_offset.second);
								std::map<std::string, int> block_attributes = tileset->get_block_attributes(tile_offset.second);
								Block* b = new Block();
								b->set_content(block_sheet_filename, offset_rect, position);
								b->set_bitmap(ImageLoader::get_instance().get_current_image(b));
								b->set_solid(solid);
								b->set_entity_attributes(block_attributes);
								b->load_entity_effects(block_sheet_filename, tile_offset.second, std::pair<int, int>(TILE_SIZE, TILE_SIZE));
								tiles[indexY][k]->set_block(b);
							}
						}
					}
					// entity group loading
					else if (layer_id == "entity_group_layer") {
						for (int k = 0; k < contents_size; k++) {
							if (contents[i][k] != null_tile) {
								std::pair<int, int> ss_offset = FileManager::string_to_pair(contents[i][k]);
								std::pair<int, int> root_off = tileset->get_entity_group_root_offset(ss_offset.second); 
								std::pair<int, int> center_off = tileset->get_entity_group_center_offset(ss_offset.second);
								std::vector<EntityComponentData*> comp_data = tileset->get_entity_group_components(ss_offset.second); 
								std::pair<int, int> root_pos(k*TILE_SIZE, indexY*TILE_SIZE);
								std::pair<int, int> group_pos(root_pos.first - root_off.first, root_pos.second - root_off.second);
								std::vector<Entity*> entity_list;
								// load the images separately for each component
								int comp_size = comp_data.size();
								for (int comp_index = 0; comp_index < comp_size; comp_index++) {
									EntityComponentData *data = comp_data[comp_index];
									std::string comp_filename = entity_group_sheet_filename + "_" + data->name.value();
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
								EntityGroup *e_group = new EntityGroup();
								e_group->set_entities(entity_list);
								e_group->set_solid(true);	//temp. consider making a set of attributes for the entire group and including solid if necessary
								e_group->set_rect(group_pos.first, group_pos.second, 
									entity_group_image_dimensions.first, entity_group_image_dimensions.second);
								e_group->set_center_offset(center_off);
								e_group->load_mask(entity_group_sheet_filename);
								add_entity(e_group);
							}
						}
					}
					indexY++;
				}
				else if (attributes[i][j] == "EndLayer") {
					if (layer_id == "tile_layer") {
						//TODO: load tile edges properly, not sure this does it
						load_tile_edges(tile_sheet_filename);
					}
				}
			}
		}
	}
	// old way of loading

	//TEMP
	//TileSet tileset;
	//tileset.set_tileset_key("tileset_1");
	//TEMP
	/*
	int size = layers.size();
	for (int layer_index = 0; layer_index < size; layer_index++) {
		std::string tile_sheet_filename = "";
		std::string layer_id = layers[layer_index];
		std::vector<std::vector<std::string>> attributes;
		std::vector<std::vector<std::string>> contents;

		// tile properties
		std::map<int, float> speed_mods; 
		std::map<int, int> edge_priorities;
		// block properties
		std::map <int, EntityData*> block_map;
		// entity properties
		std::map < std::pair<int, int>, EntityData*> entity_map;
		std::map < std::pair<int, int>, std::vector <EntityComponentData*>> entity_component_map;
		std::pair<int, int> image_dimensions;
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
				else if (attributes[i][j] == "TileSheet") {
					tile_sheet_filename = "tile_sheets/" + contents[i][j];
					//TEMP
					if (layer_id == "tile_layer") {
						tileset.set_tile_sheet_name(contents[i][j]);
						tileset.set_edge_tile_sheet_name(contents[i][j] + "_edges");
					}
					else if (layer_id == "block_layer") {
						tileset.set_block_tile_sheet_name(contents[i][j]);
					}
					else if (layer_id == "entity_group_layer") {
						tileset.set_entity_group_tile_sheet_name(contents[i][j]);
					}
					//TEMP
				}
				else if (attributes[i][j] == "speed_modifier") {
					const int contents_size = contents[i].size();
					for (int k = 0; k < contents_size; k++) {
						std::pair<std::string, std::string> speed_strs = FileManager::string_to_pair(contents[i][k], ":");
						speed_mods[::atoi(speed_strs.first.c_str())] = ::atof(speed_strs.second.c_str());
						//temp
						tileset.set_tile_speed_mod(::atoi(speed_strs.first.c_str()), ::atof(speed_strs.second.c_str()));
						//temp
					}
				}
				else if (attributes[i][j] == "edge_priority") {
					const int contents_size = contents[i].size();
					for (int k = 0; k < contents_size; k++) {
						std::pair<std::string, std::string> edge_strs = FileManager::string_to_pair(contents[i][k], ":");
						edge_priorities[::atoi(edge_strs.first.c_str())] = ::atoi(edge_strs.second.c_str());
						//temp
						tileset.set_edge_priority(::atoi(edge_strs.first.c_str()), ::atoi(edge_strs.second.c_str()));
						//temp
					}
				}
				// block loading
				else if (attributes[i][j] == "solid_blocks") {
					const int contents_size = contents[i].size();
					for (int k = 0; k < contents_size; k++) {
						const int block_index = ::atoi(contents[i][k].c_str());
						auto it = block_map.find(block_index);
						if (it == block_map.end()) {
							block_map[block_index] = new EntityData();
						}
						block_map[block_index]->solid = true;
						//temp
						tileset.set_block_solid(block_index, true);
						//temp
					}
				}
				else if (attributes[i][j] == "block_attributes") {
					const int contents_size = contents[i].size();
					for (int k = 0; k < contents_size; k++) {
						std::pair<std::string, std::string> block_attr_parts = FileManager::string_to_pair(contents[i][k], ":");
						const int block_index = ::atoi(block_attr_parts.first.c_str());
						auto it = block_map.find(block_index);
						if (it == block_map.end()) {
							block_map[block_index] = new EntityData();
						}
						std::map<std::string, int> block_attributes;
						std::vector<std::string> attr_str_list = FileManager::string_to_parts(block_attr_parts.second, ",");
						const int attr_count = attr_str_list.size();
						for (int attr_index = 0; attr_index < attr_count; attr_index++) {
							std::pair<std::string,std::string> attr_parts = FileManager::string_to_pair(attr_str_list[attr_index], "-");
							block_attributes[attr_parts.first] = ::atoi(attr_parts.second.c_str());
						}
						block_map[block_index]->set_attributes(block_attributes);
						//temp
						tileset.set_block_attributes(block_index, block_attributes);
						//temp
					}
				}
				// entity loading
				else if (attributes[i][j] == "root_offset") {
					std::pair<std::string, std::string> root_off_strs = FileManager::string_to_pair(contents[i][j], ":");
					EntityData *data = new EntityData();
					data->set_root_offset(FileManager::string_to_pair(root_off_strs.second));
					std::pair<int, int> entity_keys = FileManager::string_to_pair(root_off_strs.first);
					entity_map[entity_keys] = data;
					//TEMP
					tileset.set_entity_group_root_offset(entity_keys.first, FileManager::string_to_pair(root_off_strs.second));
					//TEMP
				}
				else if (attributes[i][j] == "center_offset") {
					std::pair<std::string, std::string> center_off_strs = FileManager::string_to_pair(contents[i][j], ":");
					std::pair<int, int> entity_keys = FileManager::string_to_pair(center_off_strs.first);
					entity_map[entity_keys]->set_center_offset
						(FileManager::string_to_pair(center_off_strs.second));
					//TEMP
					tileset.set_entity_group_center_offset(entity_keys.first, FileManager::string_to_pair(center_off_strs.second));
					//TEMP
				}
				else if (attributes[i][j] == "entity_components") {
					std::pair<std::string, std::string> entity_comp_strs = FileManager::string_to_pair(contents[i][j], ":");
					std::pair<int, int> ss_offset = FileManager::string_to_pair(entity_comp_strs.first);
					std::vector<std::string> entity_components_str_list = FileManager::string_to_parts(entity_comp_strs.second, ";");
					int entity_components_size = entity_components_str_list.size();
					std::vector<EntityComponentData*> component_data_list;
					for (int k = 0; k < entity_components_size; k++) {
						std::string comp_str = entity_components_str_list[k];
						std::string comp_name = "";
						std::vector<std::string> comp_attributes;
						if (entity_components_str_list[k].find("-") != std::string::npos) {
							std::pair<std::string, std::string> comp_strs
								= FileManager::string_to_pair(entity_components_str_list[k], "-");
							comp_name = comp_strs.first;
							comp_attributes = FileManager::string_to_parts(comp_strs.second, ",");
						} else {
							comp_name = entity_components_str_list[k];
						}
						EntityComponentData *data = new EntityComponentData(comp_name, comp_attributes);
						component_data_list.push_back(data);
					}
					entity_component_map[ss_offset] = component_data_list;
					//TEMP
					tileset.set_entity_group_components(ss_offset.first, component_data_list);
					//TEMP
				}
				else if (attributes[i][j] == "image_dimensions") {
					image_dimensions = FileManager::string_to_pair(contents[i][j]);
					//TEMP
					tileset.set_entity_group_image_dimensions(FileManager::string_to_pair(contents[i][j]));
					//TEMP
				}
				else if (attributes[i][j] == "StartLayer") {	
					const int contents_size = contents[i].size();

					if (layer_id == "tile_layer") {
						tiles.push_back(std::vector<Tile*>());
						for (int k = 0; k < contents_size; k++) {
							if (contents[i][k] != null_tile) {
								std::pair<int, int> tile_offset = FileManager::string_to_pair(contents[i][k]);
								Rect* offset_rect = new Rect(tile_offset.first*TILE_SIZE, tile_offset.second*TILE_SIZE, TILE_SIZE, TILE_SIZE);
								ImageLoader::get_instance().load_image(tile_sheet_filename, *offset_rect);
								std::pair<int, int> position(k*TILE_SIZE, indexY*TILE_SIZE);
								Tile *t = new Tile();
								t->set_content(tile_sheet_filename, offset_rect, position);
								t->set_sheet_row(tile_offset.second);
								t->set_edge_priority(edge_priorities[tile_offset.second]);
								t->set_bitmap(ImageLoader::get_instance().get_current_image(t));	
								auto it = speed_mods.find(tile_offset.second);
								if (it != speed_mods.end()) t->set_speed_mod(it->second);
								tiles[indexY].push_back(t);
							}
							else {
								Rect* offset_rect = new Rect(0, 0, TILE_SIZE, TILE_SIZE);
								ImageLoader::get_instance().load_image(tile_sheet_filename, *offset_rect);
								std::pair<int, int> position(k*TILE_SIZE, indexY*TILE_SIZE);
								Tile *t = new Tile();
								t->set_content(tile_sheet_filename, offset_rect, position);
								t->set_edge_priority(0);
								t->set_bitmap(ImageLoader::get_instance().get_current_image(t)); 
								//NOTE: currently all default tiles have a speed mod of 1.0 and an edge priority of 0.
								tiles[indexY].push_back(t);
							}
						}
					}
					// block loading
					else if (layer_id == "block_layer") {
						for (int k = 0; k < contents_size; k++) {
							if (contents[i][k] != null_tile) {	//TODO: allow for animated blocks if necessary
								std::pair<int, int>tile_offset = FileManager::string_to_pair(contents[i][k]);
								Rect* offset_rect = new Rect(tile_offset.first*TILE_SIZE, tile_offset.second*TILE_SIZE, TILE_SIZE, TILE_SIZE);
								ImageLoader::get_instance().load_image(tile_sheet_filename, *offset_rect);
								std::pair<int, int> position(k*TILE_SIZE, indexY*TILE_SIZE);
								Block* b = new Block();
								b->set_content(tile_sheet_filename, offset_rect, position);
								b->set_bitmap(ImageLoader::get_instance().get_current_image(b));
								auto it = block_map.find(tile_offset.second);
								if (it != block_map.end()) {
									b->set_solid(it->second->solid.value());
									b->set_entity_attributes(it->second->attribute_map());
								}
								b->load_entity_effects(tile_sheet_filename, tile_offset.second, std::pair<int,int>(TILE_SIZE,TILE_SIZE));
								tiles[indexY][k]->set_block(b);
							}
						}
					}
					// entity group loading
					else if (layer_id == "entity_group_layer") {
						for (int k = 0; k < contents_size; k++) {
							if (contents[i][k] != null_tile) {
								std::pair<int, int> ss_offset = FileManager::string_to_pair(contents[i][k]);
								std::pair<int, int> root_off = entity_map[ss_offset]->get_root_offset();
								std::pair<int, int> center_off = entity_map[ss_offset]->get_center_offset(); 
								std::vector<EntityComponentData*> comp_data = entity_component_map[ss_offset];
								std::pair<int, int> root_pos(k*TILE_SIZE, indexY*TILE_SIZE);
								std::pair<int, int> group_pos(root_pos.first - root_off.first, root_pos.second - root_off.second);
								std::vector<Entity*> entity_list;
								// load the images separately for each component
								int comp_size = comp_data.size();
								for (int comp_index = 0; comp_index < comp_size; comp_index++) {
									EntityComponentData *data = comp_data[comp_index];
									std::string comp_filename = tile_sheet_filename + "_" + data->name.value();
									Rect* ss_offset_rect = new Rect(ss_offset.first*image_dimensions.first, ss_offset.second*image_dimensions.second,
										image_dimensions.first, image_dimensions.second);
									ImageLoader::get_instance().load_image(comp_filename, *ss_offset_rect);
									Entity* e = new Entity();
									e->set_content(comp_filename, ss_offset_rect, group_pos);
									e->set_rect(group_pos.first, group_pos.second, image_dimensions.first, image_dimensions.second);
									e->set_bitmap(ImageLoader::get_instance().get_current_image(e));
									e->set_entity_attributes(data->get_attributes());
									entity_list.push_back(e);
								}
								EntityGroup *e_group = new EntityGroup();
								e_group->set_entities(entity_list);
								e_group->set_solid(true);	//temp. consider making a set of attributes for the entire group and including solid if necessary
								e_group->set_rect(group_pos.first, group_pos.second, image_dimensions.first, image_dimensions.second);
								e_group->set_center_offset(center_off);
								e_group->load_mask(tile_sheet_filename);
								add_entity(e_group);
							}
						}
					}
					indexY++;
				}
				else if (attributes[i][j] == "EndLayer") {
					if (layer_id == "tile_layer") {
						load_tile_edges(tile_sheet_filename);
					}
				}
			}
		}
	}
	*/
	//temp
	//std::string tileset_filename = "resources/load/tilesets";
	//std::string xml_string = tileset.toXML();
	//file_manager.replace_xml_content(tileset_filename, "SerializableClass", "TilesetKey", "tileset_1", xml_string);
	//temp
	
}

void Level::load_tile_edges(std::string tilesheet_filename)
{
	int tile_rows = tiles.size();
	int tile_cols = tiles[0].size();
	std::string edge_filename = tilesheet_filename + "_edges"; //need to test that this formats the filename correctly
	// map of sheet row/priority to groups of tiles with that priority
	std::map<std::pair<int, int>, std::map<int, bool> > edge_map;
	for (int y = 0; y < tile_rows; y++) {
		for (int x = 0; x < tile_cols; x++) {
			edge_map.clear();
			Tile *t = tiles[y][x];
			int x_off1 = x > 0 ? -1 : 0;
			int y_off1 = y > 0 ? -1 : 0;
			int x_off2 = x < tile_cols - 1 ? 1 : 0;
			int y_off2 = y < tile_rows - 1 ? 1 : 0;
			for (int y_off = y_off1; y_off <= y_off2; y_off++) {
				for (int x_off = x_off1; x_off <= x_off2; x_off++) {
					if (y_off == 0 && x_off == 0) continue;
					Tile *check_tile = tiles[y + y_off][x + x_off];
					int priority = check_tile->get_edge_priority();
					const int row = check_tile->get_sheet_row();
					auto it = edge_map.find(std::pair<int, int>(priority, row));
					if (it == edge_map.end()) {
						std::map<int, bool> sub_map;
						edge_map[std::pair<int, int>(priority, row)] = sub_map;
					}
					// little trick to map x_off, y_off combinations to the enum directional values in Tile.h
					int dir_val = (y_off + 1) * 3 + (x_off + 1);
					edge_map[std::pair<int, int>(priority, row)][dir_val] = priority > t->get_edge_priority();
				}
			}
			// each iteration of this loop represents a different priority
			for (const auto& edge_pair: edge_map) {
				std::pair<int, int> edge_key = edge_pair.first;
				std::map<int, bool> sub_map = edge_pair.second;
				if (sub_map[TILE_UP] && sub_map[TILE_DOWN] && sub_map[TILE_LEFT] && sub_map[TILE_RIGHT]) draw_edge_tile(*t, edge_filename, edge_key.second, TILE_CENTER);
				else {
					if (sub_map[TILE_UP]) draw_edge_tile(*t, edge_filename, edge_key.second, TILE_UP);
					if (sub_map[TILE_DOWN]) draw_edge_tile(*t, edge_filename, edge_key.second, TILE_DOWN);
					if (sub_map[TILE_LEFT]) draw_edge_tile(*t, edge_filename, edge_key.second, TILE_LEFT);
					if (sub_map[TILE_RIGHT]) draw_edge_tile(*t, edge_filename, edge_key.second, TILE_RIGHT);
					if (sub_map[TILE_UP_LEFT] && !sub_map[TILE_UP] && !sub_map[TILE_LEFT]) draw_edge_tile(*t, edge_filename, edge_key.second, TILE_UP_LEFT);
					if (sub_map[TILE_UP_RIGHT] && !sub_map[TILE_UP] && !sub_map[TILE_RIGHT]) draw_edge_tile(*t, edge_filename, edge_key.second, TILE_UP_RIGHT);
					if (sub_map[TILE_DOWN_LEFT] && !sub_map[TILE_DOWN] && !sub_map[TILE_LEFT]) draw_edge_tile(*t, edge_filename, edge_key.second, TILE_DOWN_LEFT);
					if (sub_map[TILE_DOWN_RIGHT] && !sub_map[TILE_DOWN] && !sub_map[TILE_RIGHT]) draw_edge_tile(*t, edge_filename, edge_key.second, TILE_DOWN_RIGHT);
				}
			}
		}
	}
}


void Level::unload_content()
{
	for (int i = 0; i < game_images.size(); i++) {
		if (game_images[i]) {
			game_images[i]->unload_content();
			delete game_images[i];
		}
	}
	for (int i = 0; i < tiles.size(); i++) {
		for (int j = 0; j < tiles[i].size(); j++) {
			tiles[i][j]->unload_content();
		}
		tiles[i].clear();
		std::vector<Tile*>().swap(tiles[i]);
	}
	std::vector<std::vector<Tile*>>().swap(tiles);
	game_images.clear();
	std::vector<GameImage*>().swap(game_images);
	entities.clear();
	std::vector<Entity*>().swap(entities);
	beings.clear();
	std::vector<Being*>().swap(beings);
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
	const int height = tiles.size();
	for (int y = 0; y < height; y++) {
		const int width = tiles[y].size();
		for (int x = 0; x < width; x++) {
			Block *b = tiles[y][x]->get_block();
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
	std::pair<int, int> off = offset;
	const int start_x = std::max(0, (-1 * off.first) / TILE_SIZE - 1);
	const int start_y = std::max(0, (-1 * off.second) / TILE_SIZE - 1);
	const int x_size = tiles[0].size(), y_size = tiles.size();
	const int end_x = std::min(x_size, start_x + al_get_display_width(display) / TILE_SIZE + 3);
	const int end_y = std::min(y_size, start_y + al_get_display_height(display) / TILE_SIZE + 3);
	for (int y = start_y; y < end_y; y++) {
		for (int x = start_x; x < end_x; x++) {
			tiles[y][x]->draw(display, off.first, off.second);
		}
	}	
	//TODO: draw game_images in order from lowest center y to highest center y
	std::sort(game_images.begin(), game_images.end(), game_image_center_comparison());
	int size = game_images.size();
	for (int i = 0; i < size; i++) {
		game_images[i]->draw(display, off.first, off.second);
	}
}

void Level::draw_edge_tile(Tile &tile, std::string edge_filename, int edge_row, int dir_key)
{
	
	Rect subsection(dir_key*TILE_SIZE, edge_row*TILE_SIZE, TILE_SIZE, TILE_SIZE);
	ImageLoader::get_instance().load_image(edge_filename, subsection);
	ALLEGRO_BITMAP* edge_image = ImageLoader::get_instance().get_image(edge_filename, subsection);
	tile.draw_onto_bitmap(edge_image);
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
	int tx_max = tiles[0].size() - 1, ty_max = tiles.size() - 1;
	int tx1 = std::max(0, xpos / TILE_SIZE - 2), ty1 = std::max(0, ypos / TILE_SIZE - 2),
		tx2 = std::min(tx_max, (xpos + width) / TILE_SIZE + 2), ty2 = std::min(ty_max, (ypos + height) / TILE_SIZE + 2);
	for (int y = ty1; y < ty2; y++) {
		for (int x = tx1; x < tx2; x++) {
			nearby_tiles.push_back(tiles[y][x]);
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

// checks to see whether a space is passable
bool Level::passable_at(int x, int y)
{
	if (x < 0 || x > width || y < 0 || y > width) return false;
	//TODO: check for collisions
	return true;
}

std::string Level::get_filename()
{
	return filename;
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

int Level::get_grid_x()
{
	return this->grid_x;
}

int Level::get_grid_y()
{
	return this->grid_y;
}

int Level::grid_width()
{
	return width/STANDARD_LEVEL_GRID_WIDTH;	//TEMP
}

int Level::grid_height()
{
	return height/STANDARD_LEVEL_GRID_HEIGHT; //TEMP
}


