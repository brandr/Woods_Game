
#ifndef CUTSCENE_H
#define CUTSCENE_H


#include <vector>
#include <string>
#include <map>
#include <allegro5/bitmap.h>
#include <allegro5/display.h>
#include <allegro5/color.h>
#include <allegro5/allegro_color.h>
#include "GlobalTime.h"

#define EFFECT_FADE_TO_BLACK "effect_fade_to_black"
#define EFFECT_DISPLAY_BLACK "effect_display_black"

#define ACTION_AWAIT_LOAD "action_await_load"
#define ACTION_SAVE_GAME "action_save_game"
#define ACTION_LOAD_GAME "action_load_game"
#define ACTION_UPDATE_GLOBAL_TIME "action_update_global_time"
#define ACTION_UPDATE_NEW_DAY "update_new_day"

struct CutsceneBlock {
	int timer = 0;
	int duration;
	std::string effect_key = "";
	std::string action_key = "";
	GlobalTime * global_time = NULL;
	CutsceneBlock();
	void update(const bool update_timer);
	const std::vector<ALLEGRO_BITMAP*> get_filters(ALLEGRO_DISPLAY * display, const int width, const int height);
	ALLEGRO_BITMAP * find_or_create_filter(ALLEGRO_DISPLAY *display, const int r, const int g, const int b, const int a);
	const bool is_finished();
	const bool process_action();
};

class Cutscene
{
private:
	bool timer_active = true;
	int current_block_index = 0;
	std::vector<std::unique_ptr<CutsceneBlock>> cutscene_blocks;
	const bool has_current_cutscene_block();
	bool is_finished = false;
	bool action_flag = false;
public:
	Cutscene();
	~Cutscene();
	void update();
	void advance_block();
	const std::vector<ALLEGRO_BITMAP *> get_filters(ALLEGRO_DISPLAY * display, const int width, const int height);
	void add_effect(const std::string effect_key, const int duration);
	void add_action(const std::string action_key);
	void add_action(const std::string action_key, const std::string effect_key);
	void add_effect_fade_block(const int duration, const int end_alpha);
	void add_global_time_update(const int day, const int time);
	void add_advance_day_update(GlobalTime * global_time, const int wake_up_time);
	void add_load_game_update(const int day, const int time);
	const bool get_is_finished();
	const bool has_action();
	const std::string get_active_action_key();
	GlobalTime * get_active_global_time();
};
#endif