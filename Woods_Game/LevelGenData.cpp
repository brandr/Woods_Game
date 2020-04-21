#include "LevelGenData.h"

LevelGenData::LevelGenData()
{
	setClassName("LevelGenData");
	Register("should_generate", &should_generate);
	Register("should_generate_new_day", &should_generate_new_day);
	Register("base_tile_type_index", &base_tile_type_index);
	Register("tile_gen_rules", &tile_gen_rules);
	Register("path_systems", &path_systems);
	Register("entity_group_gen_rules", &entity_group_gen_rules);
	Register("block_gen_rules", &block_gen_rules);
	Register("level_gen_sites", &level_gen_sites);
	Register("forced_tiles", &forced_tiles);
	Register("forced_blocks", &forced_blocks);
	Register("forced_entity_groups", &forced_entity_groups);
	Register("gen_updates", &gen_updates);
}

std::vector<LevelGenUpdate*> LevelGenData::get_valid_level_gen_updates(World * world, Level * level, GlobalTime * time)
{
	std::vector<LevelGenUpdate*> valid_updates;
	const int size = this->gen_updates.size();
	for (int i = 0; i < size; i++) {
		LevelGenUpdate* lgu = this->gen_updates.getItem(i);
		if (lgu->qualify(world, level, time)) {
			valid_updates.push_back(lgu);
		}
	}
	return valid_updates;
}

const bool LevelGenData::get_should_generate()
{
	return this->should_generate.value();
}

const bool LevelGenData::get_should_generate_new_day()
{
	return this->should_generate_new_day.value();
}

const int LevelGenData::get_base_tile_type_index()
{
	return base_tile_type_index.value();
}

const std::vector<LevelGenPathSystem*> LevelGenData::get_path_systems()
{
	std::vector<LevelGenPathSystem*> systems;
	const int size = this->path_systems.size();
	for (int i = 0; i < size; i++) {
		systems.push_back(this->path_systems.getItem(i));
	}
	return systems;
}

const std::vector<TileGenRule*> LevelGenData::get_tile_gen_rules()
{
	std::vector<TileGenRule*> rules;
	const int size = this->tile_gen_rules.size();
	for (int i = 0; i < size; i++) {
		rules.push_back(this->tile_gen_rules.getItem(i));
	}
	return rules;
}

const std::vector<EntityGroupGenRule*> LevelGenData::get_entity_group_gen_rules()
{
	std::vector<EntityGroupGenRule*> rules;
	const int size = this->entity_group_gen_rules.size();
	for (int i = 0; i < size; i++) {
		rules.push_back(this->entity_group_gen_rules.getItem(i));
	}
	return rules;
}

const std::vector<BlockGenRule*> LevelGenData::get_block_gen_rules()
{
	std::vector<BlockGenRule*> rules;
	const int size = this->block_gen_rules.size();
	for (int i = 0; i < size; i++) {
		rules.push_back(this->block_gen_rules.getItem(i));
	}
	return rules;
}

const std::vector<LevelGenSite*> LevelGenData::get_level_gen_sites()
{
	std::vector<LevelGenSite*> sites;
	const int size = this->level_gen_sites.size();
	for (int i = 0; i < size; i++) {
		sites.push_back(this->level_gen_sites.getItem(i));
	}
	return sites;
}

const std::vector<ForcedTile*> LevelGenData::get_forced_tiles()
{
	std::vector<ForcedTile*> tiles;
	const int size = this->forced_tiles.size();
	for (int i = 0; i < size; i++) {
		tiles.push_back(this->forced_tiles.getItem(i));
	}
	return tiles;
}

const std::vector<Block*> LevelGenData::get_forced_blocks()
{
	std::vector<Block*> blocks;
	const int size = this->forced_blocks.size();
	for (int i = 0; i < size; i++) {
		blocks.push_back(this->forced_blocks.getItem(i));
	}
	return blocks;
}

const std::vector<EntityGroup*> LevelGenData::get_forced_entity_groups()
{
	std::vector<EntityGroup*> egs;
	const int size = this->forced_entity_groups.size();
	for (int i = 0; i < size; i++) {
		egs.push_back(this->forced_entity_groups.getItem(i));
	}
	return egs;
}

LevelGenPathSystem::LevelGenPathSystem()
{
	setClassName("LevelGenPathSystem");
	Register("path_nodes", &path_nodes);
	Register("tile_type_index", &tile_type_index);
	Register("path_size", &path_size);
}

LevelGenPathNode::LevelGenPathNode()
{
	setClassName("LevelGenPathNode");
	Register("node_key", &node_key);
	Register("x_pos", &x_pos);
	Register("y_pos", &y_pos);
	Register("dest_node_key", &dest_node_key);
	Register("dest_site_key", &dest_site_key);
	Register("edge_key", &edge_key);
	Register("edge_range", &edge_range);
	Register("edge_center_x", &edge_center_x);
	Register("edge_center_y", &edge_center_y);	
}

TileGenRule::TileGenRule()
{
	setClassName("TileGenRule");
	Register("tile_type_index", &tile_type_index);
	Register("min_patches", &min_patches);
	Register("max_patches", &max_patches);
	Register("min_patch_size", &min_patch_size);
	Register("max_patch_size", &max_patch_size);
}

EntityGroupGenRule::EntityGroupGenRule()
{
	this->setClassName("EntityGroupGenRule");
	this->Register("entity_group_index", &entity_group_index);
	this->Register("spawn_tile_rules", &spawn_tile_rules);
	this->Register("min_groups", &min_groups);
	this->Register("max_groups", &max_groups);
	this->Register("min_spawn_dist_tx", &min_spawn_dist_tx);
	this->Register("min_spawn_dist_ty", &min_spawn_dist_ty);
}

const std::vector<EntityGroupSpawnTileRule*> EntityGroupGenRule::get_spawn_tile_rules()
{
	std::vector<EntityGroupSpawnTileRule*> rules;
	const int size = this->spawn_tile_rules.size();
	for (int i = 0; i < size; i++) {
		rules.push_back(this->spawn_tile_rules.getItem(i));
	}
	return rules;
}

const std::vector<int> EntityGroupGenRule::get_allowed_spawn_tile_indeces()
{
	std::vector<int> allowed;
	const int size = this->spawn_tile_rules.size();
	for (int i = 0; i < size; i++) {
		allowed.push_back(this->spawn_tile_rules.getItem(i)->tile_type_index.value());
	}
	return allowed;
}

EntityGroupSpawnTileRule::EntityGroupSpawnTileRule()
{
	this->setClassName("EntityGroupSpawnTileRule");
	this->Register("tile_type_index", &tile_type_index);
}

BlockGenRule::BlockGenRule()
{
	this->setClassName("BlockGenRule");
	this->Register("block_index", &block_index);
	this->Register("spawn_tile_rules", &spawn_tile_rules);
	this->Register("min_blocks", &min_blocks);
	this->Register("max_blocks", &max_blocks);
	this->Register("min_spawn_dist_tx", &min_spawn_dist_tx);
	this->Register("min_spawn_dist_ty", &min_spawn_dist_ty);
}

const std::vector<EntityGroupSpawnTileRule*> BlockGenRule::get_spawn_tile_rules()
{
	std::vector<EntityGroupSpawnTileRule*> rules;
	const int size = this->spawn_tile_rules.size();
	for (int i = 0; i < size; i++) {
		rules.push_back(this->spawn_tile_rules.getItem(i));
	}
	return rules;
}

const std::vector<int> BlockGenRule::get_allowed_spawn_tile_indeces()
{
	std::vector<int> allowed;
	const int size = this->spawn_tile_rules.size();
	for (int i = 0; i < size; i++) {
		allowed.push_back(this->spawn_tile_rules.getItem(i)->tile_type_index.value());
	}
	return allowed;
}

ForcedTile::ForcedTile()
{
	setClassName("ForcedTile");
	Register("tile_type_index", &tile_type_index);
	Register("tile_pos_x", &tile_pos_x);
	Register("tile_pos_y", &tile_pos_y);
	Register("tile_sheet_col", &tile_sheet_col);
	Register("tile_sheet_row", &tile_sheet_row);
}

LevelGenUpdate::LevelGenUpdate()
{
	setClassName("LevelGenUpdate");
	Register("qualifiers", &qualifiers);
	Register("forced_tiles", &forced_tiles);
	Register("forced_blocks", &forced_blocks);
	Register("forced_entity_groups", &forced_entity_groups);
	Register("forced_spawners", &forced_spawners);
	Register("forced_removes", &forced_removes);
	Register("forced_tiled_images", &forced_tiled_images);
	Register("forced_location_markers", &forced_location_markers);
	Register("forced_path_nodes", &forced_path_nodes);
}

const bool LevelGenUpdate::qualify(World * world, Level * level, GlobalTime * time)
{
	const int size = this->qualifiers.size();
	for (int i = 0; i < size; i++) {
		Qualifier * q = this->qualifiers.getItem(i);
		if (!q->evaluate(world, level, time)) {
			return false;
		}
	}
	return true;
}

std::vector<ForcedTile*> LevelGenUpdate::get_forced_tiles()
{
	std::vector<ForcedTile*> tiles;
	const int size = this->forced_tiles.size();
	for (int i = 0; i < size; i++) {
		tiles.push_back(this->forced_tiles.getItem(i));
	}
	return tiles;
}

std::vector<Block*> LevelGenUpdate::get_forced_blocks()
{
	std::vector<Block*> blocks;
	const int size = this->forced_blocks.size();
	for (int i = 0; i < size; i++) {
		blocks.push_back(this->forced_blocks.getItem(i));
	}
	return blocks;
}

std::vector<EntityGroup*> LevelGenUpdate::get_forced_entity_groups()
{
	std::vector<EntityGroup*> egs;
	const int size = this->forced_entity_groups.size();
	for (int i = 0; i < size; i++) {
		egs.push_back(this->forced_entity_groups.getItem(i));
	}
	return egs;
}

std::vector<Spawner*> LevelGenUpdate::get_forced_spawners()
{
	std::vector<Spawner*> spawners;
	const int size = this->forced_spawners.size();
	for (int i = 0; i < size; i++) {
		spawners.push_back(this->forced_spawners.getItem(i));
	}
	return spawners;
}

std::vector<LevelGenRemove*> LevelGenUpdate::get_forced_removes()
{
	std::vector<LevelGenRemove*> removes;
	const int size = this->forced_spawners.size();
	for (int i = 0; i < size; i++) {
		removes.push_back(this->forced_removes.getItem(i));
	}
	return removes;
}

std::vector<ForcedTiledImage*> LevelGenUpdate::get_forced_tiled_images()
{
	std::vector<ForcedTiledImage*> tiled_images;
	const int size = this->forced_spawners.size();
	for (int i = 0; i < size; i++) {
		tiled_images.push_back(this->forced_tiled_images.getItem(i));
	}
	return tiled_images;
}

std::vector<LocationMarker*> LevelGenUpdate::get_forced_location_markers()
{
	std::vector<LocationMarker*> location_markers;
	const int size = this->forced_location_markers.size();
	for (int i = 0; i < size; i++) {
		location_markers.push_back(this->forced_location_markers.getItem(i));
	}
	return location_markers;
}

std::vector<PathNode*> LevelGenUpdate::get_forced_path_nodes()
{
	std::vector<PathNode*> path_nodes;
	const int size = this->forced_path_nodes.size();
	for (int i = 0; i < size; i++) {
		path_nodes.push_back(this->forced_path_nodes.getItem(i));
	}
	return path_nodes;
}

LevelGenRemove::LevelGenRemove()
{
	setClassName("LevelGenRemove");
	Register("object_type", &object_type);
	Register("tile_pos_x", &tile_pos_x);
	Register("tile_pos_y", &tile_pos_y);
}

ForcedTiledImage::ForcedTiledImage()
{
	setClassName("ForcedTiledImage");
	Register("tiled_image_key", &tiled_image_key);
	Register("tiled_image_layer", &tiled_image_layer);
	Register("tiled_image_sheet_col", &tiled_image_sheet_col);
	Register("tiled_image_sheet_row", &tiled_image_sheet_row);
	Register("pos_x", &x_pos);
	Register("pos_y", &y_pos);
}
