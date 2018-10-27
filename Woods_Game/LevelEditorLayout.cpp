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

LevelEditorLayout::LevelEditorLayout(ALLEGRO_DISPLAY *display)
{
	// TODO: button for adding levels
	this->setMaxOnRow(4);

	// level grid label
	this->add(&level_grid_label);
	level_grid_label.setText("Level grid");
	level_grid_label.setFontColor(agui::Color(0, 0, 0));
	this->add(new agui::EmptyWidget());
	this->add(new agui::EmptyWidget());
	this->add(new agui::EmptyWidget());

	// level grid scroll pane
	this->add(&level_grid_scroll_pane);
	level_grid_scroll_pane.setSize(LEVEL_EDITOR_GRID_WIDTH, LEVEL_EDITOR_GRID_HEIGHT);
	level_grid_scroll_pane.add(&level_grid_frame);
	level_grid_frame.setSize(LEVEL_EDITOR_GRID_WIDTH, LEVEL_EDITOR_GRID_HEIGHT);
	level_grid_frame.add(&level_editor_grid);
	// TODO: action listener for level editor grid to add/remove/edit tiles
	this->add(new agui::EmptyWidget());
	this->add(new agui::EmptyWidget());
	this->add(new agui::EmptyWidget());

	// layer visiblity checkboxes

	// tile layer
	this->add(&tile_visibility_checkbox);
	tile_visibility_checkbox.setAutosizing(true);
	tile_visibility_checkbox.setText("Show tiles");
	tile_visibility_checkbox.setFontColor(agui::Color(0, 0, 0));
	tile_visibility_checkbox.setCheckBoxAlignment(agui::ALIGN_MIDDLE_LEFT);
	tile_visibility_checkbox.setChecked(true);

	// block layer
	this->add(&block_visibility_checkbox);
	block_visibility_checkbox.setAutosizing(true);
	block_visibility_checkbox.setText("Show blocks");
	block_visibility_checkbox.setFontColor(agui::Color(0, 0, 0));
	block_visibility_checkbox.setCheckBoxAlignment(agui::ALIGN_MIDDLE_LEFT);
	block_visibility_checkbox.setChecked(true);

	// entitygroup layer
	this->add(&entity_group_visibility_checkbox);
	entity_group_visibility_checkbox.setAutosizing(true);
	entity_group_visibility_checkbox.setText("Show entity groups");
	entity_group_visibility_checkbox.setFontColor(agui::Color(0, 0, 0));
	entity_group_visibility_checkbox.setCheckBoxAlignment(agui::ALIGN_MIDDLE_LEFT);
	entity_group_visibility_checkbox.setChecked(true);

	// gridlines
	this->add(&grid_lines_visibility_checkbox);
	grid_lines_visibility_checkbox.setAutosizing(true);
	grid_lines_visibility_checkbox.setText("Show gridlines");
	grid_lines_visibility_checkbox.setFontColor(agui::Color(0, 0, 0));
	grid_lines_visibility_checkbox.setCheckBoxAlignment(agui::ALIGN_MIDDLE_LEFT);
	grid_lines_visibility_checkbox.setChecked(true);

	// object select
	this->add(&level_edit_object_label);
	level_edit_object_label.setText("Select object");
	level_edit_object_label.setFontColor(agui::Color(0, 0, 0));
	this->add(new agui::EmptyWidget());
	this->add(new agui::EmptyWidget());
	this->add(new agui::EmptyWidget());

	this->add(&level_edit_object_tabbed_pane);

	level_edit_object_tabbed_pane.setSize(LEVEL_EDITOR_GRID_WIDTH / 2, 140);
	this->add(&level_edit_object_tabbed_pane);
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
	this->add(&selected_level_object_display);
	this->add(new agui::EmptyWidget());
	this->add(new agui::EmptyWidget());
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

void LevelEditorLayout::update()
{
	this->level_editor_grid.update();
	int width = LEVEL_EDITOR_GRID_WIDTH;
	int height = LEVEL_EDITOR_GRID_HEIGHT;
	LevelEditorDataManager &manager = LevelEditorDataManager::get_instance();
	if (manager.has_selected_level()) {
		const std::pair<int, int> dim = manager.selected_level_pixel_dimensions();
		width = dim.first, height = dim.second;
		this->level_editor_grid.set_layer_visible(LevelEditorDataManager::TILE_LAYER, this->tile_visibility_checkbox.checked());
		this->level_editor_grid.set_layer_visible(LevelEditorDataManager::BLOCK_LAYER, this->block_visibility_checkbox.checked());
		this->level_editor_grid.set_layer_visible(LevelEditorDataManager::ENTITY_GROUP_LAYER, this->entity_group_visibility_checkbox.checked());
		this->level_editor_grid.set_layer_visible(LevelEditorDataManager::GRID_LINES_LAYER, this->grid_lines_visibility_checkbox.checked());
	}
	// resize if necessary
	if (width != this->level_grid_frame.getWidth() || height != this->level_grid_frame.getHeight()) {
		this->level_grid_frame.setSize(width, height);
		this->level_editor_grid.setSize(width, height);
	}
	this->update_selected_level_object();
}

void LevelEditorLayout::update_selected_level_object()
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
	if (this->selected_object_tab_index() != new_tab_index) {
		this->set_selected_object_tab_index(new_tab_index);
		this->set_selected_object_select_index(-1);
	}
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
				object_bitmap = manager.get_default_tile_bitmap(object_index);
				dim = std::pair<int, int>(TILE_SIZE, TILE_SIZE);
				break;
			case BLOCK_SELECT_TAB:
				object_bitmap = manager.get_default_block_bitmap(object_index);
				dim = std::pair<int, int>(TILE_SIZE, TILE_SIZE);
				break;
			case ENTITY_GROUP_SELECT_TAB:
				object_bitmap = manager.get_default_entity_group_bitmap(object_index);
				dim = manager.get_entity_group_image_dimensions_by_index(object_index);
				break;
			default:
				break;
			}
			this->selected_level_object_display.set_bitmap(object_bitmap);
			this->selected_level_object_display.setSize(dim.first, dim.second);
		}
	}
}
