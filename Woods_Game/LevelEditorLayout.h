#ifndef LEVEL_EDITOR_LAYOUT_H
#define LEVEL_EDITOR_LAYOUT_H

#include <string>

#include "LevelEditorGrid.h"
#include "TiledImageGrid.h"
#include "GameImageDisplay.h"
#include "EditorActionListener.h"
#include "LevelGridKeyboardListener.h"

#include <Agui/FlowLayout.hpp>
#include <Agui/EmptyWidget.hpp>
#include <Agui/Widgets/Frame/Frame.hpp>
#include <Agui/Widgets/Tab/Tab.hpp>
#include <Agui/Widgets/Tab/TabbedPane.hpp>
#include <Agui/Widgets/Label/Label.hpp>
#include <Agui/Widgets/ScrollPane/ScrollPane.hpp>
#include <Agui/Widgets/ListBox/ListBox.hpp>
#include <Agui/Widgets/CheckBox/CheckBox.hpp>
#include <Agui/Widgets/TextField/TextField.hpp>
#include <Agui/Widgets/TextBox/TextBox.hpp>
#include <Agui/Backends/Allegro5/Allegro5Font.hpp>

#include <allegro5/display.h>


class LevelEditorLayout : public agui::FlowLayout {
	const int LEVEL_EDITOR_GRID_WIDTH = 550;
	const int LEVEL_EDITOR_GRID_HEIGHT = 380;
	const int TILED_IMAGE_GRID_WIDTH = 250;
	const int TILED_IMAGE_GRID_HEIGHT = 250;
	const int ATTRIBUTE_PANE_WIDTH = 460;
	const int ATTRIBUTE_PANE_HEIGHT = 240;

	enum OBJECT_SELECT_TABS {
		TILE_SELECT_TAB = 0,
		BLOCK_SELECT_TAB = 1,
		ENTITY_GROUP_SELECT_TAB = 2,
		SPAWNER_TAB = 3
	};

	enum SECONDARY_TABS {
		TILED_IMAGE_TAB = 0,
		EDIT_OBJECT_TAB = 1,
		SELECT_TILESET_TAB = 2
	};
private:

	// layouts
	agui::FlowLayout level_editor_layout_1;
	agui::FlowLayout level_editor_layout_2;
	agui::FlowLayout tiled_image_layout;
	agui::FlowLayout edit_object_layout;
	agui::FlowLayout select_tileset_layout;

	// layout frames
	agui::Frame level_editor_layout_frame_1;
	agui::Frame level_editor_layout_frame_2;
	agui::Frame tiled_image_frame;
	agui::Frame edit_object_frame;
	agui::Frame object_instance_attribute_frame;

	//components

	LevelEditorGrid level_editor_grid;
	TiledImageGrid tiled_image_grid;
	agui::Label level_grid_label;
	agui::Label tiled_images_label;
	agui::Label tiled_images_layer_label;
	agui::Label level_edit_object_col_label;
	agui::Label selected_object_label;
	agui::Label selected_level_object_instance_attributes_label;
	agui::Label selected_tileset_label;
	agui::ScrollPane level_grid_scroll_pane;
	agui::ScrollPane tiled_image_grid_scroll_pane;
	agui::ScrollPane object_instance_attributes_scroll_pane;
	agui::Frame level_grid_frame;
	agui::Frame tiled_image_grid_frame;
	agui::Tab level_edit_object_tabs[4];
	agui::Tab level_edit_secondary_tabs[3];
	agui::ScrollPane level_edit_object_tile_scroll_pane;
	agui::ScrollPane level_edit_object_block_scroll_pane;
	agui::ScrollPane level_edit_object_entity_group_scroll_pane;
	agui::ScrollPane tiled_image_scroll_pane;
	agui::ScrollPane tileset_select_scroll_pane;
	agui::ListBox level_edit_object_tile_select_box;
	agui::ListBox level_edit_object_block_select_box;
	agui::ListBox level_edit_object_entity_group_select_box;
	agui::ListBox level_edit_object_spawner_select_box;
	agui::ListBox tiled_image_select_box;
	agui::ListBox tileset_select_box;
	agui::Button level_edit_reset_level_button;
	agui::Button level_edit_reset_tile_edges_button;
	agui::Button level_edit_object_col_left_button;
	agui::Button level_edit_object_col_right_button;
	agui::Button save_instance_attributes_button;
	agui::TextField tiled_images_layer_field;

	// checkboxes
	agui::CheckBox tile_visibility_checkbox;
	agui::CheckBox block_visibility_checkbox;
	agui::CheckBox entity_group_visibility_checkbox;
	agui::CheckBox tiled_image_visibility_checkbox;
	agui::CheckBox spawner_visibility_checkbox;
	agui::CheckBox grid_lines_visibility_checkbox;

	// listeners
	EditorActionListener reset_level_select_listener;
	EditorActionListener reset_tile_edges_select_listener;
	EditorActionListener level_edit_object_col_left_listener;
	EditorActionListener level_edit_object_col_right_listener;
	EditorActionListener save_instance_attributes_select_listener;
	LevelGridKeyboardListener keyboard_grid_listener;

	// object select
	agui::CheckBox level_edit_select_mode_checkbox;
	agui::TabbedPane level_edit_object_tabbed_pane;
	agui::TabbedPane secondary_tabbed_pane;
	agui::ScrollPane level_edit_object_select_pane;
	agui::Frame edit_object_select_frame;
	GameImageDisplay selected_level_object_display;
	GameImageDisplay selected_level_object_instance_display;
	agui::TextBox selected_level_object_instance_attributes_box;
	bool selecting_objects = false;

	// edit tileset
	agui::Frame select_tileset_frame;

	// methods
	void set_selected_object_tab_index(int index);
	int selected_object_tab_index();
	void set_selected_object_select_index(int index);
	int selected_object_select_index();
	const std::string selected_object_name();
	bool check_level_listener_flag(EditorActionListener * listener, int flag);
	const int get_input_tiled_image_layer();

	// variables
	std::pair<int, int> last_selected_object_instance_pos;
	int last_selected_object_instance_index = -1;
public:
	LevelEditorLayout(ALLEGRO_DISPLAY *display);
	void load_all_tilesets();
	void load_selected_tileset_tiles();
	void load_selected_tileset_blocks();
	void load_selected_tileset_entity_groups();
	void load_selected_tileset_spawners();
	void load_selected_tileset_tiled_images();
	void update();
	void update_level_grid();
	void update_tiled_image_grid();
	void update_selected_level_object(const bool force);
	void update_selected_level_object_instance();
	void reset_selected_level_object_instance_attributes();
	void update_selected_tiled_image();
	void update_selected_tileset();
	void update_sheet_col();
	const bool should_reset_level();
	const bool should_reset_tile_edges();
	const bool should_decrement_sheet_col();
	const bool should_increment_sheet_col();
	const bool should_save_instance_attributes();
	void reset_all_grid_image_layers();
	void reset_grid_image_layer(std::string layer);
	void set_selecting_objects(const bool value);
	void save_instance_attributes();
};

#endif // !LEVEL_EDITOR_LAYOUT_H

