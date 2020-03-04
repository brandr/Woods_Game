#ifndef QUALIFIER_H
#define QUALIFIER_H

// serializable class for running boolean checks on serialized entities like NPCs

#include "GlobalTime.h"
#include "TriggerStatus.h"
#include "XMLSerialization.h"

enum COMPARATORS {
	COMPARATOR_EQUALS = 0,
	COMPARATOR_NOT_EQUALS = 1,
	COMPARATOR_LESS_THAN = 2,
	COMPARATOR_LESS_THAN_EQUAL = 3,
	COMPARATOR_GREATER_THAN = 4,
	COMPARATOR_GREATER_THAN_EQUAL = 5
};

// if you add new evaluators, make sure to put them at the end of the list
enum EVALUATORS {
	EVALUATOR_KEY_VALUE = 0,
	EVALUATOR_AND = 1,
	EVALUATOR_OR = 2,
	EVALUATOR_TIME = 3,
	EVALUATOR_TRIGGER = 4,
	EVALUATOR_DAY_OF_WEEK = 5,
	EVALUATOR_CURRENT_LEVEL = 6,
	EVALUATOR_NPC_PLACEMENT = 7,
	EVALUATOR_HAS_ITEM = 8,
	EVALUATOR_QUEST_STATE = 9,
	EVALUATOR_INVENTORY_FULL = 10,
	EVALUATOR_BLOCK_ATTRIBUTE = 11
};

struct QualifierBinding : public xmls::Serializable {
	xmls::xString binding_key;
	xmls::xString binding_value;
	QualifierBinding();
	//QualifierBinding(const std::string key, const std::string value);
};

class Level;
class Quest;
class World;
class Qualifier : public xmls::Serializable
{
protected:
	xmls::xInt comparator;
	xmls::xInt evaluator;
	xmls::Collection<Qualifier> qualifiers;
	xmls::Collection<QualifierBinding> bindings;
	xmls::xInt month;
	xmls::xInt day;
	xmls::xInt hour;
	xmls::xInt minute;
	xmls::xInt day_of_week;
	TriggerStatus trigger_status;
	xmls::xInt int_val;
	xmls::xString string_val;
	GlobalTime * other_time;
	xmls::xString npc_key;

	virtual const bool has_binding(const std::string attr_key);
	virtual const std::string get_binding(const std::string attr_key);

	virtual const bool and_evaluate(World * world, Level * level);
	virtual const bool or_evaluate(World * world, Level * level);
	virtual const bool time_evaluate();
	virtual const bool trigger_evaluate(World * world);
	virtual const bool day_of_week_evaluate();
	virtual const bool current_level_evaluate(World * world, Level * level);
	virtual const bool npc_placement_evaluate(World * world, Level * level);
	virtual const bool has_item_evaluate(World * world, Level * level);
	virtual const bool quest_state_evaluate(World * world, Level * level);
	virtual const bool inventory_full_evaluate(World * world, Level * level);
	virtual const bool block_attribute_evaluate(World * world, Level * level);	
public:
	//attributes
	static constexpr const char* Q_BINDING_BLOCK_ATTR_KEY = "block_attr_key";
	static constexpr const char* Q_BINDING_BLOCK_ATTR_VALUE = "block_attr_value";
	static constexpr const char* Q_BINDING_BLOCK_LEVEL_KEY = "block_level_key";	
	static constexpr const char* Q_BINDING_BLOCK_TX = "block_tx";
	static constexpr const char* Q_BINDING_BLOCK_TY = "block_ty";	

	Qualifier();
	~Qualifier();
	virtual const bool evaluate(World * world, Level * current_level);
	virtual const bool evaluate(const int a, const int b);
	virtual void set_other_time(GlobalTime * value);
};

#endif