#ifndef DIALOG_TREE_H
#define DIALOG_TREE_H

#include "Dialog.h"
#include "Qualifier.h"
#include "TriggerStatus.h"
#include "XMLSerialization.h"

#include <stdlib.h>     /* srand, rand */

struct DialogItemOption : public xmls::Serializable
{
	xmls::xString option_text;
	xmls::xInt next_page_index;
	xmls::xString option_action_key;
	TriggerStatus trigger_status;
	DialogItemOption();
	~DialogItemOption();
	const bool has_trigger_status();
};

struct DialogItemPage : public xmls::Serializable{
	xmls::xString dialog_text;
	xmls::xInt page_number;
	xmls::xInt next_page_index;
	xmls::Collection<DialogItemOption> options;
	DialogItemPage();
	~DialogItemPage();
	const bool has_text();
	const int get_page_number();
	std::vector<DialogItemOption *> get_options();
};

struct DialogItem : public xmls::Serializable
{
	xmls::xInt conversation_index;
	xmls::Collection<DialogItemPage> pages;
	DialogItem();
	~DialogItem();
	const bool has_text();
	std::vector<DialogItemPage *> get_pages();
};

struct DialogGroup : public xmls::Serializable
{
	xmls::Collection<DialogItem> dialog_items;
	xmls::xInt priority;
	DialogGroup();
	~DialogGroup();
	const bool has_dialog_text(const int index);
	DialogItem * get_dialog_item(const int index);
	const bool has_text();
	const int get_priority();
};

struct DialogTreeNode : public xmls::Serializable
{
	DialogGroup dialog_group;
	xmls::Collection<DialogTreeNode> child_nodes;
	xmls::Collection<Qualifier> qualifiers;
	DialogTreeNode();
	~DialogTreeNode();
	Dialog * choose_dialog(World * world, Level * level, GlobalTime * time, const int index);
	Dialog * create_dialog(const int index);
	DialogGroup * choose_dialog_group(World * world, Level * level, GlobalTime * time);
	std::vector<DialogGroup *> possible_dialog_groups(World * world, Level * level, GlobalTime * time);
	const bool qualify(World * world, Level * level, GlobalTime * time);
	const bool has_dialog_text(const int index);
};

class GlobalTime;
class Level;
class World;
class DialogTree : public xmls::Serializable
{
private:
	DialogTreeNode root_node;
	std::vector<DialogGroup *> possible_dialog_groups(World * world, Level * level, GlobalTime * time);
public:
	DialogTree();
	~DialogTree();
	DialogGroup * choose_dialog_group(World * world, Level * level, GlobalTime * time);
};

#endif // !DIALOG_TREE_H