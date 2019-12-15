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


enum GAME_MODES{
	TOP_DOWN, 
	SIDE_SCROLLING, 
	MAIN_GAME_PAUSED, 
	TAKING_MAPPABLE_INPUT, 
	MAIN_GAME_INVENTORY, 
	MAIN_GAME_DIALOG, 
	CUTSCENE,
	CALENDAR,
	TITLE_SCREEN,
	EXCHANGE_INVENTORY
};

enum TYPES{PLAYER, BLOCK, GAME_IMAGE, ENTITY_GROUP, ITEM_PICKUP, CRITTER};
enum FACING_DIRECTIONS { DIR_NEUTRAL, DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN };
enum JOYSTICKS { LEFT_STICK = 0, RIGHT_STICK = 1 };
enum COUNTERS { BOUNCE, SWING, COUNTER_PLANT_DAY_UPDATE };

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
	static constexpr const char* E_ATTR_PLANT_GATHER_ITEM_KEY = "plant_gather_item_key";
	static constexpr const char* E_ATTR_PENDING_LIGHT_FILTER = "pending_light_filter";
		// plant growth attributes
	static constexpr const char* E_ATTR_PLANT_GROWTH_CURRENT_AGE = "plant_growth_current_age";			// plant's current age in days
	static constexpr const char* E_ATTR_PLANT_GROWTH_MATURE_AGE = "plant_growth_mature_age";			// age before plant uses its "mature" sprite and can spread
	static constexpr const char* E_ATTR_PLANT_GROWTH_NUM_STAGES = "plant_growth_num_stages";			// number of stages before maturity. used to determine what sprite to show
	static constexpr const char* E_ATTR_PLANT_GROWTH_SPREAD_RATE = "plant_growth_spread_rate";			// number from 0 to 100 to tell us how likely the plant is to spread
	static constexpr const char* E_ATTR_PLANT_GROWTH_SPREAD_RANGE = "plant_growth_spread_range";		// number of tiles away the plant can spread
	static constexpr const char* E_ATTR_PLANT_GROWTH_SPREAD_CROWD_AVERSION = "plant_growth_spread_crowd_aversion"; // number from 1 to 100 for how much the plant hates spreading near other plants
	
protected:
	// serializable attributes
	xmls::xString animation_spritesheet_key;
	xmls::xInt center_offset_x;
	xmls::xInt center_offset_y;
	xmls::xInt collide_x_offset;
	xmls::xInt collide_y_offset;
	xmls::xInt collide_width;
	xmls::xInt collide_height;
	xmls::xInt spritesheet_frame_width;
	xmls::xInt spritesheet_frame_height;
	xmls::Collection<AnimationData> animation_data;
	xmls::Collection<MaskData> additional_mask_data;
	// attributes we use (consider getting rid of center offset and just serializing it)
	std::string image_filename;
	std::string mask_filename;
	Rect* image_subsection = NULL;
	std::vector<std::pair<std::string, Rect>> additional_image_layer_data;
	Rect rect;
	Rect collide_rect;
	std::map<std::string, Animation*> animations;
	SpriteSheetAnimation *ss_animation;
	int direction = 0, anim_state = 0;
	virtual const int calculate_direction(GameImage * other);
	virtual const bool should_adjust_anim_duration();
	virtual const float speed_for_anim_duration();
	virtual const bool should_update_animation();
public:
	GameImage(std::string);
	GameImage();
	GameImage(float, float, float, float);
	virtual ~GameImage();
	virtual int get_type();
	std::string get_image_filename();
	virtual const std::string image_filename_suffix();
	virtual void load_content_from_attributes();
	virtual void set_content(std::string image_filename, Rect* image_subsection, std::pair<int,int> position);
	virtual void load_mask(std::string base_filename);
	virtual void load_additional_masks_from_attributes(std::string prefix);
	virtual mask_t* get_additional_mask(const std::string mask_key, const std::string prefix, const int row, const int col);
	virtual void unload_content();
	virtual void draw(ALLEGRO_DISPLAY*, int, int);
	virtual void update();
	void set_position(int, int);
	virtual void set_rect(int x, int y, int width, int height);
	virtual void set_center_offset(std::pair<int, int> offset);
	virtual const std::pair<int,int> get_center();
	virtual void add_additional_image_layer(const std::string filename, Rect subsection);
	virtual void refresh_mask();
	mask_t* get_mask();
	std::string get_anim_state_key();
	static std::string get_anim_state_key(int key);
	static const int get_anim_state_index(const std::string key);
	virtual int get_animation_row();
	int get_animation_direction();
	void set_animation_index(const int index);
	void set_animation_direction(const int dir);
	virtual Animation* get_animation();
	SpriteSheetAnimation* get_ss_animation();
	Rect* get_image_subsection();
	Rect * get_collide_rect();
	Rect * get_rect_for_collision();
	std::vector<ALLEGRO_BITMAP*> get_additional_image_layers();
	std::pair<float, float> get_rect_center();
	float get_x();
	float get_y();
	float get_width();
	float get_height();
	virtual bool contains_point(int, int);
	virtual const bool intersects_area(Rect);
	virtual bool outside_level(std::pair<int, int>);
	static Animation *load_animation_single_row(std::string filename, int row, std::pair<int, int> frame_dimensions);
};


#endif