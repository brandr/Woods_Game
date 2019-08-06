#include "Level.h"
#include "NPC.h"
#include "Player.h"

void NPC::clear_primary_destinations()
{
	AIBeing::clear_primary_destinations();
}

const std::string NPC::calculate_destination_node_key(GlobalTime * time)
{
	//TEMP. should probably have more AIState filtering before we get here.
	const std::string scheduled_node_key = this->schedule.scheduled_node_key(time);
	return scheduled_node_key;
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
	Register("default_dialog_text", &default_dialog_text);
}

NPC::~NPC()
{
}

void NPC::update(Level * level, GlobalTime * time, const int game_mode)
{
	AIBeing::update(level, time, game_mode);
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

Dialog * NPC::choose_dialog(Player * player)
{
	//TODO: don't immediately choose default dialog text, check for other things
	//TODO: use schedule, quest triggers, etc
	const std::string default_text = this->get_default_dialog_text();
	if (!default_text.empty()) {
		Dialog * dialog = new Dialog();
		dialog->parse_text(default_text);
		return dialog;
		//TODO: avoid memory leaks when opening/closing dialogs
	}
	return NULL;
}

const bool NPC::interact_action(Player * player)
{
	//TODO: update relationship with player
	//TODO: take into account time of day, previous interactions, weather, etc.
	Dialog * dialog = this->choose_dialog(player);
	if (dialog) {
		// turn to face the player
		this->direction = this->calculate_direction(player);
		GameImage::update();
		// TODO: probably need to make them hold still for a second after talking
		player->set_open_dialog(dialog);
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
