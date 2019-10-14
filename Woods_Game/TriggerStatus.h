#ifndef TRIGGER_STATUS_H
#define TRIGGER_STATUS_H

#define SET_HAS_MET_NPC "set_has_met_npc"

// trigger keys
#define TRIGGER_HAS_MET_NPC "has_met_npc"
#define TRIGGER_HAS_VIEWED_CUTSCENE "has_viewed_cutscene"

// trigger attributes
#define TRIGGER_ATTR_NPC_KEY "npc_key"
#define TRIGGER_ATTR_CUTSCENE_KEY "cutscene_key"

#include "XMLSerialization.h"

struct TriggerAttribute : public xmls::Serializable {
	xmls::xString attribute_key;
	xmls::xString attribute_value;
	TriggerAttribute();
	TriggerAttribute(std::string key, std::string value);
};

class TriggerStatus : public xmls::Serializable
{
private:
	xmls::xString trigger_key = "";
	xmls::xInt trigger_state;
	xmls::Collection<TriggerAttribute> trigger_attributes;
public:
	TriggerStatus();
	TriggerStatus(const std::string trigger_key);
	~TriggerStatus();
	const std::string get_trigger_key();
	const int get_trigger_state();
	void set_trigger_state(const int state);
	const bool is_empty();
	void copy_attributes(TriggerStatus * other_status);
	const bool has_attribute(const std::string attr_key);
	const std::string get_attribute(const std::string attr_key);
	std::vector<TriggerAttribute *> get_attributes();
	void set_attribute(const std::string attr_key, const std::string attr_value);
};

#endif