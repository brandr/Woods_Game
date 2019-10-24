#include "QuestItem.h"



QuestItem::QuestItem()
{
	setClassName("QuestItem");
	Register("item_key", &item_key);
	Register("item_description", &item_description);
	Register("item_name", &item_name);
	Register("item_display_name", &item_display_name);
	Register("inventory_pos_x", &inventory_pos_x);
	Register("inventory_pos_y", &inventory_pos_y);
	Register("item_attributes", &item_attributes);
	Register("is_obtained", &is_obtained);
	is_obtained = false;
}


QuestItem::~QuestItem()
{
}

const bool QuestItem::get_is_obtained()
{
	return this->is_obtained.value();
}

void QuestItem::set_is_obtained(const bool value)
{
	this->is_obtained = value;
}
