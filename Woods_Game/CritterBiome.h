#ifndef CRITTER_BIOME_H
#define CRITTER_BIOME_H

#include <random>
#include "Qualifier.h"
#include "XMLSerialization.h"

class GlobalTime;
class Level;
class World;

struct CritterSpawnRule : public xmls::Serializable {
	xmls::xString critter_key;
	xmls::Collection<Qualifier> qualifiers;
	xmls::xInt max_spawned;
	xmls::xFloat spawn_rate;
	CritterSpawnRule();
	~CritterSpawnRule();
	const std::string get_critter_key();
	const int get_num_critters_to_spawn(World * world, Level * level, GlobalTime * time, const int num_critters_spawned);
	const bool qualify(World * world, Level * level, GlobalTime * time);
};

class CritterBiome : public xmls::Serializable
{
private:
	xmls::xString biome_key;
	xmls::xBool should_generate;
	xmls::xInt max_critter_override;
	xmls::Collection<CritterSpawnRule> critter_spawn_rules;
	const int max_critters_to_spawn(World * world, Level * level, GlobalTime * time);
public:
	CritterBiome();
	~CritterBiome();
	const std::string get_biome_key();
	const bool should_generate_critters(World * world, GlobalTime * time);
	const std::vector<std::string> critter_keys_to_generate(World * world, Level * level, GlobalTime * time);
};

#endif