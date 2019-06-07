#include "LevelGenData.h"

LevelGenData::LevelGenData()
{
	setClassName("LevelGenData");
	Register("should_generate", &should_generate);
	Register("base_tile_type_index", &base_tile_type_index);
	Register("tile_gen_rules", &tile_gen_rules);
	Register("path_systems", &path_systems);
	Register("entity_group_gen_rules", &entity_group_gen_rules);
	Register("block_gen_rules", &block_gen_rules);
}

const bool LevelGenData::get_should_generate()
{
	return this->should_generate.value();
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
	Register("x_pos", &x_pos);
	Register("y_pos", &y_pos);
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
