#ifndef LEVEL_EDITOR_SCREEN_H
#define LEVEL_EDITOR_SCREEN_H

#include "GameScreen.h"
#include "allegro5/allegro_font.h"  // for ALLEGRO_FONT
#include "allegro5/display.h"       // for ALLEGRO_DISPLAY
#include "allegro5/events.h"        // for ALLEGRO_EVENT
#include "allegro5/allegro_ttf.h"    

class LevelEditorScreen :
	public GameScreen
{
private:
	ALLEGRO_FONT * font;
public:
	LevelEditorScreen();
	~LevelEditorScreen();
	virtual void load_content();
	virtual void unload_content();
	virtual void update();
	virtual void draw(ALLEGRO_DISPLAY *display);
	virtual void process_event(ALLEGRO_EVENT ev);
};

#endif


