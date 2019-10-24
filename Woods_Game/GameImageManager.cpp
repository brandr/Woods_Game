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
	const std::string new_game_path = "resources/load/worlds/" + world_key + "/";
	const std::string world_filename = new_game_path + "world";
	filemanager.load_xml_content(&(this->world), world_filename, "SerializableClass", "WorldKey", world_key);
	this->world.load_dungeons();
	this->world.load_npcs();
	this->world.generate_levels();
	this->world.generate_map_images();
	const std::string initial_state_filename = new_game_path + "world_state";
	this->world.reload_world_state(initial_state_filename);
	const std::string initial_quest_data_filename = new_game_path + "quest_data";
	this->world.reload_quest_data(initial_quest_data_filename);
	this->load_item_templates();
	this->load_player();
	this->current_global_time = new GlobalTime(1, START_TIME_HOUR*TIME_RATIO);
	this->load_all_cutscenes();
	this->save_game();
	this->world.recalculate_npc_paths();
	this->thread_data.world = &(this->world);
	this->thread_data.player = this->player;
	this->thread_data.ready = true;
}

void GameImageManager::full_load_game_from_save(const std::string save_file)
{
	FileManager filemanager;
	const std::string filename = "resources/load/saves/" + save_file + "/world";
	filemanager.load_xml_content(&(this->world), filename, "SerializableClass", "WorldKey", save_file);
	const std::string world_key = this->world.get_world_key();
	const int day = this->world.get_current_day();
	this->current_global_time = new GlobalTime(day, START_TIME_HOUR*TIME_RATIO); //TODO: calculate start time as necessary
	const std::string dungeons_path = "resources/load/saves/" + world_key + "/" + "day_" + std::to_string(day) + "/dungeons/";
	this->world.load_dungeons(dungeons_path);
	this->world.load_npcs();
	this->world.generate_map_images();
	const std::string world_state_path = "resources/load/saves/" + world_key + "/" + "day_" + std::to_string(day) + "/world_state";
	this->world.reload_world_state(world_state_path);
	const std::string quest_data_path = "resources/load/saves/" + world_key + "/quest_data";
	this->world.reload_quest_data(quest_data_path);
	this->load_item_templates();
	this->load_player();
	this->load_all_cutscenes();
	this->save_game();
	this->world.recalculate_npc_paths();
	this->thread_data.world = &(this->world);
	this->thread_data.player = this->player;
	this->thread_data.ready = true;
}

void GameImageManager::load_game_from_save(const int day, const int time)
{
	FileManager filemanager;
	const std::string world_key = this->world.get_world_key();
	const std::string filename = "resources/load/saves/" + world_key + "/world";
	if (this->current_global_time != NULL) {
		delete this->current_global_time;
	}
	this->current_global_time = new GlobalTime(day, time);
	const std::string dungeons_path = "resources/load/saves/" + world_key + "/" + "day_" + std::to_string(day) + "/dungeons";
	this->world.reload_dungeons(dungeons_path);
	const std::string world_state_path = "resources/load/saves/" + world_key + "/" + "day_" + std::to_string(day) + "/world_state";
	this->world.reload_world_state(world_state_path);
	const std::string quest_data_path = "resources/load/saves/" + world_key + "/quest_data";
	this->world.reload_quest_data(quest_data_path);
	this->load_item_templates();
	this->load_player();
	this->world.recalculate_npc_paths();
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

const std::string GameImageManager::get_world_key()
{
	return this->world.get_world_key();
}

const std::string GameImageManager::get_current_dungeon_key()
{
	Dungeon * dungeon = this->world.get_current_dungeon();
	return dungeon != NULL ? dungeon->get_dungeon_name() : "";
}

const int GameImageManager::default_level_width()
{
	return this->world.get_default_level_width();
}

const int GameImageManager::default_level_height()
{
	return this->world.get_default_level_height();
}

World * GameImageManager::get_world()
{
	return &world;
}

Level * GameImageManager::get_level_with_key(const std::string level_key)
{
	return this->world.get_level_with_key(level_key);
}

std::vector<LocationMarker*> GameImageManager::get_current_dungeon_location_markers()
{
	Dungeon * current_dungeon = this->world.get_current_dungeon();
	return current_dungeon->get_all_location_markers();
}

void GameImageManager::load_player()
{
	this->player = this->world.get_player();
	this->player->load_content_from_attributes();
	player->load_additional_masks_from_attributes("player");
	this->current_level = this->world.extract_current_level(player, "");
	this->current_level->add_being(player);
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

void GameImageManager::load_all_cutscenes()
{
	FileManager filemanager;
	const std::string filename = "resources/load/cutscenes";
	filemanager.load_xml_content(&(this->cutscene_list), filename, "SerializableClass", "CutsceneListKey", "all_cutscenes");
	//TODO: any other loading necessary for cutscenes
}

void GameImageManager::load_item_templates()
{
	ItemManager::get_instance().load_item_templates();
}

CutsceneScript * GameImageManager::get_cutscene_script(const std::string cutscene_key)
{
	return this->cutscene_list.get_cutscene_script(cutscene_key);
}

Cutscene * GameImageManager::generate_cutscene(CutsceneScript * script)
{
	//TODO: do we want to always create a new object and destroy it later, or should we have a persistent map?
	Cutscene * scene = new Cutscene();
	const std::string cutscene_key = script->get_cutscene_key();
	if (!cutscene_key.empty()) {
		scene->set_cutscene_key(script->get_cutscene_key());
		this->world.mark_cutscene_viewed(cutscene_key);
	}
	
	std::vector<CutsceneScriptBlock *> script_blocks = script->get_blocks();
	for (CutsceneScriptBlock * sb : script_blocks) {
		CutsceneBlock * block = this->generate_cutscene_block(sb);
		scene->add_block(block);
	}
	
	return scene;
}

CutsceneBlock * GameImageManager::generate_cutscene_block(CutsceneScriptBlock * script_block)
{
	CutsceneBlock * block = new CutsceneBlock();
	block->block_index = script_block->get_block_index();
	block->next_block_index = script_block->get_next_block_index();
	block->duration = script_block->get_duration();
	block->action_key = script_block->get_action_key();
	block->effect_key = script_block->get_effect_key();
	block->music_key = script_block->get_music_key();
	block->cutscene_tiles = script_block->get_cutscene_tiles();
	block->cutscene_branches = script_block->get_cutscene_branches();
	block->block_triggers = script_block->get_block_triggers();
	block->agent_animations = script_block->get_agent_animations();
	block->dialog = script_block->get_dialog();
	return block;
}

const bool GameImageManager::player_update(std::map<int, bool> input_map, std::map<int, std::pair<float, float>> joystick_map)
{
	if (this->player) {
		this->pending_trigger_update();
		if (player->get_exit_level_flag()) {
			//TODO: check to make sure there is a next level in the given direction here
			change_player_level();
			return false;
		}
		if (player->has_open_dialog()) {
			set_game_mode(MAIN_GAME_DIALOG);
			return false;
		}
		this->check_player_cutscene(player);
		if (player->has_active_cutscene()) {
			set_game_mode(CUTSCENE);
			return false;
		}
		if (player->get_should_open_calendar()) {
			set_game_mode(CALENDAR);
			return false;
		}
		this->player_exploration_update();
		this->player_exchange_inventory_update();
		player->update_input(input_map, joystick_map, game_mode);
	}
	return true;
}

void GameImageManager::check_player_cutscene(Player * player)
{
	const std::string cutscene_key = player->get_active_cutscene_key();
	if (!cutscene_key.empty() && !player->has_active_cutscene()) {
		CutsceneScript * script = this->get_cutscene_script(cutscene_key);
		if (script != NULL) {
			if (script->is_replayable() || !this->world.has_viewed_cutscene(cutscene_key)) {
				Cutscene * scene = this->generate_cutscene(script);
				player->set_active_cutscene(scene);
			}
		}
	}
}

void GameImageManager::player_exploration_update()
{
	if (this->player != NULL) {
		const std::pair<int, int> current_pixel_loc = this->current_player_location_for_map();
		if (current_pixel_loc.first >= 0 && current_pixel_loc.second >= 0) {
			const int default_w = this->world.get_default_level_width(), default_h = this->world.get_default_level_height();
			const int explore_grid_x = current_pixel_loc.first / default_w, explore_grid_y = current_pixel_loc.second / default_h;
			this->world.mark_grid_explored(explore_grid_x, explore_grid_y);
		}
	}
}

void GameImageManager::player_exchange_inventory_update()
{
	if (this->player != NULL) {
		const std::string inv_key = player->get_exchange_inventory_key();
		if (!inv_key.empty()) {
			this->game_mode = EXCHANGE_INVENTORY;
		}
	}
}

void GameImageManager::pending_trigger_update()
{
	if (this->player != NULL) {
		const std::map<std::string, std::string> pending_updates = this->player->get_pending_trigger_updates();
		for (auto const &it : pending_updates) {
			if (it.first == SET_HAS_MET_NPC) {
				const std::string npc_key = it.second;
				NPC * npc = this->get_npc(npc_key);
				if (npc != NULL) {
					this->world.set_has_met_npc(npc_key);
				}
			}
			//TODO: other triggers
		}
		this->player->clear_pending_triggers();
	}
}

NPC * GameImageManager::get_npc(const std::string npc_key)
{
	return this->world.get_npc(npc_key);
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
	const std::string dungeons_path = "resources/load/saves/" + world_key + "/" + "day_" + std::to_string(data->global_time->get_day()) + "/dungeons";
	data->world->reload_dungeons(dungeons_path);
	const std::string world_state_path = "resources/load/saves/" + world_key + "/" + "day_" + std::to_string(data->global_time->get_day()) + "/world_state";
	data->world->reload_world_state(world_state_path);
	const std::string quest_data_path = "resources/load/saves/" + world_key + "/quest_data";
	data->world->reload_quest_data(quest_data_path);
	data->world->update_reload_day(data->player, data->current_level_key);
	data->world->recalculate_npc_paths();
	data->ready = true;
	al_broadcast_cond(data->cond);
	al_unlock_mutex(data->mutex);
	return NULL;
}

void GameImageManager::walk_agents_for_cutscene(Cutscene * cutscene)
{
	bool all_have_reached_dest = true;
	std::vector<CutsceneTile> cutscene_tiles = cutscene->get_current_block_cutscene_tiles();
	for (CutsceneTile ct : cutscene_tiles) {
		Being * agent = NULL;
		if (CUTSCENE_AGENT_PLAYER == ct.agent_key) {
			agent = this->player;
		} else {
			agent = this->get_npc(ct.agent_key);
		}
		if (agent != NULL) {
			if (!agent->cutscene_walk_towards_tile_dest(this->current_level, ct.tile_x, ct.tile_y)) {
				all_have_reached_dest = false;
			}
			else {
				agent->set_animation_direction(ct.direction);
			}
		} else {
			//TODO: error handling
		}
	}
	if (all_have_reached_dest) {
		cutscene->advance_block(&(this->world), this->current_level);
	}
}

Player * GameImageManager::get_player()
{
	return player;
}

Inventory * GameImageManager::get_exchange_inventory()
{
	const std::string inv_key = this->player->get_exchange_inventory_key();
	if (!inv_key.empty()) {
		return this->world.get_inventory_for_key(inv_key, true);
	}
	return NULL;
}

void GameImageManager::update_quests()
{
	this->world.update_quests();
}

QuestData * GameImageManager::get_quest_data()
{
	return this->world.get_quest_data();
}

std::vector<Quest*> GameImageManager::get_active_quests()
{
	return this->world.get_active_quests();
}

std::vector<Quest*> GameImageManager::get_failed_quests()
{
	return this->world.get_failed_quests();
}

std::vector<Quest*> GameImageManager::get_completed_quests()
{
	return this->world.get_completed_quests();
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
	if (!this->player_update(input_map, joystick_map)) {
		al_unlock_mutex(this->thread_data.mutex);
		return;
	}
	current_level->update(&(this->world), this->get_current_global_time(), game_mode);
	this->time_update();
	this->npc_update();
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
	ALLEGRO_BITMAP * filter = NULL;
	const std::string suffix = "";
	if (!ImageLoader::get_instance().keyed_image_exists(IMAGE_KEY_FILLED_RECT, width, height, suffix)) {
		filter = al_create_bitmap(width, height);
		ImageLoader::get_instance().set_keyed_image(filter, IMAGE_KEY_FILLED_RECT, width, height, suffix);
	} else {
		filter = ImageLoader::get_instance().get_keyed_image(IMAGE_KEY_FILLED_RECT, width, height, suffix);
	}
	//if (this->light_filter == NULL) {
	//	this->light_filter = al_create_bitmap(width, height);
	//}

	
	al_set_target_bitmap(filter);
	/*
	if (al_get_bitmap_width(filter) != width
		|| al_get_bitmap_height(filter) != height) {
		al_destroy_bitmap(filter);
		this->light_filter = al_create_bitmap(width, height);
	}
	*/
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
	al_draw_bitmap(filter, 0, 0, 0);
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
	if (cutscene != NULL) {
		if (cutscene->has_action()) {
			const std::string action_key = cutscene->get_active_action_key();
			if (action_key == ACTION_AWAIT_LOAD) {
				if (this->thread_data.ready) {
					al_lock_mutex(this->thread_data.mutex);
					cutscene->advance_block(&(this->world), this->current_level);
					al_unlock_mutex(this->thread_data.mutex);
					this->load_player();
				}
			}
			else if (action_key == ACTION_UPDATE_GLOBAL_TIME) {
				GlobalTime * updated_time = cutscene->get_active_global_time();
				if (updated_time != NULL) {
					this->current_global_time->copy(updated_time);
					cutscene->advance_block(&(this->world), this->current_level);
				}
			}
			else if (action_key == ACTION_SAVE_GAME) {
				if (this->loading_thread != NULL) {
					al_destroy_thread(this->loading_thread);
				}
				al_lock_mutex(this->thread_data.mutex);
				this->thread_data.global_time = this->current_global_time;
				al_unlock_mutex(this->thread_data.mutex);
				this->loading_thread = al_create_thread(load_func_save_game, &(this->thread_data));
				al_start_thread(this->loading_thread);
				cutscene->advance_block(&(this->world), this->current_level);
			}
			else if (action_key == ACTION_LOAD_GAME) {
				if (this->loading_thread != NULL) {
					al_destroy_thread(this->loading_thread);
				}
				GlobalTime * updated_time = cutscene->get_active_global_time();
				this->thread_data.current_level_key = this->current_level->get_filename();
				this->thread_data.global_time = updated_time;
				this->loading_thread = al_create_thread(load_func_load_from_save, &(this->thread_data));
				al_start_thread(this->loading_thread);
				cutscene->advance_block(&(this->world), this->current_level);
			}
			else if (action_key == ACTION_UPDATE_NEW_DAY) {
				if (this->loading_thread != NULL) {
					al_destroy_thread(this->loading_thread);
				}
				this->thread_data.current_level_key = this->current_level->get_filename();
				this->loading_thread = al_create_thread(load_func_advance_day, &(this->thread_data));
				al_start_thread(this->loading_thread);

				//temp (non-async)
				//this->world.update_new_day(this->player, this->current_level->get_filename());
				//this->world.recalculate_npc_paths();
				//temp
				cutscene->advance_block(&(this->world), this->current_level);
			}
			// other actions
			else if (action_key == ACTION_WALK_AGENTS) {
				this->walk_agents_for_cutscene(cutscene);
			}
			else if (action_key == ACTION_DIALOG) {
				if (cutscene->has_opened_dialog()) {
					cutscene->advance_block(&(this->world), this->current_level);
				}
				else if (!player->has_open_dialog()) {
					Dialog * dialog = cutscene->get_active_dialog();
					player->set_open_dialog(dialog);
					this->set_game_mode(MAIN_GAME_DIALOG);
					cutscene->set_has_opened_dialog(true);
				}
			}
		}
		if (cutscene->should_update_trigger_statuses()) {
			std::vector<TriggerStatus *> statuses = cutscene->trigger_statuses();
			for (TriggerStatus * status : statuses) {
				this->world.copy_trigger_status(status);
			}
		}
		cutscene->process_music();
	}
}

void GameImageManager::cutscene_animation_update()
{
	Player * player = this->get_player();
	Cutscene * cutscene = player->get_active_cutscene();
	if (cutscene != NULL) {
		std::vector<CutsceneAgentAnimation> agent_animations = cutscene->get_current_block_agent_animations();
		for (CutsceneAgentAnimation ct : agent_animations) {
			Being * agent = NULL;
			if (CUTSCENE_AGENT_PLAYER == ct.agent_key) {
				agent = this->player;
			}
			else {
				agent = this->get_npc(ct.agent_key);
			}
			if (agent != NULL) {
				const int anim_index = GameImage::get_anim_state_index(ct.animation_key);
				agent->set_animation_index(anim_index);
				agent->set_animation_direction(ct.direction);
			} else {
				//TODO: error handling
			}
		}
	}
	this->current_level->cutscene_animation_update();
}

const std::pair<int, int> GameImageManager::current_player_location_for_map()
{
	const int grid_x = this->current_level->get_grid_x(), grid_y = this->current_level->get_grid_y();
	if (grid_x < 0 || grid_y < 0) {
		return this->world.map_location_for_level(this->current_level);
	}
	const int standard_w = this->world.get_default_level_width(), standard_h = this->world.get_default_level_height();
	const int x_off = this->player->get_x(), y_off = this->player->get_y();
	return std::pair<int, int>(grid_x * standard_w + x_off, grid_y * standard_h + y_off);
}

const std::set<std::pair<int, int>> GameImageManager::explored_map()
{
	return this->world.explored_map();
}