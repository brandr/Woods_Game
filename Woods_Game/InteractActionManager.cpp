#include "InteractActionManager.h"
#include "Player.h"
#include "Entity.h"

const int go_to_level(
	const InteractActionManager * manager, 
	InteractAction * action,
	Player * player,
	Entity * actor)
{	
	const std::string level_key = action->get_binding("destination_level_key");
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

const int open_dialog(
	const InteractActionManager * manager,
	InteractAction * action,
	Player * player,
	Entity * actor)
{
	//make sure we avoid memory leaks, either by deleting dialogs once they close or by keying them in a dialogmanager
	std::string dialog_text = action->get_binding("dialog_text");
	if (!dialog_text.empty()) {
		Dialog * dialog = new Dialog();
		dialog->parse_text(dialog_text);
		player->set_open_dialog(dialog);
		return 1;
	}
	return 0;
}

const int close_dialog(
	const InteractActionManager * manager,
	InteractAction * action,
	Player * player,
	Entity * actor)
{
	player->close_dialog();
	return 1;
}

const int player_sleep(
	const InteractActionManager * manager,
	InteractAction * action,
	Player * player,
	Entity * actor)
{
	player->close_dialog();
	player->sleep_in_bed(InteractActionManager::get_instance().get_current_time());
	return 1;
}

const int player_open_calendar(
	const InteractActionManager * manager,
	InteractAction * action,
	Player * player,
	Entity * actor)
{
	player->open_calendar();
	return 1;
}

const int load_selected_day(
	const InteractActionManager * manager,
	InteractAction * action,
	Player * player,
	Entity * actor)
{
	if (action != NULL) {
		player->close_calendar();
		const std::string day_index_str = action->get_binding("load_day_index");
		player->load_game_for_day(::atoi(day_index_str.c_str()));
	}
	return 1;
}

const int plant_day_update(
	const InteractActionManager * manager,
	InteractAction * action,
	Player * player,
	Entity * actor)
{
	if (action != NULL && actor != NULL) {
		const int broken = actor->get_entity_attribute(GameImage::E_ATTR_BROKEN);
		if (broken < 1) {
			actor->mark_needs_plant_day_update();
		}
	}
	return 1;
}

const int trigger_cutscene(
	const InteractActionManager * manager,
	InteractAction * action,
	Player * player,
	Entity * actor)
{
	if (action != NULL && player != NULL) {
		const std::string cutscene_key = action->get_binding("cutscene_key");
		player->set_active_cutscene_key(cutscene_key);
		return 1;
	}
	return 0;
}

void InteractActionManager::initialize_functions()
{
	std::function<const int(const InteractActionManager*,
		InteractAction*,
		Player*, Entity*)> fcnPtr;
	fcnPtr = go_to_level;
	this->function_map["go_to_level"] = fcnPtr;
	fcnPtr = open_dialog;
	this->function_map["open_dialog"] = fcnPtr;
	fcnPtr = close_dialog;
	this->function_map["close_dialog"] = fcnPtr;
	fcnPtr = player_sleep;
	this->function_map["player_sleep"] = fcnPtr;
	fcnPtr = player_open_calendar;
	this->function_map["player_open_calendar"] = fcnPtr;
	fcnPtr = load_selected_day;
	this->function_map["load_selected_day"] = fcnPtr;
	fcnPtr = plant_day_update;
	this->function_map["plant_day_update"] = fcnPtr;
	fcnPtr = trigger_cutscene;
	this->function_map["trigger_cutscene"] = fcnPtr;
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

const bool InteractActionManager::run_action(World * world, Level * level, const std::string action_key, std::vector<ActionBinding*> bindings, Player * player) const
{
	std::function<const int(const InteractActionManager*, InteractAction*, Player*, Entity*)> fcnPtr
		= this->function_map.at(action_key);
	InteractAction * action = new InteractAction();
	action->set_bindings(bindings);
	if (!action->evaluate(world, level)) {
		return false;
	}
	return fcnPtr(this, action, player, NULL);
}

const bool InteractActionManager::run_action(World * world, Level * level, InteractAction * action, Player * player, Entity * actor) const
{
	if (!action->evaluate(world, level)) {
		return false;
	}
	const std::string action_key = action->get_function_name();
	std::function<const int(const InteractActionManager*, InteractAction*, Player*, Entity*)> fcnPtr
		= this->function_map.at(action_key);
	return fcnPtr(this, action, player, actor);
}

void InteractActionManager::update_current_time(GlobalTime * other_time)
{
	if (other_time != NULL) {
		if (this->current_time == NULL) {
			this->current_time = new GlobalTime(other_time->get_day(), other_time->get_time());
		}
		else {
			this->current_time->copy(other_time);
		}
	}
}

GlobalTime * InteractActionManager::get_current_time()
{
	return this->current_time;
}

