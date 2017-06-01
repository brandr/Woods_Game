#define _CRTDBG_MAP_ALLOC
#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
//TODO: replace new with DBG_NEW when it is used
#else
#define DBG_NEW new
#endif
#include <stdlib.h>
#include <crtdbg.h>
#include<allegro5/allegro_native_dialog.h>
#include<allegro5/allegro_primitives.h>

#include "ScreenManager.h"
#include<iostream>


int main()
{
	
	//before title screen

	//_CrtSetBreakAlloc(605);		
	/*
	known sources of leaks:
	-map allocation in imageloader
	-gameimage vector allocation
	*/

	//TEST
	/*
	{
		//std::vector<std::shared_ptr<GameImage>> game_images;
		std::vector<GameImage*> game_images;
		game_images.clear();
	}
	//game_images.swap(std::vector<GameImage*>());
	//delete [] game_images;
	*/
	/*
	{
		World world;
		world.unload_content();
	}
	*/
	//TEST 

	// intialize allegro
	if (!al_init()) {
		al_show_native_message_box(NULL, NULL, NULL, "Could not intialize allegro 5.", NULL, NULL);
		return -1;
	}

	const float FPS = 60.0f;

	ALLEGRO_DISPLAY *display = al_create_display(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
	
	// make usre the display was created correctly
	if (!display) {
		al_show_native_message_box(display, "Title", "settings", "Could not create Allegro window.", NULL, NULL);
	}
	al_set_window_title(display, "Game title");

	// intialize fonts, primitives, keyboard,etc.
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_init_image_addon();
	al_install_keyboard();
	al_install_joystick();
	al_install_mouse();

	// set up timer/keyboard input
	ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	//ALLEGRO_KEYBOARD_STATE keyState;

	// register all sources that we will listen for in the main loop
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_joystick_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display));
	bool done = false, draw = true;
	
	//TitleScreen title_screen;
	ScreenManager::get_instance().initilaize(new TitleScreen());
	ScreenManager::get_instance().load_content();
	ImageLoader::get_instance().load_content();
	InputManager input;
	al_start_timer(timer);
	// game loop
	while (!done) {
		ScreenManager::get_instance().refresh();
		ALLEGRO_EVENT events;
		al_wait_for_event(event_queue, &events);
		if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			done = true;
		ScreenManager::get_instance().process_event(events);
		// handle movement updates
		if (events.type == ALLEGRO_EVENT_TIMER) {
			ScreenManager::get_instance().update();
			draw = true;
		}
		if (draw) {
			draw = false;
			ScreenManager::get_instance().draw(display);
			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}
	}
	{
		ImageLoader::get_instance().unload_content();
		ScreenManager::get_instance().unload_content();
		al_destroy_display(display);
		al_destroy_timer(timer);
		al_unregister_event_source(event_queue, al_get_timer_event_source(timer));
		al_unregister_event_source(event_queue, al_get_keyboard_event_source());
		al_unregister_event_source(event_queue, al_get_joystick_event_source());
		al_unregister_event_source(event_queue, al_get_display_event_source(display));
		al_destroy_event_queue(event_queue);
		al_uninstall_joystick();
		al_uninstall_mouse();
		al_uninstall_keyboard();
		al_uninstall_system();
	}
	_CrtDumpMemoryLeaks();
	return 0;
}
