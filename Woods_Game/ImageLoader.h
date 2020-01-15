#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <map>
#include "PPCD.h"
#include "ImageLoader.h"
#include "TileType.h"
#include "Entity.h"
#include "TiledImage.h"
#include <string>
#include <iostream>
#include "xstring"  
#include "Animation.h"
#include "allegro5/allegro5.h"
#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp>

#define IMAGE_PREFIX_COLOR "color"

#define IMAGE_KEY_FILLED_RECT "filled_rect"

class GameImage;
class ImageLoader
{
private:
	ImageLoader() {}
	std::map<std::pair<std::string, std::string>, ALLEGRO_BITMAP*> image_map;
	std::map<std::pair<std::string, std::string>, mask_t*> mask_map;
	const std::string full_image_key(const std::string image_key, const int width, const int height, const std::string suffix);
public:
	ImageLoader(ImageLoader const&) = delete;
	void operator=(ImageLoader const&) = delete;
	static ImageLoader& get_instance();
	const static std::string full_filename(const std::string);
	const static std::string full_filename(const std::string filename, const std::string path_prefix);
	~ImageLoader();
	void load_content();
	void unload_content();
	void convert_bitmaps_to_video();
	const bool image_exists(const std::string filename);
	const bool keyed_image_exists(const std::string image_key, const int width, const int height, const std::string suffix);
	const bool keyed_image_exists(const std::string key);
	void set_keyed_image(ALLEGRO_BITMAP * image, const std::string image_key, const int width, const int height, const std::string suffix);
	void delete_keyed_image(const std::string image_key, const int width, const int height, const std::string suffix);
	ALLEGRO_BITMAP * get_keyed_image(const std::string image_key, const int width, const int height, const std::string suffix);
	void load_image(std::string filename);
	void load_image(std::string filename, Rect subsection);
	void load_image(std::string filename, const std::string rect_string, const bool allow_failure);
	void load_image(std::string filename, const std::string rect_string, const bool allow_failure, const std::string path_prefix);
	void load_image(std::string filename, const std::string rect_string, const bool allow_failure, const std::string path_prefix, const bool allow_partial_alpha);
	void load_spritesheet(Animation);
	void load_mask(const std::string base_filename);
	void load_mask(const std::string base_filename, const std::string rect_string);
	void load_mask(const std::string base_filename, const std::string rect_string, const bool include_suffix);
	ALLEGRO_BITMAP* get_default_tile_image(std::string, TileType*);
	ALLEGRO_BITMAP* get_default_block_image(std::string, EntityData*);
	ALLEGRO_BITMAP* get_default_entity_group_image(std::string, EntityGroupData*);
	ALLEGRO_BITMAP* get_default_tiled_image_image(std::string, TiledImageData*);
	ALLEGRO_BITMAP* get_tile_image_for_col(const std::string, TileType*, const int col);
	ALLEGRO_BITMAP* get_block_image_for_col(const std::string, EntityData*, const int col);
	ALLEGRO_BITMAP* get_entity_group_image_for_col(const std::string, EntityGroupData*, const int col);
	ALLEGRO_BITMAP* get_spawner_image_for_col(const std::string, EntityData*, const int col);
	ALLEGRO_BITMAP* get_path_node_image_for_col(const std::string, EntityData*, const int col);
	ALLEGRO_BITMAP* get_location_marker_image_for_col(const std::string, EntityData*, const int col);
	ALLEGRO_BITMAP* get_full_tiled_image_sheet(std::string, TiledImageData*);
	ALLEGRO_BITMAP* get_current_image(GameImage*);
	ALLEGRO_BITMAP* get_image(std::string filename);
	ALLEGRO_BITMAP* get_image(std::string filename, Rect subsection);
	ALLEGRO_BITMAP* get_image(const std::string filename, const std::string rect_string, const std::string path_prefix);
	mask_t * get_mask(const std::string filename);
	mask_t * get_mask(const std::string filename, const std::string rect_string);
	const bool has_mask(const std::string filename, const std::string rect_string);
	std::string rect_to_string(Rect r);
};

#endif