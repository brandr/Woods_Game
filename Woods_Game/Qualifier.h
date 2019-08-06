#ifndef QUALIFIER_H
#define QUALIFIER_H

// abstract serializable class for running boolean checks on serialized entities like NPCs

//TODO: and/or qualifiers
	// should serialize regular qualifiers in collections, 
	// but how to specify type while serializing them? (might just have to test)

#include "GlobalTime.h"
#include "XMLSerialization.h"

enum COMPARATORS {
	COMPARATOR_EQUALS,
	COMPARATOR_NOT_EQUALS,
	COMPARATOR_LESS_THAN,
	COMPARATOR_LESS_THAN_EQUAL,
	COMPARATOR_GREATER_THAN,
	COMPARATOR_GREATER_THAN_EQUAL
};

enum EVALUATORS {
	EVALUATOR_KEY_VALUE,
	EVALUATOR_AND,
	EVALUATOR_OR,
	EVALUATOR_TIME
};

class Qualifier : public xmls::Serializable
{
protected:
	xmls::xInt comparator;
	xmls::xInt evaluator;
	xmls::Collection<Qualifier> qualifiers;
	xmls::xInt month;
	xmls::xInt day;
	xmls::xInt hour;
	xmls::xInt minute;
	GlobalTime * other_time;
	virtual const bool and_evaluate();
	virtual const bool or_evaluate();
	virtual const bool time_evaluate();
public:
	Qualifier();
	~Qualifier();
	virtual const bool evaluate();
	virtual const bool evaluate(const int a, const int b);
	virtual void set_other_time(GlobalTime * value);
};

#endif
