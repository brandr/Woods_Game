#ifndef WORLD_STATE_H
#define WORLD_STATE_H

#include "XMLSerialization.h"
#include "Dialog.h"
#include "Player.h"
#include "NPC.h"
#include "Qualifier.h"

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
	xmls::Collection<TriggerStatus> trigger_statuses;
	xmls::Collection<DungeonGridCell> explored_cells;
	Player player;
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
	void set_has_met_npc(const std::string npc_key);
	const std::set<std::pair<int, int>> explored_map();
	void mark_grid_explored(const int grid_x, const int grid_y);
	void mark_cutscene_viewed(const std::string cutscene_key);
	const bool has_viewed_cutscene(const std::string cutscene_key);
};

#endif