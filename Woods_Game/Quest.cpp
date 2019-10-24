#include "Quest.h"
#include "Level.h"
#include "World.h"


Quest::Quest()
{
	setClassName("Quest");
	Register("quest_key", &quest_key);
	Register("quest_name", &quest_name);
	Register("current_page_index", &current_page_index);
	Register("pages", &pages);
	Register("quest_updates", &quest_updates);
}


Quest::~Quest()
{
}

void Quest::update(World * world, Level * level)
{
	QuestUpdate * update_to_process = NULL;
	int highest_priority = -1;
	const int size = this->quest_updates.size();
	for (int i = 0; i < size; i++) {
		QuestUpdate * qu = this->quest_updates.getItem(i);
		if (qu->qualify(world, level)) {
			const int priority = qu->priority.value();
			if (highest_priority < 0 || highest_priority < priority) {
				highest_priority = priority;
				update_to_process = qu;
			}
		}
	}
	if (update_to_process != NULL) {
		this->set_page_index(update_to_process->page_num.value());
	}
}

const std::string Quest::get_quest_key()
{
	return this->quest_key.value();
}

const std::string Quest::get_quest_name()
{
	return this->quest_name.value();
}

const int Quest::get_quest_state()
{
	const int index = this->current_page_index.value();
	if (index < 0 || index > this->pages.size()) {
		return QUEST_STATE_INACTIVE;
	} else {
		return this->get_quest_page(index)->get_quest_state();
	}
}

void Quest::set_page_index(const int index)
{
	this->current_page_index = index;
}

QuestPage * Quest::get_quest_page(const int index)
{
	const int size = this->pages.size();
	for (int i = 0; i < size; i++) {
		QuestPage * qp = this->pages.getItem(i);
		if (index == qp->page_index.value()) {
			return qp;
		}
	}
	return NULL;
}

const bool Quest::is_active()
{
	return QUEST_STATE_ACTIVE == this->get_quest_state();
}

const bool Quest::is_failed()
{
	return QUEST_STATE_FAILED == this->get_quest_state();
}

const bool Quest::is_completed()
{
	return QUEST_STATE_SUCCEEDED == this->get_quest_state();
}

const std::string Quest::get_quest_summary()
{
	const int index = this->current_page_index.value();
	if (index < 0 || index > this->pages.size()) {
		return "";
	} else {
		return this->pages.getItem(index)->summary.value();
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

QuestUpdate::QuestUpdate()
{
	setClassName("QuestUpdate");
	Register("quest_key", &quest_key);
	Register("page_num", &page_num);
	Register("priority", &priority);
	Register("qualifiers", &qualifiers);
}

const bool QuestUpdate::qualify(World * world, Level * level)
{
	const int size = this->qualifiers.size();
	for (int i = 0; i < size; i++) {
		if (!this->qualifiers.getItem(i)->evaluate(world, level)) {
			return false;
		}
	}
	return true;
}
