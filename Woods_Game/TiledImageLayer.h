#ifndef TILED_IMAGE_LAYER_H
#define TILED_IMAGE_LAYER_H

#include "XMLSerialization.h"
#include "TiledImage.h"
#include "allegro5/display.h"
#include "ImageLoader.h"

class TiledImageLayer :
	public xmls::Serializable
{
private:
	xmls::Collection<TiledImage> tiled_images;
public:
	TiledImageLayer();
	~TiledImageLayer();
	void initialize_tiled_images(const std::string filename_start);
	void add_tiled_image(TiledImage * ti);
	bool remove_tiled_image(const std::pair<int, int> pos);
	void draw_tiled_images(ALLEGRO_DISPLAY * display, const std::pair<int, int> offset, const int screen_w, const int screen_h);
	void draw_tiled_images_onto_bitmap(ALLEGRO_BITMAP *bitmap, Rect &subsection);
};

#endif

