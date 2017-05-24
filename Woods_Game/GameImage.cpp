#include "GameImage.h"
#include "ImageLoader.h"
#include<iostream>

GameImage::GameImage(std::string filename)
{
	//this->image_filename = filename;
}

GameImage::GameImage()
{
	//TEMP: actually set width and height upon loading image
	rect = Rect(0, 0, 64, 64);
}

GameImage::GameImage(int x, int y, int width, int height)
{
	rect = Rect(x, y, width, height);
}

GameImage::~GameImage()
{
	/*
	delete animation;
	delete ss_animation;
	if(image_subsection)
		delete image_subsection;
	animation = NULL;
	ss_animation = NULL;
	image_subsection = NULL;
	*/
}

int GameImage::get_type()
{
	return GAME_IMAGE;
}

std::string GameImage::get_image_filename()
{
	return image_filename;
}

void GameImage::load_content(std::vector<std::string> attributes, std::vector<std::string> contents)
{
	std::string anim_filename = "";
	std::pair<int, int> ss_frame_count;
	std::pair<int, int> ss_frame_dimensions;
	int size = attributes.size();
	for (int i = 0; i < size; i++) {
		//TODO: allow cross-section of image
		//std::cout << "attribute: "<< attributes[i]  << std::endl;
		//TODO: make it possible to load animations/spritesheets here
		if (attributes[i] == "image") {
			image_filename = contents[i];
			ImageLoader::get_instance().load_image(contents[i]);
		}
		//else if (attributes[i] == ""
		else if (attributes[i] == "position") {
			std::string tile_string = contents[i];
			rect.x = atoi(tile_string.substr(0, tile_string.find(',')).c_str());
			rect.y = atoi(tile_string.substr(tile_string.find(',') + 1).c_str());
		}
		else if (attributes[i] == "spritesheet") {
			anim_filename = "sprite_sheets/" + contents[i];
			animation = new Animation();
			ss_animation = new SpriteSheetAnimation();
		}
		else if (attributes[i] == "ss_frame_count") {
			std::string str = contents[i];
			int x = atoi(str.substr(0, str.find(',')).c_str());
			int y = atoi(str.substr(str.find(',') + 1).c_str());
			ss_frame_count = std::pair<int, int>(x, y);
		}
		else if (attributes[i] == "ss_frame_dimensions") {
			std::string str = contents[i];
			int x = atoi(str.substr(0, str.find(',')).c_str());
			int y = atoi(str.substr(str.find(',') + 1).c_str());
			ss_frame_dimensions = std::pair<int, int>(x, y);
		}
	}
	if (animation) {
		animation->load_content(anim_filename, ss_frame_count, std::pair<int, int>(0, 0), ss_frame_dimensions);
		ImageLoader::get_instance().load_spritesheet(*animation);
	}
	//TODO: set rect width and height correctly for collisions
	//TODO: some way to figure out which row of animation we should be on
}

void GameImage::set_content(std::string image_filename, Rect* image_subsection, std::pair<int, int> position)
{
	this->image_filename = image_filename;
	this->image_subsection = image_subsection;
	rect.x = position.first, rect.y = position.second;
}

void GameImage::unload_content()
{
	//std::cout << "unloading content" << std::endl;
	delete image_subsection;
	delete animation;
	delete ss_animation;
	image_subsection = NULL;
	animation = NULL;
	ss_animation = NULL;
}

void GameImage::draw(ALLEGRO_DISPLAY *display, int x_offset, int y_offset)
{
	//td::cout << "rect coords: " << rect. << std::endl;
	//TODO: handle animations 
	//TODO: makee sure bounding rect for non-square/animated gameimages is handled correctly (may need per-pixel collisions
	ALLEGRO_BITMAP* bitmap = bitmap = ImageLoader::get_instance().get_current_image(this);
	//if(!bitmap)
	//	std::cout << "no bitmap " << std::endl;
	if (bitmap && rect.x + x_offset < DEFAULT_SCREEN_WIDTH && rect.right() + x_offset > 0 && rect.y + y_offset < DEFAULT_SCREEN_HEIGHT && rect.bottom() + y_offset > 0) {
		//if (animation)
			//std::cout << "drawing sprite " << std::endl;
		al_draw_bitmap(bitmap, rect.x + x_offset, rect.y + y_offset, 0);
	}

}

void GameImage::update()
{
	if (animation && ss_animation) {	//TEMP: might sometimes only have animation
		ss_animation->update(*animation);
	}
}

void GameImage::set_position(int x, int y)
{
	rect.x = x;
	rect.y = y;
}

Animation * GameImage::get_animation()
{
	return animation;
}

SpriteSheetAnimation* GameImage::get_ss_animation()
{
	return ss_animation;
}

Rect * GameImage::get_image_subsection()
{
	return image_subsection;
}

int GameImage::get_x()
{
	return rect.x;
}

int GameImage::get_y()
{
	return rect.y;
}

int GameImage::get_width()
{
	return rect.width;
}

int GameImage::get_height()
{
	return rect.height;
}

bool GameImage::contains_point(int x, int y)
{
	//TODO: also account for pixel-perfect collision (might do this another place though)
	return x >= this->rect.x && y >= this->rect.y && x < this->get_x() + this->rect.width && y < this->rect.y + this->rect.height;
}

bool GameImage::intersects_area(Rect area)
{
	return rect.intersects_rect(area);
}

/*
void GameImage::set_current_level(Level* level)
{
	current_level = level;
}
*/

bool GameImage::outside_level(std::pair<int, int> level_dimensions)
{
	//if (!current_level) return true;
	return rect.right() < 0 || rect.x > level_dimensions.first || rect.bottom() < 0 || rect.y > level_dimensions.second;
}
