#ifndef DUNGEON_DATA_H
#define DUNGEON_DATA_H

#include "XMLSerialization.h"

class DungeonData : public xmls::Serializable
{
private:
	xmls::xString dungeon_key;
public:
	DungeonData();
	~DungeonData();
	std::string get_dungeon_key();
};

#endif