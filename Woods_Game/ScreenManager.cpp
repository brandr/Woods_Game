
#include "ScreenManager.h"


ScreenManager::ScreenManager()
{
}
/*
ScreenManager::~ScreenManager()
{
	//delete this;
}
*/
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
		if (current_screen->get_screen_flag() == CHANGE_TO_MAIN_GAME) {
			//std::cout << "adding main game screen" << std::endl;
			add_screen(new MainGameScreen());
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
	if (current_screen)
		current_screen->process_event(ev);
}

void ScreenManager::add_screen(GameScreen *screen)
{
	//new_screen = screen;
	if (current_screen) {
		current_screen->unload_content();
		delete current_screen;
	}
	
	current_screen = screen;
	current_screen->load_content();
}

std::list<GameImage> ScreenManager::get_images()
{
	if (current_screen)
		return current_screen->get_images();
	return std::list<GameImage>();
}
