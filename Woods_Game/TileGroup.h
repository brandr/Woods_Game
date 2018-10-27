#ifndef TILEGROUP_H
#define TILEGROUP_H

#include "XMLSerialization.h"
#include "Tile.h"

class TileGroup: public xmls::Serializable
{
private:
	xmls::Collection<Tile> tiles;
	xmls::xInt index;
public:
	TileGroup();
	TileGroup(int index);
	~TileGroup();
	void add_tile(Tile *t);
	Tile *get_tile(int index);
	void set_tile(TileSet *tileset, Tile *t, int index);
	int get_size();
};

#endif