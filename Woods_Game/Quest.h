#ifndef QUEST_H
#define QUEST_H

#include "Qualifier.h"
#include "XMLSerialization.h"

enum QUEST_STATES {
	QUEST_STATE_INACTIVE = 0,
	QUEST_STATE_ACTIVE = 1,
	QUEST_STATE_FAILED = 2,
	QUEST_STATE_SUCCEEDED = 3
};

class Level;
class World;
struct QuestUpdate : public xmls::Serializable {
	xmls::xString quest_key;
	xmls::xInt page_num;
	xmls::xInt priority = 0;
	xmls::Collection<Qualifier> qualifiers;
	QuestUpdate();
	const bool qualify(World * world, Level * level);
};

struct QuestPage : public xmls::Serializable {
	xmls::xInt page_index;
	xmls::xInt quest_state;
	xmls::xString summary;
	QuestPage();
	const int get_quest_state();
};

class Quest : public xmls::Serializable
{
private:
	xmls::xString quest_key;
	xmls::xString quest_name;
	xmls::xInt current_page_index = -1; // -1 means the quest is inactive
	xmls::Collection<QuestPage> pages;
	xmls::Collection<QuestUpdate> quest_updates;
public:
	Quest();
	~Quest();
	void update(World * world, Level * level);
	const std::string get_quest_key();
	const std::string get_quest_name();
	const int get_quest_state();
	void set_page_index(const int index);
	QuestPage * get_quest_page(const int index);
	const bool is_active();
	const bool is_failed();
	const bool is_completed();
	const std::string get_quest_summary();
};

#endif