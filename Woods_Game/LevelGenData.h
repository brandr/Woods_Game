#ifndef LEVEL_GEN_DATA_H
#define LEVEL_GEN_DATA_H

#include "XMLSerialization.h"

struct TileGenRule : public xmls::Serializable {
	TileGenRule();
	xmls::xInt tile_type_index;
	xmls::xInt min_patches;
	xmls::xInt max_patches;
	xmls::xInt min_patch_size;
	xmls::xInt max_patch_size;
};

struct LevelGenPathNode : public xmls::Serializable {
	LevelGenPathNode();
	xmls::xInt x_pos;
	xmls::xInt y_pos;
};

struct LevelGenPathSystem : public xmls::Serializable {
	LevelGenPathSystem();
	xmls::xInt tile_type_index;
	xmls::Collection<LevelGenPathNode> path_nodes;
};

class LevelGenData : public xmls::Serializable
{
private:
	xmls::xBool should_generate;
	xmls::xInt base_tile_type_index;
	xmls::Collection<TileGenRule> tile_gen_rules;
	xmls::Collection<LevelGenPathSystem> path_systems;
public:
	LevelGenData();
	const bool get_should_generate();
	const int get_base_tile_type_index();
	const std::vector<LevelGenPathSystem*> get_path_systems();
	const std::vector<TileGenRule*> get_tile_gen_rules();
};

#endif
