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
	//void process_joysticks(ALLEGRO_JOYSTICK_STATE left_stick, ALLEGRO_JOYSTICK_STATE right_stick);
	void process_joystick(ALLEGRO_JOYSTICK_STATE joy_state);
	void add_screen(GameScreen *screen);
	std::list<GameImage> get_images();
};

