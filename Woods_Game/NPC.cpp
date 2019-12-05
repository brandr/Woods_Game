#include "Level.h"
#include "NPC.h"
#include "Player.h"
#include "World.h"

void NPC::clear_primary_destinations()
{
	AIBeing::clear_primary_destinations();
}

const std::string NPC::get_sound_key()
{
	return "" + std::to_string(SOUND_KEY_NPC) + ":" + this->get_npc_key();
}

const std::string NPC::calculate_destination_node_key(World * world, GlobalTime * time)
{
	//TEMP. should probably have more AIState filtering before we get here.
	Level * current_level = world->get_level_with_key(this->current_level_key);
	const std::string scheduled_node_key = this->schedule.scheduled_node_key(world, current_level, time);
	return scheduled_node_key;
}

//TODO: might want tto put this in super method and make "current wander zone" the thing we calculate
const bool NPC::calculate_close_enough_to_node(World * world, Level * level, GlobalTime * time, const std::string node_key)
{
	PathNode * matching_node = level->find_path_node_with_key(node_key);
	if (matching_node != NULL) {
		WanderZone * zone = this->current_wander_zone(world, level, time); //this->schedule.scheduled_wander_zone(world, level, time);
		if (zone != NULL && zone->get_should_wander()) {
			const int node_x = matching_node->get_x(), node_y = matching_node->get_y();
			return zone->contains_object(node_x, node_y, this->get_x(), this->get_y());
		}
	}
	return AIBeing::calculate_close_enough_to_node(world, level, time, node_key);
}

const bool NPC::should_wander(World * world, Level * level, GlobalTime * time)
{
	if (!this->ai_state.may_wander()) {
		return false;
	}
	const std::string node_key = this->calculate_destination_node_key(world, time);
	PathNode * matching_node = level->find_path_node_with_key(node_key);
	if (matching_node != NULL) {
		WanderZone * zone = this->current_wander_zone(world, level, time);
		if (zone != NULL && zone->get_should_wander()) {
			const int node_x = matching_node->get_x(), node_y = matching_node->get_y();
			return zone->contains_object(node_x, node_y, this->get_x(), this->get_y());
		}
	}
	return false;
}

WanderZone * NPC::current_wander_zone(World * world, Level * level, GlobalTime * time)
{
	const std::string node_key = this->calculate_destination_node_key(world, time);
	PathNode * matching_node = level->find_path_node_with_key(node_key);
	if (matching_node != NULL) {
		WanderZone * zone = this->schedule.scheduled_wander_zone(world, level, time);
		return zone;
	}
	return NULL;
}

const std::pair<int, int> NPC::get_wander_zone_center(World * world, Level * level, GlobalTime * time)
{
	WanderZone * zone = this->current_wander_zone(world, level, time);
	if (zone != NULL && zone->get_should_wander()) {
		const std::string node_key = this->calculate_destination_node_key(world, time);
		PathNode * matching_node = level->find_path_node_with_key(node_key);
		if (matching_node != NULL) {
			return matching_node->get_center();
		}
	}
	return AIBeing::get_wander_zone_center(world, level, time);
}

const int NPC::forced_animation_state(World * world, Level * level, GlobalTime * time)
{
	const std::string node_key = this->calculate_destination_node_key(world, time);
	PathNode * matching_node = level->find_path_node_with_key(node_key);
	if (matching_node != NULL) {
		return this->schedule.scheduled_forced_animation_state(world, level, time);
	}
	return -1;
}

const int NPC::forced_animation_direction(World * world, Level * level, GlobalTime * time)
{
	const std::string node_key = this->calculate_destination_node_key(world, time);
	PathNode * matching_node = level->find_path_node_with_key(node_key);
	if (matching_node != NULL) {
		return this->schedule.scheduled_forced_animation_direction(world, level, time);
	}
	return -1;
}

const std::string NPC::get_footstep_filename_suffix()
{
	return this->get_npc_key();
}

NPC::NPC()
{
	setClassName("NPC");
	Register("npc_key", &npc_key);
	Register("schedule", &(this->schedule));
	Register("start_level_key", &start_level_key);
	Register("default_spawn_level_key", &default_spawn_level_key);
	Register("animation_spritesheet_key", &animation_spritesheet_key);
	Register("center_offset_x", &center_offset_x);
	Register("center_offset_y", &center_offset_y);
	Register("collide_x_offset", &collide_x_offset);
	Register("collide_y_offset", &collide_y_offset);
	Register("collide_width", &collide_width);
	Register("collide_height", &collide_height);
	Register("spritesheet_frame_width", &spritesheet_frame_width);
	Register("spritesheet_frame_height", &spritesheet_frame_height);
	Register("animation_data", &animation_data);
	Register("additional_mask_data", &additional_mask_data);
	Register("base_walk_speed", &base_walk_speed);
	Register("start_spawn_key", &start_spawn_key);
	Register("default_spawn_key", &default_spawn_key);
	Register("obeys_tile_rules", &obeys_tile_rules);
	Register("dialog_tree", &dialog_tree);
	Register("default_dialog_text", &default_dialog_text);
}

NPC::~NPC()
{
}

void NPC::update(World * world, Level * level, GlobalTime * time, const int game_mode)
{
	AIBeing::update(world, level, time, game_mode);
}

void NPC::draw(ALLEGRO_DISPLAY * display, int x_offset, int y_offset)
{
	AIBeing::draw(display, x_offset, y_offset);
}

const std::string NPC::get_npc_key()
{
	return this->npc_key.value();
}

const std::string NPC::get_start_level_key()
{
	return this->start_level_key.value();
}

const std::string NPC::get_current_spawn_level_key()
{
	//TODO: logic to tell us which level we should spawn in if not default
	return this->default_spawn_level_key.value();
}

const std::string NPC::get_start_spawn_key()
{
	return this->start_spawn_key.value();
}

const std::string NPC::get_current_spawn_key()
{
	//TODO: logic to tell us which spawenr we should use if not default (like schedule and quest triggers)
	return this->default_spawn_key.value();
}

const bool NPC::get_obeys_tile_rules()
{
	return this->obeys_tile_rules.value();
}

Dialog * NPC::choose_dialog(World * world, GlobalTime * time, Player * player)
{
	Dialog * dialog = NULL;
	if (this->current_dialog_group != NULL) {
		DialogItem * dialog_item = this->current_dialog_group->get_dialog_item(this->current_dialog_index);
		if (dialog_item != NULL) {
			dialog = new Dialog();
			dialog->parse_dialog(dialog_item);
		} else {
			this->current_dialog_group = NULL;
			this->current_dialog_index = 0;
		}
	}
	if (dialog == NULL) {
		Level * current_level = world->get_level_with_key(this->current_level_key);
		this->current_dialog_group = this->dialog_tree.choose_dialog_group(world, current_level, time);
		if (this->current_dialog_group != NULL) {
			DialogItem * dialog_item = this->current_dialog_group->get_dialog_item(this->current_dialog_index);
			if (dialog_item != NULL) {
				dialog = new Dialog();
				dialog->parse_dialog(dialog_item);
			} else {
				this->current_dialog_group = NULL;
				this->current_dialog_index = 0;
			}
		}
	}
	
	if (dialog == NULL) {
		const std::string default_text = this->get_default_dialog_text();
		if (!default_text.empty()) {
			dialog = new Dialog();
			dialog->parse_text(default_text);
			//TODO: avoid memory leaks when opening/closing dialogs
		}
	}
	return dialog;
}

const bool NPC::interact_action(World * world, Level * level, GlobalTime * time, Player * player)
{
	//TODO: take into account time of day, previous interactions, weather, etc.
	Dialog * dialog = this->choose_dialog(world, time, player);
	if (dialog) {
		// turn to face the player
		this->direction = this->calculate_direction(player);
		GameImage::update();
		player->set_open_dialog(dialog);
		player->set_has_met_npc(this->get_npc_key());
		this->current_dialog_index++;
		//TODO: update relationship with player (not sure what general objects handle this)
		return true;
	}
	return false;
}

const std::string NPC::get_default_dialog_text()
{
	return this->default_dialog_text.value();
}

void NPC::set_current_level_key(const std::string level_key)
{
	this->current_level_key = level_key;
}

const std::string NPC::get_current_level_key()
{
	return this->current_level_key;
}

const std::string NPC::get_current_destination_node_key()
{
	return this->ai_state.get_current_destination_node_key();
}
