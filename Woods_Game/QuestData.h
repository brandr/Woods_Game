#ifndef QUEST_DATA_H
#define QUEST_DATA_H

// unlike worldstate, questdata stays the same after reloading a save via the calendar.
// it should be updated when saving the game by sleeping

#include "QuestItem.h"
#include "Quest.h"

#include "XMLSerialization.h"

class QuestData : public xmls::Serializable
{
private:
	xmls::Collection<QuestItem> quest_items;
	xmls::Collection<Quest> quests;
	Quest * quest_with_key(const std::string quest_key);
public:
	QuestData();
	~QuestData();
	void reset();
	void load_content_from_attributes();
	std::vector<QuestItem *> get_quest_items();
	QuestItem * get_quest_item(const int inv_x, const int inv_y);
	void set_has_quest_item(const std::string item_key, const bool has_item);
	std::vector<Quest *> get_quests();
	std::vector<Quest *> get_active_quests();
};

#endif