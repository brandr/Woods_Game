#include "TileType.h"

TileType::TileType()
{
	setClassName("TileType");
	Register("tile_sheet_name", &tile_sheet_name);
	Register("tile_sheet_row", &tile_sheet_row);
	Register("speed_mod", &speed_mod);
	Register("edge_priority", &edge_priority);
}

TileType::TileType(std::string name, int row)
{
	setClassName("TileType");
	Register("tile_sheet_name", &tile_sheet_name);
	Register("tile_sheet_row", &tile_sheet_row);
	Register("speed_mod", &speed_mod);
	Register("edge_priority", &edge_priority);
	this->tile_sheet_name = name;
	this->tile_sheet_row = row;
}


TileType::~TileType()
{
}

void TileType::set_tile_sheet_name(std::string name)
{
	this->tile_sheet_name = name;
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

float TileType::get_speed_mod()
{
	return this->speed_mod.value();
}

int TileType::get_edge_priority()
{
	return edge_priority.value();
}
