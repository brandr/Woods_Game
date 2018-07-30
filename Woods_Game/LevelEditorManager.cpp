#include "LevelEditorManager.h"

LevelEditorManager::LevelEditorManager()
{
	
}

//TODO: level edit layout needs a scrollable grid for the selected level
LevelEditorManager::LevelEditorManager(agui::Gui * guiInstance, ALLEGRO_DISPLAY * display)
{
	//TODO: if these get cumbersome, factor them out into smaller classes, possibly one for each frame
	editor_gui = guiInstance;

	// dungeon layout
	this->initialize_dungeon_edit_layout(display);
	dungeon_edit_layout.setLocation(20, 20);
	editor_gui->add(&dungeon_edit_layout);
	
	// level layout
	this->initialize_level_edit_layout(display);
	level_edit_layout.setLocation(500, 20);
	editor_gui->add(&level_edit_layout);
	

	
	//dungeon_edit_frame.add(&dungeon_edit_layout);
	//editor_gui->add(&dungeon_edit_layout);
	
}

LevelEditorManager::~LevelEditorManager()
{
}

void LevelEditorManager::initialize_dungeon_edit_layout(ALLEGRO_DISPLAY *display)
{
	dungeon_edit_layout.setMaxOnRow(4);

	// dungeon select button
	dungeon_edit_layout.add(&selected_dungeon_name_label);
	selected_dungeon_name_label.setFontColor(agui::Color(255, 255, 255));
	selected_dungeon_name_label.setText("Dungeon name");
	dungeon_edit_layout.add(&selected_dungeon_name_field);
	selected_dungeon_name_field.setSize(200, 30);
	dungeon_edit_layout.add(new agui::EmptyWidget());
	dungeon_edit_layout.add(new agui::EmptyWidget());

	// load dungeon button
	dungeon_edit_layout.add(&load_dungeon_button);
	load_dungeon_button.setSize(200, 40);
	load_dungeon_button.setText("Load Dungeon");

	load_dungeon_select_listener.set_listener_type(LOAD_DUNGEON);
	load_dungeon_select_listener.set_display(display);
	load_dungeon_button.addActionListener(&load_dungeon_select_listener);

	// save dungeon button
	dungeon_edit_layout.add(&save_dungeon_button);
	save_dungeon_button.setSize(200, 40);
	save_dungeon_button.setText("Save Dungeon");

	save_dungeon_select_listener.set_listener_type(SAVE_DUNGEON);
	save_dungeon_select_listener.set_display(display);
	save_dungeon_button.addActionListener(&save_dungeon_select_listener);

	dungeon_edit_layout.add(new agui::EmptyWidget());
	dungeon_edit_layout.add(new agui::EmptyWidget());

	// level selector

	dungeon_edit_layout.add(&level_select_box);
	level_select_box.setSize(400, 400);
	dungeon_edit_layout.add(new agui::EmptyWidget());
	dungeon_edit_layout.add(new agui::EmptyWidget());
	dungeon_edit_layout.add(new agui::EmptyWidget());

	// edit fields

	// level name
	dungeon_edit_layout.add(&selected_level_name_label);
	selected_level_name_label.setText("Level name");
	selected_level_name_label.setFontColor(agui::Color(255, 255, 255));
	dungeon_edit_layout.add(&selected_level_name_field);
	selected_level_name_field.setSize(200, 30);
	dungeon_edit_layout.add(new agui::EmptyWidget());
	dungeon_edit_layout.add(new agui::EmptyWidget());

	// level x pos
	dungeon_edit_layout.add(&level_x_pos_label);
	level_x_pos_label.setText("Level x pos");
	level_x_pos_label.setFontColor(agui::Color(255, 255, 255));
	dungeon_edit_layout.add(&level_x_pos_field);
	level_x_pos_field.setSize(80, 30);

	// level y pos
	dungeon_edit_layout.add(&level_y_pos_label);
	level_y_pos_label.setText("Level y pos");
	level_y_pos_label.setFontColor(agui::Color(255, 255, 255));
	dungeon_edit_layout.add(&level_y_pos_field);
	level_y_pos_field.setSize(80, 30);

	// level width
	dungeon_edit_layout.add(&level_width_label);
	level_width_label.setText("Level width");
	level_width_label.setFontColor(agui::Color(255, 255, 255));
	dungeon_edit_layout.add(&level_width_field);
	level_width_field.setSize(80, 30);

	// level height
	dungeon_edit_layout.add(&level_height_label);
	level_height_label.setText("Level height");
	level_height_label.setFontColor(agui::Color(255, 255, 255));
	dungeon_edit_layout.add(&level_height_field);
	level_height_field.setSize(80, 30);
}

void LevelEditorManager::initialize_level_edit_layout(ALLEGRO_DISPLAY * display)
{
	level_edit_layout.setMaxOnRow(4);

	// level grid label
	level_edit_layout.add(&level_grid_label);
	level_grid_label.setText("Level grid");
	level_grid_label.setFontColor(agui::Color(255, 255, 255));
	level_edit_layout.add(new agui::EmptyWidget());
	level_edit_layout.add(new agui::EmptyWidget());
	level_edit_layout.add(new agui::EmptyWidget());

	// level grid scroll pane
	level_edit_layout.add(&level_grid_scroll_pane);
	level_grid_scroll_pane.setSize(500, 500);
	level_grid_scroll_pane.add(&level_grid_frame);
	level_grid_frame.setSize(500, 500);
	level_grid_frame.add(&level_editor_grid);
	// TODO: action listener for level editor grid
	// TODO: in level grid class, draw gridlines
	level_edit_layout.add(new agui::EmptyWidget());
	level_edit_layout.add(new agui::EmptyWidget());
	level_edit_layout.add(new agui::EmptyWidget());

	// layer visiblity checkboxes

		// tile layer
	level_edit_layout.add(&tile_visibility_checkbox);
	tile_visibility_checkbox.setAutosizing(true);
	tile_visibility_checkbox.setText("Show tiles");
	tile_visibility_checkbox.setFontColor(agui::Color(255, 255, 255));
	tile_visibility_checkbox.setCheckBoxAlignment(agui::ALIGN_MIDDLE_LEFT);
	tile_visibility_checkbox.setChecked(true);
	
		// block layer
	level_edit_layout.add(&block_visibility_checkbox);
	block_visibility_checkbox.setAutosizing(true);
	block_visibility_checkbox.setText("Show blocks");
	block_visibility_checkbox.setFontColor(agui::Color(255, 255, 255));
	block_visibility_checkbox.setCheckBoxAlignment(agui::ALIGN_MIDDLE_LEFT);
	block_visibility_checkbox.setChecked(true);

		// entitygroup layer
	level_edit_layout.add(&entity_group_visibility_checkbox);
	entity_group_visibility_checkbox.setAutosizing(true);
	entity_group_visibility_checkbox.setText("Show entity groups");
	entity_group_visibility_checkbox.setFontColor(agui::Color(255, 255, 255));
	entity_group_visibility_checkbox.setCheckBoxAlignment(agui::ALIGN_MIDDLE_LEFT);
	entity_group_visibility_checkbox.setChecked(true);

	//TODO: entitygroup layer and checkbox
	//TODO: draw gridlines in level editor grid and add gridline checkbox
	level_edit_layout.add(new agui::EmptyWidget());
}

void LevelEditorManager::reset_fields()
{
	this->selected_level_name_field.setText("");
	this->selected_dungeon_name_field.setText("");
	this->level_select_box.clear();
	this->level_x_pos_field.setText("");
	this->level_y_pos_field.setText("");
	this->level_width_field.setText("");
	this->level_height_field.setText("");
}

void LevelEditorManager::save_dungeon()
{
	//TODO: get dungeon values from input fields, updated levels, etc and save to the selected filename
	//TODO: popup notification when done saving
}

void LevelEditorManager::update()
{
	this->update_load_dungeon();
	this->update_selected_level();
	this->update_level_grid();
	this->update_save_dungeon();
}

void LevelEditorManager::update_load_dungeon()
{
	if (this->selected_dungeon == NULL) {
		this->selected_dungeon = this->load_dungeon_select_listener.selected_dungeon();
		this->levels.clear();
		this->selected_level = NULL;
		this->needs_update = this->selected_dungeon != NULL;
		this->reset_fields();
	}
	if (this->selected_dungeon != NULL && this->needs_update) {
		this->selected_dungeon_name_field.setText(this->selected_dungeon->get_dungeon_name());
		std::vector<Level*> dungeon_levels = this->selected_dungeon->get_level_list();
		const int size = dungeon_levels.size();
		if (size > 0) {
			this->levels = this->selected_dungeon->get_level_list();
			for (Level * level : this->levels) {
				this->level_select_box.addItem(level->get_filename());
			}
		}
		else {
			this->levels.clear();
		}
		this->needs_update = false;
	}
}

void LevelEditorManager::update_selected_level()
{
	const int level_index = this->level_select_box.getSelectedIndex();
	if (this->selected_dungeon != NULL && level_index >= 0) {
		std::string level_name = this->level_select_box.getItemAt(level_index);
		this->selected_level = this->selected_dungeon->level_with_name(level_name);
		if (this->selected_level != NULL) {
			this->selected_level_name_field.setText(level_name);
			this->level_x_pos_field.setText(std::to_string(this->selected_level->get_grid_x()));
			this->level_y_pos_field.setText(std::to_string(this->selected_level->get_grid_y()));
			this->level_width_field.setText(std::to_string(this->selected_level->get_grid_width()));
			this->level_height_field.setText(std::to_string(this->selected_level->get_grid_height()));
		}
	}
}

void LevelEditorManager::update_level_grid()
{
	this->level_editor_grid.set_level(this->selected_level);
	this->level_editor_grid.update();
	int width = 500;
	int height = 500;
	if (this->selected_level != NULL) {
		width = this->selected_level->get_width();
		height = this->selected_level->get_height();
		this->level_editor_grid.set_layer_visible(TILE_LAYER, this->tile_visibility_checkbox.checked());
		this->level_editor_grid.set_layer_visible(BLOCK_LAYER, this->block_visibility_checkbox.checked());
		this->level_editor_grid.set_layer_visible(ENTITY_GROUP_LAYER, this->entity_group_visibility_checkbox.checked());
	}
	// resize if necessary
	if (width != this->level_grid_frame.getWidth() || height != this->level_grid_frame.getHeight()) {
		this->level_grid_frame.setSize(width, height);
		this->level_editor_grid.setSize(width, height);
	}
}

void LevelEditorManager::update_save_dungeon()
{
	if (this->save_dungeon_select_listener.has_flag(SHOULD_SAVE)) {
		this->save_dungeon_select_listener.unset_flag(SHOULD_SAVE);
		this->save_dungeon();
	}
}

void LevelEditorManager::draw(ALLEGRO_DISPLAY * display)
{
	this->editor_gui->logic();
	this->editor_gui->render();
}
