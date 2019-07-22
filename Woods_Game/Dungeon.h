#ifndef DUNGEON_H
#define DUNGEON_H

#include "Level.h"
#include "PathNode.h"
#include "vector"  // for vector
#include <memory>  // for allocator
#include "ImageLoader.h"

class Dungeon
{
private:
	std::vector<Level*> level_list;	// needed to store level objects since dungeon_grid only has pointers
	std::string dungeon_name;
public:
	std::vector<std::vector<Level*>> dungeon_grid;
	Dungeon();
	Dungeon(std::string dungeon_name);
	~Dungeon();
	void unload_content();
	void intialize_levels();
	void generate_levels();
	static Dungeon * load_dungeon(std::string dungeon_name);
	void load_images(ImageLoader&);
	void update_new_day(Player * player);
	void add_level(Level*);
	void remove_level(int index);
	Level* level_at(int, int);
	Level* level_at_index(const int index);
	Level* level_with_name(std::string name);
	Level* find_level_with_spawn_key(const std::string spawn_key);
	Level* find_level_with_path_node_key(const std::string node_key);
	Level* find_any_level_with_node();
	PathNode * find_path_node_with_key(const std::string node_key);
	void set_dungeon_name(std::string name);
	std::string get_dungeon_name();
	std::vector<Level*> get_level_list();
	const int get_level_list_count();
	void clear_all_beings();
};

#endif