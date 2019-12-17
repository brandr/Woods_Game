#ifndef IMAGE_FILTER_H
#define IMAGE_FILTER_H

#include "allegro5/allegro.h"

#include "GlobalTime.h"

#include "XMLSerialization.h"

class ImageFilter : public xmls::Serializable {
private:
	xmls::xInt filter_id;
	xmls::xString filename;
	xmls::xInt filter_offset_x;
	xmls::xInt filter_offset_y;
	xmls::xBool draw_on_last_layer;
	xmls::xFloat alpha_f;
	xmls::xBool is_toggled;
	xmls::xInt image_layer;
	std::pair<int, int> image_dimensions;
	int x_pos = 0, y_pos = 0;
public:
	ImageFilter();
	~ImageFilter();
	void draw(ALLEGRO_DISPLAY*, ALLEGRO_BITMAP * filter_bitmap, GlobalTime*, int, int);
	void toggle();
	const bool get_is_toggled();
	const int get_filter_id();
	const std::string get_filename();
	const int get_image_layer();
	void set_position(const int x, const int y);
	void set_image_dimensions(const int w, const int h);
};

#endif