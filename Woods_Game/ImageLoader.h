#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <map>
#include "ImageLoader.h"
#include "TileType.h"
#include "Entity.h"
#include "TiledImage.h"
#include <string>
#include <iostream>
#include "xstring"  
#include "Animation.h"
#include "allegro5/allegro5.h"

class GameImage;
class ImageLoader
{
private:
	ImageLoader() {}
	std::map<std::pair<std::string, std::string>, ALLEGRO_BITMAP*> image_map;
	std::string rect_to_string(Rect r);
public:
	ImageLoader(ImageLoader const&) = delete;
	void operator=(ImageLoader const&) = delete;
	static ImageLoader& get_instance();
	static std::string full_filename(std::string);
	~ImageLoader();
	void load_content();
	void unload_content();
	void load_image(std::string filename);
	void load_image(std::string filename, Rect subsection);
	void load_spritesheet(Animation);
	ALLEGRO_BITMAP* get_default_tile_image(std::string, TileType*);
	ALLEGRO_BITMAP* get_default_block_image(std::string, EntityData*);
	ALLEGRO_BITMAP* get_default_entity_group_image(std::string, EntityGroupData*);
	ALLEGRO_BITMAP* get_default_tiled_image_image(std::string, TiledImageData*);
	ALLEGRO_BITMAP* get_tile_image_for_col(const std::string, TileType*, const int col);
	ALLEGRO_BITMAP* get_block_image_for_col(const std::string, EntityData*, const int col);
	ALLEGRO_BITMAP* get_entity_group_image_for_col(const std::string, EntityGroupData*, const int col);
	ALLEGRO_BITMAP* get_full_tiled_image_sheet(std::string, TiledImageData*);
	ALLEGRO_BITMAP* get_current_image(GameImage*);
	ALLEGRO_BITMAP* get_image(std::string filename);
	ALLEGRO_BITMAP* get_image(std::string filename, Rect subsection);
};

#endif