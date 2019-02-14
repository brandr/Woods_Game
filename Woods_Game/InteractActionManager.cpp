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

const int open_dialog(
	const InteractActionManager * manager,
	InteractAction * action,
	Player * player)
{
	//make sure we avoid memory leaks, either by deleting dialogs once they close or by keying them in a dialogmanager
	std::string dialog_text = action->get_binding("dialog_text");
	if (!dialog_text.empty()) {
		Dialog * dialog = new Dialog();
		FileManager manager;
		const std::string page_delimiter = "[p]";
		const std::string line_delimiter = "[l]";
		const std::vector< std::string> pages = manager.string_to_parts(dialog_text, page_delimiter);
		const int page_count = pages.size();
		int page_index = 0;
		for (int p = 0; p < page_count; p++) {
			bool empty_page = true;
			int line_index = 0;
			const std::string page = pages[p];
			const std::vector<std::string> lines = manager.string_to_parts(page, line_delimiter);
			const int line_count = lines.size();
			for (int l = 0; l < line_count; l++) {
				const std::string line = lines[l];
				if (line.size() > 0) {
					dialog->add_line(line, page_index, line_index);
					line_index++;
					empty_page = false;
				}
			}
			if (!empty_page) {
				page_index++;
			}
		}
		player->set_open_dialog(dialog);
		return 1;
	}
	return 0;
}

void InteractActionManager::initialize_functions()
{
	// go to level
	std::function<const int(const InteractActionManager*,
		InteractAction*,
		Player*)> fcnPtr;
	fcnPtr = go_to_level;
	this->function_map["go_to_level"] = fcnPtr;
	fcnPtr = open_dialog;
	this->function_map["open_dialog"] = fcnPtr;
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

