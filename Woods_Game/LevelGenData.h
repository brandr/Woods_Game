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
	xmls::xInt path_size;
	xmls::Collection<LevelGenPathNode> path_nodes;
};

struct EntityGroupSpawnTileRule : public xmls::Serializable {
	EntityGroupSpawnTileRule();
	xmls::xInt tile_type_index;
};

struct EntityGroupGenRule : public xmls::Serializable {
	EntityGroupGenRule();
	xmls::xInt entity_group_index;
	xmls::Collection<EntityGroupSpawnTileRule> spawn_tile_rules;
	xmls::xInt min_groups;
	xmls::xInt max_groups;
	xmls::xInt min_spawn_dist_tx;
	xmls::xInt min_spawn_dist_ty;
	const std::vector<EntityGroupSpawnTileRule*> get_spawn_tile_rules();
	const std::vector<int> get_allowed_spawn_tile_indeces();
};

struct BlockGenRule : public xmls::Serializable {
	BlockGenRule();
	xmls::xInt block_index;
	xmls::Collection<EntityGroupSpawnTileRule> spawn_tile_rules;
	xmls::xInt min_blocks;
	xmls::xInt max_blocks;
	xmls::xInt min_spawn_dist_tx;
	xmls::xInt min_spawn_dist_ty;
	const std::vector<EntityGroupSpawnTileRule*> get_spawn_tile_rules();
	const std::vector<int> get_allowed_spawn_tile_indeces();
};

class LevelGenData : public xmls::Serializable
{
private:
	xmls::xBool should_generate;
	xmls::xInt base_tile_type_index;
	xmls::Collection<TileGenRule> tile_gen_rules;
	xmls::Collection<LevelGenPathSystem> path_systems;
	xmls::Collection<EntityGroupGenRule> entity_group_gen_rules;
	xmls::Collection<BlockGenRule> block_gen_rules;
public:
	LevelGenData();
	const bool get_should_generate();
	const int get_base_tile_type_index();
	const std::vector<LevelGenPathSystem*> get_path_systems();
	const std::vector<TileGenRule*> get_tile_gen_rules();
	const std::vector<EntityGroupGenRule*> get_entity_group_gen_rules();
	const std::vector<BlockGenRule*> get_block_gen_rules();
};

#endif
