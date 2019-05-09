#include "Cutscene.h"
#include "ImageLoader.h"

CutsceneBlock * Cutscene::current_block()
{
	if (this->current_block_index >= 0 && this->current_block_index < this->cutscene_blocks.size()) {
		return this->cutscene_blocks[this->current_block_index];
	}
	return NULL;
}

Cutscene::Cutscene()
{
}

Cutscene::~Cutscene()
{
}

void Cutscene::update()
{
	if (!this->action_flag) {
		CutsceneBlock * block = this->current_block();
		if (block != NULL) {
			block->update(this->timer_active);
			if (block->is_finished()) {
				if (block->process_action()) {
					this->action_flag = true;
				} else {
					this->current_block_index++;
				}
			}
		} else {
			this->is_finished = true;
		}
	}
}

void Cutscene::advance_block()
{
	this->action_flag = false;
	this->is_finished = false;
	this->current_block_index++;
}

const std::vector<ALLEGRO_BITMAP*> Cutscene::get_filters(ALLEGRO_DISPLAY * display, const int width, const int height)
{
	std::vector<ALLEGRO_BITMAP*> filters;
	if (!this->is_finished) {
		CutsceneBlock * block = this->current_block();
		if (block != NULL) {
			const std::vector<ALLEGRO_BITMAP*> block_filters = block->get_filters(display, width, height);
			const int size = block_filters.size();
			for (int i = 0; i < size; i++) {
				filters.push_back(block_filters[i]);
			}
		}
	}
	return filters;
}

void Cutscene::add_effect(const std::string effect_key, const int duration)
{
	CutsceneBlock * block = new CutsceneBlock();
	block->effect_key = effect_key;
	block->duration = duration;
	this->cutscene_blocks.push_back(block);
}

void Cutscene::add_action(const std::string action_key)
{
	CutsceneBlock * block = new CutsceneBlock();
	block->action_key = action_key;
	block->duration = 1;
	this->cutscene_blocks.push_back(block);
}

void Cutscene::add_global_time_update(const int day, const int time)
{
	CutsceneBlock * block = new CutsceneBlock();
	block->action_key = ACTION_UPDATE_GLOBAL_TIME;
	block->global_time = new GlobalTime(day, time);
	block->duration = 1;
	this->cutscene_blocks.push_back(block);
}

void Cutscene::add_load_game_update(const int day, const int time)
{
	CutsceneBlock * block = new CutsceneBlock();
	block->action_key = ACTION_LOAD_GAME;
	block->global_time = new GlobalTime(day, time);
	block->duration = 1;
	this->cutscene_blocks.push_back(block);
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

ALLEGRO_BITMAP * CutsceneBlock::find_or_create_filter(ALLEGRO_DISPLAY *display, const int r, const int g, const int b, const int a)
{
	const int width = al_get_display_width(display), height = al_get_display_height(display);
	ALLEGRO_BITMAP * filter = NULL;
	const std::string color_prefix = IMAGE_PREFIX_COLOR;
	const std::string suffix = "";
	if (!ImageLoader::get_instance().keyed_image_exists(IMAGE_KEY_FILLED_RECT, width, height, suffix)) {
		filter = al_create_bitmap(width, height);
		ImageLoader::get_instance().set_keyed_image(filter, IMAGE_KEY_FILLED_RECT, width, height, suffix);
	}
	else {
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
		if (this->action_key == ACTION_UPDATE_GLOBAL_TIME) {
			return true;
		} else if (this->action_key == ACTION_SAVE_GAME) {
			return true;
		} else if (this->action_key == ACTION_LOAD_GAME) {
			return true;
		}
	}
	return false;
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
	CutsceneBlock * block = this->current_block();
	if (block != NULL) {
		return block->action_key;
	}
	return "";
}

GlobalTime * Cutscene::get_active_global_time()
{
	CutsceneBlock * block = this->current_block();
	if (block != NULL) {
		return block->global_time;
	}
	return NULL;
}
