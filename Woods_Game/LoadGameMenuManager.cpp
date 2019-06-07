#include "LoadGameMenuManager.h"

LoadGameMenuManager::LoadGameMenuManager()
{
}


LoadGameMenuManager::~LoadGameMenuManager()
{
}

void LoadGameMenuManager::load_content(const std::string id)
{
}

void LoadGameMenuManager::load_xml_content(const std::string id)
{
	MenuManager::load_xml_content(id);
	const std::string load_file_dir = "resources/load/saves/";
	boost::filesystem::path p("resources/load/saves");

	boost::filesystem::directory_iterator end_itr;

	int index = 0;
	for (boost::filesystem::directory_iterator itr(p); itr != end_itr; ++itr)
	{
		const boost::filesystem::path filepath = itr->path();
		if (is_directory(filepath)) {
			const string filename = filepath.filename().string();
			MenuItem * menu_item = new MenuItem();
			menu_item->set_text(filename); //TEMP- might not want to have the "name" match the filename
			menu_item->set_menu_binding(MENU_BINDING_FILENAME, filename);
			menu_item->set_position(std::pair<float, float>(30, 30 + index * 30));
			menu_item->set_selection_action_key("confirm_load_game");
			this->menu_items.addItem(menu_item);
			index++;
		}
	}
	//TODO: save files
	/*
	FileManager file_manager = this->file_manager;
	const int size = this->menu_items.size();
	for (int i = 0; i < size; i++) {
		MenuItem *menu_item = this->menu_items.getItem(i);
		std::string action_key = menu_item->get_controls_action_key();
		if (action_key != "") {
			//TODO: how to load menu item?
		}
	}
	*/
}

void LoadGameMenuManager::draw(ALLEGRO_DISPLAY * display, float x_off, float y_off, float width, float height)
{
	MenuManager::draw(display, x_off, y_off, width, height);
	//TODO: how to draw save files??
}

const std::string LoadGameMenuManager::selected_filepath()
{
	if (this->get_selected_action_key() == MenuManager::SELECTION_KEY_CONFIRM_LOAD_GAME) {
		return this->menu_items.getItem(item_index)->get_menu_binding(MENU_BINDING_FILENAME);
	}
	return "";
}
