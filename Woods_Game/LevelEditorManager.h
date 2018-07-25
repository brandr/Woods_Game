#ifndef LEVEL_EDITOR_MANAGER_H
#define LEVEL_EDITOR_MANAGER_H

#include "EditorActionListener.h"

#include "Agui/Widgets/Frame/Frame.hpp"
#include "Agui/Widgets/Button/Button.hpp"
#include <Agui/FlowLayout.hpp>

#include <Agui/Agui.hpp>
#include <Agui/Backends/Allegro5/Allegro5.hpp>

#include "Agui/Gui.hpp"
#include "Agui/Input.hpp"
#include "Agui/ImageLoader.hpp"

class LevelEditorManager
{
private:

	
	//gui
	agui::Gui* editor_gui;
	agui::FlowLayout flow;

	//components 

		// action listeners
	EditorActionListener dungeon_select_listener;

	agui::Button dungeon_select_button;
	//void intialize_components();
public:
	LevelEditorManager();
	LevelEditorManager(agui::Gui *guiInstance);
	~LevelEditorManager();
	void draw(ALLEGRO_DISPLAY *display);
};

#endif