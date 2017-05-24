#ifndef LEVEL_H
#define LEVEL_H
//#include "Player.h"
//#include "Platform.h"
//#include "GameImageManager.h"
#include "Player.h"
#include "Platform.h"
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
	//MainGameScreen* screen;
	//Player* player;
	//std::vector<Platform> platforms;
	std::string filename = "";
	std::string id = "";
	int grid_x;
	int grid_y;
	int width;
	int height;
	//std::vector<std::vector<Level*>>* dungeon_grid;
public:
	static const int STANDARD_LEVEL_GRID_WIDTH = 800;
	static const int STANDARD_LEVEL_GRID_HEIGHT = 600;
	Level();
	Level(std::string, std::string, int, int, int, int);
	Level(int, int, int, int);
	~Level();
	//void load_images(ImageLoader&);
	//void set_player(Player*);
	void update(std::map<int, bool>);
	//std::list<GameImage> get_images();
	//void change_player_level();
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