#include "WorldState.h"

TriggerStatus * WorldState::create_trigger_status(const std::string trigger_key)
{
	TriggerStatus * status = new TriggerStatus(trigger_key);
	return status;
}

TriggerStatus * WorldState::status_for_npc(const std::string trigger_key, const std::string npc_key)
{
	std::vector<TriggerStatus *> statuses = this->get_trigger_statuses(trigger_key);
	for (TriggerStatus * status : statuses) {
		if (status->get_attribute(TRIGGER_ATTR_NPC_KEY) == npc_key) {
			return status;
		}
	}
	return NULL;
}

WorldState::WorldState()
{
	setClassName("WorldState");
	Register("world_key", &world_key);
	Register("trigger_statuses", &trigger_statuses);
	Register("player", &player);
}


WorldState::~WorldState()
{
}

Player * WorldState::get_player()
{
	return &player;
}

std::vector<TriggerStatus*> WorldState::get_trigger_statuses(const std::string status_key)
{
	std::vector<TriggerStatus*> statuses;
	const int size = this->trigger_statuses.size();
	for (int i = 0; i < size; i++) {
		TriggerStatus * status = this->trigger_statuses.getItem(i);
		if (status->get_trigger_key() == status_key) {
			statuses.push_back(status);
		}
	}
	return statuses;
}

TriggerStatus * WorldState::matching_trigger_status(TriggerStatus * other_status)
{
	const std::string other_key = other_status->get_trigger_key();
	const int size = this->trigger_statuses.size();
	for (int i = 0; i < size; i++) {
		TriggerStatus * status = this->trigger_statuses.getItem(i);
		if (status->get_trigger_key() == other_key) {
			if (status->get_trigger_key() == TRIGGER_HAS_MET_NPC) {
				//TODO: if this comparison gets much messier, make a matches() methods in TriggerStatus that handles this logic
				if (status->get_attribute(TRIGGER_ATTR_NPC_KEY) == other_status->get_attribute(TRIGGER_ATTR_NPC_KEY)) {
					return status;
				}
			} else {
				return status;
			}
		}
	}
	return NULL;
}

void WorldState::set_has_met_npc(const std::string npc_key)
{
	TriggerStatus * status = this->status_for_npc(TRIGGER_HAS_MET_NPC, npc_key);
	if (status == NULL) {
		status = this->create_trigger_status(TRIGGER_HAS_MET_NPC);
		status->set_attribute(TRIGGER_ATTR_NPC_KEY, npc_key);
		this->trigger_statuses.addItem(status);
	}
	status->set_trigger_state(1);
}