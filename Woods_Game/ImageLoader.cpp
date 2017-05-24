#include "ImageLoader.h"
#include <iostream>


/*
ImageLoader::ImageLoader()
{
	//image_map = std::map<std::pair<std::string, std::string>, ALLEGRO_BITMAP*>();
}
*/
std::string ImageLoader::rect_to_string(Rect r)
{
	return "" + std::to_string(r.x) + "," + std::to_string(r.y) + "," + std::to_string(r.width) + "," + std::to_string(r.height);
}


ImageLoader &ImageLoader::get_instance()
{
	static ImageLoader loader;
	return loader;
}

std::string ImageLoader::full_filename(std::string filename)
{
	return "resources/images/" + filename + ".png";
}

ImageLoader::~ImageLoader()
{
	unload_content();
	//image_map.clear();
	//image_map.swap(std::map<std::pair<std::string, std::string>, ALLEGRO_BITMAP*>());
}

void ImageLoader::load_content()
{
	//image_map = std::map<std::pair<std::string, std::string>, std::shared_ptr<ALLEGRO_BITMAP>>();
	image_map = std::map<std::pair<std::string, std::string>, ALLEGRO_BITMAP*>();
}

void ImageLoader::unload_content()
{
	for (auto const &it : image_map) {
		al_destroy_bitmap(it.second);
		//al_destroy_bitmap(it.second.get());
//		delete it.second;
	}
	image_map.clear();
	image_map.swap(std::map<std::pair<std::string, std::string>, ALLEGRO_BITMAP*>());

	/*
	image_map.swap(std::map<std::pair<std::string, std::string>, ALLEGRO_BITMAP*>());
	image_map.swap(std::map<std::pair<std::string, std::string>, ALLEGRO_BITMAP*>());
	image_map.swap(std::map<std::pair<std::string, std::string>, ALLEGRO_BITMAP*>());
	image_map.swap(std::map<std::pair<std::string, std::string>, ALLEGRO_BITMAP*>());
	image_map.swap(std::map<std::pair<std::string, std::string>, ALLEGRO_BITMAP*>());
	*/
}



void ImageLoader::load_image(std::string filename)
{
	std::string full_filename = ImageLoader::full_filename(filename);
	auto it = image_map.find(std::pair<std::string, std::string>(full_filename, ""));
	if (it != image_map.end()) return;
	//std::shared_ptr<ALLEGRO_BITMAP> image(al_load_bitmap(full_filename.c_str()), al_destroy_bitmap);
	ALLEGRO_BITMAP *image = al_load_bitmap(full_filename.c_str());	//temp path- change in exe
	
	if (image == NULL) {
		//TODO: error handling
		std::cout << filename << std::endl;
		std::cout << "loader failed to load image" << std::endl;
	}
	//image_map[(std::pair<std::string, std::string>(full_filename, ""))] = std::move(image);
	image_map[(std::pair<std::string, std::string>(full_filename, ""))] = image;
}

// only load the part of the image encompassed by the rect
void ImageLoader::load_image(std::string filename, Rect subsection)
{
	// in order to save time, we check to see if the full image is already loaded in. if not, we load it.
	load_image(filename);
	std::string full_filename = ImageLoader::full_filename(filename);
	auto it = image_map.find(std::pair<std::string, std::string>(full_filename, rect_to_string(subsection)));
	if (it != image_map.end()) return;
	ALLEGRO_BITMAP *image = image_map[(std::pair<std::string, std::string>(full_filename, ""))];
	//std::shared_ptr<ALLEGRO_BITMAP> image(al_load_bitmap(full_filename.c_str()), al_destroy_bitmap);
	if (image == NULL) {
		//TODO: error handling
		std::cout << filename << std::endl;
		std::cout << "loader failed to load image" << std::endl;
	}
	ALLEGRO_BITMAP* sub_image = al_create_sub_bitmap(image, subsection.x, subsection.y, subsection.width, subsection.height);
	//std::shared_ptr<ALLEGRO_BITMAP> sub_image(al_create_sub_bitmap(image.get(), subsection.x, subsection.y, subsection.width, subsection.height), al_destroy_bitmap);
	//std::unique_ptr<ALLEGRO_BITMAP> sub_image(al_create_sub_bitmap(image.get(), subsection.x, subsection.y, subsection.width, subsection.height));
	image_map[(std::pair<std::string, std::string>(full_filename, rect_to_string(subsection)))] = sub_image;

	//image_map[(std::pair<std::string, std::string>(full_filename, ""))] = std::move(sub_image);
	//image.release();
}

void ImageLoader::load_spritesheet(Animation anim)
{
	std::string filename = anim.get_filename();
	load_image(filename);
	std::pair<int, int> frame_count = anim.get_frame_count();
	std::pair<int, int> frame_dim = anim.get_frame_dimensions();
	for (int y = 0 ; y < frame_count.second; y++) {
		for (int x = 0; x < frame_count.first; x++) {
			Rect rect(x*frame_dim.first, y*frame_dim.second, frame_dim.first, frame_dim.second);
  			load_image(filename, rect);
		}
	}
	std::cout << std::endl;
}

//TODO: modify this to allow for spritesheets
ALLEGRO_BITMAP * ImageLoader::get_current_image(GameImage* image)
{
	Animation *anim = image->get_animation();
	SpriteSheetAnimation *ss_anim = image->get_ss_animation();
	if (anim && ss_anim) {
		return get_image(anim->get_filename(), anim->get_current_rect());
	}
	std::string full_filename = ImageLoader::full_filename(image->get_image_filename());
	Rect* subsection = image->get_image_subsection();
	std::string rect_string = "";
	if (subsection) rect_string = rect_to_string(*subsection);
	auto it = image_map.find(std::pair<std::string, std::string>(full_filename, rect_string));
	if (it == image_map.end()) {
		//std::cout << "failed to find image with filename: " + full_filename + " and subsection: " + rect_string << std::endl;
		return nullptr;
	}
	else return it->second;
}

ALLEGRO_BITMAP * ImageLoader::get_image(std::string filename)
{
	std::string full_filename = ImageLoader::full_filename(filename);
	auto it = image_map.find(std::pair<std::string, std::string>(full_filename, ""));
	if (it == image_map.end()) return nullptr;
	else return it->second;
}

ALLEGRO_BITMAP * ImageLoader::get_image(std::string filename, Rect subsection)
{
	std::string full_filename = ImageLoader::full_filename(filename);
	std::string rect_string = rect_to_string(subsection);
	auto it = image_map.find(std::pair<std::string, std::string>(full_filename, rect_string));
	if (it == image_map.end()) return nullptr;
	else return it->second;
}

