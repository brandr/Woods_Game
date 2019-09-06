#ifndef DIALOG_TREE_H
#define DIALOG_TREE_H

#include "Dialog.h"
#include "Qualifier.h"
#include "XMLSerialization.h"

struct DialogTreeNode : public xmls::Serializable
{
	xmls::xString dialog_text;
	xmls::Collection<DialogTreeNode> child_nodes;
	xmls::Collection<Qualifier> qualifiers;
	DialogTreeNode();
	~DialogTreeNode();
	Dialog * choose_dialog(World * world, GlobalTime * time);
	Dialog * create_dialog();
	const bool qualify(World * world, GlobalTime * time);
};

class GlobalTime;
class World;
class DialogTree : public xmls::Serializable
{
private:
	DialogTreeNode root_node;
public:
	DialogTree();
	~DialogTree();
	Dialog * choose_dialog(World * world, GlobalTime * time);
};

#endif // !DIALOG_TREE_H