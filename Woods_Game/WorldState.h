#ifndef WORLD_STATE_H
#define WORLD_STATE_H

#include "XMLSerialization.h"
#include "Dialog.h"
#include "Player.h"
#include "NPC.h"
#include "Qualifier.h"
#include "Quest.h"

#include <set>

struct DungeonGridCell : public xmls::Serializable {
	xmls::xInt grid_x, grid_y;
	DungeonGridCell();
	~DungeonGridCell();
};

class WorldState : public xmls::Serializable
{
private:
	xmls::xString world_key;
	Player player;
	xmls::Collection<Inventory> inventories;
	xmls::Collection<TriggerStatus> trigger_statuses;
	xmls::Collection<DungeonGridCell> explored_cells;
	xmls::Collection<Quest> quests;
	
	
	TriggerStatus * create_trigger_status(const std::string trigger_key);
	TriggerStatus * status_for_npc(const std::string trigger_key, const std::string npc_key);
	TriggerStatus * status_for_cutscene(const std::string trigger_key, const std::string cutscene_key);
public:
	WorldState();
	~WorldState();
	void reset();
	Player * get_player();
	std::vector<TriggerStatus *> get_trigger_statuses(const std::string status_key);
	TriggerStatus * matching_trigger_status(TriggerStatus * status);
	void copy_trigger_status(TriggerStatus * status);
	void set_has_met_npc(const std::string npc_key);
	const std::set<std::pair<int, int>> explored_map();
	void mark_grid_explored(const int grid_x, const int grid_y);
	void mark_cutscene_viewed(const std::string cutscene_key);
	const bool has_viewed_cutscene(const std::string cutscene_key);
	Inventory * get_inventory_for_key(const std::string inv_key, const bool create_if_missing);
	std::vector<Quest *> get_quests();
	void process_quest_update(QuestUpdate * quest_update);
	Quest * quest_with_key(const std::string key);
	std::vector<Quest *> get_active_quests();
	std::vector<Quest *> get_failed_quests();
	std::vector<Quest *> get_completed_quests();
};

#endif