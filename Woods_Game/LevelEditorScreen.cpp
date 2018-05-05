#include "LevelEditorScreen.h"

InputManager level_editor_input;

LevelEditorScreen::LevelEditorScreen()
{
}


LevelEditorScreen::~LevelEditorScreen()
{
	font = NULL;
}

void LevelEditorScreen::load_content()
{
	font = al_load_ttf_font("resources/fonts/OpenSans-Regular.ttf", 30, NULL);
}

void LevelEditorScreen::unload_content()
{
	al_destroy_font(font);
	font = NULL;
}

void LevelEditorScreen::update()
{
	//TODO
}

void LevelEditorScreen::draw(ALLEGRO_DISPLAY * display)
{
	al_draw_text(font, al_map_rgb(255, 0, 0), 100, 100, NULL, "LEVEL EDITOR");
}

void LevelEditorScreen::process_event(ALLEGRO_EVENT ev)
{
	//TODO: process event
}
