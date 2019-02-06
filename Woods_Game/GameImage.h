#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

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



class ImageLoader;
class GameImage
{
public:
	//attributes
	static constexpr const char* E_ATTR_HIT_OTHER = "hit_other";
	static constexpr const char* E_ATTR_DURABILITY = "durability";
	static constexpr const char* E_ATTR_CURRENT_DURABILITY = "current_durability";
	static constexpr const char* E_ATTR_BROKEN = "broken";
	static constexpr const char* E_ATTR_CONTACT_SLOW = "contact_slow";
	static constexpr const char* E_ATTR_CONTACT_DAMAGE = "contact_damage";
	static constexpr const char* E_ATTR_KNOCKBACK = "knockback";
	static constexpr const char* E_ATTR_SHEARABLE = "shearable";
protected:
	// serializable attributes
	xmls::xString animation_spritesheet_key;
	xmls::xInt center_offset_x;
	xmls::xInt center_offset_y;
	xmls::xInt spritesheet_frame_width;
	xmls::xInt spritesheet_frame_height;
	xmls::Collection<AnimationData> animation_data;
	xmls::Collection<MaskData> additional_mask_data;
	// attributes we use (consider getting rid of center offset and just serializing it)
	ALLEGRO_BITMAP *bitmap;
	std::string image_filename;
	Rect* image_subsection = NULL;
	std::vector<ALLEGRO_BITMAP*> additional_image_layers;
	Rect rect;
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
	virtual void load_content_from_attributes();
	virtual void set_content(std::string image_filename, Rect* image_subsection, std::pair<int,int> position);
	virtual void load_mask(std::string base_filename);
	virtual void load_additional_masks(std::vector<std::string> attributes, std::vector<std::string> contents, std::string prefix);
	virtual void load_additional_masks_from_attributes(std::string prefix);
	virtual void unload_content();
	virtual void draw(ALLEGRO_DISPLAY*, int, int);
	virtual void update();	//TEMP, need to figure out what arguments need to be passed
	void set_position(int, int);
	virtual void set_rect(int x, int y, int width, int height);
	virtual void set_center_offset(std::pair<int, int> offset);
	virtual const std::pair<int,int> get_center();
	virtual ALLEGRO_BITMAP* get_bitmap();
	void set_bitmap(ALLEGRO_BITMAP *bitmap);
	virtual void draw_onto_bitmap(ALLEGRO_BITMAP *bitmap);
	virtual void refresh_bitmap();
	virtual void refresh_mask();
	mask_t* get_mask();
	std::string get_anim_state_key();
	static std::string get_anim_state_key(int key);
	virtual int get_animation_row();
	int get_animation_direction();
	Animation* get_animation();
	SpriteSheetAnimation* get_ss_animation();
	Rect* get_image_subsection();
	std::vector<ALLEGRO_BITMAP*> get_additional_image_layers();
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