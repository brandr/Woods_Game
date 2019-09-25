#ifndef WORLD_H
#define WORLD_H

#include "DjikstraPath.h"
#include "DjiktraPathData.h"
#include "Dungeon.h"
#include "DungeonData.h"
#include "FileManager.h"
#include "GlobalTime.h"
#include "NPC.h"
#include "PathNode.h"
#include "WorldState.h"
#include "XMLSerialization.h"
#include "FileManager.h"
#include <future>
#include <memory>
#include <set>
#include "allegro5/threads.h"

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
	xmls::xInt default_level_width;
	xmls::xInt default_level_height;
	xmls::xInt current_level_grid_x;
	xmls::xInt current_level_grid_y;
	xmls::Collection<DungeonData> dungeon_data;
	xmls::Collection<NPC> npcs;

	WorldState world_state; //not serialzed in world because it's day-specific

	std::map<std::string, PathNodeDjikstraPath *> node_djikstra_path_map;
	std::map<std::string, TileDjikstraPath *> tile_djikstra_path_map;

	std::map<std::string, DjikstraPathData *> djikstra_path_data_map;
	std::map<std::string, ALLEGRO_THREAD * > djikstra_path_thread_map;
	
	PathNodeDjikstraPath * get_node_djikstra_path(NPC * npc);
	PathNodeDjikstraPath * get_mapped_node_djikstra_path(const std::string npc_key);
	const std::string get_node_djikstra_path_key(const std::string npc_key);
	TileDjikstraPath * get_tile_djikstra_path(Level * level, NPC * npc);
	TileDjikstraPath * get_mapped_tile_djikstra_path(Level * level, NPC * npc);
	const std::string get_tile_djikstra_path_key(Level * level, NPC * npc);
	const bool calculate_npc_pathing(NPC * npc, Level * current_npc_level);
	const bool calculate_npc_pathing(NPC * npc, Level * current_npc_level, TileDjikstraPath * tile_djikstra_path, const std::string dest_node_id, const std::string dest_level_key);
	void finish_pathing(NPC * npc);
	void move_npc_to_level(NPC * npc, Level * current_level, Level * dest_level, const std::pair<int, int> position, const std::pair<int, int> offset,
		const bool animate_walking);
	Level * find_level_with_spawn_key(const std::string spawn_key);
	PathNode * find_path_node_with_key(const std::string node_key);
	const std::string get_npc_destination_node_key(NPC * npc);
public:	
	Dungeon* current_dungeon;
	World();
	~World();
	void load_dungeons();
	void load_dungeons(const std::string filepath);
	void reload_dungeons(const std::string dungeons_path);
	void reload_world_state(const std::string world_state_path);
	void load_npcs();
	void recalculate_npc_paths();
	void load_player();
	void generate_levels();
	void generate_map_images();
	void unload_content();
	void load_images(ImageLoader&);
	static void save_game(World * world, GlobalTime * global_time);
	// updates per frame
	void npc_update(GlobalTime * time, const int game_mode);
	// updates per new day
	void update_new_day(Player * player, const std::string current_level_key);
	void update_reload_day(Player * player, const std::string current_level_key);
	void update_npcs_new_day();
	void add_dungeon(Dungeon*);
	Player * get_player();
	NPC * get_npc(const std::string npc_key);
	const bool npc_is_on_node(const std::string npc_key, const std::string node_key);
	void set_has_met_npc(const std::string npc_key);
	Dungeon* get_current_dungeon();
	Level* get_current_level();
	Level * extract_current_level(Player * player, const std::string previous_level_key);
	Level * get_level_with_key(const std::string level_key);
	Level * get_npc_destination_level(NPC * npc);
	Level * find_level_with_path_node_key(const std::string node_key);
	Level * find_any_level_with_node();
	PathNode * get_npc_destination_node(NPC * npc);
	Level * level_containing_location_marker(LocationMarker *marker);
	LocationMarker * location_marker_matching_level(Level * level);
	const std::pair<int, int> map_location_for_level(Level * level);
	void clear_all_beings();
	Dungeon* get_dungeon(std::string dungeon_name);
	std::vector<std::shared_ptr<Dungeon>> get_dungeons();
	std::string get_player_key();
	const std::string get_world_key();
	const int get_current_day();
	void set_current_day(const int day);
	WorldState * get_world_state();
	TriggerStatus * matching_trigger_status(TriggerStatus * status);
	const int get_default_level_width();
	const int get_default_level_height();
	void mark_grid_explored(const int grid_x, const int grid_y);
	void mark_cutscene_viewed(const std::string cutscene_key);
	const bool has_viewed_cutscene(const std::string cutscene_key);
	const std::set<std::pair<int, int>> explored_map();
};

#endif