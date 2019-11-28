#ifndef PLAYER_H
#define PLAYER_H

#define JOYSTICK_DEADZONE 0.1f
#define PLAYER_VISION_RADIUS 400

#include "AudioManager.h"
#include "Being.h"
#include "Critter.h"
#include "Cutscene.h"
#include "Dialog.h"
#include "GlobalTime.h"
#include "Inventory.h"             // for PLAYER_DIRECTIONS::DIR_DOWN, PLAYER_DIRECTIONS::DIR_LEFT, PLAYER_DIRECTIONS::DIR_NEUTRAL, PLAYER_DIRECTIONS::DIR_RIGHT, PLAYER_DIRECTIONS::DIR_UP
#include "allegro5/bitmap_draw.h"  // for al_draw_bitmap
#include "FileManager.h"           // for FileManager
#include "ImageLoader.h"
#include "ItemManager.h"
#include "SpriteSheetAnimation.h"  // for SpriteSheetAnimation
#include "stdlib.h"                // for atoi
#include "vcruntime_new.h"         // for operator delete, operator new
#include "TileSet.h"
#include "xtree"                   // for _Tree_iterator, _Tree<>::iterator, _Tree_const_iterator
#include "XMLSerialization.h"
#include <map>

enum MAIN_GAME_INPUTS { INPUT_UP, INPUT_DOWN, INPUT_LEFT, INPUT_RIGHT, INPUT_PAUSE };
enum PLAYER_MOVES{MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT};
enum ACTION_KEYS{ACTION_NONE, ACTION_SHEAR, ACTION_SWING};

class Level;
class Player :
	public Being, public xmls::Serializable
{
private:
	xmls::xFloat jump_speed;
	xmls::xString spawn_key;
	std::map<int, bool> move_map;
	std::pair<float, float> move_joystick_pos;
	bool jumping = false;
	bool exit_level_check(std::pair<int, int>);
	bool exit_level_flag = false; //TODO: store this in a larger set of flags if we add more flags
	std::map<std::string, std::string> pending_trigger_updates;
	std::map<std::string, bool> pending_quest_item_updates;
	std::string destination_level_key_override = "";
	std::pair<int, int> destination_level_pos_override = std::pair<int,int>(-1,-1);
	bool interacting = false;
	Inventory inventory;
	int current_action = ACTION_NONE;
	Dialog * open_dialog = NULL;
	Cutscene * active_cutscene;
	std::string active_cutscene_key = "";
	std::string exchange_inventory_key = "";
	bool should_close_dialog = false;
	bool should_open_calendar = false;
	void quest_item_update(World * world, Level * level, GlobalTime * time);
	void visible_entities_update(World * world, Level * level, GlobalTime * time);
	void collect_item_pickup(World * world, Level * level, ItemPickup * pickup);
	const bool can_swing_at_entity(Entity * e, const std::string swing_key);
	void catch_critter(World* world, Level * level, Critter * critter);
protected:
	virtual void collide_with_entity(World * world, Level * level, Entity* e);
	virtual void play_sounds_for_entity(Entity* e);
	virtual const float calculate_gain_for_sound(EntitySound * sound);
	virtual const float calculate_pan_for_sound(EntitySound * sound);
	virtual void update_animation_dir();
public:
	Player();
	virtual ~Player();
	virtual int get_type();
	virtual void load_content_from_attributes();
	void reset_entity_flags();
	void reset_serialized_data();
	virtual void update(World * world, Level * level, GlobalTime *time, const int game_mode);
	void update_side_scrolling(std::vector<Entity*>, std::pair<int, int>);
	void update_top_down(World * world, Level * level);
	void update_input(std::map<int, bool>, std::map<int, std::pair<float,float>>, int);
	void update_input_side_scrolling(std::map<int, bool>, std::map<int, std::pair<float, float>>);
	void update_input_top_down(std::map<int, bool>, std::map<int, std::pair<float, float>>);
	void interact_update(World * world, Level * level, GlobalTime * time);
	virtual void draw(ALLEGRO_DISPLAY * display, int x_off, int y_off);
	//audio
	virtual void emit_sound_update(World * world, Level * level, GlobalTime * time, const int game_mode);
	virtual void play_sound_update(World * world, Level * level, GlobalTime * time, const int game_mode);
	virtual const std::string get_sound_key();
	//time
	const int wake_up_time();
	//dialog
	void dialog_update(World * world, Level * level);
	const bool has_open_dialog();
	void close_dialog();
	void open_calendar();
	void close_calendar();
	const bool get_should_open_calendar();
	//cutscenes
	const std::vector<ALLEGRO_BITMAP*> get_cutscene_filters(ALLEGRO_DISPLAY * display, const int width, const int height);
	void cutscene_update(World * world, Level * level);
	void end_active_cutscene();
	void set_active_cutscene_key(const std::string key);
	const std::string get_active_cutscene_key();
	//interact
	const bool interact(World * world, Level * level, GlobalTime * time, Entity* e);
	void swing_update(World * world, Level * level, const std::string swing_key);
	void sleep_in_bed(GlobalTime * current_time);
	void load_game_for_day(const int day);
	float get_walk_speed();
	void set_direction(int);
	void queue_move(int);
	bool check_move(int);
	void clear_input();
	void set_joystick_movement(std::pair<float, float> pos);
	void attempt_jump(std::vector<Entity*> interactables);
	void set_jumping(bool);
	bool is_jumping();
	void set_destination_level_key_override(const std::string key);
	const std::string get_destination_level_key_override();
	void set_destination_level_pos_override(const std::pair<int, int> pos);
	const std::pair<int, int> get_destination_level_pos_override();
	void reset_exit_level();
	void set_exit_level_flag(bool);
	bool get_exit_level_flag();
	// items
	void use_selected_item();
	const bool has_item_with_key(const int item_key);
	Item * get_item_with_key(const int item_key);
	void remove_item_with_key(const int item_key);
	void add_item_with_key(const int item_key);
	void hotbar_index_left();
	void hotbar_index_right();
	void interact();
	void use_shears();
	void swing_item(const std::string swing_key);
	int get_current_action();
	Item *get_selected_item();
	Inventory &get_inventory();
	void set_exchange_inventory_key(const std::string inventory_key);
	const std::string get_exchange_inventory_key();
	// money
	void adjust_money(const float adjust_amount);
	// dialog
	void advance_dialog();
	Dialog * get_open_dialog();
	void set_open_dialog(Dialog * dialog);
	void decrement_dialog_option();
	void increment_dialog_option();
	const bool has_active_cutscene();
	Cutscene * get_active_cutscene();
	void set_active_cutscene(Cutscene * scene);
	const std::string get_spawn_key();
	const std::map<std::string, std::string> get_pending_trigger_updates();
	void clear_pending_triggers();
	void set_has_met_npc(const std::string npc_key);
	// plants
	const bool gather_plant(Entity * plant);
	// quests
	void add_pending_quest_item_update(const std::string quest_item_key, const bool has_item);
	void clear_pending_quest_item_updates();
};

#endif