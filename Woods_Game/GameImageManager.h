#ifndef GAMEIMAGEMANAGER_H
#define GAMEIMAGEMANAGER_H

#include<string>
#include "utility"  // for pair
#include "xstring"  // for string
#include "World.h"
#include "Player.h"

class GameImageManager
{
private:
	int game_mode = TOP_DOWN;
	FileManager file_manager;
	std::pair<int, int> get_camera_offset(ALLEGRO_DISPLAY *display);
	std::pair<int, int> tile_image_offset(std::string tile_string);
	World world;
	void load_player();
	void load_player_from_xml(std::string filepath, std::string player_key);
public:
	Level* current_level;
	Player* player;
	GameImageManager();
	~GameImageManager();
	void load_content();
	void set_game_mode(int game_mode);
	int get_game_mode();
	void load_level(int, int);
	void load_player(std::string filename);
	void load_level_content(std::string filename, std::string id, int type);
	void load_level_from_map(Level *level);
	Player* get_player();
	void unload_content();
	void unload_level_content();
	void update(std::map<int, bool>, std::map<int, std::pair<float, float>>);
	void change_player_level();
	void draw(ALLEGRO_DISPLAY *display);
	void resume();
};

#endif