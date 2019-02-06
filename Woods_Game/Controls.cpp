#include "Controls.h"
#include "allegro5\keycodes.h"

using namespace xmls;
//make sure not to call this method every frame, only when necessary
const std::vector<int> Controls::keyboard_mappable_nums()
{
	return std::vector<int>{
		// letters
		ALLEGRO_KEY_A, ALLEGRO_KEY_B, ALLEGRO_KEY_C, ALLEGRO_KEY_D, ALLEGRO_KEY_E, ALLEGRO_KEY_F, ALLEGRO_KEY_G,
		ALLEGRO_KEY_H, ALLEGRO_KEY_I, ALLEGRO_KEY_J, ALLEGRO_KEY_K, ALLEGRO_KEY_L, ALLEGRO_KEY_M, ALLEGRO_KEY_N,
		ALLEGRO_KEY_O, ALLEGRO_KEY_P, ALLEGRO_KEY_Q, ALLEGRO_KEY_R, ALLEGRO_KEY_S, ALLEGRO_KEY_T, ALLEGRO_KEY_U,
		ALLEGRO_KEY_V, ALLEGRO_KEY_W, ALLEGRO_KEY_X, ALLEGRO_KEY_Y, ALLEGRO_KEY_Z,
		// other keys
		ALLEGRO_KEY_ENTER
	};
}

const std::vector<int> Controls::controller_mappable_nums()
{
	return std::vector<int>{
		XC_BUTTON_A, XC_BUTTON_B, XC_BUTTON_X, XC_BUTTON_Y,
		XC_BUTTON_RIGHT_SHOULDER, XC_BUTTON_LEFT_SHOULDER,
		//XC_BUTTON_PAD_RIGHT, XC_BUTTON_PAD_LEFT, XC_BUTTON_PAD_DOWN, XC_BUTTON_PAD_UP
	};
}

Controls::Controls()
{
	setClassName("Controls");
	Register("ControlsMap", &controls_map);
}


Controls::~Controls()
{
	this->controls_map.Clear();
}

Controls::InputKeyToLabelMap Controls::input_key_to_label_map = {
	// keyboard
	// letters
	{ "keyboard_a", "A" },{ "keyboard_b", "B" },{ "keyboard_c", "C" },{ "keyboard_d", "D" },{ "keyboard_e", "E" },{ "keyboard_f", "F" },
	{ "keyboard_g", "G" },{ "keyboard_h", "H" },{ "keyboard_i", "I" },{ "keyboard_j", "J" },{ "keyboard_k", "K" },{ "keyboard_l", "L" },
	{ "keyboard_m", "M" },{ "keyboard_n", "N" },{ "keyboard_o", "O" },{ "keyboard_p", "P" },{ "keyboard_q", "Q" },{ "keyboard_r", "R" },
	{ "keyboard_s", "S" },{ "keyboard_t", "T" },{ "keyboard_u", "u" },{ "keyboard_v", "V" },{ "keyboard_w", "W" },{ "keyboard_x", "X" },
	{ "keyboard_y", "Y" },{ "keyboard_z", "Z" },
	// other keys
	{ "keyboard_enter", "Enter" }, { "keyboard_escape", "Escape" },{ "keyboard_space", "Space" },
	//TODO: add other keys which are allowed
	// controller
	{ "controller_a", "A Button" },
	{ "controller_b", "B Button" },
	{ "controller_x", "X Button" },
	{ "controller_y", "Y Button" },
	{ "controller_left_shoulder", "L Shoulder" },
	{ "controller_right_shoulder", "R Shoulder" },
	{ "controller_start", "Start Button" },
};

Controls::KeyboardNumToInputKeyMap Controls::keyboard_num_to_input_key_map = {
	//letters
	{ ALLEGRO_KEY_A, "keyboard_a" },{ ALLEGRO_KEY_B, "keyboard_b" },{ ALLEGRO_KEY_C, "keyboard_c" },{ ALLEGRO_KEY_D, "keyboard_d" },
	{ ALLEGRO_KEY_E, "keyboard_e" },{ ALLEGRO_KEY_F, "keyboard_f" },{ ALLEGRO_KEY_G, "keyboard_g" },{ ALLEGRO_KEY_H, "keyboard_h" },
	{ ALLEGRO_KEY_I, "keyboard_i" },{ ALLEGRO_KEY_J, "keyboard_j" },{ ALLEGRO_KEY_K, "keyboard_k" },{ ALLEGRO_KEY_L, "keyboard_l" },
	{ ALLEGRO_KEY_M, "keyboard_m" },{ ALLEGRO_KEY_N, "keyboard_n" },{ ALLEGRO_KEY_O, "keyboard_o" },{ ALLEGRO_KEY_P, "keyboard_p" },
	{ ALLEGRO_KEY_Q, "keyboard_q" },{ ALLEGRO_KEY_R, "keyboard_r" },{ ALLEGRO_KEY_S, "keyboard_s" },{ ALLEGRO_KEY_T, "keyboard_t" },
	{ ALLEGRO_KEY_U, "keyboard_u" },{ ALLEGRO_KEY_V, "keyboard_v" },{ ALLEGRO_KEY_W, "keyboard_w" },{ ALLEGRO_KEY_X, "keyboard_x" },
	{ ALLEGRO_KEY_Y, "keyboard_y" },{ ALLEGRO_KEY_Z, "keyboard_z" },
	//other keys
	{ ALLEGRO_KEY_ENTER, "keyboard_enter" }, { ALLEGRO_KEY_ESCAPE, "keyboard_escape" },{ ALLEGRO_KEY_SPACE, "keyboard_space" },
	//TODO: other allowed inputs
};

Controls::KeyboardNumToInputKeyMap Controls::controller_num_to_input_key_map = {
	{ XC_BUTTON_A, "controller_a"},{ XC_BUTTON_B, "controller_b" }, 
	{ XC_BUTTON_X, "controller_x"},{ XC_BUTTON_Y, "controller_y" },
	{ XC_BUTTON_RIGHT_SHOULDER, "controller_right_shoulder" },{ XC_BUTTON_LEFT_SHOULDER, "controller_left_shoulder" },
	//{ XC_BUTTON_PAD_RIGHT, "controller_bpr" },{ XC_BUTTON_PAD_LEFT, "controller_bpl" },
	//{ XC_BUTTON_PAD_DOWN, "controller_bpd" },{ XC_BUTTON_PAD_UP, "controller_bpu" }
};

void Controls::load_content(std::vector<std::vector<std::string>> attributes, std::vector<std::vector<std::string>> contents)
{
	//temporary
	controls_map.Clear();
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
		controls_map.addItem(new ControlMapping(action_keys[i], input_keys[i], labels[i]));
	}
	
}

void Controls::clear()
{
	controls_map.Clear();
}

std::string Controls::input_key_for_action_key(std::string action_key)
{
	const int size = controls_map.size();
	for (int i = 0; i < controls_map.size(); i++) {
		xString mapped_action_key = controls_map.getItem(i)->ActionKey;
		if (mapped_action_key.value() == action_key) {
			return controls_map.getItem(i)->InputKey.value();
		}
	}
	return "";
}

std::string Controls::action_label_for_input_key(std::string input_key)
{
	const int size = controls_map.size();
	for (int i = 0; i < controls_map.size(); i++) {
		if (controls_map.getItem(i)->InputKey.value() == input_key) {
			return controls_map.getItem(i)->ActionKey.value();
		}
	}
	return "";
}

std::string Controls::input_label_for_input_key(std::string input_key)
{
	auto it = input_key_to_label_map.find(input_key);
	if (it == input_key_to_label_map.end()) return "";
	return it->second;
}

std::string Controls::keyboard_input_key_for_input_num(int input_num)
{
	auto it = keyboard_num_to_input_key_map.find(input_num);
	if (it == keyboard_num_to_input_key_map.end()) return "";
	return it->second;
}

std::string Controls::controller_input_key_for_input_num(int input_num)
{
	auto it = controller_num_to_input_key_map.find(input_num);
	if (it == controller_num_to_input_key_map.end()) return "";
	return it->second;
}

int Controls::keyboard_input_num_for_action_key(std::string action_key)
{
	std::string input_key = this->input_key_for_action_key(action_key);
	for (auto &i : keyboard_num_to_input_key_map) {
		if (i.second == input_key) {
			return i.first;
		}
	}
	return -1;
}

int Controls::controller_input_num_for_action_key(std::string action_key)
{
	std::string input_key = this->input_key_for_action_key(action_key);
	for (auto &i : controller_num_to_input_key_map) {
		if (i.second == input_key) {
			return i.first;
		}
	}
	return -1;
}

void Controls::set_mapping_for_input(std::string action_key, std::string input_key, std::string label)
{
	int size = this->controls_map.size();
	for (int i = 0; i < size; i++) {
		ControlMapping *cm = controls_map.getItem(i);
		if (cm->ActionKey.value() == action_key) {
			cm->InputKey = input_key;
			cm->Label = label;
			return;
		}
	}
	this->controls_map.addItem(DBG_NEW ControlMapping(action_key, input_key, label));
}

ControlMapping::ControlMapping():
	ActionKey(""),
	Label(""),
	InputKey("")
{
	setClassName("ControlMapping");
	Register("ActionKey", &ActionKey);
	Register("InputKey", &InputKey);
	Register("Label", &Label);
}

ControlMapping::ControlMapping(std::string action_key, std::string input_key, std::string label):
	ActionKey(action_key),
	Label(label),
	InputKey(input_key)
{
	setClassName("ControlMapping");
	Register("ActionKey", &ActionKey);
	Register("InputKey", &InputKey);
	Register("Label", &Label);
	ActionKey = action_key;
	InputKey = input_key;
	Label = label;
}
