#ifndef LEVEL_EDITOR_LAYOUT_H
#define LEVEL_EDITOR_LAYOUT_H

#include "LevelEditorGrid.h"
#include "GameImageDisplay.h"
#include "EditorActionListener.h"

#include <Agui/FlowLayout.hpp>
#include <Agui/EmptyWidget.hpp>
#include <Agui/Widgets/Frame/Frame.hpp>
#include <Agui/Widgets/Tab/Tab.hpp>
#include <Agui/Widgets/Tab/TabbedPane.hpp>
#include <Agui/Widgets/Label/Label.hpp>
#include <Agui/Widgets/ScrollPane/ScrollPane.hpp>
#include <Agui/Widgets/ListBox/ListBox.hpp>
#include <Agui/Widgets/CheckBox/CheckBox.hpp>

#include <allegro5/display.h>


class LevelEditorLayout : public agui::FlowLayout {
	const int LEVEL_EDITOR_GRID_WIDTH = 550;
	const int LEVEL_EDITOR_GRID_HEIGHT = 400;

	enum OBJECT_SELECT_TABS {
		TILE_SELECT_TAB = 0,
		BLOCK_SELECT_TAB = 1,
		ENTITY_GROUP_SELECT_TAB = 2,
		TILED_IMAGE_SELECT_TAB = 3
	};
private:

	//components

	LevelEditorGrid level_editor_grid;
	agui::Label level_grid_label;
	agui::ScrollPane level_grid_scroll_pane;
	agui::Frame level_grid_frame;
	agui::Tab level_edit_object_tabs[4];
	agui::ScrollPane level_edit_object_tile_scroll_pane;
	agui::ScrollPane level_edit_object_block_scroll_pane;
	agui::ScrollPane level_edit_object_entity_group_scroll_pane;
	agui::ScrollPane level_edit_object_tiled_image_scroll_pane;
	agui::ListBox level_edit_object_tile_select_box;
	agui::ListBox level_edit_object_block_select_box;
	agui::ListBox level_edit_object_entity_group_select_box;
	agui::ListBox level_edit_object_tiled_image_select_box;
	agui::Button level_edit_reset_tile_edges_button;

	// checkboxes
	agui::CheckBox tile_visibility_checkbox;
	agui::CheckBox block_visibility_checkbox;
	agui::CheckBox entity_group_visibility_checkbox;
	agui::CheckBox tiled_image_visibility_checkbox;
	agui::CheckBox grid_lines_visibility_checkbox;

	// listeners
	EditorActionListener reset_tile_edges_select_listener;

	// object select
	agui::CheckBox level_edit_select_mode_checkbox;
	agui::TabbedPane level_edit_object_tabbed_pane;
	agui::ScrollPane level_edit_object_select_pane;
	agui::Frame edit_object_select_frame;
	GameImageDisplay selected_level_object_display;

	// methods
	void set_selected_object_tab_index(int index);
	int selected_object_tab_index();
	void set_selected_object_select_index(int index);
	int selected_object_select_index();
	bool check_level_listener_flag(EditorActionListener * listener, int flag);
public:
	LevelEditorLayout(ALLEGRO_DISPLAY *display);
	void load_selected_tileset_tiles();
	void load_selected_tileset_blocks();
	void load_selected_tileset_entity_groups();
	void load_selected_tileset_tiled_images();
	void update();
	void update_selected_level_object();
	bool should_reset_tile_edges();
	void reset_grid_image_layer(std::string layer);
};

#endif // !LEVEL_EDITOR_LAYOUT_H

