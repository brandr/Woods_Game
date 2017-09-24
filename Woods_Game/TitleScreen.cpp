#include "TitleScreen.h"

InputManager title_input;

TitleScreen::TitleScreen()
{
}


TitleScreen::~TitleScreen()
{
	font = NULL;
}

void TitleScreen::load_content()
{
	font = al_load_ttf_font("resources/fonts/Xeranthemum.ttf", 30, NULL);
}

void TitleScreen::unload_content()
{
	al_destroy_font(font);
	font = NULL;
}

void TitleScreen::update()
{
	//TODO
}

void TitleScreen::draw(ALLEGRO_DISPLAY * display)
{
	al_draw_text(font, al_map_rgb(255, 0, 0), 100, 100, NULL, "TITLE");
}

void TitleScreen::process_event(ALLEGRO_EVENT ev)
{
	if (title_input.is_key_pressed(ev, ALLEGRO_KEY_ENTER))
		screen_flag = FLAG_CHANGE_TO_MAIN_GAME;
	//	ScreenManager::get_instance().
}
