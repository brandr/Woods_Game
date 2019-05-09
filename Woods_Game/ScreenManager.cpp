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
			//TODO: how to determine where to load from? need to prompt generation? how to know which save file/character name/etc?
			const std::string world_key = "world_1"; //TEMP. maybe different options for world gen?
			MainGameScreen * game_screen = new MainGameScreen();
			game_screen->start_new_game(world_key);
			add_screen(game_screen);
			//TODO: save game once it's loaded (might be inside game_screen->start_new_game())
			return;
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
	//if (current_screen)
	//	current_screen->set_joystick_pos(LEFT_STICK, joy_state.stick->axis[0], joy_state.stick->axis[1]);
}

void ScreenManager::add_screen(GameScreen *screen)
{
	if (current_screen) {
		current_screen->unload_content();
		delete current_screen;
	}
	
	current_screen = screen;
	current_screen->load_content();
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
