#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include "AudioManager.h"
#include "LoadingScreen.h"
#include "MainGameScreen.h"
#include "TitleScreen.h"

class ScreenManager
{
private:
	ScreenManager();
	GameScreen *current_screen;
	void stop_music();
public:
	ScreenManager(ScreenManager const&) = delete;
	void operator=(ScreenManager const&) = delete;
	void load_content();
	void unload_content();
	static ScreenManager& get_instance();
	void initilaize(GameScreen*);
	void refresh();
	void update();
	void draw(ALLEGRO_DISPLAY*);
	void process_event(ALLEGRO_EVENT);
	void process_joystick(ALLEGRO_JOYSTICK_STATE joy_state);
	void add_screen(GameScreen *screen, const bool should_load_content);
	bool should_close();
	std::vector<GameImage> get_images();
};

#endif