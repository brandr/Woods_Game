#ifndef INTERACT_ACTION_MANAGER_H
#define INTERACT_ACTION_MANAGER_H

#include <functional>
#include "InteractAction.h"
#include "Dialog.h"
#include "GlobalTime.h"

class Player;
class InteractActionManager
{
private:
	InteractActionManager() {}
	std::map<std::string, std::function<const int(
		const InteractActionManager*, 
		InteractAction*,
		Player*)>> function_map;
	void initialize_functions();
	GlobalTime * current_time;
public:
	InteractActionManager(InteractActionManager const&) = delete;
	void operator=(InteractActionManager const&) = delete;
	static InteractActionManager& get_instance();
	~InteractActionManager();
	const bool run_action(InteractAction * action, Player * game_image_manager) const;
	const bool run_action(const std::string action_key, Player * game_image_manager) const;
	const bool run_action(const std::string action_key, std::vector<ActionBinding *> bindings, Player * game_image_manager) const;
	void update_current_time(GlobalTime * other_time);
	GlobalTime * get_current_time();
};

#endif