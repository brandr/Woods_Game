#ifndef TILE_TYPE_H
#define TILE_TYPE_H

#include "XMLSerialization.h"

class TileType : public xmls::Serializable
{
private:
	xmls::xString tile_sheet_name;
	xmls::xInt tile_sheet_row;
	xmls::xInt tile_sheet_index;
	xmls::xString tile_sheet_key;
	xmls::xFloat speed_mod = 1.0f;
	xmls::xInt edge_priority;
	xmls::xBool npc_pathable;
public:
	TileType();
	TileType(std::string tile_sheet_name, int tile_sheet_row);
	~TileType();
	void set_tile_sheet_name(std::string name);
	void set_tile_sheet_row(int row);
	void set_speed_mod(float speed_mod);
	void set_edge_priority(int priority);
	std::string get_tile_sheet_key();
	int get_tile_sheet_row();
	float get_speed_mod();
	int get_edge_priority();
	const bool is_npc_pathable();
};
#endif
