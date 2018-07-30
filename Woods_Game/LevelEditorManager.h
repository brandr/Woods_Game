#ifndef LEVEL_EDITOR_MANAGER_H
#define LEVEL_EDITOR_MANAGER_H

#include "Dungeon.h"
#include "EditorActionListener.h"
#include "LevelEditorGrid.h"

#include <allegro5/display.h>

#include "Agui/Widgets/Frame/Frame.hpp"
#include "Agui/Widgets/Button/Button.hpp"
#include <Agui/Widgets/TextField/TextField.hpp>
#include <Agui/Widgets/Label/Label.hpp>
#include <Agui/Widgets/ListBox/ListBox.hpp>
#include <Agui/Widgets/ScrollPane/ScrollPane.hpp>
#include <Agui/Widgets/CheckBox/CheckBox.hpp>

#include <Agui/Backends/Allegro5/Allegro5.hpp>

#include "Agui/Gui.hpp"
#include "Agui/Input.hpp"
#include <Agui/FlowLayout.hpp>
#include "Agui/ImageLoader.hpp"
#include <Agui/EmptyWidget.hpp>
class LevelEditorManager
{
private:

	bool needs_update = false;

	// dungeon data
	Dungeon * selected_dungeon;
	Level * selected_level;
	std::vector<Level*> levels;
	
	//gui
	agui::Gui* editor_gui;

	// frames/layouts
	agui::FlowLayout dungeon_edit_layout;
	agui::FlowLayout level_edit_layout;

	// dungeon editor
		//components 
			// action listeners
	EditorActionListener load_dungeon_select_listener;
	EditorActionListener save_dungeon_select_listener;
	
			// labels
	agui::Label selected_dungeon_name_label;
	agui::Label selected_level_name_label;
	agui::Label level_x_pos_label;
	agui::Label level_y_pos_label;
	agui::Label level_width_label;
	agui::Label level_height_label;

			// buttons
	agui::Button load_dungeon_button;
	agui::Button save_dungeon_button;

			// listbox
	agui::ListBox level_select_box;

			// text fields
	agui::TextField selected_dungeon_name_field;
	agui::TextField selected_level_name_field;
	agui::TextField level_x_pos_field;
	agui::TextField level_y_pos_field;
	agui::TextField level_width_field;
	agui::TextField level_height_field;
	
		// init methods
	void initialize_dungeon_edit_layout(ALLEGRO_DISPLAY *display);
	void initialize_level_edit_layout(ALLEGRO_DISPLAY *display);

	void reset_fields();

	void save_dungeon();

		// update methods
	void update_load_dungeon();
	void update_selected_level();
	void update_level_grid();
	void update_save_dungeon();

	// level editor

		// level grid
	LevelEditorGrid level_editor_grid;
	agui::Label level_grid_label;
	agui::ScrollPane level_grid_scroll_pane;
	agui::Frame level_grid_frame;

		// checkboxes
	agui::CheckBox tile_visibility_checkbox;
	agui::CheckBox block_visibility_checkbox;
	agui::CheckBox entity_group_visibility_checkbox;
public:
	LevelEditorManager();
	LevelEditorManager(agui::Gui *guiInstance, ALLEGRO_DISPLAY *display);
	~LevelEditorManager();
	void update();
	void draw(ALLEGRO_DISPLAY *display);
};

#endif