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

int Block::get_type()
{
	return BLOCK;
}
