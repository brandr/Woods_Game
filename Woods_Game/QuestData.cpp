#include "QuestData.h"

Quest * QuestData::quest_with_key(const std::string quest_key)
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

QuestData::QuestData()
{
	setClassName("QuestData");
	Register("quest_items", &quest_items);
	Register("quests", &quests);
}


QuestData::~QuestData()
{
}

void QuestData::reset()
{
	this->quest_items.Clear();
}

void QuestData::load_content_from_attributes()
{
	const int size = this->quest_items.size();
	for (int i = 0; i < size; i++) {
		QuestItem *item = this->quest_items.getItem(i);
		item->load_content_from_attributes();
	}
}

std::vector<QuestItem*> QuestData::get_quest_items()
{
	std::vector<QuestItem*> quest_items_vec;
	const int size = this->quest_items.size();
	for (int i = 0; i < size; i++) {
		quest_items_vec.push_back(this->quest_items.getItem(i));
	}
	return quest_items_vec;
}

QuestItem * QuestData::get_quest_item(const int inv_x, const int inv_y)
{
	for (QuestItem * qi : this->get_quest_items()) {
		if (inv_x == qi->get_inventory_pos_x() && inv_y == qi->get_inventory_pos_y()) {
			return qi;
		}
	}
	return NULL;
}

void QuestData::set_has_quest_item(const std::string item_key, const bool has_item)
{
	const int size = this->quest_items.size();
	for (int i = 0; i < size; i++) {
		QuestItem * qi = this->quest_items.getItem(i);
		if (item_key == qi->get_item_name()) {
			qi->set_is_obtained(has_item);
			return;
		}
	}
}

std::vector<Quest*> QuestData::get_quests()
{
	std::vector<Quest*> quests_vec;
	const int size = this->quests.size();
	for (int i = 0; i < size; i++) {
		quests_vec.push_back(this->quests.getItem(i));
	}
	return quests_vec;
}

// TODO: how to sort?
std::vector<Quest*> QuestData::get_active_quests()
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
