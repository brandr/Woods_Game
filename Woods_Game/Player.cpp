#include "Player.h"
#include "Level.h"
#include "TriggerStatus.h"
#include "World.h"

#include <iostream>
#include <algorithm>

void Player::quest_item_update(World * world, Level * level, GlobalTime * time)
{
	for (auto const &it : this->pending_quest_item_updates) {
		const std::string quest_item_key = it.first;
		const bool has_item = it.second;
		world->set_has_quest_item(quest_item_key, has_item);
	}
	this->clear_pending_quest_item_updates();
}

void Player::time_update(World * world, Level * level, GlobalTime * time)
{
	const int minutes = time->get_current_minutes();
	if (minutes == 0 || minutes >= MIDNIGHT_MINUTES - 1) {
		Cutscene * cutscene = new Cutscene();
		cutscene->add_pass_out_update(this, time, this->wake_up_time()); //TODO: should we wake up later if passing out?
		this->active_cutscene = cutscene;
		//TODO: pass out cutscene, then wake up next day with lowered stamina
		// (and maybe no stamina upgrade where we normally would?)
	}
}

void Player::visible_entities_update(World * world, Level * level, GlobalTime * time)
{
	world->player_visible_entities_update(level);
}

void Player::stamina_update(World * world, Level * level, GlobalTime * time)
{
	TileSet * tileset = level->get_tileset();
	std::vector<Tile*> nearby_tiles = level->get_nearby_tiles(this);
	float speed_multiplier = get_speed_multiplier(tileset, nearby_tiles);
	float base_speed = this->absolute_move_speed() / speed_multiplier; // how fast we're "trying" to go
	//TODO: formula should make running less efficient than walking
	if (base_speed > 0.0f) {
		//TODO: tweak this formula until the results are sensible										
		float stamina_consumption = (base_speed / speed_multiplier) * STAMINA_CONSUMPTION_MULTIPLIER; 
		this->current_stamina = std::max(0, this->get_current_stamina() - (int)stamina_consumption);
	}	
	//TODO: what else consumes stamina? (might not need to check all of it per-frame)
}

const bool Player::has_low_stamina_1()
{
	return ((float)this->get_current_stamina() / (float)this->get_max_stamina()) < STAMINA_SLOW_1_RATIO;
}

const bool Player::has_low_stamina_2()
{
	return ((float)this->get_current_stamina() / (float)this->get_max_stamina()) < STAMINA_SLOW_2_RATIO;
}

void Player::set_reduced_stamina()
{
	this->current_stamina = (int)((float)this->get_max_stamina() * STAMINA_PASSED_OUT_RATIO);
}

void Player::collect_item_pickup(World * world, Level * level, ItemPickup * pickup)
{
	if (this->inventory.is_full()) {
		//TODO: something to indicate we're full?
		return;
	}
	else {
		Item * item = pickup->get_pickup_item();
		if (item != NULL) {
			const std::string sound_filename = "item_pickup_sounds/" + item->get_item_name();
			const int sound_duration = 10; //TEMP
			this->emit_sound(sound_filename, sound_duration, false);
			this->inventory.add_item(item);
			level->remove_item_pickup(pickup);
		}
	}
}
const bool Player::can_swing_at_entity(Entity * e, const std::string swing_key)
{
	if (e == NULL || swing_key.empty()) {
		return false;
	}
	if (swing_key == "shearing") {
		return e->has_entity_attribute(E_ATTR_SHEARABLE);
	}
	if (swing_key == "netting") {
		if (e->get_type() == CRITTER) {
			return ((Critter *)e)->can_be_netted();
		}
	}
	return false;
}

void Player::catch_critter(World * world, Level * level, Critter * critter)
{
	//TODO: add item?
	world->update_encyclopedia_for_critter(critter, ENTRY_KNOWN);
	level->remove_being(critter);
	Dialog * dialog = CritterManager::get_instance().get_critter_catch_dialog(critter->get_critter_key());
	if (dialog != NULL) {
		this->set_open_dialog(dialog);
	}
	//TODO: could use dialog image (need to implement) to show critter sprite (use encyclopedia sprite?)
}

//TODO: probably need pickups in the set of items we check for (see level)
void Player::collide_with_entity(World * world, Level * level, Entity * e)
{
	Being::collide_with_entity(world, level, e);
	e->contact_action(world, level, this);
	if (e->get_type() == ITEM_PICKUP) {
		this->collect_item_pickup(world, level, (ItemPickup *) e);
		this->colliding_entities.clear();
	}
}

void Player::play_sounds_for_entity(Entity * e)
{
	std::vector<EntitySound*> sounds = e->get_active_entity_sounds();
	for (EntitySound * s : sounds) {
		const std::string sound_key = e->get_sound_key();
		const float pan = this->calculate_pan_for_sound(s);
		const float gain = this->calculate_gain_for_sound(s);
		AudioManager::get_instance().play_sfx(s->filename, sound_key, gain, pan, true);
	}
}

//TODO: tweak this until it sounds good
const float Player::calculate_gain_for_sound(EntitySound * sound)
{
	const float dx = sound->source_x - this->get_x(), dy = sound->source_y - this->get_y();
	const float dist = std::pow(std::pow(std::abs(dx), 2.0) + std::pow(std::abs(dy), 2.0), 0.5);
	const float r1 = (float)(TILE_SIZE * 4.0f); // reference point of "high intensity" is about 4 tiles away
	if (dist < r1) {
		return 1.0f;
	}
	const float gain = std::pow(r1 / dist , 2.0f);
	return std::max(std::min(gain, 2.0f), 0.1f);
}

const float Player::calculate_pan_for_sound(EntitySound * sound)
{
	const float dx = sound->source_x - this->get_x(), dy = sound->source_y - this->get_y();
	const float dist = std::pow(std::pow(std::abs(dx), 2.0) + std::pow(std::abs(dy), 2.0), 0.5);
	if (std::abs(dx) < TILE_SIZE && std::abs(dy) < TILE_SIZE) {
		return 0.0;
	}
	return dx/dist;
}

void Player::update_animation_dir()
{
	if (this->xvel > 0) {
		if (this->xvel >= std::abs(this->yvel)) direction = DIR_RIGHT;
		else if (this->yvel > 0) direction = DIR_DOWN;
		else direction = DIR_UP;
	}
	else {
		if (std::abs(this->xvel) > 0 && std::abs(this->xvel) >= std::abs(this->yvel)) direction = DIR_LEFT;
		else if (this->yvel > 0) direction = DIR_DOWN;
		else if (this->yvel < 0) direction = DIR_UP;
	}
	if (std::abs(this->xvel) > 0 || std::abs(this->yvel) > 0) {
		anim_state = ANIM_STATE_WALKING;
	}
}

const std::string Player::get_footstep_filename_suffix()
{
	return "player";
}

const bool Player::should_adjust_anim_duration()
{
	return Being::should_adjust_anim_duration() || this->has_low_stamina_1() || this->has_low_stamina_2();
}

// "speed" actually means "frame duration" in this case, which is confusing but makes the calculation easier
const float Player::speed_for_anim_duration()
{
	//TODO: is this always the correct way to check if we're swinging something?
	if (this->current_action == ACTION_SWING) {
		Animation* anim = this->get_animation();
		if (anim != NULL) {
			const float base_duration = (float) anim->get_frame_duration();
			if (this->has_low_stamina_2()) {
				return base_duration / STAMINA_SLOW_2_SPEED_RATIO;
			} else if (this->has_low_stamina_1()) {
				return base_duration / STAMINA_SLOW_1_SPEED_RATIO;
			}
		}		
	}
	return Being::speed_for_anim_duration();
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
	Register("current_stamina", &current_stamina);
	Register("max_stamina", &max_stamina);
	Register("is_running", &is_running);
	Register("spawn_key", &spawn_key);
	Register("inventory", &inventory);
	Register("animation_data", &animation_data);
	Register("walk_animation_data", &walk_animation_data);
	Register("additional_mask_data", &additional_mask_data);
	Register("entity_effect_data", &entity_effect_data);	
	this->direction = DIR_NEUTRAL;
	this->anim_state = ANIM_NEUTRAL;
	this->solid = true; // the player counts as solid because NPCs will not be able to walk through
	this->active_cutscene_key = "";
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
	Being::load_content_from_attributes();
	inventory.load_content_from_attributes();
}

void Player::reset_entity_flags()
{
	set_entity_attribute(E_ATTR_HIT_OTHER, 0);
}

void Player::reset_serialized_data()
{
	this->inventory.reset();
}

void Player::update(World * world, Level * level, GlobalTime * time, const int game_mode)
{
	const std::pair<int, int> level_dimensions = level->get_dimensions();
	if (exit_level_check(level_dimensions))
		return;
	switch (game_mode) {
	case SIDE_SCROLLING:
		break;
	case TOP_DOWN:
		update_top_down(world, level);
		time_update(world, level, time);
		break;
	case MAIN_GAME_DIALOG:
		dialog_update(world, level);
		break;
	}

	Being::update(world, level, time, game_mode);
	if (this->interacting) {
		this->interact_update(world, level, time);
	}
	this->quest_item_update(world, level, time);
	this->visible_entities_update(world, level, time);
	this->stamina_update(world, level, time);
	clear_input();
}

void Player::update_side_scrolling(std::vector<Entity*> interactables, std::pair<int, int> level_dimensions)
{
	// uncomment if we ever want side-scrolling physics
	/*
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
	*/
}

void Player::update_top_down(World * world, Level * level)
{
	if (counters[BOUNCE]) return;
	if (counters[SWING]) {
		switch (current_action) {
			//case(ACTION_SHEAR):
			//	shear_update(level);
			//	break;
			case(ACTION_SWING):
				swing_update(world, level, this->get_selected_item()->get_swing_key());
				break;
		}
		return;
	}
	bool was_walking = this->anim_state == ANIM_STATE_WALKING;
	current_action = ACTION_NONE;
	reset_entity_flags();
	//TODO: figure out how best to handle joystick and keyboard inputs that occur at the same time
	xvel = 0, yvel = 0;
	anim_state = ANIM_NEUTRAL;
	//keyboard
	if (check_move(MOVE_LEFT) && !check_move(MOVE_RIGHT)) {
		xvel = -1 * get_walk_speed();
		direction = DIR_LEFT, anim_state = ANIM_STATE_WALKING;
	} else if (check_move(MOVE_RIGHT) && !check_move(MOVE_LEFT)) {
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
	} else {
		xvel = move_joystick_pos.first*get_walk_speed(), yvel = move_joystick_pos.second*get_walk_speed();
		if (xvel >= 0) {
			if (xvel >= std::abs(yvel)) direction = DIR_RIGHT;
			else if (yvel > 0) direction = DIR_DOWN;
			else direction = DIR_UP;
		} else {
			if (std::abs(xvel) >= std::abs(yvel)) direction = DIR_LEFT;
			else if (yvel > 0) direction = DIR_DOWN;
			else direction = DIR_UP;
		}
		anim_state = ANIM_STATE_WALKING;
	}
}
/*
void Player::update_top_down(std::vector<Entity*> interactables, std::vector<Tile*> nearby_tiles, std::pair<int, int> level_dimensions)
{
	//this->sound_update(); //TODO: move this if necessary
	if (counters[BOUNCE]) {
		return;
	}
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
*/
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

void Player::interact_update(World * world, Level * level, GlobalTime * time)
{
	//TODO: only get nearby interactables, not all of them
	//can't necessary just get those from the same bucket because we could be interacting across buckets
	std::vector<Entity*> interactables = level->get_nearby_interactables(this, *(this->get_rect_for_collision()), false);
	std::vector<Tile*> nearby_tiles = level->get_nearby_tiles(this);
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
			if (this->interact(world, level, time, e)) {
				return;
			}
		}
	}
}

void Player::draw(ALLEGRO_DISPLAY * display, const int x_off, const int y_off, const int screen_w, const int screen_h)
{
	Being::draw(display, x_off, y_off, screen_w, screen_h);
}

//TODO: anything player-specific goes here
void Player::emit_sound_update(World * world, Level * level, GlobalTime * time, const int game_mode)
{
	//TODO: shears? (should either go here or when swinging them)
	Being::emit_sound_update(world, level, time, game_mode);
}

void Player::play_sound_update(World * world, Level * level, GlobalTime * time, const int game_mode)
{
	//TODO: should music go here or elsewhere?
	std::vector<Entity *> moving_interactables = level->get_moving_interactables(this);
	for (Entity * e : moving_interactables) {
		this->play_sounds_for_entity(e);
	}
	this->play_sounds_for_entity(this);
}

const std::string Player::get_sound_key()
{
	return "" + SOUND_KEY_PLAYER;
}

const int Player::wake_up_time()
{
	return 1000; //temp. need some constant, along with things like alarm clocks
	// maybe set the base variable on player when loading it in?
}

void Player::dialog_update(World * world, Level * level)
{
	if (this->open_dialog != NULL) {
		const std::string action_key = this->open_dialog->get_active_action_key();
		std::vector<InteractAction *> actions = this->open_dialog->get_active_actions();
		TriggerStatus * trigger_status = this->open_dialog->get_active_trigger_status();
		if (trigger_status != NULL) {
			world->copy_trigger_status(trigger_status);
			this->open_dialog->set_active_trigger_status(NULL);
		}
		std::vector<QuestUpdate *> quest_updates = this->open_dialog->get_pending_quest_updates();
		if (!quest_updates.empty()) {
			for (QuestUpdate * qu : quest_updates) {
				world->process_quest_update(qu);
			}
			this->open_dialog->unset_pending_quest_updates();
		}
		const InteractActionManager &manager = InteractActionManager::get_instance();
		if (!actions.empty()) {
			for (InteractAction * ia : actions) {
				manager.run_action(world, level, ia, this, NULL);
			}
			this->open_dialog->clear_active_actions();
			this->open_dialog->set_active_action_key("");
		} else if (action_key.length() > 0) {
			manager.run_action(world, level, action_key, this->open_dialog->get_action_bindings(), this);
			if (this->open_dialog != NULL) {
				this->open_dialog->set_active_action_key("");
			}
		}  else {
			// no actions
			this->open_dialog->update();
			if (this->should_close_dialog) {
				this->close_dialog();
				this->should_close_dialog = false;
			}
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

void Player::cutscene_update(World * world, Level * level)
{
	if (this->has_active_cutscene()) {
		if (!this->active_cutscene->has_action()) {
			this->active_cutscene->update(world, level);	
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
	this->active_cutscene_key = "";
}

void Player::set_active_cutscene_key(const std::string key)
{
	this->active_cutscene_key = key;
}

const std::string Player::get_active_cutscene_key()
{
	return this->active_cutscene_key;
}

const bool Player::interact(World * world, Level * level, GlobalTime * time, Entity * e)
{
	return e->interact_action(world, level, time, this);
}

void Player::swing_update(World * world, Level * level, const std::string swing_key)
{
	std::vector<Entity*> interactables = level->get_nearby_interactables(this, *(this->get_rect_for_collision()), false);
	std::vector<Tile*> nearby_tiles = level->get_nearby_tiles(this);
	if (get_entity_attribute(E_ATTR_HIT_OTHER) == 1) return;
	const int t_size = nearby_tiles.size();
	for (int i = 0; i < t_size; i++) {
		Block *b = nearby_tiles[i]->get_block();
		if (b) interactables.push_back(b);
	}
	const int size = interactables.size();
	const int anim_col = this->get_animation()->get_current_frame().first;
	mask_t *swing_mask = this->get_additional_mask(swing_key, "player", this->get_animation_direction(), anim_col);
	float x_off = 0, y_off = 0;
	switch (direction) {
	case DIR_NEUTRAL:
		break;
	case DIR_UP:
		break;
	case DIR_DOWN:
		break;
	case DIR_LEFT:
		break;
	case DIR_RIGHT:
		break;
	}

	Item *swung_item = get_selected_item();
	if (swung_item != NULL && swung_item->has_item_attribute(Item::ITEM_ATTR_STAMINA_COST)) {
		const int stamina_cost = swung_item->get_item_attribute(Item::ITEM_ATTR_STAMINA_COST);
		if (stamina_cost > 0) {
			this->current_stamina = std::max(this->get_current_stamina() - stamina_cost, 0);
		}
	}

	for (int i = 0; i < size; i++) {
		if (interactables[i]->get_mask() && Mask_Collide(swing_mask, interactables[i]->get_mask(),
			get_x() + x_off - interactables[i]->get_x(),
			get_y() + y_off - interactables[i]->get_y())) {
			Entity* e = interactables[i];
			if (this->can_swing_at_entity(e, swing_key)
				&& e->get_entity_attribute(E_ATTR_BROKEN) != 1) {
				TileSet * tileset = level->get_tileset();
				set_entity_attribute(E_ATTR_HIT_OTHER, 1);				
				//TODO: if we add any more swing types, figure this out more generally
				if (swing_key == "shearing") {
					const int shear_power = swung_item->get_item_attribute(Item::ITEM_ATTR_POWER);					
					e->take_durability_damage(level, this, shear_power);
					const std::string sound_filename = "entity_sounds/damage_" + tileset->get_block_key(e->get_entity_data_index());
					const int sound_duration = 12; // TODO: how to get this?
					this->emit_sound(sound_filename, sound_duration, false);
				} else if (swing_key == "netting" && e->get_type() == CRITTER) {
					Critter * c = (Critter*)e;
					this->catch_critter(world, level, c);
					const std::string sound_filename = "entity_sounds/catch_critter";
					const int sound_duration = 12; // TODO: how to get this?
					this->emit_sound(sound_filename, sound_duration, false);
				}
				return;
			}
		}
	}
}

void Player::sleep_in_bed(GlobalTime * current_time)
{
	Cutscene * cutscene = new Cutscene();
	cutscene->add_advance_day_update(current_time, this->wake_up_time());
	this->active_cutscene = cutscene;
}

void Player::load_game_for_day(const int day)
{
	Cutscene * cutscene = new Cutscene();
	cutscene->add_load_game_update(day, this->wake_up_time());
	this->active_cutscene = cutscene;
}

float Player::get_walk_speed()
{
	float speed = this->base_walk_speed.value();
	if (!this->is_running.value()) {
		speed = speed * WALKING_SPEED_RATIO;
	}
	if (this->has_low_stamina_1()) {
		if (this->has_low_stamina_2()) {
			speed = speed * STAMINA_SLOW_2_SPEED_RATIO;
		} else if (this->get_current_stamina() > 0 ) {
			speed = speed * STAMINA_SLOW_1_SPEED_RATIO;
		} else {
			speed = speed * STAMINA_EMPTY_SPEED_RATIO;
		}
	}
	return speed;
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
	if (item && item != NULL) {
		if (item->has_item_attribute(Item::ITEM_ATTR_STAMINA_COST)) {
			const int stamina_cost = item->get_item_attribute(Item::ITEM_ATTR_STAMINA_COST);
			if (stamina_cost > this->get_current_stamina()) {
				//TODO: error sound?
				return;
			}
		}
		const std::string swing_key = item->get_swing_key();
		if (!swing_key.empty()) {
			this->swing_item(swing_key);
		}
		//TODO: other uses besides swinging
	}
}

const bool Player::has_item_with_key(const int item_key)
{
	return this->inventory.has_item_with_key(item_key);
}

Item * Player::get_item_with_key(const int item_key)
{
	return this->inventory.get_item_with_key(item_key);
}

void Player::remove_item_with_key(const int item_key)
{
	this->inventory.remove_item_with_key(item_key);
}

void Player::add_item_with_key(const int item_key)
{
	if (!this->inventory.is_full()) {
		Item * item = ItemManager::get_instance().create_item(item_key);
		this->inventory.add_item(item);
		//TODO: sound effect (probably serialized in the item)
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

//TODO: generalize a bit more for swinging animation
void Player::use_shears()
{
	xvel = 0, yvel = 0;
	counters[SWING] = 19; //TEMP. This is done to match up with the animation length.
	anim_state = ANIM_STATE_SHEARING;
	current_action = ACTION_SHEAR;
	get_animation()->reset();
	ss_animation->reset();
}

void Player::swing_item(const std::string swing_key)
{
	xvel = 0, yvel = 0;
	anim_state = this->get_anim_state_index(swing_key); 
	current_action = ACTION_SWING;
	const int duration = get_animation()->get_frame_count().first * get_animation()->get_frame_duration();
	const std::string sound_filename = "tool_sounds/" + swing_key;
	this->emit_sound(sound_filename, duration, false);
	//TODO: need to test this (and maybe make more sophisticated duration check)
	counters[SWING] = duration;
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

void Player::set_exchange_inventory_key(const std::string inventory_key)
{
	this->exchange_inventory_key = inventory_key;
}

const std::string Player::get_exchange_inventory_key()
{
	return this->exchange_inventory_key;
}

void Player::adjust_money(const float adjust_amount)
{
	this->inventory.adjust_money(adjust_amount);
}

void Player::advance_dialog()
{
	if (this->open_dialog) {
		this->open_dialog->advance_dialog();
		if (!this->open_dialog->has_current_page()) {
			this->should_close_dialog = true;
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

void Player::set_active_cutscene(Cutscene * scene)
{
	this->active_cutscene = scene;
}


const std::string Player::get_spawn_key()
{
	return this->spawn_key.value();
}

const std::map<std::string, std::string> Player::get_pending_trigger_updates()
{
	return this->pending_trigger_updates;
}

void Player::clear_pending_triggers()
{
	this->pending_trigger_updates.clear();
}

void Player::set_has_met_npc(const std::string npc_key)
{
	this->pending_trigger_updates[SET_HAS_MET_NPC] = npc_key;
}

const bool Player::gather_plant(Entity * plant)
{
	if (!this->inventory.is_full()) {
		plant->set_entity_attribute(GameImage::E_ATTR_BROKEN, 1);
		Item * plant_item = plant->get_plant_gather_item();
		this->inventory.add_item(plant_item);
		//TODO: sound effect (probably serialized in the plant)
		return true;
	}
	return false;
}

void Player::add_pending_quest_item_update(const std::string quest_item_key, const bool has_item)
{
	this->pending_quest_item_updates[quest_item_key] = has_item;
}

void Player::clear_pending_quest_item_updates()
{
	this->pending_quest_item_updates.clear();
}

void Player::take_damage(const int damage)
{
	this->current_stamina = std::max(0, this->get_current_stamina() - damage);
}

void Player::set_stamina_full()
{
	this->current_stamina = this->max_stamina.value();
}

const bool Player::has_full_stamina()
{
	return this->get_current_stamina() == this->max_stamina.value();
}

const int Player::get_max_stamina()
{
	return this->max_stamina.value();
}

const int Player::get_current_stamina()
{
	return this->current_stamina.value();
}

void Player::toggle_run()
{
	this->is_running = !this->is_running.value();
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
