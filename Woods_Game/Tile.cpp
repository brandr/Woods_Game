#include "Tile.h"



Tile::Tile()
{
}


Tile::~Tile()
{
}

void Tile::set_content(std::string image_filename, Rect * image_subsection, std::pair<int, int> position)
{
	GameImage::set_content(image_filename, image_subsection, position);
	refresh_mask();
	//mask = Mask_New();
	//bitmap = //ImageLoader::get_instance().get_current_image(this);
}

void Tile::unload_content()
{
	if (block) {
		block->unload_content();
		delete block;
	}
	block = NULL;
	GameImage::unload_content();
}

void Tile::draw(ALLEGRO_DISPLAY *display, int x_offset, int y_offset)
{
	/*
	if (bitmap) {
		//std::cout << "drawing tile" << std::endl;
		al_draw_bitmap(bitmap, rect.x + x_offset, rect.y + y_offset, 0);
	}
	*/
	GameImage::draw(display, x_offset, y_offset);
	if (block) block->draw(display, x_offset, y_offset);
}

Block * Tile::get_block()
{
	return block;
}

void Tile::set_block(Block * b)
{
	block = b;
}

void Tile::set_sheet_row(int sheet_row)
{
	this->sheet_row = sheet_row;
}

int Tile::get_sheet_row()
{
	return sheet_row;
}

void Tile::set_speed_mod(float sm)
{
	speed_mod = sm;
}

float Tile::get_speed_mod()
{
	return speed_mod;
}

void Tile::set_edge_priority(int priority)
{
	this->edge_priority = priority;
}

int Tile::get_edge_priority()
{
	return edge_priority;
}
