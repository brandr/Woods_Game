#include "Player.h"
#include<iostream>
#include<algorithm>



Player::Player()
{
	//TODO: figure out how animations will 
	//image_filename = "images/temp_player.png";
	direction = MOVE_STILL;
	walk_speed = 5;
	jump_speed = 30;
}

Player::~Player()
{
	//for (std::map<int, bool>::iterator it = move_map.begin(); it != move_map.end(); ++it)
	//	delete it->second;

	//delete move_map;
}

int Player::get_type()
{
	return PLAYER;
}

//NOTE: might not need these arguments, need to look at how we end up creating the player
void Player::load_content(std::vector<std::string> attributes, std::vector<std::string> contents)
{
	GameImage::load_content(attributes, contents);
	//TODO: if necessary, map player direction to different animations
}

void Player::update(std::vector<Entity> interactables, std::pair<int, int> level_dimensions)
{
	if (exit_level_check(level_dimensions))
		return;
	if (jumping && on_ground(interactables)) jumping = false;
	switch (direction) {
	case MOVE_LEFT:
		xvel = -1 * walk_speed;
		break;
	case MOVE_RIGHT:
		xvel = walk_speed;
		break;
	case MOVE_STILL:
		xvel = 0;
		break;
	}
	if (jumping && yvel > 0)
		jumping = false;
	std::map<int,bool>::const_iterator it = move_map.find(MOVE_UP);
	if (it != move_map.end() && it->second) {
		attempt_jump(interactables);
	}
	Being::update(interactables, level_dimensions);
	clear_move_map();
}

void Player::update_input(std::map<int, bool> input_map)
{
	//std::cout << "UPDATING INPUT " << std::endl;
	if (input_map[INPUT_LEFT] && !input_map[INPUT_RIGHT])
		set_direction(MOVE_LEFT);
	else if (!input_map[INPUT_LEFT] && input_map[INPUT_RIGHT])
		set_direction(MOVE_RIGHT);
	else
		set_direction(MOVE_STILL);
	if (input_map[INPUT_UP])
		queue_move(MOVE_UP);
	else if (is_jumping()) {
		set_yvel(0);
		set_jumping(false);
	}
}

void Player::set_direction(int dir)
{
	direction = dir;
}

void Player::queue_move(int move)
{
	move_map[move] = true;
}

void Player::clear_move_map()
{
	move_map[MOVE_UP] = false;
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
