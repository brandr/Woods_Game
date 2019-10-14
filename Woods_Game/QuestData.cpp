#include "QuestData.h"

//TODO: load quest data from its own file and save to that file when saving game
		// can probably maintain active quest data in gameimagemanager or world -- just don't serialize it
QuestData::QuestData()
{
	setClassName("QuestData");
	Register("quest_items", &quest_items);
	//TODO: quest items
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
