#ifndef GAMESCREEN_H
#define GAMESCREEN_H
#include <list>
#include<allegro5/allegro_primitives.h>
#include<allegro5/allegro_ttf.h>
#include<allegro5/allegro_font.h>
#include "ImageLoader.h"
#include "InputManager.h"

enum SCREEN_FLAGS { NONE, CHANGE_TO_MAIN_GAME, CHANGE_TO_TITLE };

class GameScreen
{
//do this so we can call key press methods
//t
typedef void(*controlFunc)(GameScreen&, ALLEGRO_EVENT, bool);
protected:
	InputManager input;
	std::vector<std::vector<int>> attributes;
	std::vector<std::vector<std::string>> contents;
	std::map<std::pair<int,int>, controlFunc> control_map;	
	std::map<int, bool> input_map;
	//std::map<int, ALLEGRO_JOYSTICK*> joystick_map;
	std::map<int, std::pair<float, float>> joystick_pos_map;
	//bool joystick_flag = false;
	//std::pair<float, float> joystick_pos; //TODO: make this handle mulitple joysticks if necessary, or distinguish between them
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
	virtual void clear_input();
	virtual void set_joystick_pos(int stick, float x, float y);
	virtual std::pair<float, float> get_joystick_pos(int stick);
	virtual std::map<int, std::pair<float, float>> get_joystick_pos_map();
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