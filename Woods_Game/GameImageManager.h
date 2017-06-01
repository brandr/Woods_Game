#include<string>
#include<algorithm>
#include "World.h"
#include"GameScreen.h"
#include <memory>
//#include "FileManager.h"


class GameImageManager
{
private:
	/*
	std::vector<std::vector<Tile>> tiles;
	std::vector<GameImage*> game_images;
	std::vector<Entity*> entities;
	std::vector<Being*> beings;
	*/
	//std::vector<Block*> blocks;
	std::pair<int, int> get_camera_offset();
	std::pair<int, int> tile_image_offset(std::string tile_string);
	World world;
public:
	//std::vector<std::vector<Level*>> dungeon_grid;	//not sure if it's better to keep this here or in dungeon
	Level* current_level;
	/*
	std::vector<GameImage*> game_images;
	std::vector<Entity*> entities;
	std::vector<Being*> beings;
	std::vector<Platform*> platforms;
	*/
	//std::vector<std::unique_ptr<GameImage>> game_images;
	
	Player* player;
	GameImageManager();
	~GameImageManager();
	void load_content();
	int get_game_mode();
	void load_level(int, int);
	void load_level_content(std::string filename, std::string id, int type);
	//void load_level_from_map(std::string layerID, std::string mapID);
	void load_level_from_map(Level level);
	void unload_content();
	void unload_level_content();
	//void add_block(Block *p);
	void update(std::map<int, bool>, std::map<int, std::pair<float, float>>);
	std::vector<Entity> get_interactables(Entity*);
	std::vector<Tile> get_nearby_tiles(Entity*);
	std::vector<Entity> get_player_interactables();
	/*
	std::vector<std::vector<Tile>> get_tiles();
	std::vector<GameImage*> get_game_images();
	std::vector<Entity*> get_entities();
	std::vector<Being*> get_beings();
	*/
	void change_player_level();
	void draw(ALLEGRO_DISPLAY *display);
};

