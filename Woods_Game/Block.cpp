#include "Block.h"

Block::Block()
{
	solid = true;
}

Block::Block(int x, int y)
{
	this->rect.x = x;
	this->rect.y = y;
	solid = true;
}


Block::~Block()
{
}

void Block::set_content(std::string image_filename, Rect * image_subsection, std::pair<int, int> position)
{
	GameImage::set_content(image_filename, image_subsection, position);
	refresh_mask();
}

int Block::get_type()
{
	return BLOCK;
}
