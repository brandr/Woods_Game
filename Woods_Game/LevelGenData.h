#ifndef LEVEL_GEN_DATA_H
#define LEVEL_GEN_DATA_H

#include "Block.h"
#include "EntityGroup.h"
#include "LevelGenSite.h"
#include "LocationMarker.h"
#include "PathNode.h"
#include "Qualifier.h"
#include "Spawner.h"
#include "Tile.h"

#include "XMLSerialization.h"

class GlobalTime;
class Level;
class World;

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
	xmls::xString node_key;
	xmls::xInt x_pos;
	xmls::xInt y_pos;
	xmls::xString dest_node_key;
	xmls::xString dest_site_key;
	xmls::xString edge_key;
	xmls::xInt edge_range;
	xmls::xInt edge_center_x;
	xmls::xInt edge_center_y;
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

struct ForcedTile : public xmls::Serializable {
	ForcedTile();
	xmls::xInt tile_type_index;
	xmls::xInt tile_pos_x, tile_pos_y;
	xmls::xInt tile_sheet_col, tile_sheet_row;
};

struct LevelGenRemove : public xmls::Serializable {
	LevelGenRemove();
	xmls::xInt object_type;
	xmls::xInt tile_pos_x;
	xmls::xInt tile_pos_y;
};

struct ForcedTiledImage : public xmls::Serializable {
	ForcedTiledImage();
	xmls::xInt tiled_image_key;
	xmls::xInt tiled_image_layer;
	xmls::xInt tiled_image_sheet_col;
	xmls::xInt tiled_image_sheet_row;
	xmls::xInt x_pos;
	xmls::xInt y_pos;
};

struct LevelGenUpdate : public xmls::Serializable {
	LevelGenUpdate();
	xmls::Collection<Qualifier> qualifiers;
	xmls::Collection<ForcedTile> forced_tiles;
	xmls::Collection<Block> forced_blocks;
	xmls::Collection<EntityGroup> forced_entity_groups;
	xmls::Collection<Spawner> forced_spawners;
	xmls::Collection<LevelGenRemove> forced_removes;
	xmls::Collection<ForcedTiledImage> forced_tiled_images;
	xmls::Collection<LocationMarker> forced_location_markers;
	xmls::Collection<PathNode> forced_path_nodes;
	const bool qualify(World * world, Level * level, GlobalTime * time);
	std::vector<ForcedTile *> get_forced_tiles();
	std::vector<Block*> get_forced_blocks();
	std::vector<EntityGroup*> get_forced_entity_groups();
	std::vector<Spawner*> get_forced_spawners();
	std::vector<LevelGenRemove*> get_forced_removes();
	std::vector<ForcedTiledImage*> get_forced_tiled_images();
	std::vector<LocationMarker*> get_forced_location_markers();
	std::vector<PathNode*> get_forced_path_nodes();
};

class LevelGenData : public xmls::Serializable
{
private:
	xmls::xBool should_generate;
	xmls::xBool should_generate_new_day;
	xmls::xInt base_tile_type_index;
	xmls::Collection<TileGenRule> tile_gen_rules;
	xmls::Collection<LevelGenPathSystem> path_systems;
	xmls::Collection<EntityGroupGenRule> entity_group_gen_rules;
	xmls::Collection<BlockGenRule> block_gen_rules;
	xmls::Collection<LevelGenSite> level_gen_sites;
	xmls::Collection<ForcedTile> forced_tiles;
	xmls::Collection<Block> forced_blocks;
	xmls::Collection<EntityGroup> forced_entity_groups;
	xmls::Collection<LevelGenUpdate> gen_updates;
public:
	LevelGenData();
	std::vector<LevelGenUpdate *> get_valid_level_gen_updates(World * world, Level * level, GlobalTime * time);
	const bool get_should_generate();
	const bool get_should_generate_new_day();
	const int get_base_tile_type_index();
	const std::vector<LevelGenPathSystem*> get_path_systems();
	const std::vector<TileGenRule*> get_tile_gen_rules();
	const std::vector<EntityGroupGenRule*> get_entity_group_gen_rules();
	const std::vector<BlockGenRule*> get_block_gen_rules();
	const std::vector<LevelGenSite*> get_level_gen_sites();
	const std::vector<ForcedTile*> get_forced_tiles();
	const std::vector<Block*> get_forced_blocks();
	const std::vector<EntityGroup*> get_forced_entity_groups();
};

#endif
