#ifndef WORLD_H
#define WORLD_H

#include "Dungeon.h"
#include "DungeonData.h"
#include "NPC.h"
#include "PathNode.h"
#include "XMLSerialization.h"
#include "FileManager.h"
#include <memory>
class ImageLoader;
class Player;
class World : public xmls::Serializable
{
private:
	std::vector <std::shared_ptr<Dungeon>> dungeons;
	xmls::xString world_key;
	xmls::xString current_dungeon_key;
	xmls::xInt current_day;
	xmls::xString player_key;
	xmls::xInt current_level_grid_x;
	xmls::xInt current_level_grid_y;
	xmls::Collection<DungeonData> dungeon_data;
	xmls::Collection<NPC> npcs;
	
	Level * find_level_with_spawn_key(const std::string spawn_key);
	Level * find_level_with_path_node_key(const std::string node_key);
	PathNode * find_path_node_with_key(const std::string node_key);
	const std::string get_npc_destination_node_key(NPC * npc);
	void clear_all_beings();
public:	
	Dungeon* current_dungeon;
	World();
	~World();
	void load_dungeons();
	void reload_dungeons(const std::string dungeons_path);
	void load_npcs();
	void load_player();
	void generate_levels();
	void unload_content();
	void load_images(ImageLoader&);
	// updates per frame
	void npc_update();
	// updates per new day
	void update_new_day(Player * player);
	void update_npcs_new_day();
	void add_dungeon(Dungeon*);
	Dungeon* get_current_dungeon();
	Level* get_current_level();
	Level * extract_current_level(Player * player);
	Level * get_level_with_key(const std::string level_key);
	Level * get_npc_destination_level(NPC * npc);
	PathNode * get_npc_destination_node(NPC * npc);
	Dungeon* get_dungeon(std::string dungeon_name);
	std::vector<std::shared_ptr<Dungeon>> get_dungeons();
	std::string get_player_key();
	const std::string get_world_key();
	const int get_current_day();
	void set_current_day(const int day);
};

#endif