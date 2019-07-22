#define _CRTDBG_MAP_ALLOC
#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
//TODO: replace new with DBG_NEW when it is used
#else
#define DBG_NEW new
#endif
#define NOMINMAX
#include "GameImageManager.h"


std::pair<int, int> GameImageManager::get_camera_offset(ALLEGRO_DISPLAY *display)
{
	int x = 0, y = 0;
	if (player) {
		if (current_level->get_width() <= al_get_display_width(display))
			x = 0;
		else {
			x = -1 * std::min((std::max(0.0f, player->get_x() - al_get_display_width(display) / 2)), (float)(current_level->get_width() - al_get_display_width(display)));
		}
		if (current_level->get_height() <= al_get_display_height(display))
			y = 0;
		else {
			y = -1 * std::min((std::max(0.0f, player->get_y() - al_get_display_height(display) / 2)), (float)(current_level->get_height() - al_get_display_height(display)));
		}
	}
	std::pair<int,int> off = std::make_pair(x, y);
	return off;
}

std::pair<int, int> GameImageManager::tile_image_offset(std::string tile_string)
{
	std::pair<int, int> tile;
	tile.first = atoi(tile_string.substr(0, tile_string.find(',')).c_str());
	tile.second = atoi(tile_string.substr(tile_string.find(',') + 1).c_str());
	return tile;
}

GameImageManager::GameImageManager()
{
}


GameImageManager::~GameImageManager()
{
}

void GameImageManager::load_content()
{
}

void GameImageManager::start_new_game(const std::string world_key)
{
	FileManager filemanager;
	const std::string filename = "resources/load/worlds";
	filemanager.load_xml_content(&(this->world), filename, "SerializableClass", "WorldKey", world_key);
	this->world.load_dungeons();
	this->world.load_npcs();
	this->world.generate_levels();
	//TODO: Should we look for a default player? Create one with some settings entered at the start?
	load_player_from_xml("resources/load/player", this->world.get_player_key()); 
	this->current_global_time = new GlobalTime(1, START_TIME_HOUR*TIME_RATIO);
	this->save_game();
	this->world.recalculate_npc_paths();
	this->thread_data.world = &(this->world);
	this->thread_data.player = this->player;
	this->thread_data.ready = true;
}

void GameImageManager::full_load_game_from_save(const std::string save_file)
{
	FileManager filemanager;
	const std::string filename = "resources/load/saves/" + save_file + "/worlds";
	filemanager.load_xml_content(&(this->world), filename, "SerializableClass", "WorldKey", save_file);
	this->world.load_dungeons();
	this->world.load_npcs();
	const std::string player_key = this->world.get_player_key();
	load_player_from_xml("resources/load/player", player_key);
	this->current_global_time = new GlobalTime(this->world.get_current_day(), START_TIME_HOUR*TIME_RATIO); //TODO: calculate start time as necessary
}

void GameImageManager::load_game_from_save(const int day, const int time)
{
	FileManager filemanager;
	const std::string world_key = this->world.get_world_key();
	const std::string filename = "resources/load/saves/" + world_key + "/worlds";
	const int x = this->player->get_x(), y = this->player->get_y();
	if (this->current_global_time != NULL) {
		delete this->current_global_time;
	}
	this->current_global_time = new GlobalTime(day, time);
	const std::string dungeons_path = "resources/load/saves/" + world_key + "/" + "day_" + std::to_string(day) + "/dungeons";
	this->world.reload_dungeons(dungeons_path);
	//this->world.load_npcs(); //TODO: need to reload NPCS?
	this->save_game();
	
}

void GameImageManager::save_game()
{
	World::save_game(&(this->world), this->current_global_time);
}

void GameImageManager::set_game_mode(int game_mode)
{
	this->game_mode = game_mode;
}

int GameImageManager::get_game_mode()
{
	return game_mode;
}

void GameImageManager::load_player_from_xml(std::string filepath, std::string player_key)
{
	FileManager file_manager;
	player = new Player();
	file_manager.load_xml_content(player, filepath, "SerializableClass", "PlayerKey", player_key);
	player->load_content_from_attributes();
	player->load_additional_masks_from_attributes("player");
	this->current_level = this->world.extract_current_level(player, "");
	this->current_level->add_being(player);
}

void * GameImageManager::load_func_advance_day(ALLEGRO_THREAD * thr, void * arg)
{
	
	LoadingData *data = (LoadingData *)arg;
	al_lock_mutex(data->mutex);
	data->ready = false;
	data->world->update_new_day(data->player, data->current_level_key);
	data->world->recalculate_npc_paths();
	data->ready = true;
	al_broadcast_cond(data->cond);
	al_unlock_mutex(data->mutex);
	return NULL;
}

void * GameImageManager::load_func_save_game(ALLEGRO_THREAD * thr, void * arg)
{
	LoadingData *data = (LoadingData *)arg;
	al_lock_mutex(data->mutex);
	data->ready = false;
	World::save_game(data->world, data->global_time);
	data->world->recalculate_npc_paths();
	data->ready = true;
	al_broadcast_cond(data->cond);
	al_unlock_mutex(data->mutex);
	return NULL;
}

void * GameImageManager::load_func_load_from_save(ALLEGRO_THREAD * thr, void * arg)
{
	FileManager filemanager;
	LoadingData *data = (LoadingData *)arg;
	al_lock_mutex(data->mutex);
	data->ready = false;

	const std::string world_key = data->world->get_world_key();
	const std::string filename = "resources/load/saves/" + world_key + "/worlds";
	//const int x = data->player->get_x(), y = data->player->get_y();
	const std::string dungeons_path = "resources/load/saves/" + world_key + "/" + "day_" + std::to_string(data->global_time->get_day()) + "/dungeons";
	data->world->reload_dungeons(dungeons_path);
	//TODO: make sure NPCs show up in the correct places after reolading
	data->world->load_npcs(); //TODO: need to reload NPCS? (this could be where we reload paths)
	data->world->recalculate_npc_paths();
	data->ready = true;
	al_broadcast_cond(data->cond);
	al_unlock_mutex(data->mutex);
	return NULL;
}

Player * GameImageManager::get_player()
{
	return player;
}

void GameImageManager::unload_content()
{
	world.unload_content();
	if (player != NULL) {
		player->unload_content();
	}
	player = NULL;
}

void GameImageManager::unload_level_content()
{
}

void GameImageManager::update(std::map<int, bool> input_map, std::map<int, std::pair<float,float>> joystick_map)
{
	// need to keep the world threadsafe
	if (!this->thread_data.ready) {
		return;
	}
	al_lock_mutex(this->thread_data.mutex);
	const int game_mode = get_game_mode();
	if (game_mode != TOP_DOWN) { return; }
	std::pair<int, int> dimensions = current_level->get_dimensions();
	if (player) {
		if (player->get_exit_level_flag()) {
			//TODO: check to make sure there is a next level in the given direction here
			change_player_level();
			al_unlock_mutex(this->thread_data.mutex);
			return;
		}
		if (player->has_open_dialog()) {
			set_game_mode(MAIN_GAME_DIALOG);
			al_unlock_mutex(this->thread_data.mutex);
			return;
		}
		if (player->has_active_cutscene()) {
			set_game_mode(CUTSCENE);
			al_unlock_mutex(this->thread_data.mutex);
			return;
		}
		if (player->get_should_open_calendar()) {
			set_game_mode(CALENDAR);
			al_unlock_mutex(this->thread_data.mutex);
			return;
		}
		player->update_input(input_map, joystick_map, game_mode);
	}
	current_level->update(this->get_current_global_time(), game_mode);
	this->time_update();
	this->npc_update();
	//al_broadcast_cond(this->thread_data.cond);
	al_unlock_mutex(this->thread_data.mutex);
}

const std::string GameImageManager::time_display_string()
{
	const int time = this->current_global_time->get_time();
	int hours = (time / TIME_RATIO) % 24;
	const int minutes = ((time % TIME_RATIO) * 60) / TIME_RATIO;
	const std::string ampm = hours > 11 ? "PM" : "AM";
	if (hours == 0) {
		hours = 12;
	}
	const std::string hourStr = std::to_string((hours > 12 ? hours - 12 : hours));
	const std::string minuteStr = minutes > 9 ? std::to_string(minutes) : "0" + std::to_string(minutes);
	return hourStr + " : " + minuteStr + " " + ampm;
}

const std::string GameImageManager::date_display_string()
{
	return this->current_global_time->date_display_string();
}

void GameImageManager::time_update()
{
	this->current_global_time->update();
	InteractActionManager::get_instance().update_current_time(this->current_global_time);
}

void GameImageManager::npc_update()
{
	this->world.npc_update(this->current_global_time, this->game_mode);
}

GlobalTime * GameImageManager::get_current_global_time()
{
	return this->current_global_time;
}

const int GameImageManager::get_current_minutes()
{
	return this->current_global_time->get_current_minutes();
}

const std::string GameImageManager::get_current_month_str()
{
	return this->current_global_time->get_current_month_str();
}

const int GameImageManager::get_current_month_index()
{
	return this->current_global_time->get_current_month_index();
}

const int GameImageManager::get_first_day_index()
{
	return this->current_global_time->get_first_displayable_day_index();
}

const int GameImageManager::get_first_day_of_month_index()
{
	return this->current_global_time->get_first_day_of_month_index();
}

const int GameImageManager::get_num_days()
{
	return this->current_global_time->get_num_days();
}

void GameImageManager::change_player_level()
{
	Level* next_level = NULL;
	int px = 0, py = 0;
	const std::string level_key_override = player->get_destination_level_key_override();
	const std::pair<int, int> pos_override = player->get_destination_level_pos_override();
	if (!level_key_override.empty() && pos_override.first >= 0 && pos_override.second >= 0) {
		px = pos_override.first, py = pos_override.second;
		next_level = world.get_current_dungeon()->level_with_name(level_key_override);
	} else {
		int x = 0;
		int y = 0;
		int grid_x = current_level->get_grid_x();
		int grid_y = current_level->get_grid_y();
		int width = current_level->get_width();
		int height = current_level->get_height();
		const int level_width = STANDARD_LEVEL_GRID_WIDTH;
		const int level_height = STANDARD_LEVEL_GRID_HEIGHT;

		enum { UP, DOWN, LEFT, RIGHT };
		int direction = 0;
		//up
		if (player->get_y() + player->get_height() < 0) {
			x = grid_x + player->get_x()/level_width, y = grid_y - 1;
			direction = UP;
		}

		//down
		else if (player->get_y() > height) {
			x = grid_x + player->get_x() / level_width, y = grid_y + current_level->get_grid_height();
			direction = DOWN;
		}
		//left
		else if (player->get_x() + player->get_width() < 0) {
			x = grid_x - 1, y = grid_y + player->get_y() / level_height;
			direction = LEFT;
		}

		//right
		else if (player->get_x() > width) {
			x = grid_x + current_level->get_grid_width(), y = grid_y + player->get_y() / level_height;
			direction = RIGHT;
		}
		else {
			std::cout << "ERROR: no direction " << std::endl;
		}
		
		next_level = world.get_current_dungeon()->level_at(x, y);
		const float player_offset_x
			= (x -next_level->get_grid_x())*level_width + fmod(player->get_x(), level_width),
			player_offset_y = (y - next_level->get_grid_y())*level_height + fmod(player->get_y(), level_height);
		int grid_offset_x = next_level->get_grid_x() - current_level->get_grid_x();
		int grid_offset_y = next_level->get_grid_y() - current_level->get_grid_y();
		switch (direction) {
		case UP:
			px = grid_offset_x * level_width + player_offset_x;
			py = next_level->get_height() - player->get_height();
			break;
		case DOWN:
			px = grid_offset_x * level_width + player_offset_x;
			py = 0;
			break;
		case LEFT:
			px = next_level->get_width() - player->get_width();
			py = grid_offset_y * level_height + player_offset_y;
			break;
		case RIGHT:
			px = 0;
			py = grid_offset_y * level_height + player_offset_y;
			break;
		}
	}
	if (next_level) {
		current_level->remove_player();
		next_level->add_being(player);
		player->set_position(px, py);
	}
	player->reset_exit_level();
	current_level = next_level;	// is this necessary?
}

void GameImageManager::draw(ALLEGRO_DISPLAY * display)
{
	if (current_level) {
		current_level->draw(display, get_camera_offset(display));
		this->draw_filters(display, get_camera_offset(display));
	}
}

void GameImageManager::draw_filters(ALLEGRO_DISPLAY * display, std::pair<int, int> offset)
{
	//TODO: check whether we actually need a light filter before drawing it
	this->draw_light_filter(display, offset);
	if (this->player != NULL && this->player->has_active_cutscene()) {
		const int width = al_get_display_width(display), height = al_get_display_height(display);
		const std::vector<ALLEGRO_BITMAP *> filters = player->get_cutscene_filters(display, width, height);
		const int size = filters.size();
		for (int i = 0; i < size; i++) {
			al_draw_bitmap(filters[i], 0, 0, 0);
		}
	}
}

void GameImageManager::draw_light_filter(ALLEGRO_DISPLAY * display, std::pair<int, int> offset)
{
	const int width = al_get_display_width(display);
	const int height = al_get_display_height(display);
	if (this->light_filter == NULL) {
		this->light_filter = al_create_bitmap(width, height);
	}

	al_set_target_bitmap(this->light_filter);
	if (al_get_bitmap_width(this->light_filter) != width 
		|| al_get_bitmap_height(this->light_filter) != height) {
		al_destroy_bitmap(this->light_filter);
		this->light_filter = al_create_bitmap(width, height);
	}
	// (4 AM) blue, darkest -> (6 AM) orange, lighter -> (8 AM) yellow, ligthest -> orange, lighter (6 PM) -> blue, darkest (8 PM)
	const float minutes = this->get_current_minutes();
	int r = 0;
	int g = 0;
	int b = NIGHT_B;
	int a = NIGHT_A;
	if (minutes >= SUNRISE_START_MINUTES && minutes < SUNRISE_END_MINUTES ) {
		const float m = ((minutes - SUNRISE_START_MINUTES) / (SUNRISE_END_MINUTES - SUNRISE_START_MINUTES));
		r = SUNRISE_END_R * m;
		g = SUNRISE_END_G * m;
		b = NIGHT_B - ((NIGHT_B - SUNRISE_END_B) * m);
		a = NIGHT_A - ((NIGHT_A - SUNRISE_END_A) * m);
	} else if (minutes >= SUNRISE_END_MINUTES && minutes < DAY_START_MINUTES) {
		const float m = ((minutes - SUNRISE_END_MINUTES) / (DAY_START_MINUTES - SUNRISE_END_MINUTES));
		r = SUNRISE_END_R + ((DAY_START_R - SUNRISE_END_R) * m);
		g = SUNRISE_END_G + ((DAY_START_G - SUNRISE_END_G) * m);
		b = SUNRISE_END_B;
		a = SUNRISE_END_A - ((SUNRISE_END_A - DAY_START_A) * m);
	} else if (minutes >= DAY_START_MINUTES && minutes < DAY_END_MINUTES) {
		const float m = ((minutes - DAY_START_MINUTES) / (DAY_END_MINUTES - DAY_START_MINUTES));
		r = DAY_START_R;
		g = DAY_START_G;
		b = 0;
		a = DAY_START_A - ((DAY_END_A - DAY_START_A) * m);
	} else if (minutes >= DAY_END_MINUTES && minutes < SUNSET_START_MINUTES) {
		const float m = ((minutes - DAY_END_MINUTES) / (SUNSET_START_MINUTES - DAY_END_MINUTES));
		r = DAY_END_R - ((DAY_END_R - SUNSET_START_R) * m);
		g = DAY_END_G - ((DAY_END_G - SUNSET_START_G) * m);
		b = 0;
		a = DAY_END_A + ((SUNSET_START_A - DAY_END_A) * m);
	} else if (minutes >= SUNSET_START_MINUTES && minutes < SUNSET_END_MINUTES) {
		const float m = ((minutes - SUNSET_START_MINUTES) / (SUNSET_END_MINUTES - SUNSET_START_MINUTES));
		r = SUNSET_START_R - ((SUNSET_START_R - SUNSET_END_R) * m);
		g = SUNSET_START_G - ((SUNSET_START_G - SUNSET_END_G) * m);
		b = (SUNSET_END_B - SUNSET_START_B) * m;
		a = SUNSET_START_A + ((SUNSET_END_A - SUNSET_START_A) * m);
	} else if (minutes >= SUNSET_END_MINUTES) {
		r = SUNSET_END_R;
		g = SUNSET_END_G;
		b = SUNSET_END_B;
		a = SUNSET_END_A;
	}
	al_clear_to_color(al_map_rgba(r, g, b, a));
	al_set_target_bitmap(al_get_backbuffer(display));
	al_draw_bitmap(this->light_filter, 0, 0, 0);
}

void GameImageManager::resume()
{
	player->clear_input();
}

void GameImageManager::decrement_dialog_option()
{
	player->decrement_dialog_option();
}

void GameImageManager::increment_dialog_option()
{
	player->increment_dialog_option();
}

void GameImageManager::process_cutscene(Cutscene * cutscene)
{
	if (cutscene != NULL && cutscene->has_action()) {
		const std::string action_key = cutscene->get_active_action_key();
		if (action_key == ACTION_AWAIT_LOAD) {
			if (this->thread_data.ready) {
				al_lock_mutex(this->thread_data.mutex);
				cutscene->advance_block();
				al_unlock_mutex(this->thread_data.mutex);
			}
		} else if (action_key == ACTION_UPDATE_GLOBAL_TIME) {
			GlobalTime * updated_time = cutscene->get_active_global_time();
			if (updated_time != NULL) {
				this->current_global_time->copy(updated_time);
				cutscene->advance_block();
			}
		} else if (action_key == ACTION_SAVE_GAME) {
			al_lock_mutex(this->thread_data.mutex);
			this->thread_data.global_time = this->current_global_time;
			al_unlock_mutex(this->thread_data.mutex);
			this->loading_thread = al_create_thread(load_func_save_game, &(this->thread_data));
			al_start_thread(this->loading_thread);
			al_rest(0.001); // give time to lock the mutex
			cutscene->advance_block();
		} else if (action_key == ACTION_LOAD_GAME) {
			if (this->loading_thread != NULL) {
				al_destroy_thread(this->loading_thread);
			}
			GlobalTime * updated_time = cutscene->get_active_global_time();
			//if (this->current_global_time != NULL) {
			//	delete this->current_global_time;
			//}
			//this->current_global_time = new GlobalTime(updated_time->get_day(), updated_time->get_time());
			this->thread_data.current_level_key = this->current_level->get_filename();
			//this->thread_data.global_time = this->current_global_time;
			this->loading_thread = al_create_thread(load_func_load_from_save, &(this->thread_data));
			al_start_thread(this->loading_thread);
			al_rest(0.01); // give time to lock the mutex
			cutscene->advance_block();
		} else if (action_key == ACTION_UPDATE_NEW_DAY) {
			if (this->loading_thread != NULL) {
				al_destroy_thread(this->loading_thread);
			}
			this->thread_data.current_level_key = this->current_level->get_filename();
			this->loading_thread = al_create_thread(load_func_advance_day, &(this->thread_data));
			al_start_thread(this->loading_thread);
			al_rest(0.001); // give time to lock the mutex
			cutscene->advance_block();
		}
	}
}



