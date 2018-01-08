#ifndef GAMEIMAGE_H
#define GAMEIMAGE_H
#include "PPCD.h"
#include "SpriteSheetAnimation.h"
#include "XMLSerialization.h"
#include "allegro5/bitmap.h"   // for ALLEGRO_BITMAP
#include "allegro5/display.h"  // for ALLEGRO_DISPLAY
#include "Animation.h"         // for Rect
#include "utility"             // for pair
#include "xstring"             // for string
#include <stddef.h>            // for NULL
#include "map"                 // for map
#include "vector"              // for vector
#include "xstddef"             // for less
#include <memory>              // for allocator

#define DEFAULT_SCREEN_WIDTH 800
#define DEFAULT_SCREEN_HEIGHT 600
#define TILE_SIZE 64


enum GAME_MODES{TOP_DOWN, SIDE_SCROLLING, MAIN_GAME_PAUSED, TAKING_MAPPABLE_INPUT, MAIN_GAME_INVENTORY};
enum TYPES{PLAYER, BLOCK, GAME_IMAGE, ENTITY_GROUP};
enum FACING_DIRECTIONS { DIR_NEUTRAL, DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN };
enum JOYSTICKS { LEFT_STICK = 0, RIGHT_STICK = 1 };
enum COUNTERS { BOUNCE, SWING };

//attributes
const static std::string E_ATTR_HIT_OTHER = "hit_other";
const static std::string E_ATTR_DURABILITY = "durability";
const static std::string E_ATTR_CURRENT_DURABILITY = "current_durability";
const static std::string E_ATTR_BROKEN = "broken";
const static std::string E_ATTR_CONTACT_SLOW = "contact_slow";
const static std::string E_ATTR_CONTACT_DAMAGE = "contact_damage";
const static std::string E_ATTR_KNOCKBACK = "knockback";
const static std::string E_ATTR_SHEARABLE = "shearable";

class ImageLoader;
class GameImage
{
protected:
	ALLEGRO_BITMAP *bitmap;
	std::string image_filename;
	Rect* image_subsection = NULL;
	std::vector<ALLEGRO_BITMAP*> additional_image_layers;
	Rect rect;
	std::pair<int, int> center_offset = std::pair<int,int>(0,0);
	std::map<std::string, Animation*> animations;
	SpriteSheetAnimation *ss_animation;
	int direction = 0, anim_state = 0;
	mask_t *mask;
	std::map<std::pair<std::string, int>, mask_t*> additional_masks;
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
	virtual void load_additional_masks(std::vector<std::string> attributes, std::vector<std::string> contents, std::string prefix);
	virtual void unload_content();
	virtual void draw(ALLEGRO_DISPLAY*, int, int);
	virtual void update();	//TEMP, need to figure out what arguments need to be passed
	void set_position(int, int);
	virtual void set_rect(int x, int y, int width, int height);
	virtual void set_center_offset(std::pair<int, int> offset);
	virtual const std::pair<int,int> get_center() const;
	void set_bitmap(ALLEGRO_BITMAP *bitmap);
	virtual void draw_onto_bitmap(ALLEGRO_BITMAP *bitmap);
	virtual void refresh_mask();
	mask_t* get_mask();
	std::string get_anim_state_key();
	static std::string get_anim_state_key(int key);
	virtual int get_animation_row();
	int get_animation_direction();
	Animation* get_animation();
	SpriteSheetAnimation* get_ss_animation();
	Rect* get_image_subsection();
	std::pair<float, float> get_rect_center();
	float get_x();
	float get_y();
	float get_width();
	float get_height();
	virtual bool contains_point(int, int);
	virtual bool intersects_area(Rect);
	virtual bool outside_level(std::pair<int, int>);
	static Animation *load_animation_single_row(std::string filename, int row, std::pair<int, int> frame_dimensions);
	//friend bool operator < (const GameImage& g1, const GameImage& g2);
	//friend bool operator> (const GameImage& g1, const GameImage& g2);
};


#endif