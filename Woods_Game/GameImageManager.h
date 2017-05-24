#include<string>
#include<algorithm>
#include "World.h"
#include"GameScreen.h"
#include <memory>
//#include "FileManager.h"


class GameImageManager
{
private:
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
	std::vector<GameImage*> game_images;
	std::vector<Entity*> entities;
	std::vector<Being*> beings;
	std::vector<Platform*> platforms;
	Player* player;
	GameImageManager();
	~GameImageManager();
	void load_content();
	void load_level(int, int);
	void load_level_content(std::string filename, std::string id, int type);
	void load_level_from_map(std::string layerID, std::string mapID);
	void unload_content();
	void unload_level_content();
	void add_platform(Platform *p);
	void update(std::map<int, bool>);
	std::vector<Entity> get_player_interactables();
	void change_player_level();
	void draw(ALLEGRO_DISPLAY *display);
};

