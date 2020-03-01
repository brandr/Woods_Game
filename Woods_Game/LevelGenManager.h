#ifndef LEVEL_GEN_MANAGER_H
#define LEVEL_GEN_MANAGER_H

#include "FileManager.h"
#include "LevelGenData.h"

#include <map>

class LevelGenManager {
private:
	std::map<std::string, LevelGenData *> gen_data_map;
	LevelGenManager() {}
public:
	LevelGenManager(LevelGenManager const&) = delete;
	void operator=(LevelGenManager const&) = delete;
	static LevelGenManager& get_instance();
	LevelGenData * get_level_gen_data(const std::string);
};

#endif
