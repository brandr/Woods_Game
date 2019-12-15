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

const int gather_plant(
	const InteractActionManager * manager,
	InteractAction * action,
	Player * player,
	Entity * actor)
{
	if (actor != NULL && player != NULL) {
		if (player->gather_plant(actor)) {
			return 1;
		}
	}
	return 0;
}

const int remove_item(
	const InteractActionManager * manager,
	InteractAction * action,
	Player * player,
	Entity * actor)
{
	if (action != NULL && player != NULL) {
		const std::string item_key = action->get_binding("item_key");
		if (!item_key.empty()) {
			const int item_key_int = ::atoi(item_key.c_str());
			if (player->has_item_with_key(item_key_int)) {
				player->remove_item_with_key(item_key_int);
				return 1;
			}
		}
	}
	return 0;
}

const int add_item(
	const InteractActionManager * manager,
	InteractAction * action,
	Player * player,
	Entity * actor)
{
	if (action != NULL && player != NULL) {
		const std::string item_key = action->get_binding("item_key");
		if (!item_key.empty()) {
			const int item_key_int = ::atoi(item_key.c_str());
			if (!player->get_inventory().is_full()) {
				player->add_item_with_key(item_key_int);
				return 1;
			}
		}
	}
	return 0;
}

const int set_has_quest_item(
	const InteractActionManager * manager,
	InteractAction * action,
	Player * player,
	Entity * actor)
{
	if (action != NULL && player != NULL) {
		const std::string quest_item_key = action->get_binding("quest_item_key");
		const std::string has_item = action->get_binding("has_quest_item");
		if (!quest_item_key.empty() && !has_item.empty()) {
			const int has_flag = ::atoi(has_item.c_str());
			player->add_pending_quest_item_update(quest_item_key, has_flag);
			return 1;
		}
	}
	return 0;
}

const int adjust_player_money(
	const InteractActionManager * manager,
	InteractAction * action,
	Player * player,
	Entity * actor)
{
	if (action != NULL && player != NULL) {
		const std::string money_amount_str = action->get_binding("money_amount");
		if (!money_amount_str.empty()) {
			const float money_amount = ::atof(money_amount_str.c_str());
			player->adjust_money(money_amount);
			return 1;
		}
	}
	return 0;
}

const int exchange_inventory(
	const InteractActionManager * manager,
	InteractAction * action,
	Player * player,
	Entity * actor)
{
	if (action != NULL && player != NULL) {
		const std::string inventory_key = action->get_binding("inventory_key");
		if (!inventory_key.empty()) {
			player->set_exchange_inventory_key(inventory_key);
			return 1;
		}
	}
	return 0;
}

const int play_sound(
	const InteractActionManager * manager,
	InteractAction * action,
	Player * player,
	Entity * actor)
{
	if (action != NULL && player != NULL) {
		const std::string sound_key = action->get_binding("sound_key");
		const std::string duration_str = action->get_binding("sound_duration");
		const int duration = ::atoi(duration_str.c_str());
		if (!sound_key.empty() && duration > 0) {
			const std::string filename = "entity_sounds/" + sound_key;
			const int require_moving = ::atoi(action->get_binding("require_moving").c_str());
			if (!require_moving || player->is_moving()) {
				player->emit_sound(filename, duration, false);
			} else {
				player->stop_sound(filename);
			}
			
			return 1;
		}
	}
	return 0;
}

const int toggle_light(
	const InteractActionManager * manager,
	InteractAction * action,
	Player * player,
	Entity * actor)
{
	if (action != NULL) {
		const std::string light_filter_key = action->get_binding("light_filter_key");
		if (!light_filter_key.empty()) {
			actor->set_entity_attribute(Entity::E_ATTR_PENDING_LIGHT_FILTER, ::atoi(light_filter_key.c_str()));
			return 1;
		}
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
	fcnPtr = gather_plant;
	this->function_map["gather_plant"] = fcnPtr;
	fcnPtr = remove_item;
	this->function_map["remove_item"] = fcnPtr;
	fcnPtr = add_item;
	this->function_map["add_item"] = fcnPtr;
	fcnPtr = set_has_quest_item;
	this->function_map["set_has_quest_item"] = fcnPtr;
	fcnPtr = adjust_player_money;
	this->function_map["adjust_player_money"] = fcnPtr;
	fcnPtr = exchange_inventory;
	this->function_map["exchange_inventory"] = fcnPtr;
	fcnPtr = play_sound;
	this->function_map["play_sound"] = fcnPtr;
	fcnPtr = toggle_light;
	this->function_map["toggle_light"] = fcnPtr;
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

