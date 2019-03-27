#ifndef GAMEIMAGEMANAGER_H
#define GAMEIMAGEMANAGER_H

#define START_TIME_HOUR 3
#define TIME_RATIO 400 // number of time counter ticks per hour (probably want 5000-10000 for actual game, not sure though)

#define SUNRISE_START_MINUTES 240
#define SUNRISE_END_MINUTES 360
#define DAY_START_MINUTES 480
#define DAY_END_MINUTES 1020
#define SUNSET_START_MINUTES 1080
#define SUNSET_END_MINUTES 1200

#define NIGHT_B 80
#define NIGHT_A 140

#define SUNRISE_END_R 160
#define SUNRISE_END_G 120
#define SUNRISE_END_B 0
#define SUNRISE_END_A 70

#define DAY_START_R 200
#define DAY_START_G 200
#define DAY_START_A 40

#define DAY_END_R 200
#define DAY_END_G 200
#define DAY_END_A 40

#define SUNSET_START_R 160
#define SUNSET_START_G 120
#define SUNSET_START_B 0
#define SUNSET_START_A 70

#define SUNSET_END_R 0
#define SUNSET_END_G 0
#define SUNSET_END_B 80
#define SUNSET_END_A 140

#include <string>
#include "utility"  // for pair
#include "xstring"  // for string
#include "allegro5/bitmap.h"   // for ALLEGRO_BITMAP
#include "World.h"
#include "Player.h"
#include "FileManager.h"

class GameImageManager
{
private:
	int game_mode = TOP_DOWN;
	FileManager file_manager;
	std::pair<int, int> get_camera_offset(ALLEGRO_DISPLAY *display);
	std::pair<int, int> tile_image_offset(std::string tile_string);
	World world;
	int time_counter = 0;
	ALLEGRO_BITMAP * light_filter = NULL;
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
	const std::string time_display_string();
	void time_update();
	const int get_current_minutes();
	void change_player_level();
	void draw(ALLEGRO_DISPLAY *display);
	void draw_light_filter(ALLEGRO_DISPLAY *display, std::pair<int, int> offset);
	void resume();
	void decrement_dialog_option();
	void increment_dialog_option();
};

#endif