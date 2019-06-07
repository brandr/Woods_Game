#include "LevelGenData.h"

LevelGenData::LevelGenData()
{
	setClassName("LevelGenData");
	Register("should_generate", &should_generate);
	Register("base_tile_type_index", &base_tile_type_index);
	Register("tile_gen_rules", &tile_gen_rules);
	Register("path_systems", &path_systems);
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

LevelGenPathSystem::LevelGenPathSystem()
{
	setClassName("LevelGenPathSystem");
	Register("path_nodes", &path_nodes);
	Register("tile_type_index", &tile_type_index);
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
