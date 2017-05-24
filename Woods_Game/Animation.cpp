#include "Animation.h"
//#include "ImageLoader.h"

Rect::Rect() {

}

Rect::Rect(int x, int y, int width, int height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

int Rect::right() {
	return x + width;
}

int Rect::bottom() {
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

void Animation::load_content(std::string filename, std::pair<int, int> frame_count, std::pair<int,int> current_frame, std::pair<int, int> frame_dimensions)
{
	active = true;	//TEMP
	this->filename = filename;
	this->frame_count = frame_count;
	this->current_frame = current_frame;
	this->frame_dimensions = frame_dimensions;
	//std::cout << "loaded frame dimensions: " << frame_dimensions.first << std::endl;
}

void Animation::unload_content()
{
}

void Animation::draw(ALLEGRO_DISPLAY * display, int x_offset, int y_offest)
{
	/*
	ALLEGRO_BITMAP* bitmap = ImageLoader::get_instance().get_current_image(this);
	//TODO: check against screen space
	if (bitmap && rect.x + x_offset < DEFAULT_SCREEN_WIDTH && rect.right() + x_offset > 0 && rect.y + y_offset < DEFAULT_SCREEN_HEIGHT && rect.bottom() + y_offset > 0) {
		al_draw_bitmap(bitmap, rect.x + x_offset, rect.y + y_offset, 0);
	}
	*/
}

void Animation::update()
{
}

bool Animation::is_active()
{
	return active;
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
	//TODO
	//std::cout << "have frame dimensions: " << frame_dimensions.first << std::endl;
	return Rect(current_frame.first*frame_dimensions.first, current_frame.second*frame_dimensions.second, frame_dimensions.first, frame_dimensions.second);
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
