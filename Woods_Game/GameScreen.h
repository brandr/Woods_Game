#ifndef GAMESCREEN_H
#define GAMESCREEN_H
#include <list>
#include<allegro5/allegro_primitives.h>
#include<allegro5/allegro_ttf.h>
#include<allegro5/allegro_font.h>
#include "ImageLoader.h"
#include "InputManager.h"

enum SCREEN_FLAGS {NONE, CHANGE_TO_MAIN_GAME, CHANGE_TO_TITLE};

class GameScreen
{
//do this so we can call key press methods
typedef void(*controlFunc)(GameScreen&, int, bool);
protected:
	InputManager input;
	std::vector<std::vector<int>> attributes;
	std::vector<std::vector<std::string>> contents;
	std::map<int, controlFunc> control_map;	//TODO: method
	std::map<int, bool> input_map;
	int screen_flag = NONE;
public:
	GameScreen();
	~GameScreen();
	virtual void load_content();
	virtual void unload_content();
	virtual void update();
	virtual void draw(ALLEGRO_DISPLAY *display);
	virtual void set_input_map();
	virtual void set_input(int, bool);
	virtual std::list<GameImage> get_images();
	virtual int get_camera_offset_x();
	virtual int get_camera_offset_y();
	virtual void load_images(ImageLoader&);
	virtual void refresh();
	virtual void process_event(ALLEGRO_EVENT ev);
	virtual void set_screen_flag(int);
	virtual int get_screen_flag();
};

#endif