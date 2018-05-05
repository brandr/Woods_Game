#ifndef PLAYER_H
#define PLAYER_H

#define JOYSTICK_DEADZONE 0.1f

//#include "Inventory.h"
#include "Inventory.h"
#include "Player.h"                // for PLAYER_DIRECTIONS::DIR_DOWN, PLAYER_DIRECTIONS::DIR_LEFT, PLAYER_DIRECTIONS::DIR_NEUTRAL, PLAYER_DIRECTIONS::DIR_RIGHT, PLAYER_DIRECTIONS::DIR_UP
#include "allegro5/bitmap_draw.h"  // for al_draw_bitmap
#include "FileManager.h"           // for FileManager
#include "ImageLoader.h"
#include "SpriteSheetAnimation.h"  // for SpriteSheetAnimation
#include "stdlib.h"                // for atoi
#include "vcruntime_new.h"         // for operator delete, operator new
#include "World.h"
#include "xtree"                   // for _Tree_iterator, _Tree<>::iterator, _Tree_const_iterator
#include "XMLSerialization.h"
#include <map>

enum MAIN_GAME_INPUTS { INPUT_UP, INPUT_DOWN, INPUT_LEFT, INPUT_RIGHT, INPUT_PAUSE };
//enum PLAYER_DIRECTIONS{DIR_NEUTRAL, DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN};
enum PLAYER_MOVES{MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT};
enum ACTION_KEYS{ACTION_NONE, ACTION_SHEAR};
//final const int TERMINAL_VELOCITY = 8;
//final const int GRAVITY = 2;
class Player :
	public Being, public xmls::Serializable
{
private:
	xmls::xFloat base_walk_speed;
	xmls::xFloat jump_speed;
	std::map<int, bool> move_map;
	std::pair<float, float> move_joystick_pos;
	bool jumping = false;
	bool exit_level_check(std::pair<int, int>);
	bool exit_level_flag = false; //TODO: store this in a larger set of flags if we add more flags
	Inventory inventory;
	int current_action = ACTION_NONE;
public:
	Player();
	virtual ~Player();
	virtual int get_type();
	virtual void load_content(std::vector<std::string> attributes, std::vector<std::string> contents);
	virtual void load_content_from_attributes();
	void reset_entity_flags();
	virtual void update(std::vector<Entity*>, std::vector<Tile*>, std::pair<int,int>, int);
	void update_side_scrolling(std::vector<Entity*>, std::pair<int, int>);
	void update_top_down(std::vector<Entity*>, std::vector<Tile*>, std::pair<int, int>);
	void update_input(std::map<int, bool>, std::map<int, std::pair<float,float>>, int);
	void update_input_side_scrolling(std::map<int, bool>, std::map<int, std::pair<float, float>>);
	void update_input_top_down(std::map<int, bool>, std::map<int, std::pair<float, float>>);
	void shear_update(std::vector<Entity*> interactables, std::vector<Tile*> nearby_tiles, std::pair<int, int> level_dimensions);
	float get_walk_speed();
	void set_direction(int);
	void queue_move(int);
	bool check_move(int);
	void clear_input();
	void set_joystick_movement(std::pair<float, float> pos);
	void attempt_jump(std::vector<Entity*> interactables);
	void set_jumping(bool);
	bool is_jumping();
	void set_exit_level_flag(bool);
	bool get_exit_level_flag();
	void use_selected_item();
	void hotbar_index_left();
	void hotbar_index_right();
	void use_shears();
	int get_current_action();
	Item *get_selected_item();
	Inventory &get_inventory();
};

#endif