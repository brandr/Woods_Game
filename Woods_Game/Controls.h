#ifndef CONTROLS_H
#define CONTROLS_H

#include "XMLSerialization.h"  // for xString, Serializable, Collection
#include "xstring"             // for string
#include "InputManager.h"

struct ControlMapping : public xmls::Serializable {
	xmls::xString Label, ActionKey, InputKey;
public:
	ControlMapping();
	ControlMapping(std::string label, std::string action_key, std::string input_key);
};

// a remappable set of controls for the main gameplay
class Controls: public xmls::Serializable
{
private:
	//xString ControlsKey;
	xmls::Collection<ControlMapping> controls_map;
	//xmls::xString controls_key;
	typedef std::map<std::string, std::string> InputKeyToLabelMap;
	static InputKeyToLabelMap input_key_to_label_map;
	typedef std::map<int, std::string> KeyboardNumToInputKeyMap;		//note: may want to rename this since we use it for the controller, too
	static KeyboardNumToInputKeyMap keyboard_num_to_input_key_map;
	static KeyboardNumToInputKeyMap controller_num_to_input_key_map;
public:
	const static std::vector<int> keyboard_mappable_nums();
	const static std::vector<int> controller_mappable_nums();
	Controls();
	~Controls();
	void load_content(std::vector<std::vector<std::string>> attributes, std::vector<std::vector<std::string>> contents);
	void clear();
	//void set_controls_key();
	// mapping methods
	std::string input_key_for_action_key(std::string action_key);
	std::string action_label_for_input_key(std::string input_key);
	std::string input_label_for_input_key(std::string input_key);
	std::string keyboard_input_key_for_input_num(int input_num);
	std::string controller_input_key_for_input_num(int input_num);
	int keyboard_input_num_for_action_key(std::string action_key);
	int controller_input_num_for_action_key(std::string action_key);
	void set_mapping_for_input(std::string input_key, std::string label, std::string action_key);
};

#endif