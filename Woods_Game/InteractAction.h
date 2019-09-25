#ifndef INTERACT_ACTION_H
#define INTERACT_ACTION_H

#include "Qualifier.h"
#include "XMLSerialization.h"
#include <functional>

struct ActionBinding : public xmls::Serializable {
	xmls::xString binding_key;
	xmls::xString binding_value;
	ActionBinding();
	ActionBinding(const std::string key, const std::string value);
};

class Level;
class World;
class InteractAction : public xmls::Serializable
{
private:
	std::map<std::string, std::function<int(InteractAction*)>> function_map;
	bool functions_intialized = false;
	xmls::xString interact_action_key;
	xmls::xString function_name;
	xmls::Collection<ActionBinding> bindings;
	xmls::Collection<Qualifier> qualifiers;
public:
	InteractAction();
	InteractAction(const std::string action_key, const std::string function_name);
	~InteractAction();
	void run_action();
	const bool evaluate(World * world, Level * level);
	const std::string get_interact_action_key();
	const std::string get_function_name();
	const std::string get_binding(const std::string key);
	std::vector<ActionBinding*> get_bindings();
	void set_bindings(std::vector<ActionBinding*> bindings);
};

#endif
