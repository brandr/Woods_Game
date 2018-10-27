#ifndef DUNGEON_EDITOR_LAYOUT_H
#define DUNGEON_EDITOR_LAYOUT_H

#include <string>
#include <filesystem>
namespace fs = std::experimental::filesystem;

#include <allegro5/display.h>

#include <Agui/FlowLayout.hpp>
#include <Agui/EmptyWidget.hpp>
#include <Agui/Widgets/Label/Label.hpp>
#include <Agui/Widgets/TextField/TextField.hpp>
#include <Agui/Widgets/Button/Button.hpp>
#include <Agui/Widgets/ListBox/ListBox.hpp>
#include <Agui/Widgets/Frame/Frame.hpp>

#include "Dungeon.h"
#include "EditorActionListener.h"
#include "LevelEditorDataManager.h"




class DungeonEditorLayout : public agui::FlowLayout {

private:
	//components
	// sections
	agui::Frame dungeon_frame;
	agui::Frame level_frame;
	agui::FlowLayout dungeon_layout;
	agui::FlowLayout level_layout;

	// action listeners
		// dungeon
	EditorActionListener load_dungeon_select_listener;
	EditorActionListener save_dungeon_select_listener;
	EditorActionListener add_dungeon_select_listener;
	EditorActionListener delete_dungeon_select_listener;
		//level
	EditorActionListener add_level_select_listener;
	EditorActionListener delete_level_select_listener;

	// labels
	agui::Label dungeon_list_label;
	agui::Label selected_dungeon_name_label;
	agui::Label selected_level_name_label;
	agui::Label level_list_label;
	agui::Label level_x_pos_label;
	agui::Label level_y_pos_label;
	agui::Label level_width_label;
	agui::Label level_height_label;
	
	// text fields
	agui::TextField selected_dungeon_name_field;
	agui::TextField selected_level_name_field;
	agui::TextField level_x_pos_field;
	agui::TextField level_y_pos_field;
	agui::TextField level_width_field;
	agui::TextField level_height_field;

	// buttons
		// dungeon
	agui::Button load_dungeon_button;
	agui::Button save_dungeon_button;
	agui::Button add_dungeon_button;
	agui::Button delete_dungeon_button;
		// level
	agui::Button add_level_button;
	agui::Button delete_level_button;

	// listbox
	agui::ListBox dungeon_select_box;
	agui::ListBox level_select_box;

	void initialize(ALLEGRO_DISPLAY *display);
	void initialize_dungeon_list();
public:
	DungeonEditorLayout(ALLEGRO_DISPLAY *display);
	void update_load_dungeon();
	bool update_selected_level();
	void reset_fields();
	void reset_dungeon_list();
	void reset_level_list();
	void reset_selected_level_fields();
	void set_selected_dungeon_name(std::string value);
	std::string get_input_dungeon_name();
	void add_level(std::string level_name);
	void refresh_levels(std::vector<Level *> levels);
	int get_selected_level_index();
	std::string get_level_name(int index);
	std::string get_input_level_name();
	std::pair<int, int> get_input_level_grid_pos();
	std::pair<int, int> get_input_level_dimensions();
	void set_selected_level_name(std::string value);
	void set_level_dimensions(int x, int y, int width, int height);
	void deselect_dungeon();
	std::string listbox_selected_dungeon_name();
	std::string listbox_selected_level_name();
	void set_selected_dungeon_index(int index);
	bool check_dungeon_listener_flag(EditorActionListener * listener, int flag);
	bool should_save_dungeon();
	bool should_load_dungeon();
	bool should_add_dungeon();
	bool should_delete_dungeon();
	bool should_add_level();
	bool should_delete_level();
};

#endif