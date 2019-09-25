#include "InteractAction.h"
#include "Level.h"
#include "World.h"

InteractAction::InteractAction()
{
	this->setClassName("InteractAction");
	this->Register("InteractActionKey", &(this->interact_action_key));
	this->Register("FunctionName", &(this->function_name));
	this->Register("Bindings", &(this->bindings));
	this->Register("Qualifiers", &(this->qualifiers));
}

InteractAction::InteractAction(const std::string action_key, const std::string fcn_name)
{
	this->setClassName("InteractAction");
	this->Register("InteractActionKey", &(this->interact_action_key));
	this->Register("FunctionName", &(this->function_name));
	this->Register("Bindings", &(this->bindings));
	this->Register("Qualifiers", &(this->qualifiers));
	this->interact_action_key = action_key;
	this->function_name = fcn_name;
}


InteractAction::~InteractAction()
{
}

int go_to_level(InteractAction * action)
{
	//TODO: how do we get the argument list? (probably need to talk to level manager or game image manager)
	return 1;
}

void InteractAction::run_action()
{
	const std::string action_key = this->function_name.value();

	std::function<int(InteractAction*)> fcnPtr = this->function_map[action_key];
	fcnPtr(this);
}

const bool InteractAction::evaluate(World * world, Level * level)
{
	const int size = this->qualifiers.size();
	for (int i = 0; i < size; i++) {
		if (!this->qualifiers.getItem(i)->evaluate(world, level)) {
			return false;
		}
	}
	return true;
}

const std::string InteractAction::get_interact_action_key()
{
	return this->interact_action_key.value();
}

const std::string InteractAction::get_function_name()
{
	return this->function_name.value();
}

const std::string InteractAction::get_binding(const std::string key)
{;
	const int size = this->bindings.size();
	for (int i = 0; i < size; i++) {
		ActionBinding * b = this->bindings.getItem(i);
		if (b->binding_key.value() == key) {
			return b->binding_value.value();
		}
	}
	return "";
}

std::vector<ActionBinding*> InteractAction::get_bindings()
{
	std::vector<ActionBinding*> bindings;
	const int size = this->bindings.size();
	for (int i = 0; i < size; i++) {
		ActionBinding * b = this->bindings.getItem(i);
		bindings.push_back(b);
	}
	return bindings;
}

void InteractAction::set_bindings(std::vector<ActionBinding*> bindings)
{
	this->bindings.Clear();
	const int size = bindings.size();
	for (int i = 0; i < size; i++) {
		this->bindings.addItem(bindings[i]);
	}
}

ActionBinding::ActionBinding()
{
	this->setClassName("ActionBinding");
	this->Register("BindingKey", &(this->binding_key));
	this->Register("BindingValue", &(this->binding_value));
}

ActionBinding::ActionBinding(const std::string key, const std::string value)
{
	this->setClassName("ActionBinding");
	this->Register("BindingKey", &(this->binding_key));
	this->Register("BindingValue", &(this->binding_value));
	this->binding_key = key;
	this->binding_value = value;
}
