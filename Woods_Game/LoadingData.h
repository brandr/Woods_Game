#ifndef LOADING_DATA_H
#define LOADING_DATA_H

#include "GameScreen.h"
#include "World.h"
#include "GlobalTime.h"
#include "allegro5/threads.h"

//class Player;
class World;
class LoadingData {

public:

	ALLEGRO_MUTEX * mutex;
	ALLEGRO_COND  *cond;
	GameScreen * next_screen;
	World * world;
	Player * player;
	std::string current_level_key;
	GlobalTime * global_time;
	std::string next_screen_key;
	bool           ready;

	LoadingData() : mutex(al_create_mutex()),
		cond(al_create_cond()),
		next_screen(NULL),
		next_screen_key(""),
		ready(false) {}

	~LoadingData() {
		al_destroy_mutex(mutex);
		al_destroy_cond(cond);
	}
};

#endif
