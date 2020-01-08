#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

#include "ScreenManager.h"


ScreenManager::ScreenManager()
{
}

void ScreenManager::load_content()
{
	if (current_screen)
		current_screen->load_content();
}

void ScreenManager::unload_content()
{
	current_screen->unload_content();
	delete current_screen;
	current_screen = NULL;
	
}

void ScreenManager::initilaize(GameScreen* screen)
{
	current_screen = screen;
}

ScreenManager &ScreenManager::get_instance() {
	static ScreenManager instance;
	return instance;
}

void ScreenManager::refresh()
{
	if (current_screen)
		current_screen->refresh();
}

void ScreenManager::update()
{
	if (current_screen) {
		if (current_screen->get_screen_flag() == FLAG_START_NEW_GAME) {
			//TODO: how to determine where to load from? 
			//options for world gen? how to know which save file/character name/etc?
			
			
			const std::string world_key = "world_1";

			// async (what we should do)

			
			al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
			LoadingScreen * loading_screen = new LoadingScreen();
			loading_screen->load_content();
			add_screen(loading_screen, false);
			loading_screen->start_new_game(world_key);
			

			//non-async for testing
			
			/*
			MainGameScreen * game_screen = new MainGameScreen();
			game_screen->start_new_game(world_key);
			game_screen->load_content();
			game_screen->load_fonts();
			
			
			add_screen(game_screen, false);
			*/
			
			//temp
			return;
		} else if (current_screen->get_screen_flag() == FLAG_LOAD_GAME) {
			const std::string world_key = current_screen->get_load_game_filepath();
			LoadingScreen * loading_screen = new LoadingScreen();
			loading_screen->load_content();
			add_screen(loading_screen, false);
			loading_screen->load_game(world_key);
			return;
		} else if (current_screen->get_screen_flag() == FLAG_FINISH_LOADING) {
			((LoadingScreen*)current_screen)->finish_loading();
			//((LoadingScreen*)current_screen)->unload_content();
			//TODO: make sure we actually need to do this every time
			//ImageLoader::get_instance().convert_bitmaps_to_video();
			
			al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
			al_convert_memory_bitmaps();
			GameScreen * next_screen = ((LoadingScreen*)current_screen)->get_next_screen();
			//TODO: load fonts async if possible
			next_screen->load_fonts();
			add_screen(next_screen, false);
		}
		current_screen->update();
	}
}

void ScreenManager::draw(ALLEGRO_DISPLAY* display)
{
	current_screen->draw(display);
}

void ScreenManager::process_event(ALLEGRO_EVENT ev)
{
	if (current_screen) {
		current_screen->process_event(ev);
	}
}

void ScreenManager::process_joystick(ALLEGRO_JOYSTICK_STATE joy_state)
{
}

void ScreenManager::add_screen(GameScreen *screen, const bool should_load_content)
{
	if (current_screen) {
		current_screen->unload_content();
		delete current_screen;
	}
	
	current_screen = screen;
	if (should_load_content) {
		current_screen->load_content();
	}
}

bool ScreenManager::should_close()
{
	return current_screen && current_screen->get_screen_flag() == FLAG_QUIT_GAME;
}

std::vector<GameImage> ScreenManager::get_images()
{
	if (current_screen)
		return current_screen->get_images();
	return std::vector<GameImage>();
}
