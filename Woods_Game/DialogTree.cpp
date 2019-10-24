#include "DialogTree.h"
#include "GlobalTime.h"
#include "Level.h"
#include "World.h"

std::vector<DialogGroup*> DialogTree::possible_dialog_groups(World * world, Level * level, GlobalTime * time)
{
	return this->root_node.possible_dialog_groups(world, level, time);
}

DialogTree::DialogTree()
{
	setClassName("DialogTree");
	Register("root_node", &root_node);
}


DialogTree::~DialogTree()
{
}

DialogGroup * DialogTree::choose_dialog_group(World * world, Level * level, GlobalTime * time)
{
	std::vector<DialogGroup *> possible_groups = this->possible_dialog_groups(world, level, time);
	std::map<int, std::vector<DialogGroup *>> groups_map;
	int highest_priority = -1;
	for (DialogGroup * dg : possible_groups) {
		std::vector<DialogGroup *> priority_groups;
		const int priority = dg->get_priority();
		if (groups_map.find(priority) != groups_map.end()) {
			priority_groups = groups_map[priority];
		}
		priority_groups.push_back(dg);
		groups_map[priority] = priority_groups;
		if (highest_priority < priority) {
			highest_priority = priority;
		}
	}
	if (groups_map.find(highest_priority) != groups_map.end()) {
		std::vector<DialogGroup *> highest_priority_groups = groups_map[highest_priority];
		const int size = highest_priority_groups.size();
		if (size > 0) {
			srand(std::time(NULL));
			const int roll_index = rand() % size;
			return highest_priority_groups[roll_index];
		}
	}
	return NULL;
}

DialogTreeNode::DialogTreeNode()
{
	setClassName("DialogTreeNode");
	Register("dialog_group", &dialog_group);
	Register("child_nodes", &child_nodes);
	Register("qualifiers", &qualifiers);
}

DialogTreeNode::~DialogTreeNode()
{
}

Dialog * DialogTreeNode::choose_dialog(World * world, Level * level, GlobalTime * time, const int index)
{
	const bool qualifies = this->qualify(world, level, time);
	if (qualifies) {
		if (this->has_dialog_text(index)) {
			return this->create_dialog(index);
		} else {
			const int size = this->child_nodes.size();
			for (int i = 0; i < size; i++) {
				DialogTreeNode * child = this->child_nodes.getItem(i);
				if (child->qualify(world, level, time)) {
					Dialog * dialog = child->choose_dialog(world, level, time, index);
					// this extra step is important, because the child might be a branch that qualifies, 
					// but have no children that qualify
					if (dialog != NULL) {
						return dialog;
					}
				}
			}
		}
	}
	return NULL;
}

Dialog * DialogTreeNode::create_dialog(const int index)
{
	if (this->has_dialog_text(index)) {
		DialogItem * item = this->dialog_group.get_dialog_item(index);
		if (item != NULL) {
			Dialog * dialog = new Dialog();
			dialog->parse_dialog(item);
			return dialog;
			//TODO: avoid memory leaks
		}
	}
	return NULL;
}

DialogGroup * DialogTreeNode::choose_dialog_group(World * world, Level * level, GlobalTime * time)
{
	const bool qualifies = this->qualify(world, level, time);
	if (qualifies) {
		if (this->dialog_group.has_text()) {
			return &(this->dialog_group);
		} else {
			const int size = this->child_nodes.size();
			for (int i = 0; i < size; i++) {
				DialogTreeNode * child = this->child_nodes.getItem(i);
				if (child->qualify(world, level, time)) {
					DialogGroup * dialog_group = child->choose_dialog_group(world, level, time);
					// this extra step is important, because the child might be a branch that qualifies, 
					// but have no children that qualify
					if (dialog_group != NULL) {
						return dialog_group;
					}
				}
			}
		}
	}
	return NULL;
}

std::vector<DialogGroup*> DialogTreeNode::possible_dialog_groups(World * world, Level * level, GlobalTime * time)
{
	std::vector<DialogGroup*> groups;
	const bool qualifies = this->qualify(world, level, time);
	if (qualifies) {
		if (this->dialog_group.has_text()) {
			groups.push_back(&(this->dialog_group));
		} else {
			const int size = this->child_nodes.size();
			for (int i = 0; i < size; i++) {
				DialogTreeNode * child = this->child_nodes.getItem(i);
				if (child->qualify(world, level, time)) {
					std::vector<DialogGroup *> child_groups = child->possible_dialog_groups(world, level, time);
					const int child_size = child_groups.size();
					for (int j = 0; j < child_size; j++) {
						DialogGroup * dialog_group = child_groups[j];
						if (dialog_group != NULL) {
							groups.push_back(dialog_group);
						}
					}
				}
			}
		}
	}
	return groups;
}

const bool DialogTreeNode::qualify(World * world, Level * level, GlobalTime * time)
{
	const int size = this->qualifiers.size();
	for (int i = 0; i < size; i++) {
		Qualifier * q = this->qualifiers.getItem(i);
		q->set_other_time(time);
		if (!q->evaluate(world, level)) {
			return false;
		}
	}
	return true;
}

const bool DialogTreeNode::has_dialog_text(const int index)
{
	return this->dialog_group.has_dialog_text(index);
}

DialogGroup::DialogGroup()
{
	setClassName("DialogGroup");
	Register("dialog_items", &dialog_items);
	Register("priority", &priority);
}

DialogGroup::~DialogGroup()
{
}

const bool DialogGroup::has_dialog_text(const int index)
{
	DialogItem * item = this->get_dialog_item(index);
	if (item != NULL) {
		return item->has_text();
	}
	return false;
}

DialogItem * DialogGroup::get_dialog_item(const int index)
{
	const int size = this->dialog_items.size();
	for (int i = 0; i < size; i++) {
		DialogItem * dialog_item = this->dialog_items.getItem(i);
		const int convo_index = dialog_item->conversation_index.value();
		if (index == convo_index) {
			return dialog_item;
		}
	}
	return NULL;
}

const bool DialogGroup::has_text()
{
	const int size = this->dialog_items.size();
	return size > 0 && this->dialog_items.getItem(0)->has_text();
}

const int DialogGroup::get_priority()
{
	return this->priority.value();
}

DialogItem::DialogItem()
{
	setClassName("DialogItem");
	Register("conversation_index", &conversation_index);
	Register("pages", &pages);
}

DialogItem::~DialogItem()
{
}

const bool DialogItem::has_text()
{
	const int page_count = this->pages.size();
	for (int i = 0; i < page_count; i++) {
		DialogItemPage * page = this->pages.getItem(i);
		if (page->has_text()) {
			return true;
		}
	}
	return false;
}

std::vector<DialogItemPage*> DialogItem::get_pages()
{
	std::vector<DialogItemPage *> dialog_pages;
	const int size = this->pages.size();
	for (int i = 0; i < size; i++) {
		dialog_pages.push_back(this->pages.getItem(i));
	}
	return dialog_pages;
}

DialogItemOption::DialogItemOption()
{
	setClassName("DialogItemOption");
	Register("option_text", &option_text);
	Register("next_page_index", &next_page_index);
	Register("option_action_key", &option_action_key);
	Register("trigger_status", &trigger_status);
}

DialogItemOption::~DialogItemOption()
{
}

const bool DialogItemOption::has_trigger_status()
{
	return !this->trigger_status.is_empty();
}

DialogItemPage::DialogItemPage()
{
	setClassName("DialogItemPage");
	Register("dialog_text", &dialog_text);
	Register("page_number", &page_number);
	Register("next_page_index", &next_page_index);
	Register("options", &options);
	Register("quest_updates", &quest_updates);
	Register("dialog_actions", &dialog_actions);
	//Register("dialog_action", &dialog_action); 
}

DialogItemPage::~DialogItemPage()
{
}

const bool DialogItemPage::has_text()
{
	return this->dialog_text.value().size() > 0;
}

const int DialogItemPage::get_page_number()
{
	return this->page_number.value();
}

std::vector<DialogItemOption*> DialogItemPage::get_options()
{
	std::vector<DialogItemOption *> dialog_options;
	const int size = this->options.size();
	for (int i = 0; i < size; i++) {
		dialog_options.push_back(this->options.getItem(i));
	}
	return dialog_options;
}

std::vector<QuestUpdate*> DialogItemPage::get_quest_updates()
{
	std::vector<QuestUpdate*> updates;
	const int size = this->quest_updates.size();
	for (int i = 0; i < size; i++) {
		updates.push_back(this->quest_updates.getItem(i));
	}
	return updates;
}

std::vector<InteractAction*> DialogItemPage::get_dialog_actions()
{
	std::vector<InteractAction*> actions;
	const int size = this->dialog_actions.size();
	for (int i = 0; i < size; i++) {
		actions.push_back(this->dialog_actions.getItem(i));
	}
	return actions;
}