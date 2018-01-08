#include "Tile.h"



Tile::Tile()
{
	setClassName("Tile");
	Register("tile_pos_x", &tile_pos_x);
	Register("tile_pos_y", &tile_pos_y);
	Register("tile_type_id", &tile_type_id);
}

Tile::~Tile()
{
}

void Tile::set_content(std::string image_filename, Rect * image_subsection, std::pair<int, int> position)
{
	GameImage::set_content(image_filename, image_subsection, position);
	refresh_mask();
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
	GameImage::draw(display, x_offset, y_offset);
	if (block) block->draw(display, x_offset, y_offset);
}

Block * Tile::get_block() const
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

int Tile::get_sheet_row() const
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
