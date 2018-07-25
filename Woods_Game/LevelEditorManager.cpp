#include "LevelEditorManager.h"

LevelEditorManager::LevelEditorManager()
{
}

LevelEditorManager::LevelEditorManager(agui::Gui *guiInstance)
{
	//TODO: if these get cumbersome, factor them out into another class specifically for setting up gui components
	editor_gui = guiInstance;
	editor_gui->add(&flow);

	// dungeon button
	flow.add(&dungeon_select_button);
	dungeon_select_button.setSize(200, 40);
	dungeon_select_button.setText("Select Dungeon");
	dungeon_select_button.setLocation(20, 20);

	dungeon_select_listener.set_listener_type(SELECT_DUNGEON);
	dungeon_select_button.addActionListener(&dungeon_select_listener);
	//TODO
}

LevelEditorManager::~LevelEditorManager()
{
}

void LevelEditorManager::draw(ALLEGRO_DISPLAY * display)
{
	this->editor_gui->logic();
	this->editor_gui->render();
}