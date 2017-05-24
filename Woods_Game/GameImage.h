#ifndef GAMEIMAGE_H
#define GAMEIMAGE_H
#include "SpriteSheetAnimation.h"

#define DEFAULT_SCREEN_WIDTH 800
#define DEFAULT_SCREEN_HEIGHT 600
#define TILE_SIZE 64


class ImageLoader;
//enum ATTRIBUTES{IMAGE, POSITION};
enum TYPES{PLAYER, PLATFORM, GAME_IMAGE};

class GameImage
{
protected:
	std::string image_filename;
	Rect* image_subsection = NULL;
	//Level* current_level;
	Rect rect;
	Animation *animation;
	SpriteSheetAnimation *ss_animation;
public:
	GameImage(std::string);
	GameImage();
	GameImage(int, int, int, int);
	virtual ~GameImage();
	virtual int get_type();
	std::string get_image_filename();
	virtual void load_content(std::vector<std::string>, std::vector<std::string>);
	virtual void set_content(std::string image_filename, Rect* image_subsection, std::pair<int,int> position);
	virtual void unload_content();
	virtual void draw(ALLEGRO_DISPLAY*, int, int);
	virtual void update();	//TEMP, need to figure out what arguments nee to be passed
	void set_position(int, int);
	Animation* get_animation();
	SpriteSheetAnimation* get_ss_animation();
	Rect* get_image_subsection();
	int get_x();
	int get_y();
	int get_width();
	int get_height();
	virtual bool contains_point(int, int);
	virtual bool intersects_area(Rect);
	//virtual void set_current_level(Level*);
	virtual bool outside_level(std::pair<int, int>);
};
#endif