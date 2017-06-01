#ifndef PLAYER_H
#define PLAYER_H

#include "Being.h"
#include<map>

enum MAIN_GAME_INPUTS { INPUT_UP, INPUT_DOWN, INPUT_LEFT, INPUT_RIGHT };
enum PLAYER_DIRECTIONS{DIR_NEUTRAL, DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN};
enum PLAYER_MOVES{MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT};
//final const int TERMINAL_VELOCITY = 8;
//final const int GRAVITY = 2;
class Player :
	public Being
{
private:
	std::map<int, bool> move_map;
	std::pair<float, float> move_joystick_pos;
	float base_walk_speed;
	float jump_speed;
	bool jumping = false;
	bool exit_level_check(std::pair<int, int>);
	bool exit_level_flag = false; //TODO: store this in a larger set of flags if we add more flags
public:
	Player();
	virtual ~Player();
	virtual int get_type();
	virtual void load_content(std::vector<std::string> attributes, std::vector<std::string> contents);
	virtual void update(std::vector<Entity>, std::vector<Tile>, std::pair<int,int>, int);
	void update_side_scrolling(std::vector<Entity>, std::pair<int, int>);
	void update_top_down(std::vector<Entity>, std::pair<int, int>);
	void update_input(std::map<int, bool>, std::map<int, std::pair<float,float>>, int);
	void update_input_side_scrolling(std::map<int, bool>, std::map<int, std::pair<float, float>>);
	void update_input_top_down(std::map<int, bool>, std::map<int, std::pair<float, float>>);
	float get_walk_speed();
	void set_direction(int);
	void queue_move(int);
	bool check_move(int);
	void clear_input();
	void set_joystick_movement(std::pair<float, float> pos);
	void attempt_jump(std::vector<Entity> interactables);
	void set_jumping(bool);
	bool is_jumping();
	void set_exit_level_flag(bool);
	bool get_exit_level_flag();
};

#endif