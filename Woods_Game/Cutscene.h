
#ifndef CUTSCENE_H
#define CUTSCENE_H

#include <vector>
#include <string>
#include <map>
#include <allegro5/bitmap.h>
#include <allegro5/display.h>
#include <allegro5/color.h>
#include <allegro5/allegro_color.h>

#include "Dialog.h"
#include "GlobalTime.h"
#include "TriggerStatus.h"

#define EFFECT_FADE_TO_BLACK "effect_fade_to_black"
#define EFFECT_DISPLAY_BLACK "effect_display_black"

// loading/global update actions
#define ACTION_AWAIT_LOAD "action_await_load"
#define ACTION_SAVE_GAME "action_save_game"
#define ACTION_LOAD_GAME "action_load_game"
#define ACTION_UPDATE_GLOBAL_TIME "action_update_global_time"
#define ACTION_UPDATE_NEW_DAY "update_new_day"

// other actions
#define ACTION_DIALOG "action_dialog"
#define ACTION_WALK_AGENTS "action_walk_agents"
#define CUTSCENE_AGENT_PLAYER "player"

class Level;
class World;

struct CutsceneAgentAnimation {
	std::string agent_key, animation_key;
	int direction;
	CutsceneAgentAnimation(const std::string agent, const std::string anim, const int dir);
};

struct CutsceneBranch {
	int next_block_index;
	std::vector<Qualifier *> qualifiers;
	CutsceneBranch(const int index, std::vector<Qualifier *> quals);
	const bool qualifies(World * world, Level * level);
};

struct CutsceneTile {
	int tile_x, tile_y, direction;
	std::string agent_key;
	CutsceneTile(const std::string agent, const int x, const int y, const int direction);
};

struct CutsceneBlock {
	int block_index = 0;
	int next_block_index = -1;
	int timer = 0;
	int duration;
	bool has_opened_dialog = false;
	bool has_processed_music = false;
	std::vector<CutsceneTile> cutscene_tiles;
	std::vector<TriggerStatus *> block_triggers;
	std::vector<CutsceneBranch> cutscene_branches;
	std::vector<CutsceneAgentAnimation> agent_animations;
	Dialog * dialog;
	std::string effect_key = "";
	std::string action_key = "";
	std::string music_key = "";
	GlobalTime * global_time = NULL;
	CutsceneBlock();
	void update(const bool update_timer);
	const int get_next_block_index(World * world, Level * level);
	const std::vector<ALLEGRO_BITMAP*> get_filters(ALLEGRO_DISPLAY * display, const int width, const int height);
	ALLEGRO_BITMAP * find_or_create_filter(ALLEGRO_DISPLAY *display, const int r, const int g, const int b, const int a);
	const bool is_finished();
	const bool process_action();
	void set_has_opened_dialog(const bool value);
};

class Cutscene
{
private:
	std::string cutscene_key = "";
	bool timer_active = true;
	int current_block_index = 0;
	std::map<int, CutsceneBlock *> cutscene_blocks;
	const bool has_current_cutscene_block();
	bool is_finished = false;
	bool action_flag = false;
	bool update_trigger_statuses = false;
public:
	Cutscene();
	~Cutscene();
	void update(World * world, Level * level);
	void advance_block(World * world, Level * level);
	const std::vector<ALLEGRO_BITMAP *> get_filters(ALLEGRO_DISPLAY * display, const int width, const int height);
	void add_block(CutsceneBlock * block);
	void add_effect(const std::string effect_key, const int duration);
	void add_action(const std::string action_key);
	void add_action(const std::string action_key, const std::string effect_key);
	void add_effect_fade_block(const int duration, const int end_alpha);
	void add_global_time_update(const int day, const int time);
	void add_advance_day_update(GlobalTime * global_time, const int wake_up_time);
	void add_load_game_update(const int day, const int time);
	void set_cutscene_key(const std::string key);
	const std::string get_cutscene_key();
	const bool get_is_finished();
	const bool has_action();
	const std::string get_active_action_key();
	GlobalTime * get_active_global_time();
	std::vector<CutsceneTile> get_current_block_cutscene_tiles();
	std::vector<CutsceneBranch> get_current_block_cutscene_branches();
	// dialog
	Dialog * get_active_dialog();
	void set_has_opened_dialog(const bool value);
	const bool has_opened_dialog();
	// triggers
	const bool should_update_trigger_statuses();
	std::vector<TriggerStatus *> trigger_statuses();
	// music
	void process_music();
	// animations
	std::vector<CutsceneAgentAnimation> get_current_block_agent_animations();
};
#endif