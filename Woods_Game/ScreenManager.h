//#include "MainGameScreen.h"
#include "TitleScreen.h"

class ScreenManager
{
private:
	ScreenManager();// = delete;
	//ScreenManager(ScreenManager const&);
	//void operator=(ScreenManager const&);
	GameScreen *current_screen;
public:
	ScreenManager(ScreenManager const&) = delete;
	void operator=(ScreenManager const&) = delete;
	//~ScreenManager();
	void load_content();
	void unload_content();
	static ScreenManager& get_instance();
	void initilaize(GameScreen*);
	void refresh();
	void update();
	void draw(ALLEGRO_DISPLAY*);
	void process_event(ALLEGRO_EVENT);
	void add_screen(GameScreen *screen);
	std::list<GameImage> get_images();
};

