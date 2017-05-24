#include "MainGameScreen.h"

//class ScreenManager;

class TitleScreen:
	public GameScreen
{
private:
	ALLEGRO_FONT *font;
public:
	TitleScreen();
	~TitleScreen();
	virtual void load_content();
	virtual void unload_content();
	virtual void update();
	virtual void draw(ALLEGRO_DISPLAY *display);
	virtual void process_event(ALLEGRO_EVENT ev);
};

