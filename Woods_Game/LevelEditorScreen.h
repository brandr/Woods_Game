#ifndef LEVEL_EDITOR_SCREEN_H
#define LEVEL_EDITOR_SCREEN_H

#include "GameScreen.h"
#include "LevelEditorManager.h"

class LevelEditorScreen :
	public GameScreen
{
private:
	ALLEGRO_FONT * font;
	LevelEditorManager * manager;
public:
	agui::Gui *gui;
	agui::Allegro5Input *inputHandler;
	agui::Allegro5Graphics *graphicsHandler;
	agui::Font *defaultFont;
	LevelEditorScreen();
	~LevelEditorScreen();
	virtual void load_content();
	virtual void unload_content();
	virtual void update();
	virtual void draw(ALLEGRO_DISPLAY *display);
	virtual void process_event(ALLEGRO_EVENT ev);

	void initialize();
	void initialize_agui();
	void initialize_widgets(agui::Gui *guiInstance);
};

#endif