#ifndef LEVEL_H
#define LEVEL_H

#include "Tile.h"
#include "Being.h"
#include "allegro5/display.h"  // for ALLEGRO_DISPLAY
#include "utility"             // for pair
#include "vector"              // for vector
#include "xstring"             // for string
#include "EntityGroup.h"

//class ImageLoader;

class Level
{
private:
	std::string filename = "";
	std::string id = "";
	//temp
	std::vector<std::vector<Tile>> tiles;
	std::vector<GameImage*> game_images;
	std::vector<Entity*> entities;
	std::vector<Being*> beings;
	//Player* player;
	//temp
	int grid_x;
	int grid_y;
	int width;
	int height;
public:
	static const int STANDARD_LEVEL_GRID_WIDTH = 800;
	static const int STANDARD_LEVEL_GRID_HEIGHT = 600;
	Level();
	Level(std::string, std::string, int, int, int, int);
	Level(std::string, int, int, int, int);
	Level(int, int, int, int);
	~Level();
	void load_from_map();
	void load_tile_edges(std::string filename);
	void unload_content();
	void update(int game_mode);
	void draw(ALLEGRO_DISPLAY *display, std::pair<int, int> offset);
	void draw_edge_tile(Tile &tile, std::string edge_filename, int edge_row, int dir_key);
	void add_entity(Entity *e);
	void add_being(Being *b);
	void remove_player();
	void remove_game_images(int type);
	void remove_entities(int type);
	void remove_beings(int type);
	std::vector<Entity*> get_interactables(Entity*);
	std::vector<Tile> get_nearby_tiles(Entity*);
	std::vector<Entity> get_player_interactables();
	std::vector<std::string> get_layers();
	bool passable_at(int, int);
	std::string get_filename();
	std::string get_id();
	std::pair<int, int> get_dimensions();
	int get_width();
	int get_height();
	int get_grid_x();
	int get_grid_y();
	int grid_width();
	int grid_height();
};
#endif