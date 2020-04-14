#ifndef DUNGEON_H
#define DUNGEON_H

#include "Level.h"
#include "PathNode.h"
#include "LocationMarker.h"
#include "vector"  // for vector
#include <set>
#include <memory>  // for allocator
#include "ImageLoader.h"

class World;
class Dungeon
{
private:
	std::vector<Level*> level_list;	// needed to store level objects since dungeon_grid only has pointers
	std::string dungeon_name;
public:
	std::vector<std::vector<Level*>> dungeon_grid;
	Dungeon();
	Dungeon(const std::string filepath, const std::string dungeon_name, const int default_level_width, const int default_level_height);
	~Dungeon();
	void unload_content();
	void intialize_levels(const int default_level_width, const int default_level_height);
	void intialize_levels(const std::string filepath, int default_level_width, const int default_level_height);
	void initialize_tiles();
	void generate_levels(World * world, Player * player, GlobalTime * time);
	ALLEGRO_BITMAP * generate_map_image(const int standard_level_width, const int standard_level_height);
	static Dungeon * load_dungeon(const std::string dungeon_name, const int default_level_width, const int default_level_height);
	void load_images(ImageLoader&);
	void update_new_day(World * world, Player * player, GlobalTime * time);
	void add_level(Level*);
	void remove_level(int index);
	Level* level_at(int, int);
	Level* level_at_index(const int index);
	Level* level_with_name(std::string name);
	Level* find_level_with_spawn_key(const std::string spawn_key);
	Level* find_level_with_path_node_key(const std::string node_key);
	Level* find_any_level_with_node();
	PathNode * find_path_node_with_key(const std::string node_key);
	std::vector<LocationMarker *> get_all_location_markers();
	LocationMarker * location_marker_matching_level(Level * level);
	void set_dungeon_name(std::string name);
	std::string get_dungeon_name();
	std::vector<Level*> get_level_list();
	const int get_level_list_count();
	void clear_all_beings();
	void spawn_critters(World * world, GlobalTime * time);
};

#endif