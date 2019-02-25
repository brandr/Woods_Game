

#include "ImageLoader.h"
#include "GameImage.h"

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
}

void ImageLoader::load_content()
{
	image_map = std::map<std::pair<std::string, std::string>, ALLEGRO_BITMAP*>();
}

void ImageLoader::unload_content()
{
	for (auto const &it : image_map) {
		al_destroy_bitmap(it.second);
	}
	image_map.clear();
	image_map.swap(std::map<std::pair<std::string, std::string>, ALLEGRO_BITMAP*>());
}

const bool ImageLoader::image_exists(const std::string filename)
{
	std::string full_filename = ImageLoader::full_filename(filename);
	return (al_load_bitmap(full_filename.c_str()));
}



void ImageLoader::load_image(std::string filename)
{
	std::string full_filename = ImageLoader::full_filename(filename);
	auto it = image_map.find(std::pair<std::string, std::string>(full_filename, ""));
	if (it != image_map.end()) return;
	al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
	ALLEGRO_BITMAP *image = al_load_bitmap(full_filename.c_str());
	
	if (image == NULL) {
		std::cout << filename << std::endl;
		std::cout << "loader failed to load image" << std::endl;
	}
	al_convert_mask_to_alpha(image, al_map_rgb(255, 0, 255));
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
	al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
	ALLEGRO_BITMAP *image = image_map[(std::pair<std::string, std::string>(full_filename, ""))];
	if (image == NULL) {
		std::cout << filename << std::endl;
		std::cout << "loader failed to load image" << std::endl;
	}
	al_convert_mask_to_alpha(image, al_map_rgb(255, 0, 255));
	ALLEGRO_BITMAP* sub_image = al_create_sub_bitmap(image, subsection.x, subsection.y, subsection.width, subsection.height);
	image_map[(std::pair<std::string, std::string>(full_filename, rect_to_string(subsection)))] = sub_image;
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
}

ALLEGRO_BITMAP * ImageLoader::get_default_tile_image(std::string tileset_name, TileType * tile_type)
{
	const std::string full_filename = ImageLoader::full_filename(tileset_name + "/tiles/" + tile_type->get_tile_sheet_key());
	Rect * subsection = DBG_NEW Rect(0, 0,
		TILE_SIZE, TILE_SIZE);
	const std::string rect_string = rect_to_string(*subsection);
	auto it = image_map.find(std::pair<std::string, std::string>(full_filename, rect_string));
	if (it == image_map.end()) {
		return NULL;
	}
	else { return it->second; }
}

ALLEGRO_BITMAP * ImageLoader::get_default_block_image(std::string sheet_filename, EntityData * block_type)
{
	const std::string full_filename = ImageLoader::full_filename(sheet_filename + "/blocks/" + block_type->get_entity_data_key());
	Rect * subsection = DBG_NEW Rect(0, 0,
		TILE_SIZE, TILE_SIZE);
	const std::string rect_string = rect_to_string(*subsection);
	auto it = image_map.find(std::pair<std::string, std::string>(full_filename, rect_string));
	if (it == image_map.end()) {
		return NULL;
	}
	else { return it->second; }
}

ALLEGRO_BITMAP * ImageLoader::get_default_entity_group_image(std::string sheet_filename, EntityGroupData * eg_type)
{
	const std::string full_filename = ImageLoader::full_filename(sheet_filename + "/entity_groups/" + eg_type->get_entity_group_name());
	Rect * subsection = DBG_NEW Rect(0, 0,
		eg_type->get_entity_group_image_dimensions().first, 
		eg_type->get_entity_group_image_dimensions().second);
	const std::string rect_string = rect_to_string(*subsection);
	auto it = image_map.find(std::pair<std::string, std::string>(full_filename, rect_string));
	if (it == image_map.end()) {
		return NULL;
	}
	else { return it->second; }
}

ALLEGRO_BITMAP * ImageLoader::get_default_tiled_image_image(std::string sheet_filename, TiledImageData *image_type)
{
	const std::string full_filename = ImageLoader::full_filename(sheet_filename + "/tiled_images/" + image_type->get_image_data_key());
	Rect * subsection = new Rect(0, 0,
		TILE_SIZE, TILE_SIZE);
	const std::string rect_string = rect_to_string(*subsection);
	auto it = image_map.find(std::pair<std::string, std::string>(full_filename, rect_string));
	if (it == image_map.end()) {
		return NULL;
	}
	else { return it->second; }
}

ALLEGRO_BITMAP * ImageLoader::get_tile_image_for_col(const std::string sheet_filename, TileType * tile_type, const int col)
{
	const std::string full_filename = ImageLoader::full_filename(sheet_filename + "/tiles/" + tile_type->get_tile_sheet_key());
	Rect * subsection = new Rect(col*TILE_SIZE, 0,
		TILE_SIZE, TILE_SIZE);
	const std::string rect_string = rect_to_string(*subsection);
	auto it = image_map.find(std::pair<std::string, std::string>(full_filename, rect_string));
	if (it == image_map.end()) {
		return NULL;
	}
	else { return it->second; }
}

ALLEGRO_BITMAP * ImageLoader::get_block_image_for_col(const std::string sheet_filename, EntityData * block_type, const int col)
{
	const std::string full_filename = ImageLoader::full_filename(sheet_filename + "/blocks/" + block_type->get_entity_data_key());
	Rect * subsection = new Rect(col*TILE_SIZE, 0,
		TILE_SIZE, TILE_SIZE);
	const std::string rect_string = rect_to_string(*subsection);
	auto it = image_map.find(std::pair<std::string, std::string>(full_filename, rect_string));
	if (it == image_map.end()) {
		return NULL;
	}
	else { return it->second; }
}

ALLEGRO_BITMAP * ImageLoader::get_entity_group_image_for_col(const std::string sheet_filename, EntityGroupData * eg_type, const int col)
{
	const std::string full_filename = ImageLoader::full_filename(sheet_filename + "/entity_groups/" + eg_type->get_entity_group_name());
	Rect * subsection = new Rect(col*eg_type->get_entity_group_image_dimensions().first, 0,
		eg_type->get_entity_group_image_dimensions().first,
		eg_type->get_entity_group_image_dimensions().second);
	const std::string rect_string = rect_to_string(*subsection);
	auto it = image_map.find(std::pair<std::string, std::string>(full_filename, rect_string));
	if (it == image_map.end()) {
		return NULL;
	}
	else { return it->second; }
}

ALLEGRO_BITMAP * ImageLoader::get_spawner_image_for_col(const std::string sheet_filename, EntityData * spawner_type, const int col)
{
	const std::string full_filename = ImageLoader::full_filename(sheet_filename + "/spawners/" + spawner_type->get_entity_data_key());
	Rect * subsection = new Rect(col*TILE_SIZE, 0,
		TILE_SIZE, TILE_SIZE);
	const std::string rect_string = rect_to_string(*subsection);
	auto it = image_map.find(std::pair<std::string, std::string>(full_filename, rect_string));
	if (it == image_map.end()) {
		return NULL;
	}
	else { return it->second; }
}

ALLEGRO_BITMAP * ImageLoader::get_full_tiled_image_sheet(std::string sheet_filename, TiledImageData *image_type)
{
	const std::string full_filename = ImageLoader::full_filename(sheet_filename + "/tiled_images/" + image_type->get_image_data_key());
	auto it = image_map.find(std::pair<std::string, std::string>(full_filename, ""));
	if (it == image_map.end()) {
		return NULL;
	}
	else { return it->second; }
}

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

