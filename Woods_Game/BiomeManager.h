#ifndef BIOME_MANAGER_H
#define BIOME_MANAGER_H

#include "FileManager.h"
#include "CritterBiome.h"

class GlobalTime;
class Level;
class World;
class BiomeManager
{
private:
	BiomeManager() {}
	std::map<std::string, CritterBiome*> biome_map;
public:
	BiomeManager(BiomeManager const&) = delete;
	void operator=(BiomeManager const&) = delete;
	static BiomeManager& get_instance();
	~BiomeManager();
	void initialize_biome(const std::string biome_key);
	const bool should_generate_critters(World * world, GlobalTime * time, const std::string biome_key);
	const std::vector<std::string> critter_keys_to_generate(World * world, Level * level, GlobalTime * time);
};

#endif