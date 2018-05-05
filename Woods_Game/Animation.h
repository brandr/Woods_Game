#ifndef ANIMATION_H
#define ANIMATION_H
#include "allegro5/display.h"  // for ALLEGRO_DISPLAY
#include "utility"             // for pair
#include "xstring"             // for string
#include "XMLSerialization.h"

enum ANIMATION_DIRECTIONS{ANIM_NEUTRAL = 0, ANIM_RIGHT = 1, ANIM_LEFT = 2, ANIM_UP = 3, ANIM_DOWN = 4};
enum ANIMATION_STATES { ANIM_STATE_NEUTRAL = 0, ANIM_STATE_WALKING = 1, ANIM_STATE_SHEARING = 2};
enum AXES { HORIZONTAL, VERTICAL };

static const int DEFAULT_EFFECT_FRAME_DURATION = 4;
//const static std::string EFFECT_BREAK = "break";
//std::vector<std::string> ENTITY_EFFECT_NAMES{ EFFECT_BREAK };

struct Rect {
	float x, y, width, height;
public:
	Rect();
	Rect(float, float, float, float);
	bool intersects_rect(Rect);
	float right();
	float bottom();	
};

struct MaskData : xmls::Serializable {
	xmls::xString mask_key;
	xmls::xInt mask_width;
	xmls::xInt mask_height;
	xmls::xInt mask_frame_count;
	MaskData();
};

struct AnimationData : xmls::Serializable {
	xmls::xString animation_key;
	xmls::xInt animation_frame_width;
	xmls::xInt animation_frame_height;
	xmls::xInt animation_frame_duration;
	AnimationData();
};

class Animation
{
public:
	Animation();
	~Animation();
	void load_content(std::string, std::pair<int,int>, std::pair<int, int>, std::pair<int, int>, int);
	void unload_content();
	void draw(ALLEGRO_DISPLAY*, int, int);
	virtual void update();
	bool is_active();
	void reset();
	void set_filename(std::string);
	std::string get_filename();
	Rect get_current_rect();
	void set_row(int row);
	std::pair<int, int> &get_frame_count();
	std::pair<int, int> &get_current_frame();
	std::pair<int, int> &get_frame_dimensions();
	std::pair<int, int> get_image_dimensions();
	int get_frame_duration();
protected:
	//TODO: necessary data to to get the image via imageloader
	float alpha;
	bool active;
	std::string filename;
	std::pair<int, int> frame_count;
	std::pair<int, int> current_frame;
	std::pair<int, int> frame_dimensions;
	int frame_duration = 1;
};

#endif