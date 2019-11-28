#include "BiomeManager.h"
#include "GlobalTime.h"
#include "Level.h"
#include "World.h"

BiomeManager & BiomeManager::get_instance()
{
	static BiomeManager manager;
	return manager;
}

BiomeManager::~BiomeManager()
{
}

void BiomeManager::initialize_biome(const std::string biome_key)
{
	if (this->biome_map.find(biome_key) == this->biome_map.end()) {
		FileManager fm;
		CritterBiome * cb = new CritterBiome();
		fm.load_xml_content(cb, "resources/load/biomes", "SerializableClass", "BiomeKey", biome_key);
		this->biome_map[biome_key] = cb;
	}
}

const bool BiomeManager::should_generate_critters(World * world, GlobalTime * time, const std::string biome_key)
{
	if (this->biome_map.find(biome_key) != this->biome_map.end()) {
		return this->biome_map[biome_key]->should_generate_critters(world, time);
	}
	return false;
}

const std::vector<std::string> BiomeManager::critter_keys_to_generate(World * world, Level * level, GlobalTime * time)
{
	std::vector<std::string> critter_keys;
	const std::string biome_key = level->get_biome_key();
	if (!biome_key.empty() && this->biome_map.find(biome_key) != this->biome_map.end()) {
		return this->biome_map[biome_key]->critter_keys_to_generate(world, level, time);
	}
	return critter_keys;
}
