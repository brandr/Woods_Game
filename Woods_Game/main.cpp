// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
//TODO: replace new with DBG_NEW when it is used
#include<stdlib.h>
#include<allegro5/allegro_native_dialog.h>
#include<allegro5/allegro_primitives.h>

#include "ScreenManager.h"
#include "allegro5/allegro_font.h"           // for al_init_font_addon
#include "allegro5/allegro_ttf.h"           
#include "allegro5/allegro_image.h"
#include "allegro5/blender.h"                // for al_set_blender, ALLEGRO_BLEND_MODE::ALLEGRO_ALPHA, ALLEGRO_BLEND_MODE::ALLEGRO_INVERSE_ALPHA, ALLEGRO_BLEND_OPERATIONS::ALLEGRO_ADD
#include "allegro5/color.h"                  // for al_map_rgb
#include "allegro5/display.h"                // for al_create_display, al_get_display_event_source, al_set_new_display_flags, al_destroy_display, al_flip_display, al_set_window_title, ::ALLEGRO_FULLSCREEN, ::ALLEGRO_FULLSCREEN_WINDOW, ALLEGRO_DISPLAY
#include "allegro5/drawing.h"                // for al_clear_to_color
#include "allegro5/events.h"                 // for al_register_event_source, al_unregister_event_source, ALLEGRO_EVENT, al_create_event_queue, al_destroy_event_queue, al_wait_for_event, ::ALLEGRO_EVENT_DISPLAY_CLOSE, ::ALLEGRO_EVENT_TIMER, ALLEGRO_EVENT_QUEUE
#include "allegro5/fullscreen_mode.h"        // for ALLEGRO_DISPLAY_MODE, al_get_display_mode, al_get_num_display_modes
#include "allegro5/joystick.h"               // for al_get_joystick_event_source, al_install_joystick, al_uninstall_joystick
#include "allegro5/keyboard.h"               // for al_get_keyboard_event_source, al_install_keyboard, al_uninstall_keyboard
#include "allegro5/mouse.h"                  // for al_get_mouse_event_source, al_install_mouse, al_uninstall_mouse
#include "allegro5/system.h"                 // for al_uninstall_system, al_init
#include "allegro5/timer.h"                  // for al_get_timer_event_source, al_create_timer, al_destroy_timer, al_start_timer, ALLEGRO_TIMER
#include "FileManager.h"                     // for FileManager
#include "GameScreen.h"                      // for SCREEN_STYLE_FULLSCREEN, SCREEN_STYLE_WINDOWED, SCREEN_STYLE_WINDOWED_FULLSCREEN
#include "ImageLoader.h"                     // for ImageLoader
#include "InputManager.h"                    // for InputManager
#include "MainGameScreen.h"                  // for DEFAULT_SCREEN_HEIGHT, DEFAULT_SCREEN_WIDTH
#include "TitleScreen.h"                     // for TitleScreen
#include "utility"                           // for pair
#include "vcruntime_new.h"                   // for operator new
#include "vector"                            // for vector
#include "xstring"                           // for string, operator==, basic_string
#include <memory>                            // for allocator


int main()
{
	//before title screen

	//_CrtSetBreakAlloc(605);		
	/*
	known sources of leaks:

	-map allocation in imageloader
	-gameimage vector allocation
	*/

	// intialize allegro
	if (!al_init()) {
		al_show_native_message_box(NULL, NULL, NULL, "Could not intialize allegro 5.", NULL, NULL);
		return -1;
	}

	const float FPS = 60.0f;

	FileManager filemanager;
	std::vector<std::vector<std::string>> attributes, contents;
	filemanager.load_content("resources/config.ini", attributes, contents);

	//TODO: handle config settings here

	int screen_width = DEFAULT_SCREEN_WIDTH, screen_height = DEFAULT_SCREEN_HEIGHT;
	ALLEGRO_DISPLAY *display;// = al_create_display(screen_width, screen_height);
	std::string display_style;
	for (int i = 0; i < attributes.size(); i++) {
		for (int j = 0; j < attributes[i].size(); j++) {
			if (attributes[i][j] == "screen_resolution") {
				std::pair <std::string, std::string> resolution = FileManager::string_to_pair(contents[i][j], "x");
				screen_width = ::atoi(resolution.first.c_str()), screen_height = ::atoi(resolution.second.c_str());
			}
			else if (attributes[i][j] == "screen_style") {
				display_style = contents[i][j];
			}
		}
	}

	if (display_style == SCREEN_STYLE_FULLSCREEN) {
		ALLEGRO_DISPLAY_MODE   disp_data;
		al_get_display_mode(al_get_num_display_modes() - 1, &disp_data);
		al_set_new_display_flags(ALLEGRO_FULLSCREEN);
		display = al_create_display(disp_data.width, disp_data.height);
	}
	else if (display_style == SCREEN_STYLE_WINDOWED_FULLSCREEN) {
		ALLEGRO_DISPLAY_MODE   disp_data;
		al_get_display_mode(al_get_num_display_modes() - 1, &disp_data);
		al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
		display = al_create_display(disp_data.width, disp_data.height);
	}
	else if (display_style == SCREEN_STYLE_WINDOWED) {
		display = al_create_display(screen_width, screen_height);
	}
	else {	//windowed by default, but the above block is included for the sake of consistency.
		display = al_create_display(screen_width, screen_height);
	}

	//ALLEGRO_DISPLAY *display = al_create_display(screen_width, screen_height);
	
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

	// register all sources that we will listen for in the main loop
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_joystick_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display));
	bool done = false, draw = true;

	al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
	
	//TitleScreen title_screen;
	ScreenManager::get_instance().initilaize(new TitleScreen());
	ScreenManager::get_instance().load_content();
	ImageLoader::get_instance().load_content();
	//Controls::load_input_key_to_label_map();
	InputManager input;
	al_start_timer(timer);
	// game loop
	while(!done && !ScreenManager::get_instance().should_close()){
	//while (!done) {
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
	//_CrtDumpMemoryLeaks();
	return 0;
}
