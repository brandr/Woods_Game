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
	const int size = this->tiles.size();
	return index >= 0 && index < size ? this->tiles.getItem(index) : NULL;
}

void TileGroup::set_tile(TileSet * tileset, Tile * t, int index)
{
	while (index >= (int) this->tiles.size()) {
		this->tiles.addItem(new Tile());
	}
	Tile * tile = this->tiles.getItem(index);
	if (tile != NULL) {
		tile->reset(tileset, t, index, this->index.value());
	}
}

int TileGroup::get_size()
{
	return tiles.size();
}
