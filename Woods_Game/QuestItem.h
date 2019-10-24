#ifndef QUEST_ITEM_H
#define QUEST_ITEM_H

#include "ItemBase.h"

#include "XMLSerialization.h"

class QuestItem :
	public ItemBase, public xmls::Serializable
{
private:
	xmls::xBool is_obtained;
public:
	QuestItem();
	~QuestItem();
	const bool get_is_obtained();
	void set_is_obtained(const bool value);
};

#endif