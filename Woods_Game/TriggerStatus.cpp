#include "TriggerStatus.h"

TriggerStatus::TriggerStatus()
{
	setClassName("TriggerStatus");
	Register("trigger_key", &trigger_key);
	Register("trigger_state", &trigger_state);
	Register("trigger_attributes", &trigger_attributes);
	//TODO: bindings
}

TriggerStatus::TriggerStatus(const std::string trig_key)
{
	setClassName("TriggerStatus");
	Register("trigger_key", &trigger_key);
	Register("trigger_state", &trigger_state);
	Register("trigger_attributes", &trigger_attributes);
	this->trigger_key = trig_key;
	//TODO: bindings
}


TriggerStatus::~TriggerStatus()
{
}

const std::string TriggerStatus::get_trigger_key()
{
	return this->trigger_key.value();
}

const int TriggerStatus::get_trigger_state()
{
	return this->trigger_state.value();
}

void TriggerStatus::set_trigger_state(const int state)
{
	this->trigger_state = state;
}

const bool TriggerStatus::has_attribute(const std::string attr_key)
{
	const int size = this->trigger_attributes.size();
	for (int i = 0; i < size; i++) {
		TriggerAttribute * attr = this->trigger_attributes.getItem(i);
		if (attr->attribute_key.value() == attr_key) {
			return true;
		}
	}
	return false;
}

const std::string TriggerStatus::get_attribute(const std::string attr_key)
{
	const int size = this->trigger_attributes.size();
	for (int i = 0; i < size; i++) {
		TriggerAttribute * attr = this->trigger_attributes.getItem(i);
		if (attr->attribute_key.value() == attr_key) {
			return attr->attribute_value.value();
		}
	}
	return "";
}

void TriggerStatus::set_attribute(const std::string attr_key, const std::string attr_value)
{
	TriggerAttribute * attr = NULL;
	if (!this->has_attribute(attr_key)) {
		attr = new TriggerAttribute(attr_key, attr_value);
		this->trigger_attributes.addItem(attr);
	} else {
		const int size = this->trigger_attributes.size();
		for (int i = 0; i < size; i++) {
			TriggerAttribute * a = this->trigger_attributes.getItem(i);
			if (a->attribute_key.value() == attr_key) {
				a->attribute_value = attr_value;
				break;
			}
		}
	}
}

TriggerAttribute::TriggerAttribute()
{
	setClassName("TriggerAttribute");
	Register("attribute_key", &attribute_key);
	Register("attribute_value", &attribute_value);
}

TriggerAttribute::TriggerAttribute(std::string key, std::string value)
{
	setClassName("TriggerAttribute");
	Register("attribute_key", &attribute_key);
	Register("attribute_value", &attribute_value);
	this->attribute_key = key, this->attribute_value = value;
}
