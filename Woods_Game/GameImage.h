#ifndef GAMEIMAGE_H
#define GAMEIMAGE_H
#include "SpriteSheetAnimation.h"
#include <map>
#include "PPCD.h"

#define DEFAULT_SCREEN_WIDTH 800
#define DEFAULT_SCREEN_HEIGHT 600
#define TILE_SIZE 64


class ImageLoader;
//enum ATTRIBUTES{IMAGE, POSITION};
enum GAME_MODES{TOP_DOWN, SIDE_SCROLLING};
enum TYPES{PLAYER, BLOCK, GAME_IMAGE, ENTITY_GROUP};
enum JOYSTICKS { LEFT_STICK = 0, RIGHT_STICK = 1 };

class GameImage
{
protected:
	ALLEGRO_BITMAP *bitmap;
	std::string image_filename;
	Rect* image_subsection = NULL;
	//Level* current_level;
	Rect rect;
	//Animation *animation;
	std::map<std::string, Animation*> animations;
	SpriteSheetAnimation *ss_animation;
	int direction = 0, anim_state = 0;
	//std::map<std::pair<int, int>, int> animation_dir_map;
	mask_t *mask;

	//temp
	//ALLEGRO_BITMAP *temp_mask_image;
	//temp
public:
	GameImage(std::string);
	GameImage();
	GameImage(float, float, float, float);
	virtual ~GameImage();
	virtual int get_type();
	std::string get_image_filename();
	virtual void load_content(std::vector<std::string>, std::vector<std::string>);
	virtual void set_content(std::string image_filename, Rect* image_subsection, std::pair<int,int> position);
	virtual void load_mask(std::string base_filename);
	virtual void unload_content();
	virtual void draw(ALLEGRO_DISPLAY*, int, int);
	virtual void update();	//TEMP, need to figure out what arguments nee to be passed
	void set_position(int, int);
	virtual void set_rect(int x, int y, int width, int height);
	void set_bitmap(ALLEGRO_BITMAP *bitmap);
	virtual void refresh_mask();
	//virtual int get_animation_index();
	mask_t* get_mask();
	std::string get_anim_state_key();
	int get_animation_direction();
	Animation* get_animation();
	SpriteSheetAnimation* get_ss_animation();
	Rect* get_image_subsection();

	float get_x();
	float get_y();
	float get_width();
	float get_height();
	virtual bool contains_point(int, int);
	virtual bool intersects_area(Rect);
	//virtual void set_current_level(Level*);
	virtual bool outside_level(std::pair<int, int>);
};
#endif