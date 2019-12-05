#include "Cutscene.h"
#include "ImageLoader.h"
#include "Level.h"
#include "World.h"

const bool Cutscene::has_current_cutscene_block()
{
	return this->current_block_index >= 0 && this->current_block_index < (int) this->cutscene_blocks.size();
}

Cutscene::Cutscene()
{
}

Cutscene::~Cutscene()
{
	this->cutscene_blocks.clear();
}

void Cutscene::update(World * world, Level * level)
{
	if (!this->action_flag) {
		if (this->has_current_cutscene_block()) {
			this->cutscene_blocks[this->current_block_index]->update(this->timer_active);
			if (this->cutscene_blocks[this->current_block_index]->is_finished()) {
				//TODO: process triggers for the block
				if (this->cutscene_blocks[this->current_block_index]->process_action()) {
					this->action_flag = true;
				} else if (!this->update_trigger_statuses 
					&& this->cutscene_blocks[this->current_block_index]->block_triggers.size() > 0) {
					this->update_trigger_statuses = true;
				} else {
					this->update_trigger_statuses = false;
					this->advance_block(world, level);
				}
			}
		} else {
			this->is_finished = true;
		}
	}
}

void Cutscene::advance_block(World * world, Level * level)
{
	int next_block_index = -1;
	if (this->has_current_cutscene_block()) {
		next_block_index = this->cutscene_blocks[this->current_block_index]->get_next_block_index(world, level);
	}
	this->action_flag = false;
	this->is_finished = false;
	this->current_block_index = next_block_index >= 0 ? next_block_index : current_block_index + 1;
}

const std::vector<ALLEGRO_BITMAP*> Cutscene::get_filters(ALLEGRO_DISPLAY * display, const int width, const int height)
{
	std::vector<ALLEGRO_BITMAP*> filters;
	if (!this->is_finished) {
		if (this->has_current_cutscene_block()) {
			const std::vector<ALLEGRO_BITMAP*> block_filters = this->cutscene_blocks[this->current_block_index]->get_filters(display, width, height);
			const int size = block_filters.size();
			for (int i = 0; i < size; i++) {
				filters.push_back(block_filters[i]);
			}
		}
	}
	return filters;
}

void Cutscene::add_block(CutsceneBlock * block)
{
	this->cutscene_blocks[block->block_index] = block;
}

void Cutscene::add_effect(const std::string effect_key, const int duration)
{
	CutsceneBlock * block = new CutsceneBlock();
	block->effect_key = effect_key;
	block->duration = duration;
	block->block_index = this->cutscene_blocks.size(); // this works if we are incrementing by 1 each time
	this->add_block(block);
}

void Cutscene::add_action(const std::string action_key)
{
	this->add_action(action_key, "");
}

void Cutscene::add_action(const std::string action_key, const std::string effect_key)
{
	CutsceneBlock * block = new CutsceneBlock();
	block->action_key = action_key;
	block->duration = 1;
	if (effect_key != "") {
		block->effect_key = effect_key;
	}
	block->block_index = this->cutscene_blocks.size();
	this->add_block(block);
}

void Cutscene::add_global_time_update(const int day, const int time)
{
	CutsceneBlock * block = new CutsceneBlock();
	block->action_key = ACTION_UPDATE_GLOBAL_TIME;
	block->global_time = new GlobalTime(day, time);
	block->duration = 1;
	block->effect_key = EFFECT_DISPLAY_BLACK;
	block->block_index = this->cutscene_blocks.size();
	this->add_block(block);
}

void Cutscene::add_advance_day_update(GlobalTime * global_time, const int wake_up_time)
{
	CutsceneBlock * block = new CutsceneBlock();
	block->action_key = ACTION_UPDATE_NEW_DAY;
	block->duration = 1;
	block->effect_key = EFFECT_DISPLAY_BLACK;
	block->block_index = this->cutscene_blocks.size();
	this->add_block(block); // do this here if we're updating async
	this->add_effect(EFFECT_FADE_TO_BLACK, 175);
	//this->add_block(block); // do this here if we're not updating async
	this->add_action(ACTION_AWAIT_LOAD, EFFECT_DISPLAY_BLACK);
	this->add_global_time_update(global_time->get_day() + 1, wake_up_time);
	this->add_action(ACTION_SAVE_GAME, EFFECT_DISPLAY_BLACK);
	this->add_action(ACTION_AWAIT_LOAD, EFFECT_DISPLAY_BLACK);

}

void Cutscene::add_load_game_update(const int day, const int time)
{
	CutsceneBlock * block = new CutsceneBlock();
	block->action_key = ACTION_LOAD_GAME;
	block->duration = 1;
	block->effect_key = EFFECT_DISPLAY_BLACK;
	block->global_time = new GlobalTime(day, time);
	block->block_index = this->cutscene_blocks.size();
	this->add_block(block);
	this->add_effect(EFFECT_FADE_TO_BLACK, 175);
	this->add_global_time_update(day, time);
	this->add_action(ACTION_AWAIT_LOAD, EFFECT_DISPLAY_BLACK);
	this->add_action(ACTION_SAVE_GAME, EFFECT_DISPLAY_BLACK);
	this->add_action(ACTION_AWAIT_LOAD, EFFECT_DISPLAY_BLACK);
}

void Cutscene::set_cutscene_key(const std::string key)
{
	this->cutscene_key = key;
}

const std::string Cutscene::get_cutscene_key()
{
	return this->cutscene_key;
}


CutsceneBlock::CutsceneBlock()
{
}

void CutsceneBlock::update(const bool update_timer)
{
	//TODO: do other update stuff
	if (update_timer) {
		this->timer++;
	}
}

const int CutsceneBlock::get_next_block_index(World * world, Level * level)
{
	for (CutsceneBranch cb : this->cutscene_branches) {
		if (cb.qualifies(world, level)) {
			return cb.next_block_index;
		}
	}
	return this->next_block_index;
}

const std::vector<ALLEGRO_BITMAP*> CutsceneBlock::get_filters(ALLEGRO_DISPLAY * display, const int width, const int height)
{
	//TODO: other filters
	std::vector<ALLEGRO_BITMAP*> filters;
	if (EFFECT_FADE_TO_BLACK == this->effect_key) {
		const float a = 255 * ((float)this->timer / (float)this->duration);	//alpha
		ALLEGRO_BITMAP * fade_filter = this->find_or_create_filter(display, 0, 0, 0, a);
		filters.push_back(fade_filter);
	}
	if (EFFECT_DISPLAY_BLACK == this->effect_key) {
		ALLEGRO_BITMAP * black_filter = this->find_or_create_filter(display, 0, 0, 0, 255);
		filters.push_back(black_filter);
	}
	return filters;
}

//TODO: probably want to handle all bitmap creation through ImageLoader
ALLEGRO_BITMAP * CutsceneBlock::find_or_create_filter(ALLEGRO_DISPLAY *display, const int r, const int g, const int b, const int a)
{
	const int width = al_get_display_width(display), height = al_get_display_height(display);
	ALLEGRO_BITMAP * filter = NULL;
	const std::string color_prefix = IMAGE_PREFIX_COLOR;
	const std::string suffix = "";
	if (!ImageLoader::get_instance().keyed_image_exists(IMAGE_KEY_FILLED_RECT, width, height, suffix)) {
		filter = al_create_bitmap(width, height);
		ImageLoader::get_instance().set_keyed_image(filter, IMAGE_KEY_FILLED_RECT, width, height, suffix);
	} else {
		filter = ImageLoader::get_instance().get_keyed_image(IMAGE_KEY_FILLED_RECT, width, height, suffix);
	}
	al_set_target_bitmap(filter);
	al_clear_to_color(al_map_rgba(r, g, b, a));
	al_set_target_bitmap(al_get_backbuffer(display));
	return filter;
}

const bool CutsceneBlock::is_finished()
{
	//TODO: other logic to end block early?
	if (this->timer >= this->duration) {
		return true;
	}
	return false;
}

const bool CutsceneBlock::process_action()
{
	if (this->action_key.length() > 0) {
		// save/load actions
		if (this->action_key == ACTION_UPDATE_GLOBAL_TIME) {
			return true;
		} else if (this->action_key == ACTION_SAVE_GAME) {
			return true;
		} else if (this->action_key == ACTION_LOAD_GAME) {
			return true;
		} else if (this->action_key == ACTION_UPDATE_NEW_DAY) {
			return true;
		} else if (this->action_key == ACTION_AWAIT_LOAD) {
			return true;
		} // agent actions
		else if (this->action_key == ACTION_WALK_AGENTS) {
			return true;
		} else if (this->action_key == ACTION_DIALOG) {
			return true;
		}
	}
	return false;
}

void CutsceneBlock::set_has_opened_dialog(const bool value)
{
	this->has_opened_dialog = value;
}

void Cutscene::add_effect_fade_block(const int duration, const int end_alpha)
{
}

const bool Cutscene::get_is_finished()
{
	return this->is_finished;
}

const bool Cutscene::has_action()
{
	return this->action_flag;
}

const std::string Cutscene::get_active_action_key()
{
	if (this->has_current_cutscene_block()) {
		return this->cutscene_blocks[this->current_block_index]->action_key;
	}
	return "";
}

GlobalTime * Cutscene::get_active_global_time()
{
	if (this->has_current_cutscene_block()) {
		return this->cutscene_blocks[this->current_block_index]->global_time;
	}
	return NULL;
}

std::vector<CutsceneTile> Cutscene::get_current_block_cutscene_tiles()
{
	if (this->has_current_cutscene_block()) {
		return this->cutscene_blocks[this->current_block_index]->cutscene_tiles;
	}
	return std::vector<CutsceneTile>();
}

std::vector<CutsceneBranch> Cutscene::get_current_block_cutscene_branches()
{
	if (this->has_current_cutscene_block()) {
		return this->cutscene_blocks[this->current_block_index]->cutscene_branches;
	}
	return std::vector<CutsceneBranch>();
}

Dialog * Cutscene::get_active_dialog()
{
	//TODO: populate the dialog when creating the cutscene
	if (this->has_current_cutscene_block()) {
		return this->cutscene_blocks[this->current_block_index]->dialog;
	}
	return NULL;
}

void Cutscene::set_has_opened_dialog(const bool value)
{
	if (this->has_current_cutscene_block()) {
		this->cutscene_blocks[this->current_block_index]->set_has_opened_dialog(value);
	}
}

const bool Cutscene::has_opened_dialog()
{
	if (this->has_current_cutscene_block()) {
		return this->cutscene_blocks[this->current_block_index]->has_opened_dialog;
	}
	return false;
}

const bool Cutscene::should_update_trigger_statuses()
{
	return this->update_trigger_statuses;
}

std::vector<TriggerStatus*> Cutscene::trigger_statuses()
{
	if (this->has_current_cutscene_block()) {
		return this->cutscene_blocks[this->current_block_index]->block_triggers;
	}
	return std::vector<TriggerStatus*>();
}

void Cutscene::process_music()
{
	if (this->has_current_cutscene_block()) {
		const std::string music_key = this->cutscene_blocks[this->current_block_index]->music_key;
		if (!this->cutscene_blocks[this->current_block_index]->has_processed_music
			&& music_key != "") {
			AudioManager::get_instance().play_music(music_key);
		}
	}
}

std::vector<CutsceneAgentAnimation> Cutscene::get_current_block_agent_animations()
{
	if (this->has_current_cutscene_block()) {
		return this->cutscene_blocks[this->current_block_index]->agent_animations;
	}
	return std::vector<CutsceneAgentAnimation>();
}

CutsceneTile::CutsceneTile(const std::string agent, const int x, const int y, const int dir)
{
	this->agent_key = agent;
	this->tile_x = x, this->tile_y = y, this->direction = dir;
}

CutsceneBranch::CutsceneBranch(const int index, std::vector<Qualifier*> quals)
{
	this->next_block_index = index;
	this->qualifiers = quals;
}

const bool CutsceneBranch::qualifies(World * world, Level * level)
{
	for (Qualifier * q : this->qualifiers) {
		if (!q->evaluate(world, level)) {
			return false;
		}
	}
	return true;
}

CutsceneAgentAnimation::CutsceneAgentAnimation(const std::string agent, const std::string anim, const int dir)
{
	this->agent_key = agent, this->animation_key = anim, this->direction = dir;
}
