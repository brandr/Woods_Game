#define _USE_MATH_DEFINES
#ifndef NPC_H
#define NPC_H

#include "AIBeing.h"
#include "Dialog.h"
#include "DjikstraPath.h"
#include "GlobalTime.h"
#include "NPCSchedule.h"
#include "PathNode.h"
#include "XMLSerialization.h"
#include <math.h>

class Level;
class Player;
class NPC : public AIBeing, public xmls::Serializable
{
private:
	NPCSchedule schedule;
	xmls::xString npc_key;
	xmls::xString start_level_key; // level where the NPC starts the game
	xmls::xString default_spawn_level_key;
	xmls::xString start_spawn_key;
	xmls::xString default_spawn_key;
	xmls::xString default_dialog_text;
	xmls::xBool obeys_tile_rules;

	std::string current_level_key = "";
	std::string current_position_node_key = "";
	bool is_processing = false; //TEMP. replace with AI state for AIBeing
protected:
	virtual const std::string calculate_destination_node_key(GlobalTime * time);
public:
	NPC();
	~NPC();
	virtual void update(Level * level, GlobalTime * time, const int game_mode);
	virtual void draw(ALLEGRO_DISPLAY* display, int x_offset, int y_offset);
	const std::string get_npc_key();
	const std::string get_start_level_key();
	const std::string get_current_spawn_level_key();
	const std::string get_start_spawn_key();
	const std::string get_current_spawn_key();
	virtual const bool get_obeys_tile_rules();
	Dialog * choose_dialog(Player * player);
	virtual const bool interact_action(Player * player);
	const std::string get_default_dialog_text();
	void set_current_level_key(const std::string level_key);
	const std::string get_current_level_key();
	const std::string get_current_destination_node_key();
	virtual void clear_primary_destinations();
};

#endif