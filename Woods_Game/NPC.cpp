#include "NPC.h"
#include "Player.h"

NPC::NPC()
{
	setClassName("NPC");
	Register("animation_spritesheet_key", &animation_spritesheet_key);
	Register("center_offset_x", &center_offset_x);
	Register("center_offset_y", &center_offset_y);
	Register("spritesheet_frame_width", &spritesheet_frame_width);
	Register("spritesheet_frame_height", &spritesheet_frame_height);
	Register("animation_data", &animation_data);
	Register("additional_mask_data", &additional_mask_data);
	Register("spawn_key", &spawn_key);
	Register("default_dialog_text", &default_dialog_text);
	direction = DIR_NEUTRAL;
	anim_state = ANIM_NEUTRAL;
	solid = true;
}

NPC::~NPC()
{
}

const std::string NPC::get_spawn_key()
{
	return this->spawn_key.value();
}

Dialog * NPC::choose_dialog(Player * player)
{
	//TODO: don't immediately choose default dialog text, check for other things
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
		player->set_open_dialog(dialog);
		return true;
	}
	return false;
}

const std::string NPC::get_default_dialog_text()
{
	return this->default_dialog_text.value();
}
