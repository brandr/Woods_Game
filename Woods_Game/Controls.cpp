#include "Controls.h"

Controls::Controls()
{
}


Controls::~Controls()
{
}

//TODO: consider sending in attributes and contents instead
void Controls::load_content(std::vector<std::vector<std::string>> attributes, std::vector<std::vector<std::string>> contents)
{
	const int size = attributes.size();
	std::vector<std::string> labels;
	std::vector<std::string> input_keys;
	std::vector<std::string> action_keys;
	for (int i = 0; i < size; i++) {
		const int i_size = attributes[i].size();
		for (int j = 0; j < i_size; j++) {
			
			if (attributes[i][j] == "label") {
				labels.push_back(contents[i][j]);
			}
			else if (attributes[i][j] == "action_key") {
				action_keys.push_back(contents[i][j]);
			}
			else if (attributes[i][j] == "input_key") {
				input_keys.push_back(contents[i][j]);
			}
		}
	}

	const int mapping_size = labels.size();
	for (int i = 0; i < mapping_size; i++) {
		controls_map[input_keys[i]] = ControlMapping(labels[i], action_keys[i]);
		std::cout << "mapped input: " << controls_map[input_keys[i]].label.c_str() << "\n";
	}
	
	//TODO
	/*
	std::string controls_id = "";
	switch (mapping_type) {
	case DEFAULT_CONTROL_MAPPING:
		controls_id = "default_controls_";
		break;
	case CURRENT_CONTROL_MAPPING:
		controls_id = "current_controls_";
		break;
	}
	switch (input_type) {
	case KEYBOARD_INPUT_SOURCE:
		controls_id += "keyboard";
		break;
	case CONTROLLER_INPUT_SOURCE:
		controls_id = "controller";
		break;
	}
	*/
	
}

ControlMapping::ControlMapping()
{
}

ControlMapping::ControlMapping(std::string label, std::string action_key)
{
	this->label = label, this->action_key = action_key;
}
