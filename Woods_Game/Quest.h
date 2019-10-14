#ifndef QUEST_H
#define QUEST_H

#include "XMLSerialization.h"

enum QUEST_STATES {
	QUEST_STATE_INACTIVE = 0,
	QUEST_STATE_ACTIVE = 1,
	QUEST_STATE_FAILED = 2,
	QUEST_STATE_SUCCEEDED = 3
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
	// TODO: branching works by "next index" rules (compare to dialog trees and cutscene trees)
	// TODO: store displayable summary on page
public:
	Quest();
	~Quest();
	const int get_quest_state();
};

#endif