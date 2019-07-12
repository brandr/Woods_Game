#include "Player.h"
#include "Level.h"
#include <iostream>
#include <algorithm>



void Player::collide_with_entity(Entity * e)
{
	Being::collide_with_entity(e);
	e->contact_action(this);
}

Player::Player()
{
	// xml serialization attributes
	setClassName("Player");
	Register("start_pos_x", &entity_starting_pos_x);
	Register("start_pos_y", &entity_starting_pos_y);
	Register("animation_spritesheet_key", &animation_spritesheet_key);
	Register("center_offset_x", &center_offset_x);
	Register("center_offset_y", &center_offset_y);
	Register("spritesheet_frame_width", &spritesheet_frame_width);
	Register("spritesheet_frame_height", &spritesheet_frame_height);
	Register("base_walk_speed", &base_walk_speed);
	Register("jump_speed", &jump_speed);
	Register("animation_data", &animation_data);
	Register("additional_mask_data", &additional_mask_data);
	Register("inventory", &inventory);
	Register("spawn_key", &spawn_key);
	this->direction = DIR_NEUTRAL;
	this->anim_state = ANIM_NEUTRAL;
	this->solid = true; // the player counts as solid because NPCs will not be able to walk through
}

Player::~Player()
{
}

int Player::get_type()
{
	return PLAYER;
}

void Player::load_content_from_attributes()
{
	Entity::load_content_from_attributes();
	inventory.load_content_from_attributes();
}

void Player::reset_entity_flags()
{
	set_entity_attribute(E_ATTR_HIT_OTHER, 0);
}

void Player::update(Level * level, const int game_mode)
{
	const std::pair<int, int> level_dimensions = level->get_dimensions();
	std::vector<Entity*> interactables = level->get_interactables(this);
	std::vector<Tile*> nearby_tiles = level->get_nearby_tiles(this);
	//TEMP. can probably pass the level into the check instead
	if (exit_level_check(level_dimensions))
		return;
	switch (game_mode) {
	case SIDE_SCROLLING:
		update_side_scrolling(interactables, level_dimensions);
		break;
	case TOP_DOWN:
		update_top_down(interactables, nearby_tiles, level_dimensions);
		break;
	case MAIN_GAME_DIALOG:
		dialog_update();
		break;
	}

	Being::update(level, game_mode);
	if (this->interacting) {
		this->interact_update(interactables, nearby_tiles, level_dimensions);
	}
	clear_input();
}

void Player::update_side_scrolling(std::vector<Entity*> interactables, std::pair<int, int> level_dimensions)
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
	if (jumping && yvel > 0) {
		jumping = false;
	}
	if (check_move(MOVE_UP)) {
		attempt_jump(interactables);
	}
}

void Player::update_top_down(std::vector<Entity*> interactables, std::vector<Tile*> nearby_tiles, std::pair<int, int> level_dimensions)
{
	if (counters[BOUNCE]) return;
	if (counters[SWING]) {
		switch (current_action) {
		case(ACTION_SHEAR):
			shear_update(interactables, nearby_tiles, level_dimensions);
			break;
		}
		return;
	}
	current_action = ACTION_NONE;
	reset_entity_flags();
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
	} else if (check_move(MOVE_UP) && !check_move(MOVE_DOWN)) {
		yvel = -1 * get_walk_speed();
		direction = DIR_UP, anim_state = ANIM_STATE_WALKING;
	}
	//joystick
	if (abs(move_joystick_pos.first) <= JOYSTICK_DEADZONE && abs(move_joystick_pos.second) <= JOYSTICK_DEADZONE) {
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

void Player::interact_update(std::vector<Entity*> interactables, std::vector<Tile*> nearby_tiles, std::pair<int, int> level_dimensions)
{
	const int t_size = nearby_tiles.size();
	for (int i = 0; i < t_size; i++) {
		Block *b = nearby_tiles[i]->get_block();
		if (b) interactables.push_back(b);
	}
	const int size = interactables.size();

	float x_off = 0, y_off = 0;
	switch (direction) {
	case DIR_NEUTRAL:
		x_off = 0, y_off = 14;
		break;
	case DIR_UP:
		x_off = 0, y_off = -14;
		break;
	case DIR_DOWN:
		x_off = 0, y_off = 14;
		break;
	case DIR_LEFT:
		x_off = -8, y_off = 0;
		break;
	case DIR_RIGHT:
		x_off = 8, y_off = 0;
		break;
	}

	const int x1 = this->get_rect_center().first, y1 = this->get_rect_center().second;
	for (int i = 0; i < size; i++) {
		if (interactables[i]->contains_point(x1 + x_off, y1 + y_off)) {
			Entity* e = interactables[i];
			if (this->interact(e)) {
				return;
			}
		}
	}
}

const int Player::wake_up_time()
{
	return 1000; //temp. need some constant, along with things like alarm clocks
	// maybe set the base variable on player when loading it in?
}

void Player::dialog_update()
{
	if (this->open_dialog) {
		const std::string action_key = this->open_dialog->get_active_action_key();
		if (action_key.length() > 0) {
			const InteractActionManager &manager = InteractActionManager::get_instance();
			manager.run_action(action_key, this->open_dialog->get_action_bindings(), this);
		}  else {
			this->open_dialog->update();
		}
	}
}

const bool Player::has_open_dialog()
{
	return this->open_dialog != NULL;
}

void Player::close_dialog()
{
	if (this->open_dialog != NULL) {
		delete(this->open_dialog);
		this->open_dialog = NULL;
	}
}

void Player::open_calendar()
{
	this->should_open_calendar = true;
}

void Player::close_calendar()
{
	this->should_open_calendar = false;
}

const bool Player::get_should_open_calendar()
{
	return this->should_open_calendar;
}

const std::vector<ALLEGRO_BITMAP*> Player::get_cutscene_filters(ALLEGRO_DISPLAY * display, const int width, const int height)
{
	std::vector<ALLEGRO_BITMAP*> filters;
	if (this->has_active_cutscene()) {
		filters = this->active_cutscene->get_filters(display, width, height);
	}
	return filters;
}

void Player::cutscene_update()
{
	if (this->has_active_cutscene()) {
		if (!this->active_cutscene->has_action()) {
			this->active_cutscene->update();	
		}
		if (!this->active_cutscene->has_action() && this->active_cutscene->get_is_finished()) {
			this->end_active_cutscene();
		}
	}
}

void Player::end_active_cutscene()
{
	if (this->active_cutscene != NULL) {
		delete(this->active_cutscene);
		this->active_cutscene = NULL;
	}
}

const bool Player::interact(Entity * e)
{
	return e->interact_action(this);
}

void Player::shear_update(std::vector<Entity*> interactables, std::vector<Tile*> nearby_tiles, std::pair<int, int> level_dimensions)
{
	if (get_entity_attribute(E_ATTR_HIT_OTHER) == 1) return;
	const int t_size = nearby_tiles.size();
	for(int i = 0; i < t_size; i++){
		Block *b = nearby_tiles[i]->get_block();
		if (b) interactables.push_back(b);
	}
	const int size = interactables.size();
	
	// TODO: define mask prefix ("player") more generally
	mask_t *shear_mask = this->get_additional_mask("slicing", "player", direction); // additional_masks[std::pair<std::string, int>("slicing", direction)];
	float x_off = 0, y_off = 0;
	switch(direction){
		case DIR_NEUTRAL:
			x_off = 16, y_off = 52;
			break;
		case DIR_UP:
			x_off = 16, y_off = 8;
			break;
		case DIR_DOWN:
			x_off = 16, y_off = 52;
			break;
		case DIR_LEFT:
			x_off = -8, y_off = 16;
			break;
		case DIR_RIGHT:
			x_off = 40, y_off = 16;
			break;
	}

	for (int i = 0; i < size; i++) {
		if (interactables[i]->get_mask() && Mask_Collide(shear_mask, interactables[i]->get_mask(),
			get_x() + x_off - interactables[i]->get_x(),
			get_y() + y_off - interactables[i]->get_y())) {
				Entity* e = interactables[i];
				if (e->has_entity_attribute(E_ATTR_SHEARABLE) 
					&& e->get_entity_attribute(E_ATTR_BROKEN) != 1) {
					set_entity_attribute(E_ATTR_HIT_OTHER, 1);
					Item *shears = get_selected_item();
					const int shear_power = shears->get_item_attribute(Item::ITEM_ATTR_POWER);
					e->take_durability_damage(shear_power);
					return;
				}
			}
	}
}

void Player::sleep_in_bed(GlobalTime * current_time)
{
	Cutscene * cutscene = new Cutscene();
	cutscene->add_effect(EFFECT_FADE_TO_BLACK, 175);
	cutscene->add_effect(EFFECT_DISPLAY_BLACK, 60);
	cutscene->add_advance_day_update(current_time, this->wake_up_time());
	cutscene->add_action(ACTION_SAVE_GAME);
	this->active_cutscene = cutscene;
}

void Player::load_game_for_day(const int day)
{
	Cutscene * cutscene = new Cutscene();
	cutscene->add_effect(EFFECT_FADE_TO_BLACK, 175);
	cutscene->add_load_game_update(day, this->wake_up_time());
	cutscene->add_effect(EFFECT_DISPLAY_BLACK, 60);
	this->active_cutscene = cutscene;
}

float Player::get_walk_speed()
{
	return base_walk_speed.value();
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
	move_joystick_pos = std::pair<float, float>(0.0f, 0.0f);
	interacting = false;
}

void Player::set_joystick_movement(std::pair<float, float> pos)
{
	move_joystick_pos = pos;
}

void Player::attempt_jump(std::vector<Entity*> interactables)
{
	if (on_ground(interactables)) {
		jumping = true;
		yvel = -1 * jump_speed.value();
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

void Player::set_destination_level_key_override(const std::string key)
{
	this->destination_level_key_override = key;
}

const std::string Player::get_destination_level_key_override()
{
	return this->destination_level_key_override;
}

void Player::set_destination_level_pos_override(const std::pair<int, int> pos)
{
	this->destination_level_pos_override = pos;
}

const std::pair<int, int> Player::get_destination_level_pos_override()
{
	return this->destination_level_pos_override;
}

void Player::reset_exit_level()
{
	this->exit_level_flag = false;
	this->destination_level_key_override = "";
	this->destination_level_pos_override = std::pair<int, int>(-1, -1);
}

void Player::set_exit_level_flag(bool flag)
{
	exit_level_flag = flag;
}

bool Player::get_exit_level_flag()
{
	return exit_level_flag;
}

void Player::use_selected_item()
{
	if (current_action != ACTION_NONE) return;
	if (counters[BOUNCE] || counters[SWING]) return;
	Item *item = inventory.get_selected_hotbar_item();
	if (item) {
		switch (item->get_item_key()) {
		case ITEM_SHEARS:
			use_shears();
			break;
		}
	}
}

void Player::hotbar_index_left()
{
	if (current_action != ACTION_NONE) return;
	inventory.hotbar_index_left();
}

void Player::hotbar_index_right()
{
	if (current_action != ACTION_NONE) return;
	inventory.hotbar_index_right();
}

void Player::interact()
{
	this->interacting = true;
}

void Player::use_shears()
{
	xvel = 0, yvel = 0;
	counters[SWING] = 19; //TEMP. This is done to match up with the animation length.
	anim_state = ANIM_STATE_SHEARING;
	current_action = ACTION_SHEAR;
	get_animation()->reset();
	ss_animation->reset();
}

int Player::get_current_action()
{
	return current_action;
}

Item * Player::get_selected_item()
{
	return inventory.get_selected_hotbar_item();
}

Inventory& Player::get_inventory()
{
	return inventory;
}

void Player::advance_dialog()
{
	if (this->open_dialog) {
		this->open_dialog->advance_dialog();
		if (!this->open_dialog->has_current_page()) {
			this->close_dialog();
		}
	}
}

Dialog * Player::get_open_dialog()
{
	return this->open_dialog;
}

void Player::set_open_dialog(Dialog * dialog)
{
	if (this->open_dialog != NULL) {
		delete (this->open_dialog);
	}
	this->open_dialog = dialog;
}

void Player::decrement_dialog_option()
{
	if (this->open_dialog != NULL) {
		this->open_dialog->decrement_option();
	}
}

void Player::increment_dialog_option()
{
	if (this->open_dialog != NULL) {
		this->open_dialog->increment_option();
	}
}

const bool Player::has_active_cutscene()
{
	return this->active_cutscene != NULL;
}

Cutscene * Player::get_active_cutscene()
{
	return this->active_cutscene;
}

const std::string Player::get_spawn_key()
{
	return this->spawn_key.value();
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
