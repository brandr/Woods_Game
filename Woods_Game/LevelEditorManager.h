#ifndef LEVEL_EDITOR_MANAGER_H
#define LEVEL_EDITOR_MANAGER_H

#include "Dungeon.h"
#include "TileType.h"
#include "Entity.h"
#include "EditorActionListener.h"
#include "LevelEditorGrid.h"
#include "GameImageDisplay.h"
#include "DungeonEditorLayout.h"
#include "LevelEditorLayout.h"
#include "TilesetEditorLayout.h"
#include "LevelEditorDataManager.h"

#include <allegro5/display.h>

#include "Agui/Widgets/Frame/Frame.hpp"
#include "Agui/Widgets/Button/Button.hpp"
#include <Agui/Widgets/TextField/TextField.hpp>
#include <Agui/Widgets/Label/Label.hpp>
#include <Agui/Widgets/ListBox/ListBox.hpp>
#include <Agui/Widgets/ScrollPane/ScrollPane.hpp>
#include <Agui/Widgets/CheckBox/CheckBox.hpp>
#include <Agui/Widgets/Tab/Tab.hpp>
#include <Agui/Widgets/Tab/TabbedPane.hpp>
#include <Agui/Widgets/ImageWidget/ImageWidget.hpp>

#include <Agui/Backends/Allegro5/Allegro5.hpp>

#include "Agui/Gui.hpp"
#include "Agui/Input.hpp"
#include <Agui/FlowLayout.hpp>
#include "Agui/ImageLoader.hpp"
#include <Agui/EmptyWidget.hpp>

class LevelEditorManager
{
	const int DEFAULT_WINDOW_WIDTH = 1200;
	const int DEFAULT_WINDOW_HEIGHT = 800;

private:
	bool needs_update = false;
	FileManager file_manager;
	
	//gui
	agui::Gui* editor_gui;

	// tabs
	agui::TabbedPane tabbed_pane;
	agui::Tab tabs[3];

	// frames/layouts
	agui::FlowLayout main_layout;
	DungeonEditorLayout *dungeon_edit_layout;
	LevelEditorLayout *level_edit_layout;
	TilesetEditorLayout *tileset_edit_layout;

	void save_dungeon();
	void update_attributes();
	void update_level_attributes();
	void load_all_tilesets();

	void reset_fields();
	void load_selected_tileset_tiles();
	void load_selected_tileset_blocks();
	void load_selected_tileset_entity_groups();

		// update methods
	void dungeon_update();
	void level_update();
	void tileset_update();
	void update_load_dungeon();
	void update_selected_level();
	void update_level_grid();
	void update_save_dungeon();
	void update_add_dungeon();
	void update_delete_dungeon();
	void update_add_level();
	void update_delete_level();
	void update_selected_tileset();
	void update_reset_tile_edges();
public:
	LevelEditorManager();
	LevelEditorManager(agui::Gui *guiInstance, ALLEGRO_DISPLAY *display);
	~LevelEditorManager();
	void update();
	void draw(ALLEGRO_DISPLAY *display);
};

#endif