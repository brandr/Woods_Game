#include "TileType.h"

TileType::TileType()
{
	setClassName("TileType");
	Register("tile_sheet_name", &tile_sheet_name);
	Register("tile_sheet_row", &tile_sheet_row);
	Register("tile_type_index", &tile_sheet_index);
	Register("tile_sheet_key", &tile_sheet_key);
	Register("speed_mod", &speed_mod);
	Register("edge_priority", &edge_priority);
	Register("npc_pathable", &npc_pathable);
}

TileType::TileType(std::string name, int row)
{
	setClassName("TileType");
	Register("tile_sheet_name", &tile_sheet_name);
	Register("tile_sheet_row", &tile_sheet_row);
	Register("tile_type_index", &tile_sheet_index);
	Register("tile_sheet_key", &tile_sheet_key);
	Register("speed_mod", &speed_mod);
	Register("edge_priority", &edge_priority);
	Register("npc_pathable", &npc_pathable);
	this->tile_sheet_name = name;
	this->tile_sheet_row = row;
}


TileType::~TileType()
{
}

void TileType::set_tile_sheet_name(std::string name)
{
	this->tile_sheet_name = xmls::xString(name);
}

void TileType::set_tile_sheet_row(int row)
{
	this->tile_sheet_row = row;
}


void TileType::set_speed_mod(float speed_mod)
{
	this->speed_mod = speed_mod;
}

void TileType::set_edge_priority(int priority)
{
	this->edge_priority = priority;
}

std::string TileType::get_tile_sheet_key()
{
	return tile_sheet_key.value();
}

int TileType::get_tile_sheet_row()
{
	return this->tile_sheet_row.value();
}

float TileType::get_speed_mod()
{
	return this->speed_mod.value();
}

int TileType::get_edge_priority()
{
	return edge_priority.value();
}

const bool TileType::is_npc_pathable(const bool obeys_rules)
{
	const int pathable_value = this->npc_pathable.value();
	return obeys_rules ? pathable_value == NPC_PATHABLE : pathable_value > NPC_UNPATHABLE_HARD;
}
