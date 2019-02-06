#ifndef EDITOR_ACTION_LISTENER_H
#define EDITOR_ACTION_LISTENER_H

#include "FileDialogManager.h"
#include "Dungeon.h"
#include "TileSet.h"

#include <Agui/ActionListener.hpp>

enum ListenerTypes
{
	LOAD_DUNGEON, SAVE_DUNGEON, ADD_DUNGEON, DELETE_DUNGEON, 
	ADD_LEVEL, DELETE_LEVEL,
	LOAD_TILESET, SAVE_TILESET,
	RESET_LEVEL, RESET_TILE_EDGES,
	SAVE_INSTANCE_ATTRIBUTES,
	SHEET_COL_LEFT, SHEET_COL_RIGHT
};

enum FlagTypes
{
	SHOULD_UPDATE
};

class EditorActionListener : public agui::ActionListener
{
private:
	ALLEGRO_DISPLAY * display;
	FileDialogManager * dialog_manager;
	int listener_type;
	std::map<int, int> flags;
public:
	EditorActionListener();
	virtual void actionPerformed(const agui::ActionEvent &evt);
	void set_listener_type(int value);
	int get_listener_type();
	void set_display(ALLEGRO_DISPLAY *display);
	void set_dialog_manager(FileDialogManager *value);
	Dungeon * selected_dungeon();
	void unset_flag(int flag);
	bool has_flag(int flag);
};

#endif