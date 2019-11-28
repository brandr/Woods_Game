#include "Critter.h"
#include "Level.h"
#include "World.h"

void Critter::mark_destination_reached(const std::string dest_key)
{
	AIBeing::mark_destination_reached(dest_key);
	this->ai_state.set_is_waiting(100); //TEMP: may want to randomize within range and/or get from serialized ai struct
}

Critter::Critter()
{
}

Critter::Critter(CritterTemplate * critter_template, const int index)
{
	this->critter_key = critter_template->get_critter_key();
	this->spritesheet_frame_width = critter_template->get_spritesheet_frame_width();
	this->spritesheet_frame_height = critter_template->get_spritesheet_frame_height();
	this->animation_data = critter_template->animation_data; //TODO: does this need to be copied? (test with multiple critters)
	this->animation_spritesheet_key = "critters/" + critter_template->get_critter_key();
	GameImage::load_content_from_attributes(); //TODO: make sure the relevant attributes are passed in before here
	this->rect.x = 0, this->rect.y = 0; // position won't be set just yet
	this->rect.width = std::max((int)this->rect.width, critter_template->get_spritesheet_frame_width());
	this->rect.height = std::max((int)this->rect.height, critter_template->get_spritesheet_frame_height());
	this->collide_rect.x = this->rect.x + critter_template->get_collide_x_offset(); 
	this->collide_rect.y = this->rect.y + critter_template->get_collide_y_offset();
	this->collide_rect.width = critter_template->get_collide_width();
	this->collide_rect.height = critter_template->get_collide_height();
	this->solid = critter_template->get_solid();
	this->base_walk_speed = critter_template->get_base_speed();
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

void Critter::request_pathing_update(World * world, Level * level, GlobalTime * time)
{
	//TODO: right now, this will always wander. What other behaviors do we want critters to do to get pathing destinations?
	//TODO: do we need to call super method too/instead?
	if (this->ai_state.is_idle()) {
		const float x_pos = this->get_x(), y_pos = this->get_y();
		const Rect * collide_rect = this->get_rect_for_collision();
		const int collide_w = collide_rect->width, collide_h = collide_rect->height;
		const int start_tx = x_pos / collide_w;
		const int start_ty = y_pos / collide_h;
		
		//TODO: is it bad to have this the same as other seed?
		srand(std::time(NULL) + time->get_time() + this->get_x() + this->get_y() * 1001 + this->level_critter_index * 10001);
		// how to determine min/max wander range?
		const int min_range = 1; //temp
		const int max_range = 3; //temp
		const int range = (std::rand() % (max_range - min_range + 1)) + min_range;
		std::vector<std::pair<int, int>> open_tiles;
		const std::pair<int, int> level_dimensions = level->get_dimensions();
		for (int ty = -1 * range; ty < 1 + range; ty++) {
			for (int tx = -1 * range; tx < 1 + range; tx++) {
				const std::pair<int, int>
					tile_center(x_pos / collide_w + tx, y_pos / collide_h + ty);
				const int cx = std::max(
					0, std::min(level_dimensions.first - collide_w, (int) x_pos + tx * collide_w));
				const int cy = std::max(
					0, std::min(level_dimensions.second - collide_h, (int) y_pos + ty * collide_h));
				if (!this->pathing_blocked_at(cx, cy, level, false) && !(cx/ collide_w == start_tx && cy / collide_h == start_ty)) {
					open_tiles.push_back(std::pair<int, int>(cx / collide_w, cy / collide_h));
				}
			}
		}
		const int size = open_tiles.size();
		if (size > 0) {
			srand(std::time(NULL) + time->get_time() + this->get_x() + this->get_y() * 1001 + this->level_critter_index * 10001);
			const int roll_index = rand() % size;
			std::cout << "rolled " + std::to_string(roll_index) + " out of " + std::to_string(size) + "\n"; //temp
			const std::pair<int, int> rolled_t_pos = open_tiles[roll_index];
			const std::pair<int, int> rolled_pos(rolled_t_pos.first * collide_w, rolled_t_pos.second * collide_h);
			this->primary_destinations.push_back(std::pair<std::string, std::pair<int, int>>("", rolled_pos));
			this->set_ai_state(AI_STATE_STARTING_PATH);
		} else {
			//TODO: is it bad to have this the same as other seed?
			srand(std::time(NULL) + time->get_time() + this->get_x() + this->get_y() * 1001 + this->level_critter_index * 10001);
			//TODO: how to get min/rax wait duration?
			const int min_duration = 75; //temp
			const int max_duration = 125; //temp
			const int duration = (std::rand() % (max_duration - min_duration + 1)) + min_duration;
			this->ai_state.set_is_waiting(100); //TEMP -- store wait time as constant? Categoriezed by AI type w/ overrides? randomize in range?
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
