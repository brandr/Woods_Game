#include "DungeonEditorLayout.h"

void DungeonEditorLayout::initialize(ALLEGRO_DISPLAY *display)
{
	this->setMaxOnRow(2);
	this->add(&dungeon_frame);
	this->add(&level_frame);

	// dungeon UI

	this->dungeon_frame.setSize(500, 500);
	this->dungeon_frame.add(&dungeon_layout);
	this->dungeon_layout.setMaxOnRow(4);

	// dungeon list label
	this->dungeon_layout.add(&dungeon_list_label);
	dungeon_list_label.setText("Dungeons");
	this->dungeon_layout.add(new agui::EmptyWidget());
	this->dungeon_layout.add(new agui::EmptyWidget());
	this->dungeon_layout.add(new agui::EmptyWidget());
	
	// dungeon select box
	this->dungeon_layout.add(&dungeon_select_box);
	dungeon_select_box.setSize(400, 180);
	this->dungeon_layout.add(new agui::EmptyWidget());
	this->dungeon_layout.add(new agui::EmptyWidget());
	this->dungeon_layout.add(new agui::EmptyWidget());

	// dungeon select button
	this->dungeon_layout.add(&selected_dungeon_name_label);
	selected_dungeon_name_label.setFontColor(agui::Color(0, 0, 0));
	selected_dungeon_name_label.setText("Dungeon name");
	this->dungeon_layout.add(&selected_dungeon_name_field);
	selected_dungeon_name_field.setSize(200, 30);
	this->dungeon_layout.add(new agui::EmptyWidget());
	this->dungeon_layout.add(new agui::EmptyWidget());

	// load dungeon button
	this->dungeon_layout.add(&load_dungeon_button);
	load_dungeon_button.setSize(200, 40);
	load_dungeon_button.setText("Load Dungeon");
	load_dungeon_select_listener.set_listener_type(LOAD_DUNGEON);
	load_dungeon_select_listener.set_display(display);
	load_dungeon_button.addActionListener(&load_dungeon_select_listener);

	// save dungeon button
	this->dungeon_layout.add(&save_dungeon_button);
	save_dungeon_button.setSize(200, 40);
	save_dungeon_button.setText("Save Dungeon");
	save_dungeon_select_listener.set_listener_type(SAVE_DUNGEON);
	save_dungeon_select_listener.set_display(display);
	save_dungeon_button.addActionListener(&save_dungeon_select_listener);
	this->dungeon_layout.add(new agui::EmptyWidget());
	this->dungeon_layout.add(new agui::EmptyWidget());

	// add dungeon button
	this->dungeon_layout.add(&add_dungeon_button);
	add_dungeon_button.setSize(200, 40);
	add_dungeon_button.setText("Add Dungeon");
	add_dungeon_select_listener.set_listener_type(ADD_DUNGEON);
	add_dungeon_select_listener.set_display(display);
	add_dungeon_button.addActionListener(&add_dungeon_select_listener);

	// delete dungeon button
	this->dungeon_layout.add(&delete_dungeon_button);
	delete_dungeon_button.setSize(200, 40);
	delete_dungeon_button.setText("Delete Dungeon");
	delete_dungeon_select_listener.set_listener_type(DELETE_DUNGEON);
	delete_dungeon_select_listener.set_display(display);
	delete_dungeon_button.addActionListener(&delete_dungeon_select_listener);
	this->dungeon_layout.add(new agui::EmptyWidget());
	this->dungeon_layout.add(new agui::EmptyWidget());

	this->initialize_dungeon_list();

	// level UI

	this->level_frame.setSize(500, 500);
	this->level_frame.add(&level_layout);
	this->level_layout.setMaxOnRow(4);
	
	// level list label
	this->level_layout.add(&level_list_label);
	this->level_list_label.setText("Levels");
	this->level_layout.add(new agui::EmptyWidget());
	this->level_layout.add(new agui::EmptyWidget());
	this->level_layout.add(new agui::EmptyWidget());

	// level selector
	this->level_layout.add(&level_select_box);
	level_select_box.setSize(400, 180);
	this->level_layout.add(new agui::EmptyWidget());
	this->level_layout.add(new agui::EmptyWidget());
	this->level_layout.add(new agui::EmptyWidget());

	// edit fields

	// level name
	this->level_layout.add(&selected_level_name_label);
	selected_level_name_label.setText("Level name");
	this->level_layout.add(&selected_level_name_field);
	selected_level_name_field.setSize(200, 30);
	this->level_layout.add(new agui::EmptyWidget());
	this->level_layout.add(new agui::EmptyWidget());

	// level x pos
	this->level_layout.add(&level_x_pos_label);
	level_x_pos_label.setText("Level x pos");
	this->level_layout.add(&level_x_pos_field);
	level_x_pos_field.setSize(80, 30);

	// level y pos
	this->level_layout.add(&level_y_pos_label);
	level_y_pos_label.setText("Level y pos");
	this->level_layout.add(&level_y_pos_field);
	level_y_pos_field.setSize(80, 30);

	// level width
	this->level_layout.add(&level_width_label);
	level_width_label.setText("Level width");
	this->level_layout.add(&level_width_field);
	level_width_field.setSize(80, 30);

	// level height
	this->level_layout.add(&level_height_label);
	level_height_label.setText("Level height");
	this->level_layout.add(&level_height_field);
	level_height_field.setSize(80, 30);

	// add level button
	this->level_layout.add(&add_level_button);
	add_level_button.setSize(200, 40);
	add_level_button.setText("Add Level");
	add_level_select_listener.set_listener_type(ADD_LEVEL);
	add_level_select_listener.set_display(display);
	add_level_button.addActionListener(&add_level_select_listener);

	// delete level button
	this->level_layout.add(&delete_level_button);
	delete_level_button.setSize(200, 40);
	delete_level_button.setText("Delete Level");
	delete_level_select_listener.set_listener_type(DELETE_LEVEL);
	delete_level_select_listener.set_display(display);
	delete_level_button.addActionListener(&delete_level_select_listener);
	this->level_layout.add(new agui::EmptyWidget());
	this->level_layout.add(new agui::EmptyWidget());
}

void DungeonEditorLayout::initialize_dungeon_list()
{
	const std::string filepath = "resources/load/dungeons/";
	for (auto & p : fs::directory_iterator(filepath)) {
		const std::string filename = p.path().string();
		if (filename.substr(filename.length() - 4, filename.length()) == ".xml") {
			this->dungeon_select_box.addItem(filename.substr(0, filename.length() - 4));
		}
	}
}

DungeonEditorLayout::DungeonEditorLayout(ALLEGRO_DISPLAY *display)
{
	this->initialize(display);
}

void DungeonEditorLayout::update_load_dungeon()
{
	if (this->should_load_dungeon()) {
		const std::string dungeon_name = this->dungeon_select_box.getItemAt(this->dungeon_select_box.getSelectedIndex());
		if (dungeon_name.length() > 0) {
			LevelEditorDataManager::get_instance().load_dungeon(dungeon_name);
			this->reset_fields();
		}
	}
}

bool DungeonEditorLayout::update_selected_level()
{
	bool did_update = false;
	const int level_index = this->get_selected_level_index();
	const int prev_index = LevelEditorDataManager::get_instance().get_selected_level_index();
	if (level_index != prev_index) {
		LevelEditorDataManager::get_instance().set_selected_level_index(level_index);
		this->reset_selected_level_fields();
		did_update = true;
	}
	return did_update;
}

void DungeonEditorLayout::reset_fields()
{
	this->reset_dungeon_list();
	this->level_x_pos_field.setText("");
	this->level_y_pos_field.setText("");
	this->level_width_field.setText("");
	this->level_height_field.setText("");
	this->selected_dungeon_name_field.setText(
		LevelEditorDataManager::get_instance().get_active_dungeon_name());
	this->reset_level_list();
	this->reset_selected_level_fields();
}

void DungeonEditorLayout::reset_dungeon_list()
{
	this->dungeon_select_box.clearItems();
	this->initialize_dungeon_list();
}

void DungeonEditorLayout::reset_level_list()
{
	this->level_select_box.clearItems();
	std::vector<std::string> level_names = LevelEditorDataManager::get_instance().get_active_level_names();
	for (std::string name : level_names) {
		this->level_select_box.addItem(name);
	}
}

void DungeonEditorLayout::reset_selected_level_fields()
{
	if (LevelEditorDataManager::get_instance().has_selected_level()) {
		this->selected_level_name_field.setText(
			LevelEditorDataManager::get_instance().get_selected_level_name());
		std::pair<int, int> selected_level_pos = LevelEditorDataManager::get_instance().selected_level_grid_pos();
		std::pair<int, int> selected_level_dim = LevelEditorDataManager::get_instance().selected_level_grid_dimensions();
		this->level_x_pos_field.setText(std::to_string(selected_level_pos.first));
		this->level_y_pos_field.setText(std::to_string(selected_level_pos.second));
		this->level_width_field.setText(std::to_string(selected_level_dim.first));
		this->level_height_field.setText(std::to_string(selected_level_dim.second));
	}
}

void DungeonEditorLayout::set_selected_dungeon_name(std::string value)
{
	this->selected_dungeon_name_field.setText(value);
}

std::string DungeonEditorLayout::get_input_dungeon_name()
{
	return this->selected_dungeon_name_field.getText();
}

void DungeonEditorLayout::add_level(std::string level_name)
{
	this->level_select_box.addItem(level_name);
}

void DungeonEditorLayout::refresh_levels(std::vector<Level*> levels)
{
	this->level_select_box.clearItems();
	for (Level * level : levels) {
		this->add_level(level->get_filename());
	}
}

int DungeonEditorLayout::get_selected_level_index()
{
	return this->level_select_box.getSelectedIndex();
}

std::string DungeonEditorLayout::get_level_name(int index)
{
	return this->level_select_box.getItemAt(index);
}

std::string DungeonEditorLayout::get_input_level_name()
{
	return this->selected_level_name_field.getText();
}

inline bool isInteger(const std::string & s)
{
	if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

	char * p;
	strtol(s.c_str(), &p, 10);

	return (*p == 0);
}

std::pair<int, int> DungeonEditorLayout::get_input_level_grid_pos()
{
	const std::string x_pos_str = this->level_x_pos_field.getText(),
		y_pos_str = this->level_y_pos_field.getText();
	const int x = isInteger(x_pos_str) ? ::atoi(x_pos_str.c_str()) : 0,
		y = isInteger(y_pos_str) ? ::atoi(y_pos_str.c_str()) : 0;
	return std::pair<int, int>(std::abs(x), std::abs(y));
}

std::pair<int, int> DungeonEditorLayout::get_input_level_dimensions()
{
	const std::string width_str = this->level_width_field.getText(),
		height_str = this->level_height_field.getText();
	const int width = isInteger(width_str) ? ::atoi(width_str.c_str()) : 0,
		height = isInteger(height_str) ? ::atoi(height_str.c_str()) : 0;
	return std::pair<int, int>(std::abs(width), std::abs(height));
}

void DungeonEditorLayout::set_selected_level_name(std::string value)
{
	this->selected_level_name_field.setText(value);
}

void DungeonEditorLayout::set_level_dimensions(int x, int y, int width, int height)
{
	this->level_x_pos_field.setText(std::to_string(x));
	this->level_y_pos_field.setText(std::to_string(y));
	this->level_width_field.setText(std::to_string(width));
	this->level_height_field.setText(std::to_string(height));
}

void DungeonEditorLayout::deselect_dungeon()
{
	this->dungeon_select_box.setSelectedIndex(-1);
}

std::string DungeonEditorLayout::listbox_selected_dungeon_name()
{
	return this->dungeon_select_box.getItemAt(this->dungeon_select_box.getSelectedIndex());
}

std::string DungeonEditorLayout::listbox_selected_level_name()
{
	return this->level_select_box.getItemAt(this->level_select_box.getSelectedIndex());
}

void DungeonEditorLayout::set_selected_dungeon_index(int index)
{
	this->dungeon_select_box.setSelectedIndex(index);
}

bool DungeonEditorLayout::check_dungeon_listener_flag(EditorActionListener * listener, int flag)
{
	bool value = listener->has_flag(flag);
	if (value) {
		listener->unset_flag(flag);
	}
	return value;
}

bool DungeonEditorLayout::should_save_dungeon()
{
	return this->check_dungeon_listener_flag(&save_dungeon_select_listener, SHOULD_UPDATE);
	
}

bool DungeonEditorLayout::should_load_dungeon()
{
	return this->check_dungeon_listener_flag(&load_dungeon_select_listener, SHOULD_UPDATE);
}

bool DungeonEditorLayout::should_add_dungeon()
{
	return this->check_dungeon_listener_flag(&add_dungeon_select_listener, SHOULD_UPDATE);
}

bool DungeonEditorLayout::should_delete_dungeon()
{
	return this->check_dungeon_listener_flag(&delete_dungeon_select_listener, SHOULD_UPDATE);
}

bool DungeonEditorLayout::should_add_level()
{
	return this->check_dungeon_listener_flag(&add_level_select_listener, SHOULD_UPDATE);
}

bool DungeonEditorLayout::should_delete_level()
{
	return this->check_dungeon_listener_flag(&delete_level_select_listener, SHOULD_UPDATE);
}

