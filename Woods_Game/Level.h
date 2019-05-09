#ifndef LEVEL_H
#define LEVEL_H

#include "Tile.h"
#include "TileSet.h"
#include "Being.h"
#include "NPC.h"
#include "allegro5/display.h"  // for ALLEGRO_DISPLAY
#include "utility"             // for pair
#include "vector"              // for vector
#include "xstring"             // for string
#include "EntityGroup.h"
#include "XMLSerialization.h"
#include "Spawner.h"
#include "TileGroup.h"
#include "TiledImageLayer.h"

class Level : public xmls::Serializable
{
private:
	std::string map_filename = "";
	std::string dungeon_filename;
	std::string id = "";
	TileSet *tileset;
	xmls::xString tileset_key;
	xmls::Collection<TileGroup> tile_rows;	//serialized blocks are stored in here
	xmls::Collection<EntityGroup> entity_groups; // these may also be stored in entities
	xmls::Collection<Spawner> spawners;
	xmls::Collection<TiledImageLayer> tiled_image_layers;
	std::vector<GameImage*> game_images;
	std::vector<Entity*> entities;
	std::vector<Being*> beings;
	xmls::xInt grid_x;
	xmls::xInt grid_y;
	xmls::xInt grid_width;
	xmls::xInt grid_height;
	int width;
	int height;
	void draw_tiled_images(ALLEGRO_DISPLAY *display, const std::pair<int, int> offset, const int layer);
public:
	static const int STANDARD_LEVEL_GRID_WIDTH = 832;
	static const int STANDARD_LEVEL_GRID_HEIGHT = 640;
	//draw layers
	static const int LAYER_INDEX_TILES = 0;
	static const int LAYER_INDEX_BLOCKS = 10;
	static const int LAYER_INDEX_BEINGS = 20;
	Level();
	Level(std::string map_filename, std::string dungeon_filename, std::string level_key);
	Level(std::string map_filename, std::string dungeon_filename, std::string level_key, int, int, int, int);
	Level(std::string, int, int, int, int);
	Level(int, int, int, int);
	~Level();
	void load_tileset();
	void load_from_xml();
	void reload_from_xml(Level &copy_level);
	void reset_for_reload();
	void reload_tiles(Level &copy_level);
	void intialize_dimensions();
	void initialize_empty();
	void initialize_tiles();
	void initialize_blocks();
	void generate_blocks();
	void initialize_entity_groups();
	void initialize_entity_group(EntityGroup *eg);
	void initialize_tiled_images();
	void initialize_spawners();
	void clear_level();
	void remove_tile_edges();
	void load_tile_edges();
	void draw_tile_edge_bitmaps();
	void save_to_xml();
	void unload_content();
	void update(int game_mode);
	void draw(ALLEGRO_DISPLAY *display, std::pair<int, int> offset);
	void draw_edge_tile_onto_bitmap(Tile &tile, std::string edge_filename, int edge_row, int dir_key);
	void add_edge_to_tile(Tile *tile, int edge_row, int dir_key, std::string tile_key);
	void add_entity(Entity *e);
	void add_being(Being *b);
	void remove_player();
	void remove_game_images(int type);
	void remove_entities(int type);
	void remove_beings(int type);
	std::vector<Entity*> get_interactables(Entity*);
	std::vector<Tile*> get_nearby_tiles(Entity*);
	std::vector<Entity> get_player_interactables();
	std::vector<std::string> get_layers();
	void remove_tile(std::pair<int, int> pos);
	void remove_block(std::pair<int, int> pos);
	void remove_entity_group(std::pair<int, int> pos);
	bool remove_tiled_image(const std::pair<int, int> pos, const int layer_index);
	void remove_spawner(const std::pair<int, int> pos);
	void replace_tile(int tile_index, std::pair<int, int> ss_pos, std::pair<int, int> pos);
	void replace_block(int block_index, std::pair<int, int> ss_pos, std::pair<int, int> pos);
	void add_entity_group(int eg_index, std::pair<int, int> ss_pos, std::pair<int, int> pos);
	EntityGroup * create_entity_group(std::string filename_start, int index, std::pair<int, int> ss_pos, std::pair<int, int> pos);
	void add_tiled_image(const int ti_index, const std::pair<int, int> ss_pos, const std::pair<int, int> pos, const int layer_index);
	TiledImage * create_tiled_image(std::string filename_start, int index, std::pair<int, int> ss_pos, std::pair<int, int> pos);
	void add_spawner(const int spawner_index, const std::pair<int, int> ss_pos, const std::pair<int, int> pos);
	Spawner * create_spawner(std::string filename_start, int index, std::pair<int, int> ss_pos, std::pair<int, int> pos);
	const bool has_spawner_for_key(const std::string spawn_key);
	Spawner * spawner_for_key(const std::string spawn_key);
	void add_npc_at_spawner(NPC * npc, const std::string spawn_key);
	void set_tile(Tile * tile, std::pair<int, int> pos);
	Tile *get_tile(int x, int y);
	bool passable_at(int, int);
	TileSet *get_tileset();
	void set_tileset_key(std::string key);
	const std::string get_tileset_key();
	EntityGroup * entity_group_at_tile_pos(const std::pair<int, int> pos);
	Spawner * spawner_at_tile_pos(const std::pair<int, int> pos);
	std::string get_dungeon_filename();
	void set_dungeon_filename(std::string value);
	std::string get_filename();
	void set_filename(std::string value);
	std::string get_id();
	std::pair<int, int> get_dimensions();
	int get_width();
	int get_height();
	void set_grid_x(const int value);
	int get_grid_x();
	void set_grid_y(const int value);
	int get_grid_y();
	void set_grid_width(const int value);
	int get_grid_width();
	void set_grid_height(const int value);
	int get_grid_height();
	// methods for level editor
	void draw_tiles_onto_bitmap(ALLEGRO_BITMAP *bitmap);
	void draw_blocks_onto_bitmap(ALLEGRO_BITMAP *bitmap);
	void draw_entity_groups_onto_bitmap(ALLEGRO_BITMAP *bitmap);
	void draw_tiled_images_onto_bitmap(ALLEGRO_BITMAP *bitmap);
	void draw_spawners_onto_bitmap(ALLEGRO_BITMAP *bitmap);
};
#endif