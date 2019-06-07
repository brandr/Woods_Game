#ifndef INTERACT_ACTION_MANAGER_H
#define INTERACT_ACTION_MANAGER_H

#include <functional>
#include "InteractAction.h"
#include "Dialog.h"
#include "GlobalTime.h"

class Entity;
class Player;
class InteractActionManager
{
private:
	InteractActionManager() {}
	std::map<std::string, std::function<const int(
		const InteractActionManager*, 
		InteractAction*,
		Player*, Entity*)>> function_map;
	void initialize_functions();
	GlobalTime * current_time;
public:
	InteractActionManager(InteractActionManager const&) = delete;
	void operator=(InteractActionManager const&) = delete;
	static InteractActionManager& get_instance();
	~InteractActionManager();
	const bool run_action(InteractAction * action, Player * player) const;
	const bool run_action(const std::string action_key, Player * player) const;
	const bool run_action(const std::string action_key, std::vector<ActionBinding *> bindings, Player * player) const;
	const bool run_action(InteractAction * action, Player * player, Entity * actor) const;
	void update_current_time(GlobalTime * other_time);
	GlobalTime * get_current_time();
};

#endif