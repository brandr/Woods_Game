#include "TileGroup.h"



TileGroup::TileGroup()
{
	this->setClassName("TileGroup");
	this->Register("Tiles", &tiles);
	this->Register("Index", &index);
}

TileGroup::TileGroup(int index)
{
	this->setClassName("TileGroup");
	this->Register("Tiles", &tiles);
	this->Register("Index", &(this->index));
	this->index = index;
}


TileGroup::~TileGroup()
{
}

void TileGroup::add_tile(Tile * t)
{
	this->tiles.addItem(t);
}

Tile * TileGroup::get_tile(int index)
{
	return this->tiles.getItem(index);
}

int TileGroup::get_size()
{
	return tiles.size();
}
