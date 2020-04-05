#ifndef GAMEIMAGEMANAGER_H
#define GAMEIMAGEMANAGER_H

#define START_TIME_HOUR 3

#define STILL_SCREEN_IMAGE "still_screen_image"

#include <string>
#include <set>
#include "utility"  // for pair
#include "xstring"  // for string
#include "allegro5/bitmap.h"   // for ALLEGRO_BITMAP
#include "allegro5/threads.h"
#include "FilterManager.h"
#include "World.h"
#include "Player.h"
#include "FileManager.h"
#include "GlobalTime.h"
#include "Cutscene.h"
#include "CutsceneScript.h"
#include "InteractActionManager.h"
#include "ItemManager.h"
#include "LoadingData.h"
#include "LocationMarker.h"
#include "Quest.h"
#include "QuestData.h"
#include "CritterManager.h"
#include "Encyclopedia.h"
#include "boost/filesystem.hpp"



class GameImageManager
{
private:
	int game_mode = TOP_DOWN;
	FileManager file_manager;
	CutsceneList cutscene_list;
	std::pair<int, int> get_camera_offset(ALLEGRO_DISPLAY *display);
	std::pair<int, int> tile_image_offset(std::string tile_string);
	World world;
	GlobalTime * current_global_time = NULL;
	void load_player();
	void load_player_from_xml(std::string filepath, std::string player_key);
	void load_all_cutscenes();
	void load_item_templates();
	void load_critter_templates();
	CutsceneScript * get_cutscene_script(const std::string cutscene_key);
	Cutscene * generate_cutscene(CutsceneScript * script);
	CutsceneBlock * generate_cutscene_block(CutsceneScriptBlock * block);
	const bool player_update(std::map<int, bool>, std::map<int, std::pair<float, float>>);
	void check_player_cutscene(Player * player);
	void player_exploration_update();
	void player_exchange_inventory_update();
	void pending_trigger_update();
	NPC * get_npc(const std::string npc_key);

	ALLEGRO_THREAD * loading_thread;
	LoadingData thread_data;

	std::string still_screen_image_key = "";

	// async functions
	static void *load_func_advance_day(ALLEGRO_THREAD *thr, void *arg);
	static void *load_func_save_game(ALLEGRO_THREAD *thr, void *arg);
	static void *load_func_load_from_save(ALLEGRO_THREAD *thr, void *arg);

	void walk_agents_for_cutscene(Cutscene * cutscene);
	const bool has_still_screen_image(const int width, const int height);
	ALLEGRO_BITMAP * get_still_screen_image(const int width, const int height);
	void freeze_screen();
	void unfreeze_screen();
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
	void set_game_mode(int game_mode);
	int get_game_mode();
	const std::string get_world_key();
	const std::string get_current_dungeon_key();
	const int default_level_width();
	const int default_level_height();
	World * get_world();
	Level * get_level_with_key(const std::string level_key);
	std::vector<LocationMarker *> get_current_dungeon_location_markers();
	Player* get_player();
	Inventory * get_exchange_inventory();
	void update_quests();
	QuestData* get_quest_data();
	std::vector<Quest *> get_active_quests();
	std::vector<Quest *> get_failed_quests();
	std::vector<Quest *> get_completed_quests();
	Encyclopedia * get_encyclopedia();
	void unload_content();
	void unload_level_content();
	void update(std::map<int, bool>, std::map<int, std::pair<float, float>>);
	const std::string time_display_string();
	const std::string date_display_string();
	void time_update();
	void npc_update();
	void music_update();
	void wait(const int duration);
	GlobalTime * get_current_global_time();
	const int get_current_minutes();
	const std::string get_current_month_str();
	const int get_current_month_index();
	const int get_first_day_index();
	const int get_first_day_of_month_index();
	const int get_num_days();
	void change_player_level();
	void draw(ALLEGRO_DISPLAY *display);
	void draw_still_screen_image(ALLEGRO_DISPLAY *display, const int width, const int height);
	void draw_filters(ALLEGRO_DISPLAY *display, std::pair<int, int> offset);
	void draw_time_light_filter(ALLEGRO_DISPLAY *display, std::pair<int, int> offset);
	void resume();
	void decrement_dialog_option();
	void increment_dialog_option();
	// cutscene
	void process_cutscene(Cutscene * cutscene);
	void cutscene_animation_update();
	// map
	const std::pair<int, int> current_player_location_for_map();
	const std::set<std::pair<int, int>> explored_map();
	// controls
	void mouse_scroll_update(const int scroll);
	void process_mouse_click_left(const int mouse_x, const int mouse_y);
	void process_mouse_click_right(const int mouse_x, const int mouse_y);
};

#endif