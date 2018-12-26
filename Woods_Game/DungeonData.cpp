#include "DungeonData.h"

DungeonData::DungeonData()
{
	setClassName("DungeonData");
	Register("dungeon_key", &dungeon_key);
}

DungeonData::~DungeonData()
{
}

std::string DungeonData::get_dungeon_key()
{
	return this->dungeon_key.value();
}
