#include "CritterBiome.h"
#include "GlobalTime.h"
#include "Level.h"
#include "World.h"

const int CritterBiome::max_critters_to_spawn(World * world, Level * level, GlobalTime * time)
{
	const int max_override = this->max_critter_override.value();
	//TODO: probably want to limit by size of level
	return max_override;
}

CritterBiome::CritterBiome()
{
	setClassName("CritterBiome");
	Register("biome_key", &biome_key);
	Register("should_generate", &should_generate);
	Register("critter_spawn_rules", &critter_spawn_rules);
	Register("max_critter_override", &max_critter_override);
}


CritterBiome::~CritterBiome()
{
}

const std::string CritterBiome::get_biome_key()
{
	return this->biome_key.value();
}

//TODO: should still return false if we have no critters-- binding only acts as an override if false
const bool CritterBiome::should_generate_critters(World * world, GlobalTime * time)
{
	return this->should_generate.value();
}

const std::vector<std::string> CritterBiome::critter_keys_to_generate(World * world, Level * level, GlobalTime * time) {
	int num_spawned = 0;
	std::vector<std::string> critter_keys;
	const int size = this->critter_spawn_rules.size();
	const int max_to_spawn = this->max_critters_to_spawn(world, level, time);
	for (int i = 0; i < size; i++) {
		if (num_spawned >= max_to_spawn) {
			break;
		}
		CritterSpawnRule * csr = this->critter_spawn_rules.getItem(i);
		const std::string critter_key = csr->get_critter_key();
		const int num_to_spawn = csr->get_num_critters_to_spawn(world, level, time, num_spawned);
		for (int i = 0; i < num_to_spawn; i++) {
			if (num_spawned >= max_to_spawn) {
				break;
			}
			critter_keys.push_back(critter_key);
			num_spawned++;
		}
	}
	return critter_keys;
}

CritterSpawnRule::CritterSpawnRule()
{
	setClassName("CritterSpawnRule");
	Register("critter_key", &critter_key);
	Register("qualifiers", &qualifiers);
	Register("max_spawned", &max_spawned);
	Register("spawn_rate", &spawn_rate);
}

CritterSpawnRule::~CritterSpawnRule()
{
}

const std::string CritterSpawnRule::get_critter_key()
{
	return this->critter_key.value();
}

const int CritterSpawnRule::get_num_critters_to_spawn(World * world, Level * level, GlobalTime * time, const int num_critters_spawned)
{
	if (this->qualify(world, level, time)) {
		const int max_spawn = this->max_spawned.value();
		const float rate = this->spawn_rate.value();
		int num_to_spawn = 0;
		for (int i = 0; i < max_spawn; i++) {
			srand(std::time(NULL) + i * 1001);
			const float roll = ((float)(std::rand() % 100)) / 100.0f;
			if (roll < rate) {
				num_to_spawn++;
			}
		}
		//TODO: random chance w/ seed
		//TODO: avoid crowding w/ other critters (incorporate level size and num critters already spawned, including those we spawn in this method)
		return num_to_spawn;
	}
	
	return 0;
}

// this can be used to make some critters only spawn at night, in certain times of year, or under other conditions
const bool CritterSpawnRule::qualify(World * world, Level * level, GlobalTime * time)
{
	const int size = this->qualifiers.size();
	for (int i = 0; i < size; i++) {
		if (!this->qualifiers.getItem(i)->evaluate(world, level)) {
			return false;
		}
	}
	return true;
}
