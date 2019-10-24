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

TriggerStatus * WorldState::status_for_cutscene(const std::string trigger_key, const std::string cutscene_key)
{
	std::vector<TriggerStatus *> statuses = this->get_trigger_statuses(trigger_key);
	for (TriggerStatus * status : statuses) {
		if (status->get_attribute(TRIGGER_ATTR_CUTSCENE_KEY) == cutscene_key) {
			return status;
		}
	}
	return NULL;
}

WorldState::WorldState()
{
	setClassName("WorldState");
	Register("world_key", &world_key);
	Register("player", &player);
	Register("inventories", &inventories);
	Register("trigger_statuses", &trigger_statuses);
	Register("explored_cells", &explored_cells);
	Register("quests", &quests);
}


WorldState::~WorldState()
{
}

void WorldState::reset()
{
	this->trigger_statuses.Clear();
	this->explored_cells.Clear();
	this->quests.Clear();
	const int inv_size = this->inventories.size();
	for (int i = 0; i < inv_size; i++) {
		Inventory * inv = this->inventories.getItem(i);
		inv->reset();
	}
	this->inventories.Clear();
	this->player.reset_serialized_data();
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

void WorldState::copy_trigger_status(TriggerStatus * other_status)
{
	TriggerStatus * status = this->matching_trigger_status(other_status);
	if (status == NULL) {
		status = this->create_trigger_status(other_status->get_trigger_key());
		this->trigger_statuses.addItem(status);
	}
	status->set_trigger_state(other_status->get_trigger_state());
	status->copy_attributes(other_status);
	//status->Copy(other_status); //TODO: make sure this does what we want (may be better to copy attributes individually)
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

const std::set<std::pair<int, int>> WorldState::explored_map()
{
	std::set<std::pair<int, int>> map;
	const int size = this->explored_cells.size();
	for (int i = 0; i < size; i++) {
		DungeonGridCell * cell = this->explored_cells.getItem(i);
		map.insert(std::pair<int, int>(cell->grid_x.value(), cell->grid_y.value()));
	}
	return map;
}

void WorldState::mark_grid_explored(const int grid_x, const int grid_y)
{
	const int size = this->explored_cells.size();
	for (int i = 0; i < size; i++) {
		DungeonGridCell * cell = this->explored_cells.getItem(i);
		if (grid_x == cell->grid_x.value() && grid_y == cell->grid_y.value()) {
			return;
		}
	}
	DungeonGridCell * explored_cell = new DungeonGridCell();
	explored_cell->grid_x = grid_x, explored_cell->grid_y = grid_y;
	this->explored_cells.addItem(explored_cell);
}

void WorldState::mark_cutscene_viewed(const std::string cutscene_key)
{
	TriggerStatus * status = this->status_for_cutscene(TRIGGER_HAS_VIEWED_CUTSCENE, cutscene_key);
	if (status == NULL) {
		status = this->create_trigger_status(TRIGGER_HAS_VIEWED_CUTSCENE);
		status->set_attribute(TRIGGER_ATTR_CUTSCENE_KEY, cutscene_key);
		this->trigger_statuses.addItem(status);
	}
	status->set_trigger_state(1);
}

const bool WorldState::has_viewed_cutscene(const std::string cutscene_key)
{
	TriggerStatus * status = this->status_for_cutscene(TRIGGER_HAS_VIEWED_CUTSCENE, cutscene_key);
	if (status == NULL) {
		return false;
	} else {
		return status->get_trigger_state() > 0;
	}
}

// TODO: if inventories can be different sizes, pass in width and height as args
Inventory * WorldState::get_inventory_for_key(const std::string inv_key, const bool create_if_missing)
{
	if (!inv_key.empty()) {
		const int size = this->inventories.size();
		for (int i = 0; i < size; i++) {
			Inventory * inventory = this->inventories.getItem(i);
			if (inv_key == inventory->get_inventory_key()) {
				return inventory;
			}
		}
		if (create_if_missing) {
			Inventory * inv = new Inventory();
			inv->set_inventory_key(inv_key);
			inv->initialize_empty();
			inventories.addItem(inv);
			return inventories.getItem(inventories.size() - 1);
			//TODO: what needs to be done to initialize?
		}
	}
	return NULL;
}

std::vector<Quest*> WorldState::get_quests()
{
	std::vector<Quest*> all_quests;
	const int size = this->quests.size();
	for (int i = 0; i < size; i++) {
		Quest * q = this->quests.getItem(i);
		all_quests.push_back(q);
	}
	return all_quests;
}

void WorldState::process_quest_update(QuestUpdate * quest_update)
{
	const std::string quest_key = quest_update->quest_key.value();
	Quest * quest = this->quest_with_key(quest_key);
	if (quest != NULL) {
		quest->set_page_index(quest_update->page_num.value());
	}
}

Quest * WorldState::quest_with_key(const std::string quest_key)
{
	const int size = this->quests.size();
	for (int i = 0; i < size; i++) {
		Quest * q = this->quests.getItem(i);
		if (quest_key == q->get_quest_key()) {
			return q;
		}
	}
	return NULL;
}

// TODO: how to sort?
std::vector<Quest*> WorldState::get_active_quests()
{
	std::vector<Quest*> quests_vec;
	const int size = this->quests.size();
	for (int i = 0; i < size; i++) {
		Quest * q = this->quests.getItem(i);
		if (q->is_active()) {
			quests_vec.push_back(q);
		}
	}
	return quests_vec;
}

std::vector<Quest*> WorldState::get_failed_quests()
{
	std::vector<Quest*> quests_vec;
	const int size = this->quests.size();
	for (int i = 0; i < size; i++) {
		Quest * q = this->quests.getItem(i);
		if (q->is_failed()) {
			quests_vec.push_back(q);
		}
	}
	return quests_vec;
}

std::vector<Quest*> WorldState::get_completed_quests()
{
	std::vector<Quest*> quests_vec;
	const int size = this->quests.size();
	for (int i = 0; i < size; i++) {
		Quest * q = this->quests.getItem(i);
		if (q->is_completed()) {
			quests_vec.push_back(q);
		}
	}
	return quests_vec;
}


DungeonGridCell::DungeonGridCell()
{
	setClassName("DungeonGridCell");
	Register("grid_x", &grid_x);
	Register("grid_y", &grid_y);
}

DungeonGridCell::~DungeonGridCell()
{
}
