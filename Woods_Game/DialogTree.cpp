#include "DialogTree.h"
#include "GlobalTime.h"
#include "World.h"

DialogTree::DialogTree()
{
	setClassName("DialogTree");
	Register("root_node", &root_node);
}


DialogTree::~DialogTree()
{
}

Dialog * DialogTree::choose_dialog(World * world, GlobalTime * time)
{
	return this->root_node.choose_dialog(world, time);
}

DialogTreeNode::DialogTreeNode()
{
	setClassName("DialogTreeNode");
	Register("dialog_text", &dialog_text);
	Register("child_nodes", &child_nodes);
	Register("qualifiers", &qualifiers);
}

DialogTreeNode::~DialogTreeNode()
{
}

Dialog * DialogTreeNode::choose_dialog(World * world, GlobalTime * time)
{
	const bool qualifies = this->qualify(world, time);
	if (qualifies) {
		if (!this->dialog_text.value().empty()) {
			return this->create_dialog();
		} else {
			const int size = this->child_nodes.size();
			for (int i = 0; i < size; i++) {
				DialogTreeNode * child = this->child_nodes.getItem(i);
				if (child->qualify(world, time)) {
					Dialog * dialog = child->choose_dialog(world, time);
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

Dialog * DialogTreeNode::create_dialog()
{
	if (!this->dialog_text.value().empty()) {
		Dialog * dialog = new Dialog();
		dialog->parse_text(this->dialog_text.value());
		return dialog;
		//TODO: avoid memory leaks
	}
	return NULL;
}

const bool DialogTreeNode::qualify(World * world, GlobalTime * time)
{
	const int size = this->qualifiers.size();
	for (int i = 0; i < size; i++) {
		Qualifier * q = this->qualifiers.getItem(i);
		q->set_other_time(time);
		if (!q->evaluate(world)) {
			return false;
		}
	}
	return true;
}
