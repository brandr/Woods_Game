#include "LevelGenManager.h"

LevelGenManager & LevelGenManager::get_instance()
{
	static LevelGenManager manager;
	return manager;
}

LevelGenData * LevelGenManager::get_level_gen_data(const std::string data_key)
{
	if (gen_data_map.find(data_key) == gen_data_map.end()) {
		FileManager fm;
		LevelGenData * gen_data = new LevelGenData();
		const std::string filename = "resources/load/level_gen_data";
		fm.load_xml_content(gen_data, filename, "SerializableClass", "GenDataKey", data_key);
		gen_data_map[data_key] = gen_data;
		return gen_data;
	} else {
		return gen_data_map[data_key];
	}
}
