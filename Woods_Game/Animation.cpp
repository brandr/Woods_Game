#include "Animation.h"
//#include "ImageLoader.h"

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

void Animation::load_content(std::string filename, std::pair<int, int> frame_count, std::pair<int,int> current_frame, std::pair<int, int> frame_dimensions, int frame_duration)
{
	active = true;	//TEMP
	this->filename = filename;
	this->frame_count = frame_count;
	this->current_frame = current_frame;
	this->frame_dimensions = frame_dimensions;
	this->frame_duration = frame_duration;
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
	//TODO: do this instead if I can figure out includes
	 //std::pair<int, int> fd();	
	 //ALLEGRO_BITMAP *image = ImageLoader::get_instance().get_image(filename);
	 //return std::pair<int, int>(al_get_bitmap_width(image) / frame_count.first, al_get_bitmap_height(image) / frame_count.second);
}

std::pair<int, int> Animation::get_image_dimensions()
{
	return std::pair<int, int>(frame_dimensions.first*frame_count.first, frame_dimensions.second*frame_count.second);
}

int Animation::get_frame_duration()
{
	return frame_duration;
}

AnimationData::AnimationData()
{
	setClassName("AnimationData");
	Register("animation_key", &animation_key);
	Register("animation_frame_width", &animation_frame_width);
	Register("animation_frame_height", &animation_frame_height);
	Register("animation_frame_duration", &animation_frame_duration);
}

MaskData::MaskData()
{
	setClassName("MaskData");
	Register("mask_key", &mask_key);
	Register("mask_width", &mask_width);
	Register("mask_height", &mask_height);
	Register("mask_frame_count", &mask_frame_count);
}
