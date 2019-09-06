#ifndef WORLD_STATE_H
#define WORLD_STATE_H

#include "XMLSerialization.h"
#include "Dialog.h"
#include "Player.h"
#include "NPC.h"
#include "Qualifier.h"

class WorldState : public xmls::Serializable
{
private:
	xmls::xString world_key;
	xmls::Collection<TriggerStatus> trigger_statuses;
	Player player;
	TriggerStatus * create_trigger_status(const std::string trigger_key);
	TriggerStatus * status_for_npc(const std::string trigger_key, const std::string npc_key);
public:
	WorldState();
	~WorldState();
	Player * get_player();
	std::vector<TriggerStatus *> get_trigger_statuses(const std::string status_key);
	TriggerStatus * matching_trigger_status(TriggerStatus * status);
	void set_has_met_npc(const std::string npc_key);
};

#endif