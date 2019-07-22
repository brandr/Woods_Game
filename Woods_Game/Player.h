#ifndef PLAYER_H
#define PLAYER_H

#define JOYSTICK_DEADZONE 0.1f

#include "Cutscene.h"
#include "Dialog.h"
#include "GlobalTime.h"
#include "Inventory.h"
#include "Player.h"                // for PLAYER_DIRECTIONS::DIR_DOWN, PLAYER_DIRECTIONS::DIR_LEFT, PLAYER_DIRECTIONS::DIR_NEUTRAL, PLAYER_DIRECTIONS::DIR_RIGHT, PLAYER_DIRECTIONS::DIR_UP
#include "allegro5/bitmap_draw.h"  // for al_draw_bitmap
#include "FileManager.h"           // for FileManager
#include "ImageLoader.h"
#include "SpriteSheetAnimation.h"  // for SpriteSheetAnimation
#include "stdlib.h"                // for atoi
#include "vcruntime_new.h"         // for operator delete, operator new
#include "TileSet.h"
#include "World.h"
#include "xtree"                   // for _Tree_iterator, _Tree<>::iterator, _Tree_const_iterator
#include "XMLSerialization.h"
#include <map>

enum MAIN_GAME_INPUTS { INPUT_UP, INPUT_DOWN, INPUT_LEFT, INPUT_RIGHT, INPUT_PAUSE };
enum PLAYER_MOVES{MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT};
enum ACTION_KEYS{ACTION_NONE, ACTION_SHEAR};

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
	std::string destination_level_key_override = "";
	std::pair<int, int> destination_level_pos_override = std::pair<int,int>(-1,-1);
	bool interacting = false;
	Inventory inventory;
	int current_action = ACTION_NONE;
	Dialog * open_dialog;
	Cutscene * active_cutscene;
	bool should_open_calendar = false;
protected:
	virtual void collide_with_entity(Entity* e);
public:
	Player();
	virtual ~Player();
	virtual int get_type();
	virtual void load_content_from_attributes();
	void reset_entity_flags();
	virtual void update(Level * level, GlobalTime *time, const int game_mode);
	void update_side_scrolling(std::vector<Entity*>, std::pair<int, int>);
	void update_top_down(std::vector<Entity*>, std::vector<Tile*>, std::pair<int, int>);
	void update_input(std::map<int, bool>, std::map<int, std::pair<float,float>>, int);
	void update_input_side_scrolling(std::map<int, bool>, std::map<int, std::pair<float, float>>);
	void update_input_top_down(std::map<int, bool>, std::map<int, std::pair<float, float>>);
	void interact_update(std::vector<Entity*> interactables, std::vector<Tile*> nearby_tiles, std::pair<int, int> level_dimensions);
	//time
	const int wake_up_time();
	//dialog
	void dialog_update();
	const bool has_open_dialog();
	void close_dialog();
	void open_calendar();
	void close_calendar();
	const bool get_should_open_calendar();
	//cutscenes
	const std::vector<ALLEGRO_BITMAP*> get_cutscene_filters(ALLEGRO_DISPLAY * display, const int width, const int height);
	void cutscene_update();
	void end_active_cutscene();
	//interact
	const bool interact(Entity* e);
	void shear_update(std::vector<Entity*> interactables, std::vector<Tile*> nearby_tiles, std::pair<int, int> level_dimensions);
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
	void use_selected_item();
	void hotbar_index_left();
	void hotbar_index_right();
	void interact();
	void use_shears();
	int get_current_action();
	Item *get_selected_item();
	Inventory &get_inventory();
	void advance_dialog();
	Dialog * get_open_dialog();
	void set_open_dialog(Dialog * dialog);
	void decrement_dialog_option();
	void increment_dialog_option();
	const bool has_active_cutscene();
	Cutscene * get_active_cutscene();
	const std::string get_spawn_key();
};

#endif