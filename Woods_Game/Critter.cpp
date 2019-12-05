#include "Critter.h"
#include "Level.h"
#include "World.h"

Critter::Critter()
{
}

Critter::Critter(CritterTemplate * critter_template, const int index)
{
	this->critter_key = critter_template->get_critter_key();
	this->spritesheet_frame_width = critter_template->get_spritesheet_frame_width();
	this->spritesheet_frame_height = critter_template->get_spritesheet_frame_height();
	this->animation_data = critter_template->animation_data;
	this->animation_spritesheet_key = "critters/" + critter_template->get_critter_key();
	GameImage::load_content_from_attributes(); 
	this->rect.x = 0, this->rect.y = 0; // position won't be set just yet
	this->rect.width = std::max((int)this->rect.width, critter_template->get_spritesheet_frame_width());
	this->rect.height = std::max((int)this->rect.height, critter_template->get_spritesheet_frame_height());
	this->collide_rect.x = this->rect.x + critter_template->get_collide_x_offset(); 
	this->collide_rect.y = this->rect.y + critter_template->get_collide_y_offset();
	this->collide_rect.width = critter_template->get_collide_width();
	this->collide_rect.height = critter_template->get_collide_height();
	this->solid = critter_template->get_solid();
	this->base_walk_speed = critter_template->get_base_speed();
	this->wander_zone = &critter_template->wander_zone;
	this->anim_state = ANIM_STATE_NEUTRAL;
	this->set_ai_state(AI_STATE_IDLE);
	this->image_filename = "critters/" + critter_template->get_critter_key();
	this->image_subsection = new Rect(0, 0, critter_template->get_spritesheet_frame_width(), critter_template->get_spritesheet_frame_height());
	const std::string rect_string = ImageLoader::get_instance().rect_to_string(*this->image_subsection);
	this->load_mask("sprite_sheets/" + this->image_filename);
	ImageLoader::get_instance().load_image("sprite_sheets/" + this->image_filename, *this->image_subsection);
	ImageLoader::get_instance().load_mask("sprite_sheets/" + this->image_filename, rect_string);
	this->level_critter_index = index;
}


Critter::~Critter()
{
}

int Critter::get_type()
{
	return CRITTER;
}

void Critter::update(World * world, Level * level, GlobalTime * time, const int game_mode)
{
	AIBeing::update(world, level, time, game_mode);
}

const std::string Critter::get_critter_key()
{
	return this->critter_key;
}

const bool Critter::can_be_netted()
{
	//TODO: should be serializable (get through template I think)
	return true;
}

void Critter::update_visibility(World * world, Level * level)
{
	const int state = world->encyclopedia_state_for_critter(this);
	if (state == ENTRY_UNDISCOVERED) {
		world->update_encyclopedia_for_critter(this, ENTRY_SEEN);
	}
}

const bool Critter::has_visibility_actions(Level * level)
{
	return true;
}

void Critter::set_critter_spawn_pos(const int x, const int y)
{
	this->critter_spawn_pos = std::pair<int, int>(x, y);
}

void Critter::request_pathing_update(World * world, Level * level, GlobalTime * time)
{
	//TODO: right now, this will always wander. What other behaviors do we want critters to do to get pathing destinations?
	//TODO: should call super method too/instead?
	if (this->ai_state.is_wandering()) {
		this->wander_update(world, level, time);
	} else if (this->ai_state.is_idle()) {
		if (this->ai_state.may_wander()) {
			this->set_ai_state(AI_STATE_WANDERING);
		}		
	}
}

void Critter::face_other_update(Level * level, GlobalTime * time)
{
	//TODO: do we want to do this?
}

void Critter::walk_to_next_level_update(Level * level)
{
	// never walk to next level (but might want to exit here and disappear)
}

WanderZone * Critter::current_wander_zone(World * world, Level * level, GlobalTime * time)
{
	return this->wander_zone;
}

const std::pair<int, int> Critter::get_wander_zone_center(World * world, Level * level, GlobalTime * time)
{
	return std::pair<int, int>(this->critter_spawn_pos.first, this->critter_spawn_pos.second);
}

const int Critter::get_seed_index()
{
	return this->level_critter_index;
}

const std::string Critter::get_footstep_filename_suffix()
{
	return this->critter_key;
}

CritterTemplate::CritterTemplate()
{
	setClassName("CritterTemplate");
	Register("critter_key", &critter_key);
	Register("encyclopedia_category_name", &encyclopedia_category_name);
	Register("encyclopedia_entry_name", &encyclopedia_entry_name);
	Register("spritesheet_frame_width", &spritesheet_frame_width);
	Register("spritesheet_frame_height", &spritesheet_frame_height);
	Register("collide_x_offset", &collide_x_offset);
	Register("collide_y_offset", &collide_y_offset);
	Register("collide_width", &collide_width);
	Register("collide_height", &collide_height);
	Register("solid", &solid);
	Register("base_speed", &base_speed);
	Register("animation_data", &animation_data);
	Register("wander_zone", &wander_zone);
	Register("catch_dialog_item", &catch_dialog_item);
}

CritterTemplate::~CritterTemplate()
{
}

const std::string CritterTemplate::get_critter_key()
{
	return this->critter_key.value();
}

const std::string CritterTemplate::get_encyclopedia_category_name()
{
	return this->encyclopedia_category_name.value();
}

const std::string CritterTemplate::get_encyclopedia_entry_name()
{
	return encyclopedia_entry_name.value();
}

const int CritterTemplate::get_spritesheet_frame_width()
{
	return this->spritesheet_frame_width.value();
}

const int CritterTemplate::get_spritesheet_frame_height()
{
	return this->spritesheet_frame_height.value();
}

const int CritterTemplate::get_collide_x_offset()
{
	return this->collide_x_offset.value();
}

const int CritterTemplate::get_collide_y_offset()
{
	return this->collide_y_offset.value();
}

const int CritterTemplate::get_collide_width()
{
	return this->collide_width.value();
}

const int CritterTemplate::get_collide_height()
{
	return this->collide_height.value();
}

const bool CritterTemplate::get_solid()
{
	return this->solid.value();
}

const float CritterTemplate::get_base_speed()
{
	return this->base_speed.value();
}

Dialog * CritterTemplate::get_catch_dialog()
{
	if (this->catch_dialog_item.has_text()) {
		Dialog * d = new Dialog();
		d->parse_dialog(&this->catch_dialog_item);
		return d;
	}
	return NULL;
}
