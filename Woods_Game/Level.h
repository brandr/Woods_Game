#ifndef LEVEL_H
#define LEVEL_H

#include "Tile.h"
#include "TileSet.h"
#include "Being.h"
#include "allegro5/display.h"  // for ALLEGRO_DISPLAY
#include "utility"             // for pair
#include "vector"              // for vector
#include "xstring"             // for string
#include "EntityGroup.h"
#include "XMLSerialization.h"
#include "TileGroup.h"

class Level : public xmls::Serializable
	//TODO: serialize
{
private:
	std::string map_filename = "";
	std::string dungeon_filename;
	std::string id = "";
	TileSet *tileset;
	xmls::Collection<TileGroup> tile_rows;	//serialized blocks are stored in here
	xmls::Collection<EntityGroup> entity_groups; // these may also be stored in entities
	std::vector<GameImage*> game_images;
	std::vector<Entity*> entities;
	std::vector<Being*> beings;
	xmls::xInt grid_x;
	xmls::xInt grid_y;
	xmls::xInt grid_width;
	xmls::xInt grid_height;
	int width;
	int height;
public:
	static const int STANDARD_LEVEL_GRID_WIDTH = 800;
	static const int STANDARD_LEVEL_GRID_HEIGHT = 600;
	Level();
	Level(std::string map_filename, std::string dungeon_filename, std::string level_key);
	Level(std::string map_filename, std::string dungeon_filename, std::string level_key, int, int, int, int);
	Level(std::string, int, int, int, int);
	Level(int, int, int, int);
	~Level();
	void load_tileset();
	void load_from_map();
	void load_from_xml();
	void intialize_dimensions();
	void initialize_tiles();
	void initialize_blocks();
	void generate_blocks();
	void initialize_entity_groups();
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
	Tile *get_tile(int x, int y);
	bool passable_at(int, int);
	std::string get_dungeon_filename();
	void set_dungeon_filename(std::string value);
	std::string get_filename();
	std::string get_id();
	std::pair<int, int> get_dimensions();
	int get_width();
	int get_height();
	int get_grid_x();
	int get_grid_y();
	int get_grid_width();
	int get_grid_height();
	// methods for level editor
	void draw_tiles_onto_bitmap(ALLEGRO_BITMAP *bitmap);
	void draw_blocks_onto_bitmap(ALLEGRO_BITMAP *bitmap);
	void draw_entity_groups_onto_bitmap(ALLEGRO_BITMAP *bitmap);
};
#endif