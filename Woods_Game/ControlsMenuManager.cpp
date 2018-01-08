#include "ControlsMenuManager.h"

using namespace xmls;

ControlsMenuManager::ControlsMenuManager()
{
	Register("ControlsID", &controls_id);
}


ControlsMenuManager::~ControlsMenuManager()
{
}

void ControlsMenuManager::load_content(std::string id)
{
	MenuManager::load_content(id);
	FileManager file_manager = this->file_manager;
	this->controls_id = id;
	if (id == "pause_controls_menu_keyboard") {
		this->load_controls("resources/load/controls", "default_controls_keyboard", &default_controls);
		this->load_controls("resources/load/controls", "current_controls_keyboard", &current_controls);
	} else if (id == "pause_controls_menu_controller"){
		this->load_controls("resources/load/controls", "default_controls_controller", &default_controls);
		this->load_controls("resources/load/controls", "current_controls_controller", &current_controls);
	}
	const int size = this->menu_items.size();
	for (int i = 0; i < size; i++) {
		MenuItem *menu_item = this->menu_items.getItem(i);
		std::string action_key = menu_item->get_controls_action_key();
		if (action_key != "") {
			std::string input_key = current_controls.input_key_for_action_key(action_key);
			if (input_key != "") {
				std::string label = current_controls.input_label_for_input_key(input_key);
				menu_item->set_controls_input_key(input_key);
				menu_item->set_controls_input_label(label);
			}
		}
	}
}

void ControlsMenuManager::load_xml_content(std::string id)
{
	MenuManager::load_xml_content(id);
	FileManager file_manager = this->file_manager;
	this->controls_id = id;
	if (id == "pause_controls_menu_keyboard") {
		this->load_controls("resources/load/controls", "default_controls_keyboard", &default_controls);
		this->load_controls("resources/load/controls", "current_controls_keyboard", &current_controls);
	}
	else if (id == "pause_controls_menu_controller") {
		this->load_controls("resources/load/controls", "default_controls_controller", &default_controls);
		this->load_controls("resources/load/controls", "current_controls_controller", &current_controls);
	}
	const int size = this->menu_items.size();
	for (int i = 0; i < size; i++) {
		MenuItem *menu_item = this->menu_items.getItem(i);
		std::string action_key = menu_item->get_controls_action_key();
		if (action_key != "") {
			std::string input_key = current_controls.input_key_for_action_key(action_key);
			if (input_key != "") {
				std::string label = current_controls.input_label_for_input_key(input_key);
				menu_item->set_controls_input_key(input_key);
				menu_item->set_controls_input_label(label);
			}
		}
	}
}

void ControlsMenuManager::reset()
{
	const std::string id = this->controls_id.value();
	std::vector<std::vector<std::string>> default_controls_attributes;
	std::vector<std::vector<std::string>> default_controls_contents;
	std::vector<std::vector<std::string>> current_controls_attributes;
	std::vector<std::vector<std::string>> current_controls_contents;
	default_controls.clear();
	current_controls.clear();
	if (id == "pause_controls_menu_keyboard") {
		this->load_controls("resources/load/controls", "default_controls_keyboard", &default_controls);
		this->load_controls("resources/load/controls", "current_controls_keyboard", &current_controls);
	}
	else if (id == "pause_controls_menu_controller") {
		this->load_controls("resources/load/controls", "default_controls_controller", &default_controls);
		this->load_controls("resources/load/controls", "current_controls_controller", &current_controls);
	}
	const int size = this->menu_items.size();
	for (int i = 0; i < size; i++) {
		MenuItem *menu_item = this->menu_items.getItem(i);
		std::string action_key = menu_item->get_controls_action_key();
		if (action_key != "") {
			std::string input_key = current_controls.input_key_for_action_key(action_key);
			if (input_key != "") {
				std::string label = current_controls.input_label_for_input_key(input_key);
				menu_item->set_controls_input_key(input_key);
				menu_item->set_controls_input_label(label);
			}
		}
	}
	MenuManager::reset();
}

void ControlsMenuManager::load_controls(std::string filename, std::string controls_key, Controls *controls)
{
	file_manager.load_xml_content(controls, filename,
		"SerializableClass", "ControlsKey", controls_key);
}

void ControlsMenuManager::save_controls()
{
	std::string controls_key = "";
	if (this->controls_id.value() == "pause_controls_menu_keyboard") {
		controls_key = "current_controls_keyboard";
	} else if (this->controls_id.value() == "pause_controls_menu_controller") {
		controls_key = "current_controls_controller";
	}
	std::string xml_string = this->current_controls.toXML();
	std::string filename = "resources/load/controls";
	file_manager.replace_xml_content(filename, "SerializableClass", "ControlsKey", controls_key, xml_string);
}

void ControlsMenuManager::revert_controls_default()
{
	this->current_controls.clear();
	if (this->controls_id.value() == "pause_controls_menu_keyboard") {
		this->load_controls("resources/load/controls", "default_controls_keyboard", &current_controls);
	} else if (this->controls_id.value() == "pause_controls_menu_controller") {
		this->load_controls("resources/load/controls", "default_controls_controller", &current_controls);
	}
	this->save_controls();
	this->reset();
}

void ControlsMenuManager::set_keyboard_mappable_input(int keycode)
{
	//TODO: refactor this and controller version into a single function
	if (!this->is_selecting_input()) return;
	std::string input_key = current_controls.keyboard_input_key_for_input_num(keycode);
	if (input_key != "") {
		std::string input_label = current_controls.input_label_for_input_key(input_key);
		const int size = this->menu_items.size();
		for (int i = 0; i < size; i++) {
			MenuItem* menu_item = this->menu_items.getItem(i);
			std::string action_key = menu_item->get_controls_action_key();
			if (action_key != "" && i != this->item_index) {
				std::string current_input_key = menu_item->get_controls_input_key();
				// check for another item that has the same input key we are trying to set
				if (current_input_key != "" && current_input_key == input_key) {
					std::string old_input_key = this -> menu_items.getItem(this->item_index)->get_controls_input_key();
					std::string old_input_label = this -> menu_items.getItem(this->item_index)->get_controls_input_label();
					set_controls_mapping(this -> menu_items.getItem(i), old_input_key, old_input_label);
					break;
				}
			}
		}
		set_controls_mapping(this->menu_items.getItem(this->item_index), input_key, input_label);
	}
	this->set_selecting_input(false);
}

void ControlsMenuManager::set_controller_mappable_input(int keycode)
{
	if (!this->is_selecting_input()) return;
	std::string input_key = current_controls.controller_input_key_for_input_num(keycode);
	if (input_key != "") {
		std::string input_label = current_controls.input_label_for_input_key(input_key);
		const int size = this->menu_items.size();
		for (int i = 0; i < size; i++) {
			MenuItem* menu_item = this->menu_items.getItem(i);
			std::string action_key = menu_item->get_controls_action_key();
			if (action_key != "" && i != this->item_index) {
				std::string current_input_key = menu_item->get_controls_input_key();
				// check for another item that has the same input key we are trying to set
				if (current_input_key != "" && current_input_key == input_key) {
					std::string old_input_key = this->menu_items.getItem(this->item_index)->get_controls_input_key();
					std::string old_input_label = this->menu_items.getItem(this->item_index)->get_controls_input_label();
					set_controls_mapping(this->menu_items.getItem(i), old_input_key, old_input_label);
					break;
				}
			}
		}
		set_controls_mapping(this->menu_items.getItem(this->item_index), input_key, input_label);
	}
	this->set_selecting_input(false);
}

void ControlsMenuManager::set_controls_mapping(MenuItem * menu_item, std::string input_key, std::string input_label)
{
	menu_item->set_controls_input_key(input_key);
	menu_item->set_controls_input_label(input_label);
	std::string action_label = menu_item->get_text();
	std::string action_key = menu_item->get_controls_action_key();
	this->current_controls.set_mapping_for_input(action_key, input_key, action_label);
}
