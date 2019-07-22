#ifndef LOADING_SCREEN_H
#define LOADING_SCREEN_H

#include "GameScreen.h"
#include "MainGameScreen.h"
#include "MenuManager.h"
#include "GameImageManager.h"
#include "LoadingData.h"

#include "allegro5/allegro_font.h"  // for ALLEGRO_FONT
#include "allegro5/display.h"       // for ALLEGRO_DISPLAY
#include "allegro5/events.h"        // for ALLEGRO_EVENT
#include "allegro5/allegro_ttf.h"    
#include "allegro5/bitmap.h"
#include "allegro5/threads.h"

#include <thread>
#include <chrono>

class LoadingScreen :
	public GameScreen
{
private:
	
	ALLEGRO_THREAD * loading_thread;
	LoadingData thread_data;
	ALLEGRO_FONT * font;
	std::map<int, std::string> backdrop_filenames;
	int animation_index = 0;

	// async functions
	static void *load_func_start_new_game(ALLEGRO_THREAD *thr, void *arg);
	static void *load_func_load_game(ALLEGRO_THREAD *thr, void *arg);
public:
	LoadingScreen();
	~LoadingScreen();
	virtual void load_content();
	virtual void unload_content();
	virtual void update();
	virtual void draw(ALLEGRO_DISPLAY *display);
	void finish_loading();
	void start_new_game(const std::string world_key);
	void load_game(const std::string world_key);
	GameScreen * get_next_screen();
};

#endif