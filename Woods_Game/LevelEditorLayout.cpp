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

const std::string LevelEditorLayout::selected_object_name()
{
	return LevelEditorDataManager::get_instance().get_selected_object_display_name();
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

	this->level_editor_layout_frame_1.setSize(700, 720);
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

	// spawner layer
	this->level_editor_layout_1.add(&spawner_visibility_checkbox);
	spawner_visibility_checkbox.setAutosizing(true);
	spawner_visibility_checkbox.setText("Show spawners");
	spawner_visibility_checkbox.setFontColor(agui::Color(0, 0, 0));
	spawner_visibility_checkbox.setCheckBoxAlignment(agui::ALIGN_MIDDLE_LEFT);
	spawner_visibility_checkbox.setChecked(true);

	// path node layer
	this->level_editor_layout_1.add(&path_node_visibility_checkbox);
	path_node_visibility_checkbox.setAutosizing(true);
	path_node_visibility_checkbox.setText("Show path nodes");
	path_node_visibility_checkbox.setFontColor(agui::Color(0, 0, 0));
	path_node_visibility_checkbox.setCheckBoxAlignment(agui::ALIGN_MIDDLE_LEFT);
	path_node_visibility_checkbox.setChecked(true);
	
	// gridlines
	this->level_editor_layout_1.add(&grid_lines_visibility_checkbox);
	grid_lines_visibility_checkbox.setAutosizing(true);
	grid_lines_visibility_checkbox.setText("Show gridlines");
	grid_lines_visibility_checkbox.setFontColor(agui::Color(0, 0, 0));
	grid_lines_visibility_checkbox.setCheckBoxAlignment(agui::ALIGN_MIDDLE_LEFT);
	grid_lines_visibility_checkbox.setChecked(true);
	this->level_editor_layout_1.add(new agui::EmptyWidget());

	// reset level button
	this->level_editor_layout_1.add(&level_edit_reset_level_button);
	level_edit_reset_level_button.setSize(180, 36);
	level_edit_reset_level_button.setText("Reset level");
	reset_level_select_listener.set_listener_type(RESET_LEVEL);
	reset_level_select_listener.set_display(display);
	level_edit_reset_level_button.addActionListener(&reset_level_select_listener);
	
	// reset tile edges button
	this->level_editor_layout_1.add(&level_edit_reset_tile_edges_button);
	level_edit_reset_tile_edges_button.setSize(180, 36);
	level_edit_reset_tile_edges_button.setText("Reset tile edges");
	reset_tile_edges_select_listener.set_listener_type(RESET_TILE_EDGES);
	reset_tile_edges_select_listener.set_display(display);
	level_edit_reset_tile_edges_button.addActionListener(&reset_tile_edges_select_listener);

	this->level_editor_layout_1.add(&level_edit_object_tabbed_pane);

	level_edit_object_tabbed_pane.setSize(2 * LEVEL_EDITOR_GRID_WIDTH / 3, 140);
	level_edit_object_tabs[TILE_SELECT_TAB].setText("Tiles");
	level_edit_object_tabs[BLOCK_SELECT_TAB].setText("Blocks");
	level_edit_object_tabs[ENTITY_GROUP_SELECT_TAB].setText("EntityGroups");
	level_edit_object_tabs[SPAWNER_TAB].setText("Spawners");
	level_edit_object_tabs[PATH_NODE_TAB].setText("Nodes");

	// tabs for selecting objects to place in the level
	level_edit_object_tabbed_pane.addTab(&level_edit_object_tabs[TILE_SELECT_TAB],
		&level_edit_object_tile_select_box);
	level_edit_object_tabbed_pane.addTab(&level_edit_object_tabs[BLOCK_SELECT_TAB],
		&level_edit_object_block_select_box);
	level_edit_object_tabbed_pane.addTab(&level_edit_object_tabs[ENTITY_GROUP_SELECT_TAB],
		&level_edit_object_entity_group_select_box);
	level_edit_object_tabbed_pane.addTab(&level_edit_object_tabs[SPAWNER_TAB],
		&level_edit_object_spawner_select_box);
	level_edit_object_tabbed_pane.addTab(&level_edit_object_tabs[PATH_NODE_TAB],
		&level_edit_object_path_node_select_box);

	level_edit_object_tile_select_box.setMargins(0, 0, 28, 0);
	level_edit_object_block_select_box.setMargins(0, 0, 28, 0);
	level_edit_object_entity_group_select_box.setMargins(0, 0, 28, 0);
	level_edit_object_spawner_select_box.setMargins(0, 0, 28, 0);
	level_edit_object_path_node_select_box.setMargins(0, 0, 28, 0);

	// set select box sizes
	level_edit_object_tile_select_box.setSize(2 * LEVEL_EDITOR_GRID_WIDTH / 3, 140);
	level_edit_object_block_select_box.setSize(2 * LEVEL_EDITOR_GRID_WIDTH / 3, 140);
	level_edit_object_entity_group_select_box.setSize(2 * LEVEL_EDITOR_GRID_WIDTH / 3, 140);
	level_edit_object_spawner_select_box.setSize(2 * LEVEL_EDITOR_GRID_WIDTH / 3, 140);
	level_edit_object_path_node_select_box.setSize(2 * LEVEL_EDITOR_GRID_WIDTH / 3, 140);

	// selected object display
	this->level_editor_layout_1.add(&selected_level_object_display);

	// row buttons
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

	// layout 2

	this->level_editor_layout_frame_2.setSize(500, 720);
	this->level_editor_layout_frame_2.add(&level_editor_layout_2);
	this->level_editor_layout_2.setMaxOnRow(2);

	this->level_editor_layout_2.add(&secondary_tabbed_pane);
	secondary_tabbed_pane.setSize(470, 670);

	level_edit_secondary_tabs[TILED_IMAGE_TAB].setText("Tiled Images");
	level_edit_secondary_tabs[EDIT_OBJECT_TAB].setText("Edit Object");

	secondary_tabbed_pane.addTab(&level_edit_secondary_tabs[TILED_IMAGE_TAB],
		&tiled_image_frame);
	secondary_tabbed_pane.addTab(&level_edit_secondary_tabs[EDIT_OBJECT_TAB],
		&edit_object_frame);

	this->tiled_image_frame.add(&tiled_image_layout);
	this->edit_object_frame.add(&edit_object_layout);
	this->tiled_image_layout.setMaxOnRow(4);
	this->edit_object_layout.setMaxOnRow(4);

	this->tiled_image_frame.setSize(470, 670);
	this->edit_object_frame.setSize(470, 670);

	// tiled images label
	this->tiled_image_layout.add(&tiled_images_label);
	tiled_images_label.setText("Tiled Images");
	tiled_images_label.setFontColor(agui::Color(0, 0, 0));
	this->tiled_image_layout.add(new agui::EmptyWidget());
	this->tiled_image_layout.add(new agui::EmptyWidget());
	this->tiled_image_layout.add(new agui::EmptyWidget());

	// tiled image select box
	this->tiled_image_layout.add(&tiled_image_scroll_pane);
	tiled_image_scroll_pane.add(&tiled_image_select_box);
	tiled_image_scroll_pane.setSize(LEVEL_EDITOR_GRID_WIDTH / 2, 140);
	tiled_image_select_box.setSize(LEVEL_EDITOR_GRID_WIDTH / 2, 140);
	this->tiled_image_layout.add(new agui::EmptyWidget());
	this->tiled_image_layout.add(new agui::EmptyWidget());
	this->tiled_image_layout.add(new agui::EmptyWidget());

	// tiled image grid
	tiled_image_grid_frame.add(&tiled_image_grid);

	this->tiled_image_layout.add(&tiled_image_grid_scroll_pane);
	tiled_image_grid_scroll_pane.setSize(TILED_IMAGE_GRID_WIDTH, TILED_IMAGE_GRID_HEIGHT);
	tiled_image_grid_scroll_pane.add(&tiled_image_grid_frame);
	tiled_image_grid_frame.setSize(TILED_IMAGE_GRID_WIDTH, TILED_IMAGE_GRID_HEIGHT);
	tiled_image_grid_frame.add(&tiled_image_grid);

	// layer selection
	this->tiled_image_layout.add(&tiled_images_layer_label);
	tiled_images_layer_label.setText("Layer: ");
	tiled_images_layer_label.setFontColor(agui::Color(0, 0, 0));
	this->tiled_image_layout.add(&tiled_images_layer_field);
	tiled_images_layer_field.setText("1");
	tiled_images_layer_field.setSize(80, 30);
	this->tiled_image_layout.add(new agui::EmptyWidget());

	// edit object layout

	this->edit_object_layout.add(&selected_object_label);
	this->selected_object_label.setText("Selected object: " + this->selected_object_name());
	this->edit_object_layout.add(new agui::EmptyWidget());
	this->edit_object_layout.add(new agui::EmptyWidget());
	this->edit_object_layout.add(new agui::EmptyWidget());

	this->edit_object_layout.add(&selected_level_object_instance_display);
	this->edit_object_layout.add(new agui::EmptyWidget());
	this->edit_object_layout.add(new agui::EmptyWidget());
	this->edit_object_layout.add(new agui::EmptyWidget());

	// attributes
	this->edit_object_layout.add(&selected_level_object_instance_attributes_label);
	selected_level_object_instance_attributes_label.setText("Attributes");
	selected_level_object_instance_attributes_label.setFontColor(agui::Color(0, 0, 0));
	this->edit_object_layout.add(new agui::EmptyWidget());
	this->edit_object_layout.add(new agui::EmptyWidget());
	this->edit_object_layout.add(new agui::EmptyWidget());

	// scrollable text edit pane
	agui::Allegro5Font * attributes_font = new agui::Allegro5Font("resources/fonts/Monospace.ttf", 12);
	selected_level_object_instance_attributes_box.setFont(attributes_font);
	this->edit_object_layout.add(&object_instance_attributes_scroll_pane);
	object_instance_attributes_scroll_pane.setVScrollPolicy(agui::ScrollPolicy::SHOW_ALWAYS);
	object_instance_attributes_scroll_pane.setSize(ATTRIBUTE_PANE_WIDTH, ATTRIBUTE_PANE_HEIGHT);
	object_instance_attributes_scroll_pane.add(&object_instance_attribute_frame);
	object_instance_attribute_frame.setSize(ATTRIBUTE_PANE_WIDTH, ATTRIBUTE_PANE_HEIGHT);
	object_instance_attribute_frame.add(&selected_level_object_instance_attributes_box);
	selected_level_object_instance_attributes_box.setSize(ATTRIBUTE_PANE_WIDTH, ATTRIBUTE_PANE_HEIGHT);
	this->edit_object_layout.add(new agui::EmptyWidget());
	this->edit_object_layout.add(new agui::EmptyWidget());
	this->edit_object_layout.add(new agui::EmptyWidget());

	// save button
	this->edit_object_layout.add(&save_instance_attributes_button);
	save_instance_attributes_button.setSize(60, 40);
	save_instance_attributes_button.setText("Save");
	save_instance_attributes_select_listener.set_listener_type(SAVE_INSTANCE_ATTRIBUTES);
	save_instance_attributes_select_listener.set_display(display);
	save_instance_attributes_button.addActionListener(&save_instance_attributes_select_listener);
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
		this->level_edit_object_tile_select_box.resizeHeightToContents();
		this->level_edit_object_tile_scroll_pane.resizeHeightToContents();
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
		this->level_edit_object_entity_group_select_box.resizeHeightToContents();
	}
}

void LevelEditorLayout::load_selected_tileset_spawners()
{
	LevelEditorDataManager &manager = LevelEditorDataManager::get_instance();
	this->level_edit_object_spawner_select_box.clearItems();
	if (manager.has_selected_tileset()) {
		std::vector<std::string> spawner_keys = manager.all_selected_spawner_keys();
		for (std::string spawner_key : spawner_keys) {
			// level editor tab selection
			this->level_edit_object_spawner_select_box.addItem(spawner_key);
		}
		this->level_edit_object_spawner_select_box.resizeHeightToContents();
	}
}

void LevelEditorLayout::load_selected_tileset_path_nodes()
{
	LevelEditorDataManager &manager = LevelEditorDataManager::get_instance();
	this->level_edit_object_path_node_select_box.clearItems();
	if (manager.has_selected_tileset()) {
		std::vector<std::string> node_keys = manager.all_selected_path_node_keys();
		for (std::string node_key : node_keys) {
			// level editor tab selection
			this->level_edit_object_path_node_select_box.addItem(node_key);
		}
		this->level_edit_object_path_node_select_box.resizeHeightToContents();
	}
}

//TODO: is this actually used?
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
	this->update_selected_level_object_instance();
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
		this->level_editor_grid.set_layer_visible(LevelEditorDataManager::SPAWNER_LAYER, this->spawner_visibility_checkbox.checked());
		this->level_editor_grid.set_layer_visible(LevelEditorDataManager::PATH_NODE_LAYER, this->path_node_visibility_checkbox.checked());
		this->level_editor_grid.set_layer_visible(LevelEditorDataManager::GRID_LINES_LAYER, this->grid_lines_visibility_checkbox.checked());
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
	case SPAWNER_TAB:
		object_box = &(this->level_edit_object_spawner_select_box);
		break;
	case PATH_NODE_TAB:
		object_box = &(this->level_edit_object_path_node_select_box);
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
	if (manager.has_selected_tileset()) {
		if (object_index >= 0) {
			const std::string object_name
				= object_box->getItemAt(object_index);
			update_object = this->selected_object_select_index() < 0
				|| object_index != this->selected_object_select_index();
			this->set_selected_object_select_index(object_index);
			if (update_object) {
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
				case SPAWNER_TAB:
					object_bitmap = manager.get_spawner_bitmap_for_selected_col(object_index);
					dim = std::pair<int, int>(TILE_SIZE, TILE_SIZE);
					break;
				case PATH_NODE_TAB:
					object_bitmap = manager.get_path_node_bitmap_for_selected_col(object_index);
					dim = std::pair<int, int>(TILE_SIZE, TILE_SIZE);
					break;
				default:
					break;
				}
				this->selected_level_object_display.set_bitmap(object_bitmap);
				this->selected_level_object_display.setSize(dim.first, dim.second);
				manager.set_selected_tiled_image_index(-1);
				manager.select_tiled_image(std::pair<int, int>(-1, -1));
				manager.set_selected_object_sheet_col(0); // TODO: is this part right?
				this->tiled_image_select_box.setSelectedIndex(-1);
			}
		}
	}
}

void LevelEditorLayout::update_selected_level_object_instance()
{
	LevelEditorDataManager &manager = LevelEditorDataManager::get_instance();
	if (!manager.selected_level_object_instance_matches(this->last_selected_object_instance_pos,
		this->last_selected_object_instance_index)) {
		this->selected_object_label.setText("Selected object: " + this->selected_object_name());
		const std::pair<int, int> dim = manager.get_selected_object_instance_dimensions();
		ALLEGRO_BITMAP * object_bitmap = manager.get_selected_object_instance_bitmap();
		this->selected_level_object_instance_display.set_bitmap(object_bitmap);
		this->selected_level_object_instance_display.setSize(dim.first, dim.second);
		this->last_selected_object_instance_pos = manager.get_selected_object_grid_pos();
		this->last_selected_object_instance_index = manager.get_selected_object_grid_index();
		this->reset_selected_level_object_instance_attributes();
	}
}

void LevelEditorLayout::reset_selected_level_object_instance_attributes()
{
	LevelEditorDataManager &manager = LevelEditorDataManager::get_instance();
	FileManager * file_manager = new FileManager();
	this->selected_level_object_instance_attributes_box.setText("");
	const std::string xml = manager.get_selected_object_instance_xml();
	if (xml.size() > 0) {
		this->selected_level_object_instance_attributes_box.clear();
		this->selected_level_object_instance_attributes_box.setText(xml);
		this->selected_level_object_instance_attributes_box.resizeToContents();
		this->object_instance_attribute_frame.setSize(this->selected_level_object_instance_attributes_box.getWidth() + 12,
			this->selected_level_object_instance_attributes_box.getHeight() + 12);
	}
}

void LevelEditorLayout::update_selected_tiled_image()
{
	const int current_tiled_image_index = LevelEditorDataManager::get_instance().get_selected_tiled_image_index();
	const int new_tiled_image_index = this->tiled_image_select_box.getSelectedIndex();
	if (current_tiled_image_index != new_tiled_image_index) {
		LevelEditorDataManager::get_instance().set_selected_tiled_image_index(new_tiled_image_index);
	}
	LevelEditorDataManager::get_instance().set_tiled_image_layer_index(this->get_input_tiled_image_layer()); //TODO: is this part necessary?
}

void LevelEditorLayout::update_selected_tileset()
{
	const int current_tileset_index = LevelEditorDataManager::get_instance().get_selected_tileset_index();
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

const bool LevelEditorLayout::should_reset_level()
{
	return this->check_level_listener_flag(&reset_level_select_listener, SHOULD_UPDATE);
}

const bool LevelEditorLayout::should_reset_tile_edges()
{
	return this->check_level_listener_flag(&reset_tile_edges_select_listener, SHOULD_UPDATE);
}

const bool LevelEditorLayout::should_decrement_sheet_col()
{
	return this->check_level_listener_flag(&level_edit_object_col_left_listener, SHOULD_UPDATE);
}

const bool LevelEditorLayout::should_increment_sheet_col()
{
	return this->check_level_listener_flag(&level_edit_object_col_right_listener, SHOULD_UPDATE);
}

const bool LevelEditorLayout::should_save_instance_attributes()
{
	return this->check_level_listener_flag(&save_instance_attributes_select_listener, SHOULD_UPDATE);
}

void LevelEditorLayout::reset_all_grid_image_layers()
{
	this->level_editor_grid.reset_all_grid_image_layers();
}

void LevelEditorLayout::reset_grid_image_layer(const std::string layer)
{
	const std::string level_name = LevelEditorDataManager::get_instance().get_selected_level_name();
	this->level_editor_grid.reset_image_layer(layer, level_name);
}

void LevelEditorLayout::set_selecting_objects(const bool value)
{
	this->selecting_objects = value;
}

void LevelEditorLayout::save_instance_attributes()
{
	const std::string xml = this->selected_level_object_instance_attributes_box.getText();
	const bool did_serialize = LevelEditorDataManager::get_instance().replace_selected_object_instance_xml(xml);
	if (did_serialize) {
		const std::string level_name = LevelEditorDataManager::get_instance().get_selected_level_name();
		this->level_editor_grid.reset_image_layer(LevelEditorDataManager::get_instance().get_selected_object_tab_index(), level_name);
	}
}
