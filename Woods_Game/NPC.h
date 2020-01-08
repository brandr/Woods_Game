#define _USE_MATH_DEFINES
#ifndef NPC_H
#define NPC_H

#include "AIBeing.h"
#include "Dialog.h"
#include "DialogTree.h"
#include "DjikstraPath.h"
#include "GlobalTime.h"
#include "NPCSchedule.h"
#include "PathNode.h"
#include "XMLSerialization.h"
#include <math.h>

class Level;
class Player;
class World;
class NPC : public AIBeing, public xmls::Serializable
{
private:
	NPCSchedule schedule;
	xmls::xString npc_key;
	xmls::xString start_level_key; // level where the NPC starts the game
	xmls::xString default_spawn_level_key;
	xmls::xString start_spawn_key;
	xmls::xString default_spawn_key;
	DialogTree dialog_tree;
	xmls::xString default_dialog_text;
	xmls::xBool obeys_tile_rules;
	std::string current_level_key = "";
	std::string current_position_node_key = "";
	bool is_processing = false; //TEMP. replace with AI state for AIBeing
	int current_dialog_index = 0;
	DialogGroup * current_dialog_group = NULL;
protected:
	virtual const std::string calculate_destination_node_key(World * world, GlobalTime * time);
	virtual const bool calculate_close_enough_to_node(World * world, Level * level, GlobalTime * time, const std::string node_key);
	virtual const bool should_wander(World * world, Level * level, GlobalTime * time);
	virtual WanderZone * current_wander_zone(World * world, Level * level, GlobalTime * time);
	virtual const std::pair<int, int> get_wander_zone_center(World * world, Level * level, GlobalTime * time);
	virtual const int forced_animation_state(World * world, Level * level, GlobalTime * time);
	virtual const int forced_animation_direction(World * world, Level * level, GlobalTime * time);
	virtual const std::string get_footstep_filename_suffix();
public:
	NPC();
	~NPC();
	virtual void update(World * world, Level * level, GlobalTime * time, const int game_mode);
	virtual void draw(ALLEGRO_DISPLAY* display, const int x_offset, const int y_offset, const int screen_w, const int screen_h);
	const std::string get_npc_key();
	const std::string get_start_level_key();
	const std::string get_current_spawn_level_key();
	const std::string get_start_spawn_key();
	const std::string get_current_spawn_key();
	virtual const bool get_obeys_tile_rules();
	Dialog * choose_dialog(World * world, GlobalTime * time, Player * player);
	virtual const bool interact_action(World * world, Level * level, GlobalTime * time, Player * player);
	const std::string get_default_dialog_text();
	void set_current_level_key(const std::string level_key);
	const std::string get_current_level_key();
	const std::string get_current_destination_node_key();
	virtual void clear_primary_destinations();
	virtual const std::string get_sound_key();
};

#endif