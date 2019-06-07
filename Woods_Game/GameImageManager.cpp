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
	load_player_from_xml("resources/load/player", this->world.get_player_key()); //TODO: Should we look for a default player? Create one with some settings entered at the start?
	this->current_global_time = new GlobalTime(1, START_TIME_HOUR*TIME_RATIO);
	this->save_game();

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
	//TODO: fix the part where we cut to black right after loading (maybe need to load asynchronously while fading to black)
	//TODO: fix memory issues that hapen when reloading
}

void GameImageManager::save_game()
{
	//TODO: make sure "world" data can be managed across days in terms of things like NPC interactions
	//		might want different types of data files besides world to manage some NPC stuff
	const std::string save_file_name = this->world.get_world_key();
	const std::string day_str = "day_" + std::to_string(this->current_global_time->get_day());
	this->world.set_current_day(this->current_global_time->get_day());
	std::string save_file_dir = "resources/load/saves/" + save_file_name;
	if (!boost::filesystem::is_directory(save_file_dir)) {
		boost::filesystem::create_directory(save_file_dir);
	}
	save_file_dir += "/" + day_str;
	if (!boost::filesystem::is_directory(save_file_dir)) {
		boost::filesystem::create_directory(save_file_dir);
	}
	FileManager filemanager;
	const std::string world_file_path = "resources/load/saves/" + save_file_name + "/worlds";
	std::map<std::string, std::string> attributes;
	attributes["Type"] = "World";
	attributes["Version"] = "1";
	attributes["WorldKey"] = save_file_name;
	file_manager.create_xml_file(world_file_path);
	file_manager.save_xml_content(world_file_path, "SerializableClass", attributes);
	const std::string world_xml = this->world.toXML();
	//TODO: when to update NPCs? (I think these get serialized in world)
	file_manager.replace_xml_content(world_file_path, "SerializableClass", "WorldKey", save_file_name, world_xml);
	const std::string dungeon_dir = save_file_dir + "/dungeons";
	if (!boost::filesystem::is_directory(dungeon_dir)) {
		boost::filesystem::create_directory(dungeon_dir);
	}
	for (std::shared_ptr<Dungeon> d : this->world.get_dungeons()) {
		const std::string dungeon_path = dungeon_dir + "/" + d->get_dungeon_name();
		file_manager.create_xml_file(dungeon_path);
		std::vector<Level*> levels = d->get_level_list();
		for (Level *level : levels) {
			std::map<std::string, std::string> attributes;
			attributes["Type"] = "Level";
			attributes["Version"] = "1";
			attributes["LevelKey"] = level->get_filename();
			file_manager.save_xml_content(dungeon_path, "SerializableClass", attributes);
			const std::string level_xml = level->toXML();
			file_manager.replace_xml_content(dungeon_path, "SerializableClass", "LevelKey", level->get_filename(), level_xml);
		}
	}
	const std::string save_file_path = save_file_dir + "/" + day_str;
	file_manager.create_xml_file(save_file_path);
}

void GameImageManager::set_game_mode(int game_mode)
{
	this->game_mode = game_mode;
}

int GameImageManager::get_game_mode()
{
	return game_mode;
}

void GameImageManager::load_level(int grid_x, int grid_y)
{
	Level *level = world.get_current_dungeon()->level_at(grid_x, grid_y);
	if (level) {
		load_level_from_map(level);
	} else {
		std::cout << "ERROR: failed to load level." << std::endl; //TODO: error handling
	}
}

void GameImageManager::load_player(std::string filename)
{
	std::vector<std::vector<std::string>> attributes;
	std::vector<std::vector<std::string>> contents;
	FileManager file_manager;
	file_manager.load_content(filename.c_str(), attributes, contents, "gameimage_properties");
	player = new Player();
	player->load_content(attributes[0], contents[0]);
	player->set_bitmap(ImageLoader::get_instance().get_current_image(player));
	attributes.clear(), contents.clear();
	file_manager.load_content(filename.c_str(), attributes, contents, "additional_masks");
	player->load_additional_masks(attributes[0], contents[0], "player");
	current_level->add_being(player);
}

void GameImageManager::load_level_content(std::string filename, std::string id, int type)
{
	std::vector<std::vector<std::string>> attributes;
	std::vector<std::vector<std::string>> contents;
	FileManager file_manager;
	if (id == "") {
		file_manager.load_content(filename.c_str(), attributes, contents);
	} else {
		file_manager.load_content(filename.c_str(), attributes, contents, id);
	}
	int size = attributes.size();
	for (int i = 0; i < size; i++) {
		GameImage *image;
		switch (type) {
		case PLAYER:
			image = new Player();
			player = dynamic_cast<Player*>(image);
			player->load_content(attributes[i], contents[i]);
			player->set_bitmap(ImageLoader::get_instance().get_current_image(player));
			current_level->add_being(player);
			break;
		}
	}
}

void GameImageManager::load_level_from_map(Level *level)
{
	//TODO?
}

void GameImageManager::load_player()
{
	
}

void GameImageManager::load_player_from_xml(std::string filepath, std::string player_key)
{
	FileManager file_manager;
	player = new Player();
	file_manager.load_xml_content(player, filepath, "SerializableClass", "PlayerKey", player_key);
	player->load_content_from_attributes();
	player->set_bitmap(ImageLoader::get_instance().get_current_image(player));
	player->load_additional_masks_from_attributes("player");
	this->current_level = this->world.extract_current_level(player);
	this->current_level->add_being(player);
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
	//TODO: where to do dialog updates?
	const int game_mode = get_game_mode();
	if (game_mode != TOP_DOWN) return; //TODO: make sure this is the right check
	std::pair<int, int> dimensions = current_level->get_dimensions();
	if (player) {
		if (player->get_exit_level_flag()) {
			//TODO: check to make sure there is a next level in the given direction here
			change_player_level();
			return;
		}
		if (player->has_open_dialog()) {
			set_game_mode(MAIN_GAME_DIALOG);
			return;
		}
		if (player->has_active_cutscene()) {
			set_game_mode(CUTSCENE);
			return;
		}
		if (player->get_should_open_calendar()) {
			set_game_mode(CALENDAR);
			return;
		}
		player->update_input(input_map, joystick_map, game_mode);
	}
	current_level->update(game_mode);
	this->time_update();
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

void GameImageManager::update_new_day()
{
	this->world.update_new_day(this->player);
	//TODO: plants grow, NPCs go to appropriate starting positions, etc
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

		enum { UP, DOWN, LEFT, RIGHT };
		int direction = 0;
		//up
		if (player->get_y() + player->get_height() < 0) {
			x = grid_x, y = grid_y - 1;
			direction = UP;
		}

		//down
		else if (player->get_y() > height) {
			x = grid_x, y = grid_y + current_level->get_grid_height();
			direction = DOWN;
		}
		//left
		else if (player->get_x() + player->get_width() < 0) {
			x = grid_x - 1, y = grid_y;
			direction = LEFT;
		}

		//right
		else if (player->get_x() > width) {
			x = grid_x + current_level->get_grid_width(), y = grid_y;
			direction = RIGHT;
		}
		else {
			std::cout << "ERROR: no direction " << std::endl;
		}
		next_level = world.get_current_dungeon()->level_at(x, y);
		const int level_width = STANDARD_LEVEL_GRID_WIDTH;
		const int level_height = STANDARD_LEVEL_GRID_HEIGHT;
		const float player_offset_x = fmod(player->get_x(), level_width), player_offset_y = fmod(player->get_y(), level_height);
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
	if (al_get_bitmap_width(this->light_filter) != width || al_get_bitmap_height(this->light_filter) != height) {
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
		const float m = ((minutes - SUNRISE_END_MINUTES) / (DAY_END_MINUTES - DAY_START_MINUTES));
		r = DAY_START_R;
		g = DAY_START_G;
		b = 0;
		a = DAY_START_A - ((SUNRISE_END_A - DAY_START_A) * m);
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
		if (action_key == ACTION_UPDATE_GLOBAL_TIME) {
			GlobalTime * updated_time = cutscene->get_active_global_time();
			if (updated_time != NULL) {
				this->current_global_time->copy(updated_time);
				cutscene->advance_block();
			}
		} else if (action_key == ACTION_SAVE_GAME) {
			this->save_game();
			cutscene->advance_block();
		} else if (action_key == ACTION_LOAD_GAME) {
			GlobalTime * updated_time = cutscene->get_active_global_time();
			if (updated_time != NULL) {
				this->load_game_from_save(updated_time->get_day(), updated_time->get_time());
				cutscene->advance_block();
			}
		} else if (action_key == ACTION_UPDATE_NEW_DAY) {
			this->update_new_day();
			cutscene->advance_block();
		}
	}
}



