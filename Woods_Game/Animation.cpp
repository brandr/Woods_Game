#include "Animation.h"

Rect::Rect() {

}

Rect::Rect(float x, float y, float width, float height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

float Rect::right() {
	return x + width;
}

float Rect::bottom() {
	return y + height;
}


bool Rect::intersects_rect(Rect other)
{
	if (x >= other.x + other.width || y >= other.y + other.height) return false;
	if (x + width <= other.x || y + height <= other.y) return false;
	return true;
}

// animation

Animation::Animation()
{
}


Animation::~Animation()
{
}

void Animation::load_content(std::string filename, std::pair<int, int> frame_count, std::pair<int,int> current_frame, std::pair<int, int> frame_dimensions, 
	const int frame_duration, const int animation_pixel_step_size, const float anim_min_speed, const float anim_max_speed)
{
	active = true;
	this->filename = filename;
	this->frame_count = frame_count;
	this->current_frame = current_frame;
	this->frame_dimensions = frame_dimensions;
	this->frame_duration = frame_duration;
	this->pixel_step_size = animation_pixel_step_size;
	this->min_speed = anim_min_speed;
	this->max_speed = anim_max_speed;
}

void Animation::unload_content()
{
}

void Animation::draw(ALLEGRO_DISPLAY * display, int x_offset, int y_offest)
{
}

void Animation::update()
{
}

bool Animation::is_active()
{
	return active;
}

void Animation::reset()
{
	get_current_frame().first = 0;
}

void Animation::set_filename(std::string filename)
{
	this->filename = filename;
}

std::string Animation::get_filename()
{
	return filename;
}

Rect Animation::get_current_rect()
{
	return Rect(current_frame.first*frame_dimensions.first, current_frame.second*frame_dimensions.second, frame_dimensions.first, frame_dimensions.second);
}

void Animation::set_row(int row)
{
	if (row != get_current_frame().second) {
		get_current_frame().first = 0;
		get_current_frame().second = row;
	}
}

std::pair<int, int>& Animation::get_frame_count()
{
	return frame_count;
}

std::pair<int, int>& Animation::get_current_frame()
{
	return current_frame;
}

std::pair<int, int>& Animation::get_frame_dimensions()
{
	return frame_dimensions;
}

std::pair<int, int> Animation::get_image_dimensions()
{
	return std::pair<int, int>(frame_dimensions.first*frame_count.first, frame_dimensions.second*frame_count.second);
}

int Animation::get_frame_duration()
{
	return frame_duration;
}

const int Animation::get_pixel_step_size()
{
	return this->pixel_step_size;
}

const float Animation::get_min_speed()
{
	return this->min_speed;
}

const float Animation::get_max_speed()
{
	return this->max_speed;
}

const bool Animation::speed_qualifies(const float speed)
{
	return (this->min_speed < 0 || this->max_speed < 0) || (speed >= this->min_speed && speed < this->max_speed);
}

AnimationData::AnimationData()
{
	setClassName("AnimationData");
	Register("animation_key", &animation_key);
	Register("animation_frame_width", &animation_frame_width);
	Register("animation_frame_height", &animation_frame_height);
	Register("animation_frame_duration", &animation_frame_duration);
	Register("animation_pixel_step_size", &animation_pixel_step_size);
	Register("animation_min_speed", &animation_min_speed);
	Register("animation_max_speed", &animation_max_speed);
}

MaskData::MaskData()
{
	setClassName("MaskData");
	Register("mask_key", &mask_key);
	Register("mask_width", &mask_width);
	Register("mask_height", &mask_height);
	Register("mask_frame_count", &mask_frame_count);
}
