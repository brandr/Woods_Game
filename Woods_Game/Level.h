#ifndef LEVEL_H
#define LEVEL_H
#include "Player.h"
//#include "Tile.h"
#include<map>
#include<list>
#include <iostream>
#include <utility>

//class MainGameScreen;
class ImageLoader;
class GameImageManager;


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
	void unload_content();
	void update(int game_mode);
	void draw(ALLEGRO_DISPLAY *display, std::pair<int, int> offset);
	std::pair<int, int> string_to_pair(std::string tile_string);
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