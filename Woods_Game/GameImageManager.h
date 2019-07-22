#ifndef GAMEIMAGEMANAGER_H
#define GAMEIMAGEMANAGER_H

#define START_TIME_HOUR 3

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
#include "allegro5/threads.h"
#include "World.h"
#include "Player.h"
#include "FileManager.h"
#include "GlobalTime.h"
#include "Cutscene.h"
#include "InteractActionManager.h"
#include "LoadingData.h"
#include "boost/filesystem.hpp"



class GameImageManager
{
private:
	int game_mode = TOP_DOWN;
	FileManager file_manager;
	std::pair<int, int> get_camera_offset(ALLEGRO_DISPLAY *display);
	std::pair<int, int> tile_image_offset(std::string tile_string);
	World world;
	GlobalTime * current_global_time = NULL;
	ALLEGRO_BITMAP * light_filter = NULL;
	void load_player_from_xml(std::string filepath, std::string player_key);

	ALLEGRO_THREAD * loading_thread;
	LoadingData thread_data;

	// async functions
	static void *load_func_advance_day(ALLEGRO_THREAD *thr, void *arg);
	static void *load_func_save_game(ALLEGRO_THREAD *thr, void *arg);
	static void *load_func_load_from_save(ALLEGRO_THREAD *thr, void *arg);
public:
	Level* current_level;
	Player* player;
	GameImageManager();
	~GameImageManager();
	void load_content();
	void start_new_game(const std::string world_key);
	void full_load_game_from_save(const std::string filepath);
	void load_game_from_save(const int day, const int time);
	void save_game();
	static void save_game(World * world, GlobalTime * global_time);
	void set_game_mode(int game_mode);
	int get_game_mode();
	Player* get_player();
	void unload_content();
	void unload_level_content();
	void update(std::map<int, bool>, std::map<int, std::pair<float, float>>);
	const std::string time_display_string();
	const std::string date_display_string();
	void time_update();
	void npc_update();
	GlobalTime * get_current_global_time();
	const int get_current_minutes();
	const std::string get_current_month_str();
	const int get_current_month_index();
	const int get_first_day_index();
	const int get_first_day_of_month_index();
	const int get_num_days();
	void change_player_level();
	void draw(ALLEGRO_DISPLAY *display);
	void draw_filters(ALLEGRO_DISPLAY *display, std::pair<int, int> offset);
	void draw_light_filter(ALLEGRO_DISPLAY *display, std::pair<int, int> offset);
	void resume();
	void decrement_dialog_option();
	void increment_dialog_option();
	// cutscene
	void process_cutscene(Cutscene * cutscene);
};

#endif