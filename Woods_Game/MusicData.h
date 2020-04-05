#ifndef MUSIC_DATA_H
#define MUSIC_DATA_H

#include "Qualifier.h"
#include "XMLSerialization.h"

class Level;
class World;
class GlobalTime;
class Qualifier;
struct MusicNode : public xmls::Serializable
{
	MusicNode();
	xmls::xString music_filename;
	xmls::xInt weight;
	xmls::Collection<Qualifier> qualifiers;
	const std::string get_music_filename();
	const bool is_valid(World * world, Level * level, GlobalTime * time);
	const int get_weight();
};

class MusicData : public xmls::Serializable
{
private:
	xmls::xInt music_update_interval;
	xmls::Collection<MusicNode> music_nodes;
	std::vector<MusicNode*> valid_music_nodes(World * world, Level * level, GlobalTime * time);
public:
	MusicData();
	~MusicData();
	const int get_music_update_interval();
	MusicNode * select_music_node(World * world, Level * level, GlobalTime * time);
};

#endif