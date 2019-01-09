#include "LevelEditorLayout.h"

void LevelEditorLayout::set_selected_object_tab_index(int index)
{
	LevelEditorDataManager::get_instance().set_selected_object_tab_index(index);
}

int LevelEditorLayout::selected_object_tab_index()
{
	return LevelEditorDataManager::get_instance().get_selected_object_tab_index();
}

void LevelEditorLayout::set_selected_object_select_index(int index)
{
	LevelEditorDataManager::get_instance().set_selected_object_select_index(index);
}

int LevelEditorLayout::selected_object_select_index()
{
	return LevelEditorDataManager::get_instance().get_selected_object_select_index();
}

bool LevelEditorLayout::check_level_listener_flag(EditorActionListener * listener, int flag)
{
	bool value = listener->has_flag(flag);
	if (value) {
		listener->unset_flag(flag);
	}
	return value;
}

inline bool isInteger(const std::string & s)
{
	if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

	char * p;
	strtol(s.c_str(), &p, 10);

	return (*p == 0);
}

const int LevelEditorLayout::get_input_tiled_image_layer()
{
	const std::string layer_str = this->tiled_images_layer_field.getText();
	const int layer = isInteger(layer_str) ? ::atoi(layer_str.c_str()) : 0;
	return std::abs(layer);
}

LevelEditorLayout::LevelEditorLayout(ALLEGRO_DISPLAY *display)
{
	this->setMaxOnRow(2);	
	this->add(&level_editor_layout_frame_1);
	this->add(&level_editor_layout_frame_2);

	// layout 1

	this->level_editor_layout_frame_1.setSize(600, 720);
	this->level_editor_layout_frame_1.add(&level_editor_layout_1);
	this->level_editor_layout_1.setMaxOnRow(4);

	// level grid label
	this->level_editor_layout_1.add(&level_grid_label);
	level_grid_label.setText("Level grid");
	level_grid_label.setFontColor(agui::Color(0, 0, 0));
	this->level_editor_layout_1.add(new agui::EmptyWidget());
	this->level_editor_layout_1.add(new agui::EmptyWidget());
	this->level_editor_layout_1.add(new agui::EmptyWidget());

	// level grid scroll pane
	//TODO: how to add level grid keyboard listener so that we see it from tab
	//this->addKeyboardListener(&keyboard_grid_listener);
	this->level_editor_layout_1.add(&level_grid_scroll_pane);
	level_grid_scroll_pane.setSize(LEVEL_EDITOR_GRID_WIDTH, LEVEL_EDITOR_GRID_HEIGHT);
	level_grid_scroll_pane.add(&level_grid_frame);
	level_grid_frame.setSize(LEVEL_EDITOR_GRID_WIDTH, LEVEL_EDITOR_GRID_HEIGHT);
	level_grid_frame.add(&level_editor_grid);
	this->level_editor_layout_1.add(new agui::EmptyWidget());
	this->level_editor_layout_1.add(new agui::EmptyWidget());
	this->level_editor_layout_1.add(new agui::EmptyWidget());

	// layer visiblity checkboxes

	// tile layer
	this->level_editor_layout_1.add(&tile_visibility_checkbox);
	tile_visibility_checkbox.setAutosizing(true);
	tile_visibility_checkbox.setText("Show tiles");
	tile_visibility_checkbox.setFontColor(agui::Color(0, 0, 0));
	tile_visibility_checkbox.setCheckBoxAlignment(agui::ALIGN_MIDDLE_LEFT);
	tile_visibility_checkbox.setChecked(true);

	// block layer
	this->level_editor_layout_1.add(&block_visibility_checkbox);
	block_visibility_checkbox.setAutosizing(true);
	block_visibility_checkbox.setText("Show blocks");
	block_visibility_checkbox.setFontColor(agui::Color(0, 0, 0));
	block_visibility_checkbox.setCheckBoxAlignment(agui::ALIGN_MIDDLE_LEFT);
	block_visibility_checkbox.setChecked(true);

	// entitygroup layer
	this->level_editor_layout_1.add(&entity_group_visibility_checkbox);
	entity_group_visibility_checkbox.setAutosizing(true);
	entity_group_visibility_checkbox.setText("Show entity groups");
	entity_group_visibility_checkbox.setFontColor(agui::Color(0, 0, 0));
	entity_group_visibility_checkbox.setCheckBoxAlignment(agui::ALIGN_MIDDLE_LEFT);
	entity_group_visibility_checkbox.setChecked(true);

	// tiled images layer
	this->level_editor_layout_1.add(&tiled_image_visibility_checkbox);
	tiled_image_visibility_checkbox.setAutosizing(true);
	tiled_image_visibility_checkbox.setText("Show tiled images");
	tiled_image_visibility_checkbox.setFontColor(agui::Color(0, 0, 0));
	tiled_image_visibility_checkbox.setCheckBoxAlignment(agui::ALIGN_MIDDLE_LEFT);
	tiled_image_visibility_checkbox.setChecked(true);

	// gridlines
	this->level_editor_layout_1.add(&grid_lines_visibility_checkbox);
	grid_lines_visibility_checkbox.setAutosizing(true);
	grid_lines_visibility_checkbox.setText("Show gridlines");
	grid_lines_visibility_checkbox.setFontColor(agui::Color(0, 0, 0));
	grid_lines_visibility_checkbox.setCheckBoxAlignment(agui::ALIGN_MIDDLE_LEFT);
	grid_lines_visibility_checkbox.setChecked(true);

	// object select
	this->level_editor_layout_1.add(&level_edit_select_mode_checkbox);
	level_edit_select_mode_checkbox.setAutosizing(true);
	level_edit_select_mode_checkbox.setText("Select objects");
	level_edit_select_mode_checkbox.setFontColor(agui::Color(0, 0, 0));
	level_edit_select_mode_checkbox.setCheckBoxAlignment(agui::ALIGN_MIDDLE_LEFT);
	level_edit_select_mode_checkbox.setChecked(false);
	
	// reset tile edges button
	this->level_editor_layout_1.add(&level_edit_reset_tile_edges_button);
	level_edit_reset_tile_edges_button.setSize(200, 40);
	level_edit_reset_tile_edges_button.setText("Reset tile edges");
	reset_tile_edges_select_listener.set_listener_type(RESET_TILE_EDGES);
	reset_tile_edges_select_listener.set_display(display);
	level_edit_reset_tile_edges_button.addActionListener(&reset_tile_edges_select_listener);
	this->level_editor_layout_1.add(new agui::EmptyWidget());

	this->level_editor_layout_1.add(&level_edit_object_tabbed_pane);

	level_edit_object_tabbed_pane.setSize(LEVEL_EDITOR_GRID_WIDTH / 2, 140);
	level_edit_object_tabs[0].setText("Tiles");
	level_edit_object_tabs[1].setText("Blocks");
	level_edit_object_tabs[2].setText("EntityGroups");

	// tabs for selecting objects to place in the level
	level_edit_object_tabbed_pane.addTab(&level_edit_object_tabs[TILE_SELECT_TAB],
		&level_edit_object_tile_scroll_pane);
	level_edit_object_tabbed_pane.addTab(&level_edit_object_tabs[BLOCK_SELECT_TAB],
		&level_edit_object_block_scroll_pane);
	level_edit_object_tabbed_pane.addTab(&level_edit_object_tabs[ENTITY_GROUP_SELECT_TAB],
		&level_edit_object_entity_group_scroll_pane);

	// add select boxes to scroll panes
	level_edit_object_tile_scroll_pane.add(&level_edit_object_tile_select_box);
	level_edit_object_block_scroll_pane.add(&level_edit_object_block_select_box);
	level_edit_object_entity_group_scroll_pane.add(&level_edit_object_entity_group_select_box);

	// set scroll pane sizes
	level_edit_object_tile_scroll_pane.setSize(LEVEL_EDITOR_GRID_WIDTH / 2, 140);
	level_edit_object_block_scroll_pane.setSize(LEVEL_EDITOR_GRID_WIDTH / 2, 140);
	level_edit_object_entity_group_scroll_pane.setSize(LEVEL_EDITOR_GRID_WIDTH / 2, 140);

	// set select box sizes
	level_edit_object_tile_select_box.setSize(LEVEL_EDITOR_GRID_WIDTH / 2, 140);
	level_edit_object_block_select_box.setSize(LEVEL_EDITOR_GRID_WIDTH / 2, 140);
	level_edit_object_entity_group_select_box.setSize(LEVEL_EDITOR_GRID_WIDTH / 2, 140);

	// selected object display
	this->level_editor_layout_1.add(&selected_level_object_display);
	//this->level_editor_layout_1.add(new agui::EmptyWidget());
	//this->level_editor_layout_1.add(new agui::EmptyWidget());

	// row buttons
	//this->level_editor_layout_1.add(&level_edit_object_col_label);
	//level_edit_object_col_label.setText("0");
	//level_edit_object_col_label.setFontColor(agui::Color(0, 0, 0));
	this->level_editor_layout_1.add(&level_edit_object_col_left_button);
	level_edit_object_col_left_button.setSize(40, 40);
	level_edit_object_col_left_button.setText("-");
	level_edit_object_col_left_listener.set_listener_type(SHEET_COL_LEFT);
	level_edit_object_col_left_listener.set_display(display);
	level_edit_object_col_left_button.addActionListener(&level_edit_object_col_left_listener);
	this->level_editor_layout_1.add(&level_edit_object_col_right_button);
	level_edit_object_col_right_button.setSize(40, 40);
	level_edit_object_col_right_button.setText("+");
	level_edit_object_col_right_listener.set_listener_type(SHEET_COL_RIGHT);
	level_edit_object_col_right_listener.set_display(display);
	level_edit_object_col_right_button.addActionListener(&level_edit_object_col_right_listener);
	//this->level_editor_layout_1.add(new agui::EmptyWidget());

	// layout 2

	this->level_editor_layout_frame_2.setSize(500, 720);
	this->level_editor_layout_frame_2.add(&level_editor_layout_2);
	this->level_editor_layout_2.setMaxOnRow(4);

	// tiled images label
	this->level_editor_layout_2.add(&tiled_images_label);
	tiled_images_label.setText("Tiled Images");
	tiled_images_label.setFontColor(agui::Color(0, 0, 0));
	this->level_editor_layout_2.add(new agui::EmptyWidget());
	this->level_editor_layout_2.add(new agui::EmptyWidget());
	this->level_editor_layout_2.add(new agui::EmptyWidget());

	// tiled image select box
	this->level_editor_layout_2.add(&tiled_image_scroll_pane);
	tiled_image_scroll_pane.add(&tiled_image_select_box);
	tiled_image_scroll_pane.setSize(LEVEL_EDITOR_GRID_WIDTH / 2, 140);
	tiled_image_select_box.setSize(LEVEL_EDITOR_GRID_WIDTH / 2, 140);
	this->level_editor_layout_2.add(new agui::EmptyWidget());
	this->level_editor_layout_2.add(new agui::EmptyWidget());
	this->level_editor_layout_2.add(new agui::EmptyWidget());

	// tiled image grid
	tiled_image_grid_frame.add(&tiled_image_grid);

	this->level_editor_layout_2.add(&tiled_image_grid_scroll_pane);
	tiled_image_grid_scroll_pane.setSize(TILED_IMAGE_GRID_WIDTH, TILED_IMAGE_GRID_HEIGHT);
	tiled_image_grid_scroll_pane.add(&tiled_image_grid_frame);
	tiled_image_grid_frame.setSize(TILED_IMAGE_GRID_WIDTH, TILED_IMAGE_GRID_HEIGHT);
	tiled_image_grid_frame.add(&tiled_image_grid);

	// layer selection
	this->level_editor_layout_2.add(&tiled_images_layer_label);
	tiled_images_layer_label.setText("Layer: ");
	tiled_images_layer_label.setFontColor(agui::Color(0, 0, 0));
	this->level_editor_layout_2.add(&tiled_images_layer_field);
	tiled_images_layer_field.setText("1");
	tiled_images_layer_field.setSize(80, 30);
	this->level_editor_layout_2.add(new agui::EmptyWidget());
}

void LevelEditorLayout::load_selected_tileset_tiles()
{
	LevelEditorDataManager &manager = LevelEditorDataManager::get_instance();
	this->level_edit_object_tile_select_box.clearItems();
	if (manager.has_selected_tileset()) {
		std::vector<std::string> tile_keys = manager.all_selected_tile_keys();
		for (std::string tile_key : tile_keys) {
			// level editor tab selection
			this->level_edit_object_tile_select_box.addItem(tile_key);
		}
	}
}

void LevelEditorLayout::load_selected_tileset_blocks()
{
	LevelEditorDataManager &manager = LevelEditorDataManager::get_instance();
	this->level_edit_object_block_select_box.clearItems();
	if (manager.has_selected_tileset()) {
		std::vector<std::string> block_keys = manager.all_selected_block_keys();
		for (std::string block_key : block_keys) {
			// level editor tab selection
			this->level_edit_object_block_select_box.addItem(block_key);
		}
	}
}

void LevelEditorLayout::load_selected_tileset_entity_groups()
{
	LevelEditorDataManager &manager = LevelEditorDataManager::get_instance();
	this->level_edit_object_entity_group_select_box.clearItems();
	if (manager.has_selected_tileset()) {
		std::vector<std::string> entity_group_keys = manager.all_selected_entity_group_keys();
		for (std::string entity_group_key : entity_group_keys) {
			// level editor tab selection
			this->level_edit_object_entity_group_select_box.addItem(entity_group_key);
		}
	}
}

void LevelEditorLayout::load_selected_tileset_tiled_images()
{
	LevelEditorDataManager &manager = LevelEditorDataManager::get_instance();
	this->tiled_image_select_box.clearItems();
	if (manager.has_selected_tileset()) {
		std::vector<std::string> tile_image_keys = manager.all_selected_tiled_image_keys();
		for (std::string tile_image_key : tile_image_keys) {
			this->tiled_image_select_box.addItem(tile_image_key);
		}
	}
}



void LevelEditorLayout::update()
{
	this->update_level_grid();
	this->update_tiled_image_grid();
	this->update_selected_level_object(false);
	this->update_selected_tiled_image();
}

void LevelEditorLayout::update_level_grid()
{
	this->level_editor_grid.set_select_mode(this->selecting_objects ? 1 : 0);
	this->level_editor_grid.update();
	int width = LEVEL_EDITOR_GRID_WIDTH;
	int height = LEVEL_EDITOR_GRID_HEIGHT;
	LevelEditorDataManager &manager = LevelEditorDataManager::get_instance();
	if (manager.has_selected_level()) {
		const std::pair<int, int> dim = manager.selected_level_pixel_dimensions();
		width = dim.first + 8, height = dim.second + 32;
		this->level_editor_grid.set_layer_visible(LevelEditorDataManager::TILE_LAYER, this->tile_visibility_checkbox.checked());
		this->level_editor_grid.set_layer_visible(LevelEditorDataManager::BLOCK_LAYER, this->block_visibility_checkbox.checked());
		this->level_editor_grid.set_layer_visible(LevelEditorDataManager::ENTITY_GROUP_LAYER, this->entity_group_visibility_checkbox.checked());
		this->level_editor_grid.set_layer_visible(LevelEditorDataManager::TILED_IMAGE_LAYER, this->tiled_image_visibility_checkbox.checked());
		this->level_editor_grid.set_layer_visible(LevelEditorDataManager::GRID_LINES_LAYER, this->grid_lines_visibility_checkbox.checked());
		//this->level_editor_grid.set_select_mode(this->level_edit_select_mode_checkbox.checked() ? 1 : 0);
		this->level_editor_grid.set_select_mode(this->level_edit_select_mode_checkbox.checked() ? 1 : 0);
	}
	// resize if necessary
	if (width != this->level_grid_frame.getWidth() || height != this->level_grid_frame.getHeight()) {
		this->level_grid_frame.setSize(width, height);
		this->level_editor_grid.setSize(width, height);
	}
}

void LevelEditorLayout::update_tiled_image_grid()
{
	this->tiled_image_grid.update();
	int width = TILED_IMAGE_GRID_WIDTH;
	int height = TILED_IMAGE_GRID_HEIGHT;
	LevelEditorDataManager &manager = LevelEditorDataManager::get_instance();
	if (manager.has_selected_tiled_image_key()) {
		const std::pair<int, int> dim = manager.selected_tiled_image_sheet_dimensions();
		width = dim.first + 16, height = dim.second + 32;
	}
	// resize if necessary
	if (width != this->tiled_image_grid_frame.getWidth() || height != this->tiled_image_grid_frame.getHeight()) {
		this->tiled_image_grid_frame.setSize(width, height);
		this->tiled_image_grid.setSize(width, height);
	}
}

void LevelEditorLayout::update_selected_level_object(const bool force)
{
	agui::ListBox *object_box = NULL;
	LevelEditorDataManager &manager = LevelEditorDataManager::get_instance();
	const int new_tab_index = this->level_edit_object_tabbed_pane.getSelectedIndex();
	switch (new_tab_index) {
	case TILE_SELECT_TAB:
		object_box = &(this->level_edit_object_tile_select_box);
		break;
	case BLOCK_SELECT_TAB:
		object_box = &(this->level_edit_object_block_select_box);
		break;
	case ENTITY_GROUP_SELECT_TAB:
		object_box = &(this->level_edit_object_entity_group_select_box);
		break;
	default:
		break;
	}
	bool update_object = false;
	int object_index = -1;
	if (object_box != NULL) {
		object_index = object_box->getSelectedIndex();
	}
	// new tab
	if (this->selected_object_tab_index() != new_tab_index) {
		this->set_selected_object_tab_index(new_tab_index);
		this->set_selected_object_select_index(-1);
		manager.set_selected_tiled_image_index(-1);
		manager.select_tiled_image(std::pair<int, int>(-1, -1));
		manager.set_selected_object_sheet_col(0);
		this->tiled_image_select_box.setSelectedIndex(-1);
	}
	// new object
	if (object_index >= 0 && manager.has_selected_tileset()) {
		const std::string object_name
			= object_box->getItemAt(object_index);
		update_object = this->selected_object_select_index() < 0
			|| object_index != this->selected_object_select_index();
		this->set_selected_object_select_index(object_index);
		if (update_object && manager.has_selected_tileset()) {
			ALLEGRO_BITMAP * object_bitmap = NULL;
			std::string updated_object_name = "";
			std::pair<int, int> dim(0, 0);
			switch (new_tab_index) {
			case TILE_SELECT_TAB:
				object_bitmap = manager.get_tile_bitmap_for_selected_col(object_index);
				dim = std::pair<int, int>(TILE_SIZE, TILE_SIZE);
				break;
			case BLOCK_SELECT_TAB:
				object_bitmap = manager.get_block_bitmap_for_selected_col(object_index);
				dim = std::pair<int, int>(TILE_SIZE, TILE_SIZE);
				break;
			case ENTITY_GROUP_SELECT_TAB:
				object_bitmap = manager.get_entity_group_bitmap_for_selected_col(object_index);
				dim = manager.get_entity_group_image_dimensions_by_index(object_index);
				break;
			default:
				break;
			}
			this->selected_level_object_display.set_bitmap(object_bitmap);
			this->selected_level_object_display.setSize(dim.first, dim.second);
			manager.set_selected_tiled_image_index(-1);
			manager.select_tiled_image(std::pair<int, int>(-1, -1));
			manager.set_selected_object_sheet_col(0);
			this->tiled_image_select_box.setSelectedIndex(-1);
		}
	}
}

void LevelEditorLayout::update_selected_tiled_image()
{
	const int current_tiled_image_index = LevelEditorDataManager::get_instance().get_selected_tiled_image_index();
	const int new_tiled_image_index = this->tiled_image_select_box.getSelectedIndex();
	if (current_tiled_image_index != new_tiled_image_index) {
		LevelEditorDataManager::get_instance().set_selected_tiled_image_index(new_tiled_image_index);
	}
	LevelEditorDataManager::get_instance().set_tiled_image_layer_index(this->get_input_tiled_image_layer());
}

void LevelEditorLayout::update_sheet_col()
{
	LevelEditorDataManager &manager = LevelEditorDataManager::get_instance();
	ALLEGRO_BITMAP * object_bitmap = NULL;
	std::string updated_object_name = "";
	std::pair<int, int> dim(0, 0);
	int object_index = 0;
	switch (this->selected_object_tab_index()) {
	case TILE_SELECT_TAB:
		object_index = this->level_edit_object_tile_select_box.getSelectedIndex();
		object_bitmap = manager.get_tile_bitmap_for_selected_col(object_index);
		dim = std::pair<int, int>(TILE_SIZE, TILE_SIZE);
		break;
	case BLOCK_SELECT_TAB:
		object_index = this->level_edit_object_block_select_box.getSelectedIndex();
		object_bitmap = manager.get_block_bitmap_for_selected_col(object_index);
		dim = std::pair<int, int>(TILE_SIZE, TILE_SIZE);
		break;
	case ENTITY_GROUP_SELECT_TAB:
		object_index = this->level_edit_object_tile_select_box.getSelectedIndex();
		object_bitmap = manager.get_entity_group_bitmap_for_selected_col(object_index);
		dim = manager.get_entity_group_image_dimensions_by_index(object_index);
		break;
	default:
		break;
	}
	this->selected_level_object_display.set_bitmap(object_bitmap);
	this->selected_level_object_display.setSize(dim.first, dim.second);
}

bool LevelEditorLayout::should_reset_tile_edges()
{
	return this->check_level_listener_flag(&reset_tile_edges_select_listener, SHOULD_UPDATE);
}

bool LevelEditorLayout::should_decrement_sheet_col()
{
	return this->check_level_listener_flag(&level_edit_object_col_left_listener, SHOULD_UPDATE);
}

bool LevelEditorLayout::should_increment_sheet_col()
{
	return this->check_level_listener_flag(&level_edit_object_col_right_listener, SHOULD_UPDATE);
}

void LevelEditorLayout::reset_grid_image_layer(std::string layer)
{
	this->level_editor_grid.reset_image_layer(layer);
}

void LevelEditorLayout::set_selecting_objects(const bool value)
{
	this->selecting_objects = value;
}
