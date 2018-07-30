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

void LevelEditorScreen::initialize(ALLEGRO_DISPLAY *display)
{
	LevelEditorScreen::initialize_agui();
	LevelEditorScreen::initialize_widgets(gui, display);
	//TODO
}

void LevelEditorScreen::initialize_agui()
{
	//Set the image loader
	agui::Image::setImageLoader(new agui::Allegro5ImageLoader);

	//Set the font loader
	agui::Font::setFontLoader(new agui::Allegro5FontLoader);

	//Instance the input handler
	inputHandler = new agui::Allegro5Input();

	//Instance the graphics handler
	graphicsHandler = new agui::Allegro5Graphics();

	//Allegro does not automatically premultiply alpha so let Agui do it
	agui::Color::setPremultiplyAlpha(true);

	//Instance the gui
	gui = new agui::Gui();

	//Set the input handler
	gui->setInput(inputHandler);

	//Set the graphics handler
	gui->setGraphics(graphicsHandler);

	defaultFont = agui::Font::load("resources/fonts/OpenSans-Regular.ttf", 16);

	//Setting a global font is required and failure to do so will crash.
	agui::Widget::setGlobalFont(defaultFont);
}

void LevelEditorScreen::initialize_widgets(agui::Gui * guiInstance, ALLEGRO_DISPLAY *display)
{
	this->manager = new LevelEditorManager(guiInstance, display);
}


void LevelEditorScreen::unload_content()
{
	al_destroy_font(font);
	font = NULL;
}

void LevelEditorScreen::update()
{
	this->manager->update();
}

void LevelEditorScreen::process_event(ALLEGRO_EVENT ev) {
	inputHandler->processEvent(ev);
}

void LevelEditorScreen::draw(ALLEGRO_DISPLAY * display)
{
	this->manager->draw(display);
}



