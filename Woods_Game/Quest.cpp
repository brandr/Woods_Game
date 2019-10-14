#include "Quest.h"



Quest::Quest()
{
	setClassName("Quest");
	Register("quest_key", &quest_key);
	Register("quest_name", &quest_name);
	Register("current_page_index", &current_page_index);
	Register("pages", &pages);
}


Quest::~Quest()
{
}

const int Quest::get_quest_state()
{
	const int index = this->current_page_index.value();
	if (index < 0 || index > this->pages.size()) {
		return QUEST_STATE_INACTIVE;
	} else {
		return this->pages.getItem(index)->get_quest_state();
	}
}

QuestPage::QuestPage()
{
	setClassName("QuestPage");
	Register("page_index", &page_index);
	Register("quest_state", &quest_state);
	Register("summary", &summary);
}

const int QuestPage::get_quest_state()
{
	return this->quest_state.value();
}
