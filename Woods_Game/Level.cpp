#include "Level.h"
#include "GameImageManager.h"

Level::Level()
{
	//TEMP
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

void Level::load_from_map()
{
	FileManager file_manager;	//NOTE: see if filemanager can be chhanged to a singleton
	std::string filename = "resources/load/maps/" + get_filename() + ".txt";
	std::vector<std::string> layers = get_layers();
	int size = layers.size();
	for (int layer_index = 0; layer_index < size; layer_index++) {
		std::string layer_id = layers[layer_index];
		std::vector<std::vector<std::string>> attributes;
		std::vector<std::vector<std::string>> contents;
		// map tile offsets to speed mods
		std::map<std::pair<int, int>, float> speed_mods;
		// map entity offsets to root offsets
		std::map<std::pair<int, int>, std::pair<int, int>> root_offsets;
		// map entity offsets to entity components
		std::map < std::pair<int, int>, std::vector <EntityComponentData >> entity_component_map;
		std::pair<int, int> image_dimensions;
		file_manager.load_content(filename.c_str(), attributes, contents, layer_id);
		int indexY = 0;
		int size = attributes.size();
		std::string tile_sheet_filename = "";
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
				}
				else if (attributes[i][j] == "speed_modifier") {
					const int contents_size = contents[i].size();
					for (int k = 0; k < contents_size; k++) {
						std::string speed_string = contents[i][k];
						std::string tile_string = speed_string.substr(0, speed_string.find(":"));
						std::pair<int, int> tile_offset = string_to_pair(tile_string);
						float speed_mod = ::atof(speed_string.substr(speed_string.find(":") + 1).c_str());
						speed_mods[tile_offset] = speed_mod;
					}
				}
				// entity loading
				else if (attributes[i][j] == "root_offset") {
					std::string root_offset_data = contents[i][j];
					std::pair<int, int> ss_offset = string_to_pair(root_offset_data.substr(0, root_offset_data.find(":")));
					std::pair<int, int> root_offset = string_to_pair(root_offset_data.substr(root_offset_data.find(":") + 1).c_str());
					root_offsets[ss_offset] = root_offset;
				}
				else if (attributes[i][j] == "entity_components") {
					std::pair<int, int> ss_offset = string_to_pair(contents[i][j].substr(0, contents[i][j].find(":")));
					std::string entity_components_str = contents[i][j].substr(contents[i][j].find(":") + 1).c_str();
					std::vector<std::string> entity_components_str_list = FileManager::string_to_parts(entity_components_str, ";");
					int entity_components_size = entity_components_str_list.size();
					std::vector<EntityComponentData> component_data_list;
					for (int k = 0; k < entity_components_size; k++) {
						std::string comp_str = entity_components_str_list[k];
						std::string comp_name = comp_str.substr(0, comp_str.find("-"));
						std::string comp_attributes_str = comp_str.substr(comp_str.find("-") + 1);
						std::vector<std::string> comp_attributes = FileManager::string_to_parts(comp_attributes_str, ",");
						EntityComponentData data(comp_name, comp_attributes);
						component_data_list.push_back(data);
					}
					entity_component_map[ss_offset] = component_data_list;
				}
				else if (attributes[i][j] == "image_dimensions") {
					image_dimensions = string_to_pair(contents[i][j]);
				}
				else if (attributes[i][j] == "StartLayer") {	//TODO: figure out what to do with EndLayer tag
					const int contents_size = contents[i].size();

					if (layer_id == "tile_layer") {
						tiles.push_back(std::vector<Tile>());
						for (int k = 0; k < contents_size; k++) {
							if (contents[i][k] != null_tile) {
								std::pair<int, int> tile_offset = string_to_pair(contents[i][k]);
								Rect* offset_rect = new Rect(tile_offset.first*TILE_SIZE, tile_offset.second*TILE_SIZE, TILE_SIZE, TILE_SIZE);
								ImageLoader::get_instance().load_image(tile_sheet_filename, *offset_rect);
								std::pair<int, int> position(k*TILE_SIZE, indexY*TILE_SIZE);
								Tile t;
								t.set_content(tile_sheet_filename, offset_rect, position);
								t.set_bitmap(ImageLoader::get_instance().get_current_image(&t));	
								auto it = speed_mods.find(tile_offset);
								if (it != speed_mods.end()) t.set_speed_mod(it->second);
								tiles[indexY].push_back(t);
							}
							else {
								Rect* offset_rect = new Rect(0, 0, TILE_SIZE, TILE_SIZE);
								ImageLoader::get_instance().load_image(tile_sheet_filename, *offset_rect);
								std::pair<int, int> position(k*TILE_SIZE, indexY*TILE_SIZE);
								Tile t;
								t.set_content(tile_sheet_filename, offset_rect, position);
								t.set_bitmap(ImageLoader::get_instance().get_current_image(&t)); 
								//NOTE: currently all default tiles have a speed mod of 1.0.
								tiles[indexY].push_back(t);
							}
						}
					}
					// block loading
					else if (layer_id == "block_layer") {
						for (int k = 0; k < contents_size; k++) {
							if (contents[i][k] != null_tile) {	//TODO: allow for animated blocks if necessary
								std::pair<int, int>tile_offset = string_to_pair(contents[i][k]);
								Rect* offset_rect = new Rect(tile_offset.first*TILE_SIZE, tile_offset.second*TILE_SIZE, TILE_SIZE, TILE_SIZE);
								ImageLoader::get_instance().load_image(tile_sheet_filename, *offset_rect);
								std::pair<int, int> position(k*TILE_SIZE, indexY*TILE_SIZE);
								Block* b = new Block();
								b->set_content(tile_sheet_filename, offset_rect, position);
								b->set_bitmap(ImageLoader::get_instance().get_current_image(b));
								tiles[indexY][k].set_block(b);
							}
						}
					}
					// entity group loading
					
					else if (layer_id == "entity_group_layer") {
						for (int k = 0; k < contents_size; k++) {
							if (contents[i][k] != null_tile) {
								std::pair<int, int> ss_offset = string_to_pair(contents[i][k]);
								std::pair<int, int> root_off = root_offsets[ss_offset];
								std::vector<EntityComponentData> comp_data = entity_component_map[ss_offset];
								std::pair<int, int> root_pos(k*TILE_SIZE, indexY*TILE_SIZE);
								std::pair<int, int> group_pos(root_pos.first - root_off.first, root_pos.second - root_off.second);
								std::vector<Entity*> entity_list;
								// load the images separately for each component
								int comp_size = comp_data.size();
								for (int comp_index = 0; comp_index < comp_size; comp_index++) {
									EntityComponentData data = comp_data[comp_index];
									std::string comp_filename = tile_sheet_filename + "_" + data.name;
									Rect* ss_offset_rect = new Rect(ss_offset.first*image_dimensions.first, ss_offset.second*image_dimensions.second,
										image_dimensions.first, image_dimensions.second);
									ImageLoader::get_instance().load_image(comp_filename, *ss_offset_rect);
									Entity* e = new Entity();
									e->set_content(comp_filename, ss_offset_rect, group_pos);
									e->set_rect(group_pos.first, group_pos.second, image_dimensions.first, image_dimensions.second);
									e->set_bitmap(ImageLoader::get_instance().get_current_image(e));
									//TODO: set entity attributes like solid, transparent on collide, etc
									//TODO: set whatever is necessary for objects to be drawn on the correct layer
									//TODO: set rect as necessary
									entity_list.push_back(e);
								}
								EntityGroup *e_group = new EntityGroup();
								e_group->set_entities(entity_list);
								e_group->set_solid(true);	//temp. consider making a set of attributes for the entire group and including solid if necessary
								e_group->set_rect(group_pos.first, group_pos.second, image_dimensions.first, image_dimensions.second);
								e_group->load_mask(tile_sheet_filename);
								add_entity(e_group);
							}
						}
					}
					indexY++;
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
			tiles[i][j].unload_content();
		}
		tiles[i].clear();
		std::vector<Tile>().swap(tiles[i]);
	}
	std::vector<std::vector<Tile>>().swap(tiles);
	game_images.clear();
	std::vector<GameImage*>().swap(game_images);
	entities.clear();
	std::vector<Entity*>().swap(entities);
	beings.clear();
	std::vector<Being*>().swap(beings);
	//player = NULL;
}

void Level::update(int game_mode)
{
	std::pair<int, int> dimensions = get_dimensions();
	int size = beings.size();
	for (int i = 0; i < size; i++) {
		if (beings[i]) {	// note that the player's update is called here, so we don't need to call it above.
			std::vector<Entity*> interactables = get_interactables(beings[i]);

			std::vector<Tile> tiles = get_nearby_tiles(beings[i]);
			beings[i]->update(interactables, tiles, dimensions, game_mode);	//TEMP. need to handle gamemode some other way, maybe by accessing it from level or storing it in GameImageManager
			beings[i]->set_bitmap(ImageLoader::get_instance().get_current_image(beings[i]));
		}
		else
			std::cout << "NULL BEING" << std::endl;
	}
}

void Level::draw(ALLEGRO_DISPLAY * display, std::pair<int, int> offset)
{
	std::pair<int, int> off = offset;
	const int start_x = std::max(0, (-1 * off.first) / TILE_SIZE - 1);
	const int start_y = std::max(0, (-1 * off.second) / TILE_SIZE - 1);
	const int x_size = tiles[0].size(), y_size = tiles.size();
	const int end_x = std::min(x_size, start_x + DEFAULT_SCREEN_WIDTH / TILE_SIZE + 3);
	const int end_y = std::min(y_size, start_y + DEFAULT_SCREEN_HEIGHT / TILE_SIZE + 3);
	for (int y = start_y; y < end_y; y++) {
		for (int x = start_x; x < end_x; x++) {
			tiles[y][x].draw(display, off.first, off.second);
		}
	}	
	int size = game_images.size();
	for (int i = 0; i < size; i++) {
		game_images[i]->draw(display, off.first, off.second);
	}
}

std::pair<int, int> Level::string_to_pair(std::string tile_string)
{
	std::pair<int, int> pair;
	pair.first = atoi(tile_string.substr(0, tile_string.find(',')).c_str());
	pair.second = atoi(tile_string.substr(tile_string.find(',') + 1).c_str());
	return pair;
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
		if (e && e != entity) {	//TODO: make sure this check actually works
			interactables.push_back(e);
		}
	}
	return interactables;
}

std::vector<Tile> Level::get_nearby_tiles(Entity *entity)
{
	std::vector<Tile> nearby_tiles;
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


