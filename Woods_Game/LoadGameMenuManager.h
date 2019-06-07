#ifndef LOAD_GAME_MENU_MANAGER
#define LOAD_GAME_MENU_MANAGER

#include <iostream>
#include "MenuManager.h"
#include "Controls.h"
#include "xstring"        // for string
#include "XMLSerialization.h"
#include "boost/filesystem.hpp"

enum SAVE_FILE_KEYS {
	SAVE_FILE_1 = 1, SAVE_FILE_2 = 2, SAVE_FILE_3 = 3
};

class LoadGameMenuManager :
	public MenuManager
{
private:
	//Controls default_controls;
	//Controls current_controls;
	//xmls::xString controls_id;
public:
	LoadGameMenuManager();
	~LoadGameMenuManager();
	virtual void load_content(const std::string id);
	virtual void load_xml_content(const std::string id);
	virtual void draw(ALLEGRO_DISPLAY *display, float x_off, float y_off, float width, float height);
	virtual const std::string selected_filepath();
	//virtual void reset();
	//void load_controls(std::string filename, std::string controls_key, Controls *controls);
	//virtual void save_controls();
	//virtual void revert_controls_default();
	//virtual void set_keyboard_mappable_input(int keycode);
	//virtual void set_controller_mappable_input(int keycode);
	//void set_controls_mapping(MenuItem* menu_item, std::string input_key, std::string input_label);
};

#endif
