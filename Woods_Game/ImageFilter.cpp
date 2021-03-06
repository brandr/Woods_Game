#include "ImageFilter.h"
#include "ImageLoader.h"
#include "FilterManager.h"

ImageFilter::ImageFilter()
{
	setClassName("ImageFilter");
	Register("filter_id", &filter_id);
	Register("filename", &filename);
	Register("filter_offset_x", &filter_offset_x);
	Register("filter_offset_y", &filter_offset_y);
	Register("draw_on_last_layer", &draw_on_last_layer);
	Register("alpha_f", &alpha_f);
	Register("is_toggled", &is_toggled);
	Register("image_layer", &image_layer);
}

ImageFilter::~ImageFilter()
{
}

//TODO: need to determine if this causes performance issues and fix them if so
void ImageFilter::draw(ALLEGRO_DISPLAY * display, ALLEGRO_BITMAP * target_bitmap, ALLEGRO_BITMAP * filter_bitmap,
	GlobalTime* global_time, const int x_offset, const int y_offset, const int screen_w, const int screen_h)
{
	if (target_bitmap != NULL) {
		al_set_target_bitmap(target_bitmap);
	}
	ALLEGRO_BITMAP* draw_bitmap = ImageLoader::get_instance().get_image("filters/" + this->get_filename());
	const int x = this->x_pos + this->filter_offset_x.value(), 
		y = this->y_pos + this->filter_offset_y.value();
	const int image_width = this->image_dimensions.first;
	const int image_height = this->image_dimensions.second;
	if (draw_bitmap
		&& x + x_offset < screen_w
		&& x + image_width + x_offset >= 0
		&& y + y_offset < screen_h
		&& y + image_height + y_offset >= 0) {
		const int minutes = global_time->get_current_minutes();
		ALLEGRO_COLOR time_filter_color = FilterManager::get_instance().time_light_filter_color(minutes);
		if (this->draw_on_last_layer.value()) {
			// draw on the main filter, which will be drawn last
			al_set_target_bitmap(filter_bitmap);
			al_set_blender(ALLEGRO_DEST_MINUS_SRC, ALLEGRO_CONST_COLOR, ALLEGRO_ONE);
			al_set_blend_color(time_filter_color);
			// subtract from the main filter in the shape of the filter image
			al_draw_scaled_bitmap(draw_bitmap, 0, 0,
				image_width, image_height,
				x + x_offset, y + y_offset,
				image_width, image_height,
				0);
			time_filter_color.a = time_filter_color.a * (float) this->alpha_f.value();
			al_set_blend_color(time_filter_color);
			al_set_separate_blender(ALLEGRO_ADD, ALLEGRO_CONST_COLOR, ALLEGRO_ONE, ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ONE);
			al_set_blend_color(time_filter_color);
			al_draw_tinted_bitmap(draw_bitmap, time_filter_color, x + x_offset, y + y_offset, 0);
			// go back to default blender
			al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
			if (target_bitmap != NULL) {
				al_set_target_bitmap(target_bitmap);
			} else {
				al_set_target_bitmap(al_get_backbuffer(display));
			}			
		} else {
			// draw directly to the display (other objects will render on top of this one)
			al_draw_tinted_bitmap(draw_bitmap, al_map_rgba_f(1.0, 1.0, 1.0, this->alpha_f.value()), x + x_offset, y + y_offset, 0);
			//TODO: logic for drawing shadows if different from light
		}
	}
}

void ImageFilter::toggle()
{
	this->is_toggled = !(this->is_toggled.value());
}

const bool ImageFilter::get_is_toggled()
{
	return this->is_toggled.value();
}

const int ImageFilter::get_filter_id()
{
	return this->filter_id.value();
}

const std::string ImageFilter::get_filename()
{
	return this->filename.value();
}

const int ImageFilter::get_image_layer()
{
	return this->image_layer.value();
}

void ImageFilter::set_position(const int x, const int y)
{
	this->x_pos = x, this->y_pos = y;
}

void ImageFilter::set_image_dimensions(const int w, const int h)
{
	this->image_dimensions = std::pair<int, int>(w, h);
}

