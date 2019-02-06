#include "InteractActionManager.h"
#include "Player.h"

const int go_to_level(
	const InteractActionManager * manager, 
	InteractAction * action,
	Player * player)
{	
	std::string level_key = action->get_binding("destination_level_key");
	if (!level_key.empty()) {
		std::string x_str = action->get_binding("destination_level_pos_x");
		std::string y_str = action->get_binding("destination_level_pos_y");
		if (!x_str.empty() && !y_str.empty()) {
			const int x = ::atoi(x_str.c_str()), y = ::atoi(y_str.c_str());
			player->set_exit_level_flag(true);
			player->set_destination_level_key_override(level_key);
			player->set_destination_level_pos_override(std::pair<int, int>(x, y));
			return 1;
		}
	}
	return 0;
}

void InteractActionManager::initialize_functions()
{
	// go to level
	std::function<const int(const InteractActionManager*, 
		InteractAction*,
		Player*)> fcnPtr = go_to_level;
	fcnPtr = go_to_level;
	this->function_map["go_to_level"] = fcnPtr;
}

InteractActionManager & InteractActionManager::get_instance()
{
	static InteractActionManager manager;
	manager.initialize_functions();
	return manager;
}

InteractActionManager::~InteractActionManager()
{
}

const bool InteractActionManager::run_action(InteractAction * action, Player * player) const
{
	const std::string action_key = action->get_function_name();
	std::function<const int(const InteractActionManager*, InteractAction*, Player*)> fcnPtr 
		= this->function_map.at(action_key);
	return fcnPtr(this, action, player);
}

