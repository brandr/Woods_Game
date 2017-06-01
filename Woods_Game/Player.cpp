#include "Player.h"
#include<iostream>
#include<algorithm>



Player::Player()
{
	//TODO: figure out how animations will 
	//image_filename = "images/temp_player.png";
	direction = DIR_NEUTRAL;
	anim_state = ANIM_NEUTRAL;
	base_walk_speed = 5;
	jump_speed = 30;
}

Player::~Player()
{
}

int Player::get_type()
{
	return PLAYER;
}

//NOTE: might not need these arguments, need to look at how we end up creating the player
void Player::load_content(std::vector<std::string> attributes, std::vector<std::string> contents)
{
	GameImage::load_content(attributes, contents);
	animation_dir_map[std::pair<int, int>(ANIM_STATE_NEUTRAL, DIR_NEUTRAL)] = 0;
	animation_dir_map[std::pair<int, int>(ANIM_STATE_WALKING, DIR_RIGHT)] = 1;
	animation_dir_map[std::pair<int, int>(ANIM_STATE_WALKING, DIR_LEFT)] = 2;
	animation_dir_map[std::pair<int, int>(ANIM_STATE_WALKING, DIR_UP)] = 3;
	animation_dir_map[std::pair<int, int>(ANIM_STATE_WALKING, DIR_DOWN)] = 4;
	refresh_mask();
	//TODO: if necessary, map player direction to different animations
}

void Player::update(std::vector<Entity> interactables, std::vector<Tile> nearby_tiles, std::pair<int, int> level_dimensions, int game_mode)
{
	if (exit_level_check(level_dimensions))
		return;
	//TODO
	switch (game_mode) {
	case SIDE_SCROLLING:
		update_side_scrolling(interactables, level_dimensions);
		break;
	case TOP_DOWN:
		update_top_down(interactables, level_dimensions);
		break;
	}
	Being::update(interactables, nearby_tiles, level_dimensions, game_mode);
	clear_input();
}

void Player::update_side_scrolling(std::vector<Entity> interactables, std::pair<int, int> level_dimensions)
{
	//TODO: joystick-based movement
	if (jumping && on_ground(interactables)) jumping = false;
	switch (direction) {
	case DIR_LEFT:
		xvel = -1 * get_walk_speed();
		break;
	case DIR_RIGHT:
		xvel = get_walk_speed();
		break;
	case DIR_NEUTRAL:
		xvel = 0;
		break;
	}
	if (jumping && yvel > 0)
		jumping = false;
	//std::map<int, bool>::const_iterator it = move_map.find(MOVE_UP);
	//if (it != move_map.end() && it->second) {
	if(check_move(MOVE_UP))
		attempt_jump(interactables);
	//}
}

//TODO: may want to update direction if necessary for animations
void Player::update_top_down(std::vector<Entity> interactables, std::pair<int, int> level_dimensions)
{
	//TODO: figure out how best to handle joystick and keyboard inputs that occur at the same time
	xvel = 0, yvel = 0;
	anim_state = ANIM_NEUTRAL;
	//keyboard
	if (check_move(MOVE_LEFT) && !check_move(MOVE_RIGHT)) {
		xvel = -1 * get_walk_speed();
		direction = DIR_LEFT, anim_state = ANIM_STATE_WALKING;
	}
	else if (check_move(MOVE_RIGHT) && !check_move(MOVE_LEFT)) {
		xvel = get_walk_speed();
		direction = DIR_RIGHT, anim_state = ANIM_STATE_WALKING;
	}
	if (check_move(MOVE_DOWN) && !check_move(MOVE_UP)) {
		yvel = get_walk_speed();
		direction = DIR_DOWN, anim_state = ANIM_STATE_WALKING;
	}
	else if (check_move(MOVE_UP) && !check_move(MOVE_DOWN)) {
		yvel = -1 * get_walk_speed();
		direction = DIR_UP, anim_state = ANIM_STATE_WALKING;
	}
	//joystick
	if (abs(move_joystick_pos.first) <= 0.05f && abs(move_joystick_pos.second) <= 0.05f) {
		return;
	}
	xvel = move_joystick_pos.first*get_walk_speed(), yvel = move_joystick_pos.second*get_walk_speed();
	if (xvel >= 0) {
		if (xvel >= std::abs(yvel)) direction = DIR_RIGHT;
		else if (yvel > 0) direction = DIR_DOWN;
		else direction = DIR_UP;
	}
	else {
		if (std::abs(xvel) >= std::abs(yvel)) direction = DIR_LEFT;
		else if (yvel > 0) direction = DIR_DOWN;
		else direction = DIR_UP;
	}
	anim_state = ANIM_STATE_WALKING;
}

void Player::update_input(std::map<int, bool> input_map, std::map<int, std::pair<float, float>> joystick_map, int game_mode)
{
	auto it = joystick_map.find(LEFT_STICK);
	if (it != joystick_map.end())
		set_joystick_movement(it->second);
	switch (game_mode) {
	case SIDE_SCROLLING:
		update_input_side_scrolling(input_map, joystick_map);
		break;
	case TOP_DOWN:
		update_input_top_down(input_map, joystick_map);
		break;
	}
}

void Player::update_input_side_scrolling(std::map<int, bool> input_map, std::map<int, std::pair<float, float>> joystick_map)
{
	if (input_map[INPUT_LEFT] && !input_map[INPUT_RIGHT])
		set_direction(DIR_LEFT);
	else if (!input_map[INPUT_LEFT] && input_map[INPUT_RIGHT])
		set_direction(DIR_RIGHT);
	else
		set_direction(DIR_NEUTRAL);
	if (input_map[INPUT_UP])
		queue_move(MOVE_UP);
	else if (is_jumping()) {
		set_yvel(0);
		set_jumping(false);
	}
}

void Player::update_input_top_down(std::map<int, bool> input_map, std::map<int, std::pair<float, float>> joystick_map)
{
	if (input_map[INPUT_UP])
		queue_move(MOVE_UP);
	if (input_map[INPUT_DOWN])
		queue_move(MOVE_DOWN);
	if (input_map[INPUT_LEFT])
		queue_move(MOVE_LEFT);
	if (input_map[INPUT_RIGHT])
		queue_move(MOVE_RIGHT);
}

float Player::get_walk_speed()
{
	//TODO: get tile modifier

	return base_walk_speed;
}

void Player::set_direction(int dir)
{
	direction = dir;
}

void Player::queue_move(int move)
{
	move_map[move] = true;
}

bool Player::check_move(int move)
{
	std::map<int, bool>::const_iterator it = move_map.find(move);
	return (it != move_map.end() && it->second);
}

void Player::clear_input()
{
	//TODO: consider iterating through entire move map unless we want some moves to stay for some reason
	move_map[MOVE_UP] = false;
	move_map[MOVE_DOWN] = false;
	move_map[MOVE_LEFT] = false;
	move_map[MOVE_RIGHT] = false;
	//move_joystick_pos = std::pair<float, float>(0.0f, 0.0f);
}

void Player::set_joystick_movement(std::pair<float, float> pos)
{
	move_joystick_pos = pos;
}

void Player::attempt_jump(std::vector<Entity> interactables)
{
	if (on_ground(interactables)) {
		jumping = true;
		yvel = -1 * jump_speed;
	}
}

void Player::set_jumping(bool jumping)
{
	this->jumping = jumping;
}

bool Player::is_jumping()
{
	return jumping;
}

void Player::set_exit_level_flag(bool flag)
{
	exit_level_flag = flag;
}

bool Player::get_exit_level_flag()
{
	return exit_level_flag;
}

//TODO: need to check for no next level in the given direction
bool Player::exit_level_check(std::pair<int, int> level_dimensions)
{
	if (outside_level(level_dimensions)) {
		exit_level_flag = true;
		return true;
	}
	return false;
}
