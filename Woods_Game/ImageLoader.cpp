

#include "ImageLoader.h"
#include "GameImage.h"

std::string ImageLoader::rect_to_string(Rect r)
{
	return "" + std::to_string(r.x) + "," + std::to_string(r.y) + "," + std::to_string(r.width) + "," + std::to_string(r.height);
}

const std::string ImageLoader::full_image_key(const std::string image_key, const int width, const int height, const std::string suffix)
{
	return image_key + "_" + "DIM-" + std::to_string(width) + "." + std::to_string(height) + "." + "_" + suffix;
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
	const std::string full_filename = ImageLoader::full_filename(filename);
	return boost::filesystem::exists(full_filename.c_str());
}

const bool ImageLoader::keyed_image_exists(const std::string image_key, const int width, const int height, const std::string suffix)
{
	const std::string full_key = this->full_image_key(image_key, width, height, suffix);
	return this->keyed_image_exists(full_key);
}

const bool ImageLoader::keyed_image_exists(const std::string key)
{
	auto it = image_map.find(std::pair<std::string, std::string>(key, ""));
	return (it != image_map.end());
}

void ImageLoader::set_keyed_image(ALLEGRO_BITMAP * image, const std::string image_key, const int width, const int height, const std::string suffix)
{
	const std::string full_key = this->full_image_key(image_key, width, height, suffix);
	image_map[std::pair<std::string, std::string>(full_key, "")] = image;
}

ALLEGRO_BITMAP * ImageLoader::get_keyed_image(const std::string image_key, const int width, const int height, const std::string suffix)
{
	const std::string full_key = this->full_image_key(image_key, width, height, suffix);
	if (this->keyed_image_exists(full_key)) {
		return image_map[std::pair<std::string, std::string>(full_key, "")];
	}
	return NULL;
}

void ImageLoader::load_image(std::string filename)
{
	ImageLoader::get_instance().load_image(filename, "", false);
}


void ImageLoader::load_image(std::string filename, Rect subsection)
{
	std::string full_filename = ImageLoader::full_filename(filename);
	const std::string rect_string = rect_to_string(subsection);
	ImageLoader::get_instance().load_image(filename, rect_string, false);
}

// only load the part of the image encompassed by the rect
void ImageLoader::load_image(const std::string filename, const std::string rect_string, const bool allow_failure)
{
	// if the full image is not already loaded in, we load it here.
	const std::string full_filename = ImageLoader::full_filename(filename);
	if (image_map.find(std::pair<std::string, std::string>(full_filename, rect_string)) == image_map.end()
		&& rect_string != ""
		&& (image_map.find(std::pair<std::string, std::string>(full_filename, "")) == image_map.end())) {
		load_image(filename, "", allow_failure);
	} else {
		// don't do anything if we have already lodaed this subsection
		auto it = image_map.find(std::pair<std::string, std::string>(full_filename, rect_string));
		if (it != image_map.end()) {
			return;
		}
	}
	ALLEGRO_BITMAP *image = al_load_bitmap(full_filename.c_str());
	if (image == NULL) {
		if (allow_failure) {
			return;
		} else {
			std::cout << filename << std::endl;
			std::cout << "loader failed to load image" << std::endl;
			//TODO: better error handling
		}
	}
	al_convert_mask_to_alpha(image, al_map_rgb(255, 0, 255));
	FileManager fm;
	std::vector<std::string> rect_parts;
	if (rect_string.length() > 0) {
		rect_parts = fm.string_to_parts(rect_string, ",");
		Rect subsection(::atoi(rect_parts[0].c_str()), 
			::atoi(rect_parts[1].c_str()), 
			::atoi(rect_parts[2].c_str()),
			::atoi(rect_parts[3].c_str()));
		ALLEGRO_BITMAP * sub_image = al_create_sub_bitmap(image, subsection.x, subsection.y, subsection.width, subsection.height);
		image_map[(std::pair<std::string, std::string>(full_filename, rect_string))] = sub_image;
	} else {
		image_map[(std::pair<std::string, std::string>(full_filename, ""))] = image;
	}
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

void ImageLoader::load_mask(const std::string base_filename)
{
	load_mask(base_filename, "", true);
}

void ImageLoader::load_mask(const std::string base_filename, const std::string rect_string)
{
	load_mask(base_filename, rect_string, true);
}

void ImageLoader::load_mask(const std::string base_filename, const std::string rect_string, const bool include_suffix)
{
	std::string mask_filename = base_filename;
	if (include_suffix) {
		mask_filename += "_mask";
	}
	auto it = mask_map.find(std::pair<std::string, std::string>(base_filename, rect_string));
	if (it != mask_map.end()) {
		return;
	}
	mask_t * loaded_mask = NULL;
	ImageLoader::get_instance().load_image(mask_filename);
	ALLEGRO_BITMAP * mask_image = NULL;
	if (rect_string.length() > 0) {
		FileManager fm;
		const std::vector<std::string> rect_parts = fm.string_to_parts(rect_string, ",");
		Rect subsection(::atoi(rect_parts[0].c_str()),
			::atoi(rect_parts[1].c_str()),
			::atoi(rect_parts[2].c_str()),
			::atoi(rect_parts[3].c_str()));
		ImageLoader::get_instance().load_image(mask_filename, subsection);
		mask_image = ImageLoader::get_instance().get_image(mask_filename, subsection);
	} else {
		mask_image = ImageLoader::get_instance().get_image(mask_filename);
	}
	if (mask_image != NULL) {
		al_convert_mask_to_alpha(mask_image, al_map_rgb(255, 0, 255));
		loaded_mask = Mask_New(mask_image);
	}
	//store base filename, not mask filename, because we will never have both on the same image anyway
	mask_map[std::pair<std::string, std::string>(base_filename, rect_string)] = loaded_mask;
}

// TODO: refactor all the repeated code below

ALLEGRO_BITMAP * ImageLoader::get_default_tile_image(std::string tileset_name, TileType * tile_type)
{
	const std::string full_filename = ImageLoader::full_filename(tileset_name + "/tiles/" + tile_type->get_tile_sheet_key());
	Rect * subsection = new Rect(0, 0,
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
	Rect * subsection = new Rect(0, 0,
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
	Rect * subsection = new Rect(0, 0,
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
	} else { 
		return it->second; 
	}
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
	} else { 
		return it->second;
	}
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
	} else { 
		return it->second;
	}
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
	} else { 
		return it->second;
	}
}

ALLEGRO_BITMAP * ImageLoader::get_full_tiled_image_sheet(std::string sheet_filename, TiledImageData *image_type)
{
	const std::string full_filename = ImageLoader::full_filename(sheet_filename + "/tiled_images/" + image_type->get_image_data_key());
	auto it = image_map.find(std::pair<std::string, std::string>(full_filename, ""));
	if (it == image_map.end()) {
		return NULL;
	} else { 
		return it->second; 
	}
}

ALLEGRO_BITMAP * ImageLoader::get_current_image(GameImage* image)
{
	Animation *anim = image->get_animation();
	SpriteSheetAnimation *ss_anim = image->get_ss_animation();
	if (anim && ss_anim) {
		return get_image(anim->get_filename(), anim->get_current_rect());
	}
	const std::string full_filename = ImageLoader::full_filename(image->get_image_filename());
	Rect* subsection = image->get_image_subsection();
	const std::string rect_string = subsection ? rect_to_string(*subsection) : "";
	auto it = image_map.find(std::pair<std::string, std::string>(full_filename, rect_string));
	if (it == image_map.end()) {
		return NULL;
	} else {
		return it->second;
	}
}

ALLEGRO_BITMAP * ImageLoader::get_image(std::string filename)
{
	const std::string full_filename = ImageLoader::full_filename(filename);
	auto it = image_map.find(std::pair<std::string, std::string>(full_filename, ""));
	if (it == image_map.end()) {
		return NULL;
	}
	else return it->second;
}

ALLEGRO_BITMAP * ImageLoader::get_image(std::string filename, Rect subsection)
{
	const std::string full_filename = ImageLoader::full_filename(filename);
	const std::string rect_string = rect_to_string(subsection);
	auto it = image_map.find(std::pair<std::string, std::string>(full_filename, rect_string));
	if (it == image_map.end()) return nullptr;
	else return it->second;
}

mask_t * ImageLoader::get_mask(const std::string base_filename)
{
	return get_mask(base_filename, "");
}

mask_t * ImageLoader::get_mask(const std::string base_filename, const std::string rect_string)
{
	const std::string mask_filename = base_filename;
	auto it = mask_map.find(std::pair<std::string, std::string>(mask_filename, rect_string));
	if (it == mask_map.end()) {
		return NULL;
	} else {
		return it->second;
	}
}

