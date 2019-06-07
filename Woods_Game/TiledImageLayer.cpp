#include "TiledImageLayer.h"

TiledImageLayer::TiledImageLayer()
{
	setClassName("TiledImageLayer");
	this->Register("TiledImages", &tiled_images);
}

TiledImageLayer::~TiledImageLayer()
{
}

void TiledImageLayer::initialize_tiled_images(const std::string tileset_name)
{
	const int size = this->tiled_images.size();
	for (int i = 0; i < size; i++) {
		TiledImage *ti = this->tiled_images.getItem(i);
		if (!ti->is_empty()) {
			Rect *subsection = ti->get_bitmap_subsection();
			const std::string filename = tileset_name + "/tiled_images/" + ti->get_tiled_image_name();
			std::pair<int, int> position = ti->get_starting_pos();
			ti->set_content(filename, subsection, position);
			ti->set_bitmap(ImageLoader::get_instance().get_current_image(ti));
		}
	}
}

void TiledImageLayer::add_tiled_image(TiledImage * ti)
{
	this->tiled_images.addItem(ti);
}

bool TiledImageLayer::remove_tiled_image(const std::pair<int, int> pos)
{
	const int size = this->tiled_images.size();
	for (int i = 0; i < size; i++) {
		TiledImage *ti = this->tiled_images.getItem(i);
		if (ti != NULL && !ti->is_empty() && ti->contains_point(pos.first*TILE_SIZE, pos.second*TILE_SIZE)) {
			ti->set_empty();
			return true;
		}
	}
	return false;
}

void TiledImageLayer::draw_tiled_images(ALLEGRO_DISPLAY * display, const std::pair<int, int> offset)
{
	const int size = this->tiled_images.size();
	for (int i = 0; i < size; i++) {
		TiledImage *ti = this->tiled_images.getItem(i);
		if (!ti->is_empty()) {
			ti->draw(display, offset.first, offset.second);
		}
	}
}

void TiledImageLayer::draw_tiled_images_onto_bitmap(ALLEGRO_BITMAP * bitmap)
{
	const int size = this->tiled_images.size();
	for (int i = 0; i < size; i++) {
		TiledImage *ti = this->tiled_images.getItem(i);
		if (!ti->is_empty()) {
			ALLEGRO_BITMAP *ti_bitmap = ti->get_bitmap();
			const std::pair<int, int> pos = ti->get_starting_pos();
			al_draw_bitmap(ti_bitmap, pos.first, pos.second, 0);
		}
	}
}

void TiledImageLayer::draw_tiled_images_onto_bitmap(ALLEGRO_BITMAP * bitmap, Rect & subsection)
{
	const int size = this->tiled_images.size();
	for (int i = 0; i < size; i++) {
		TiledImage *ti = this->tiled_images.getItem(i);
		if (!ti->is_empty()) {
			ALLEGRO_BITMAP *ti_bitmap = ti->get_bitmap();
			const std::pair<int, int> pos = ti->get_starting_pos();
			const int x = pos.first - subsection.x, y = pos.second - subsection.y;
			if (x >= 0 && y >= 0
				&& x < subsection.width && y < subsection.height) {
				al_draw_bitmap(ti_bitmap, pos.first, pos.second, 0);
			}
		}
	}
}
