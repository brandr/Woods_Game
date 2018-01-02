#ifndef CONTROLSMENUMANAGER_H
#define CONTROLSMENUMANAGER_H

#include "MenuManager.h"
#include "Controls.h"
#include "xstring"        // for string
#include "XMLSerialization.h"

class MenuItem;

class ControlsMenuManager :
	public MenuManager
{
private:

	Controls default_controls;
	Controls current_controls;
	xmls::xString controls_id;
public:
	ControlsMenuManager();
	~ControlsMenuManager();
	virtual void load_content(std::string id);
	virtual void load_xml_content(std::string id);
	virtual void reset();
	void load_controls(std::string filename, std::string controls_key, Controls *controls);
	virtual void save_controls();
	virtual void revert_controls_default();
	virtual void set_keyboard_mappable_input(int keycode);
	virtual void set_controller_mappable_input(int keycode);
	void set_controls_mapping(MenuItem* menu_item, std::string input_key, std::string input_label);
};

#endif