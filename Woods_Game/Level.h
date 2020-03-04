#ifndef LEVEL_H
#define LEVEL_H

#include "Tile.h"
#include "TileSet.h"
#include "GameImage.h"
#include "Being.h"
#include "BiomeManager.h"
#include "Critter.h"
#include "NPC.h"
#include "allegro5/display.h"  // for ALLEGRO_DISPLAY
#include "utility"             // for pair
#include "vector"              // for vector
#include "xstring"             // for string
#include "EntityGroup.h"
#include "ItemPickup.h"
#include "LevelGenData.h"
#include "LevelGenManager.h"
#include "LocationMarker.h"
#include "XMLSerialization.h"
#include "PathNode.h"
#include "Spawner.h"
#include "TileGroup.h"
#include "TiledImageLayer.h"
#include "MathUtilities.cpp"
#include <set>
#include <stdlib.h>     /* srand, rand */

#define STANDARD_LEVEL_GRID_WIDTH 832
#define STANDARD_LEVEL_GRID_HEIGHT 832
#define COLLIDE_BUCKET_LEVEL_COLS 16
#define COLLIDE_BUCKET_LEVEL_ROWS 16

#define LEVEL_MAP_SCALE 30 // actual pixels to map pixels

#define LEVEL_GEN_PASS_DAYS 5 // this many "days" pass when generating a level

class World;
class Level : public xmls::Serializable
{
private:
	std::string map_filename = "";
	std::string dungeon_filename;
	std::string id = "";
	TileSet *tileset;
	xmls::xString tileset_key;
	xmls::xString biome_key;
	xmls::Collection<TileGroup> tile_rows;	
	xmls::Collection<Block> blocks;
	xmls::Collection<EntityGroup> entity_groups; // these may also be stored in entities
	xmls::Collection<Spawner> spawners;
	xmls::Collection<PathNode> path_nodes;
	xmls::Collection<LocationMarker> location_markers;
	xmls::Collection<TiledImageLayer> tiled_image_layers;
	xmls::xString gen_data_key;
	std::vector<Being*> beings;
	std::vector<ItemPickup*> item_pickups;
	xmls::xInt grid_x;
	xmls::xInt grid_y;
	xmls::xInt grid_width;
	xmls::xInt grid_height;
	std::map<std::string, std::set<Entity *>> collide_buckets; 
	std::map<std::string, std::pair<int, int>> named_path_nodes;
	int width;
	int height;
	void draw_tiled_images(ALLEGRO_DISPLAY *display, const std::pair<int, int> offset, const int layer, const int screen_w, const int screen_h);
	const bool generate_paths(LevelGenData * gen_data, std::vector<SiteRect *> site_rects, const std::string site_key,
		const int start_x, const int start_y, const int width, const int height);
	const bool generate_tiles(LevelGenData * gen_data, const int start_x, const int start_y, const int width, const int height);
	const bool generate_entity_groups(LevelGenData * gen_data, const int start_x, const int start_y, const int width, const int height);
	const bool generate_blocks(LevelGenData * gen_data, const int start_x, const int start_y, const int width, const int height);
	void generate_forced_tiles(LevelGenData * gen_data, const int x_off, const int y_off);
	void generate_forced_blocks(LevelGenData * gen_data, const int x_off, const int y_off);
	void generate_forced_entity_groups(LevelGenData * gen_data, const int x_off, const int y_off);
	std::pair<bool, std::vector<SiteRect *>> generate_sites(LevelGenData * gen_data, const int start_x, const int start_y, const int width, const int height);
	void generate_pass_days(World * world, Player * player);
	void update_collide_buckets(Entity * e);
	void reset_collide_buckets();
	const std::vector<std::string> collide_bucket_keys(Rect * collide_rect);
	const std::vector<std::pair<std::string, std::pair<int, int>>> connect_path_nodes(const std::pair<int, int> offset, const std::pair<int, int> dimensions,
		const int tile_index, std::vector<SiteRect *> site_rects,
		const std::pair<std::string, std::pair<int, int>> pos1,
		const std::pair<std::string, std::pair<int, int>> pos2, 
		const std::vector<std::pair<std::string, std::pair<int, int>>> visited, const int path_size);
	const std::vector<std::pair<std::string, std::pair<int, int>>> generate_weaving_paths(const std::pair<int, int> offset, const std::pair<int, int> dimensions,
		const int tile_index, std::vector<SiteRect *> site_rects, 
		const std::string key_1, const std::string key_2, 
		const std::pair<int, int> pos1, const std::pair<int, int> pos2,
		const std::vector<std::pair<std::string, std::pair<int, int>>> visited, const int path_size);
	const std::vector<std::pair<std::string, std::pair<int, int>>> generate_weaving_coordinates(const std::pair<int, int> offset, const std::pair<int, int> dimensions,
		std::vector<SiteRect *> site_rects,
		const std::string key_1, const std::string key_2,
		const int x1, const int y1, const int x2, const int y2, const int path_size, int seed);
	const std::vector<std::pair<int, int>> connect_path_nodes_straight_line(const int tile_index, 
		const std::pair<int, int> pos1, 
		const std::pair<int, int> pos2, const int path_size);
	const bool valid_for_sites(std::vector<SiteRect*> site_rects, const std::set<std::string> ignore_keys, const int tx, const int ty);
	std::vector<Tile *> get_tiles_in_line(const int x1, const int y1, const int x2, const int y2);
	std::vector<Tile *> get_tiles_in_line(const int x1, const int y1, const int x2, const int y2, const int line_width);
	LevelGenData * get_level_gen_data(const std::string data_key);
public:
	//draw layers
	static const int LAYER_INDEX_TILES = 0;
	static const int LAYER_INDEX_BLOCKS = 10;
	static const int LAYER_INDEX_BEINGS = 20;
	Level();
	Level(std::string map_filename, std::string dungeon_filename, std::string level_key);
	~Level();
	void load_tileset();
	void load_from_xml();
	void reload_from_xml(Level &copy_level);
	void reset_for_reload();
	void reload_tiles(Level &copy_level);
	ALLEGRO_BITMAP * generate_cell_map_image(const int width, const int height, const int grid_x, const int grid_y);
	void update_new_day(World * world, Player * player);
	void update_tiles_new_day(World * world, Player * player);
	void update_npcs_new_day();
	void plant_day_update(World * world, Entity * plant, const int x, const int y);
	void intialize_dimensions(const int standard_width, const int standard_height);
	void initialize_empty();
	void initialize_tiles();
	void initialize_blocks();
	void generate_level(World * world, Player * player);
	void initialize_entity_groups();
	void initialize_entity_group(EntityGroup *eg);
	void initialize_tiled_images();
	void initialize_spawners();
	void initialize_path_nodes();
	void initialize_location_markers();
	void initialize_biome();
	void generate_critters(World * world, GlobalTime * time);
	void clear_level();
	void remove_tile_edges();
	void load_tile_edges();
	void draw_tile_edge_bitmaps();
	void clear_all_beings();
	void save_to_xml();
	void unload_content();
	void update(World * world, GlobalTime * time, const int game_mode);
	void cutscene_animation_update();
	void draw(ALLEGRO_DISPLAY *display, GlobalTime * global_time, const std::pair<int, int> offset);
	void draw_edge_tile_onto_bitmap(Tile &tile, const std::string edge_filename, const int edge_row, const int dir_key);
	void add_edge_to_tile(Tile *tile, const int edge_row, const int dir_key, const std::string tile_key);
	void add_being(Being *b);
	void add_item_pickup(ItemPickup *ip);
	void remove_item_pickup(ItemPickup *ip);
	void remove_player();
	void remove_being(Being *b);
	void remove_beings(const int type);
	Being * get_player();
	std::vector<Entity*> get_interactables(Entity*);
	std::vector<Entity*> get_interactables(Entity*, const bool ignore_moving_obstacles);
	std::vector<Entity*> get_colliding_interactables(Entity* e, Rect collide_rect, const bool ignore_moving_obstacles);
	std::vector<Entity*> get_moving_interactables(Entity* entity);
	std::vector<Entity*> get_nearby_interactables(Entity* entity, Rect collide_rect,  const bool ignore_moving_obstacles);
	std::vector<Entity*> player_visible_entities();
	std::vector<Tile*> get_nearby_tiles(Entity*);
	std::vector<Tile*> get_tiles_in_range(Entity* entity, const int range);
	std::vector<Tile*> get_tiles_in_range(const int tx, const int ty, const int t_width, const int t_height, const int range);
	Block * get_block(const int tx, const int ty);
	void remove_tile(std::pair<int, int> pos);
	void remove_block(std::pair<int, int> pos);
	void remove_entity_group(std::pair<int, int> pos);
	bool remove_tiled_image(const std::pair<int, int> pos, const int layer_index);
	void remove_spawner(const std::pair<int, int> pos);
	void remove_path_node(const std::pair<int, int> pos);
	void remove_location_marker(const std::pair<int, int> pos);
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
	void add_critter(Critter * critter, const float x_pos, const float y_pos);
	void add_critter_with_key(const std::string critter_key);
	void add_path_node(const int path_node_index, const std::pair<int, int> ss_pos, const std::pair<int, int> pos);
	PathNode * create_path_node(const std::string filename_start, const int index, const std::pair<int, int> ss_pos, const std::pair<int, int> pos);
	const bool has_path_node_for_key(const std::string node_key);
	PathNode * find_path_node_with_key(const std::string node_key);
	const bool has_any_path_node();
	PathNode * find_any_path_node();
	void add_location_marker(const int marker_index, const std::pair<int, int> ss_pos, const std::pair<int, int> pos);
	LocationMarker * create_location_marker(const std::string filename_start, const int index, const std::pair<int, int> ss_pos, const std::pair<int, int> pos);
	std::vector<LocationMarker *> get_location_markers();
	LocationMarker * find_location_marker_matching_level(const std::string level_key);
	const std::string get_biome_key();

	// controls
	void process_mouse_click_left(World * world, GlobalTime * time, const std::pair<int, int> offset, const int mouse_x, const int mouse_y);
	void process_mouse_click_right(World * world, GlobalTime * time, const std::pair<int, int> offset, const int mouse_x, const int mouse_y);

	// tile stuff
	void set_tile(Tile * tile, std::pair<int, int> pos);
	Tile *get_tile(int x, int y);
	bool passable_at(int, int);
	TileSet *get_tileset();
	void set_tileset_key(std::string key);
	const std::string get_tileset_key();
	EntityGroup * entity_group_at_tile_pos(const std::pair<int, int> pos);
	EntityGroup * entity_group_at_tile_pos(const std::pair<int, int> pos, const bool root_only);
	Spawner * spawner_at_tile_pos(const std::pair<int, int> pos);
	PathNode * path_node_at_tile_pos(const std::pair<int, int> pos);
	LocationMarker * location_marker_at_tile_pos(const std::pair<int, int> pos);
	std::vector<PathNode *> get_path_nodes();
	const bool has_rect_collisions(Entity * e, Rect collide_rect, const bool ignore_moving_obstacles);
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
	// for frozen screen
	ALLEGRO_BITMAP * take_screenshot(ALLEGRO_DISPLAY * display, GlobalTime * global_time, const int x_off, const int y_off, const int width, const int height);
	// methods for level editor
	void draw_tiles_onto_bitmap(ALLEGRO_BITMAP *bitmap, Rect &subsection);
	void draw_blocks_onto_bitmap(ALLEGRO_BITMAP *bitmap, Rect &subsection, const bool check_visible);
	void draw_entity_groups_onto_bitmap(ALLEGRO_BITMAP *bitmap, Rect &subsection);
	void draw_tiled_images_onto_bitmap(ALLEGRO_BITMAP *bitmap, Rect &subsection);
	void draw_tiled_images_onto_bitmap(ALLEGRO_BITMAP *bitmap, Rect &subsection, const int layer);
	void draw_spawners_onto_bitmap(ALLEGRO_BITMAP *bitmap, Rect &subsection);
	void draw_path_nodes_onto_bitmap(ALLEGRO_BITMAP *bitmap, Rect &subsection);
	void draw_location_markers_onto_bitmap(ALLEGRO_BITMAP *bitmap, Rect &subsection);
};
#endif