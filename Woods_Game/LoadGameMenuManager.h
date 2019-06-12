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
public:
	LoadGameMenuManager();
	~LoadGameMenuManager();
	virtual void load_content(const std::string id);
	virtual void load_xml_content(const std::string id);
	virtual void draw(ALLEGRO_DISPLAY *display, float x_off, float y_off, float width, float height);
	virtual const std::string selected_filepath();
};

#endif
