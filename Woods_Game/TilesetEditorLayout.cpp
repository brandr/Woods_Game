#include "TilesetEditorLayout.h"

TilesetEditorLayout::TilesetEditorLayout(ALLEGRO_DISPLAY * display)
{
	this->setMaxOnRow(4);

	// tileset name
	this->add(&tileset_tile_frame);
	this->setMaxOnRow(4);
	tileset_tile_frame.setSize(380, 600);
	tileset_tile_frame.add(&tileset_tile_layout);
	tileset_tile_layout.add(&tileset_label);
	tileset_label.setText("Tileset name");
	tileset_label.setFontColor(agui::Color(0, 0, 0));
	tileset_tile_layout.add(&selected_tileset_name_field);
	selected_tileset_name_field.setSize(200, 30);
	tileset_tile_layout.add(new agui::EmptyWidget());
	tileset_tile_layout.add(new agui::EmptyWidget());

	// tileset select box
	tileset_tile_layout.add(&tileset_select_box);
	tileset_select_box.setSize(380, 180);
	tileset_tile_layout.add(new agui::EmptyWidget());
	tileset_tile_layout.add(new agui::EmptyWidget());
	tileset_tile_layout.add(new agui::EmptyWidget());

	//tiles
	tileset_tile_layout.add(&tileset_tile_label);
	tileset_tile_label.setText("Tiles");
	tileset_tile_layout.add(new agui::EmptyWidget());
	tileset_tile_layout.add(new agui::EmptyWidget());
	tileset_tile_layout.add(new agui::EmptyWidget());

	//tile select window
	tileset_tile_layout.add(&tileset_tile_select_box);
	tileset_tile_select_box.setSize(380, 180);
	tileset_tile_layout.add(new agui::EmptyWidget());
	tileset_tile_layout.add(new agui::EmptyWidget());
	tileset_tile_layout.add(new agui::EmptyWidget());

	tileset_tile_layout.add(&selected_tile_name_label);
	selected_tile_name_label.setText("Tile Name");
	tileset_tile_layout.add(new agui::EmptyWidget());
	tileset_tile_layout.add(new agui::EmptyWidget());

	tileset_tile_layout.add(&selected_tile_name_field);
	selected_tile_name_field.setSize(200, 30);
	tileset_tile_layout.add(&tile_image_display);
	tile_image_display.setSize(TILE_SIZE, TILE_SIZE);
	tileset_tile_layout.add(new agui::EmptyWidget());
	tileset_tile_layout.add(new agui::EmptyWidget());

	//blocks

	this->add(&tileset_block_frame);
	tileset_block_frame.setSize(380, 600);
	tileset_block_frame.add(&tileset_block_layout);

	tileset_block_layout.setMaxOnRow(4);
	tileset_block_layout.add(&tileset_block_label);
	tileset_block_label.setText("Blocks");
	tileset_block_layout.add(new agui::EmptyWidget());
	tileset_block_layout.add(new agui::EmptyWidget());
	tileset_block_layout.add(new agui::EmptyWidget());

	//block select window
	tileset_block_layout.add(&tileset_block_select_box);
	tileset_block_select_box.setSize(380, 180);
	tileset_block_layout.add(new agui::EmptyWidget());
	tileset_block_layout.add(new agui::EmptyWidget());
	tileset_block_layout.add(new agui::EmptyWidget());

	tileset_block_layout.add(&selected_block_name_label);
	selected_block_name_label.setText("Block Name");
	tileset_block_layout.add(&selected_block_name_field);
	selected_block_name_field.setSize(200, 30);
	tileset_block_layout.add(&block_image_display);
	block_image_display.setSize(TILE_SIZE, TILE_SIZE);
	tileset_block_layout.add(new agui::EmptyWidget());

	//entitygroup
	tileset_entity_group_layout.setMaxOnRow(4);
	tileset_entity_group_layout.add(&tileset_block_label);
	tileset_block_label.setText("Blocks");
	tileset_entity_group_layout.add(new agui::EmptyWidget());
	tileset_entity_group_layout.add(new agui::EmptyWidget());
	tileset_entity_group_layout.add(new agui::EmptyWidget());

	//entitygroup select window

	this->add(&tileset_entity_group_frame);
	tileset_entity_group_frame.setSize(380, 600);
	tileset_entity_group_frame.add(&tileset_entity_group_layout);

	tileset_entity_group_layout.add(&tileset_entity_group_label);
	tileset_entity_group_label.setText("EntityGroups");
	tileset_entity_group_layout.add(new agui::EmptyWidget());
	tileset_entity_group_layout.add(new agui::EmptyWidget());
	tileset_entity_group_layout.add(new agui::EmptyWidget());

	tileset_entity_group_layout.add(&tileset_entity_group_select_box);
	tileset_entity_group_select_box.setSize(380, 180);
	tileset_entity_group_layout.add(new agui::EmptyWidget());
	tileset_entity_group_layout.add(new agui::EmptyWidget());
	tileset_entity_group_layout.add(new agui::EmptyWidget());

	tileset_entity_group_layout.add(&selected_entity_group_name_label);
	selected_entity_group_name_label.setText("EntityGroup Name");
	tileset_entity_group_layout.add(&selected_entity_group_name_field);
	selected_entity_group_name_field.setSize(200, 30);
	tileset_entity_group_layout.add(&entity_group_image_display);
	tileset_entity_group_layout.add(new agui::EmptyWidget());
}

void TilesetEditorLayout::load_tilesets()
{
	std::vector<std::string> tileset_names = LevelEditorDataManager::get_instance().all_tileset_names();
	for (std::string tileset_name : tileset_names) {
		tileset_select_box.addItem(tileset_name);
	}
}

void TilesetEditorLayout::load_selected_tileset_tiles()
{
	LevelEditorDataManager &manager = LevelEditorDataManager::get_instance();
	this->tileset_tile_select_box.clearItems();
	if (manager.has_selected_tileset()) {
		std::vector<std::string> tile_keys = manager.all_selected_tile_keys();
		for (std::string tile_key : tile_keys) {
			// tileset tab selection
			this->tileset_tile_select_box.addItem(tile_key);
		}
	}
}

void TilesetEditorLayout::load_selected_tileset_blocks()
{
	LevelEditorDataManager &manager = LevelEditorDataManager::get_instance();
	this->tileset_block_select_box.clearItems();
	if (manager.has_selected_tileset()) {
		std::vector<std::string> block_keys
			= manager.all_selected_block_keys();
		for (std::string block_key : block_keys) {
			// tileset tab selection
			this->tileset_block_select_box.addItem(block_key);
		}
	}
}

void TilesetEditorLayout::load_selected_tileset_entity_groups()
{
	LevelEditorDataManager &manager = LevelEditorDataManager::get_instance();
	this->tileset_entity_group_select_box.clearItems();
	if (manager.has_selected_tileset()) {
		std::vector<std::string> entity_group_keys
			= manager.all_selected_entity_group_keys();
		for (std::string entity_group_key : entity_group_keys) {
			// tileset tab selection
			this->tileset_entity_group_select_box.addItem(entity_group_key);
		}
	}
}

void TilesetEditorLayout::update_selected_tileset()
{
	this->update_selected_tileset_tiles();
	this->update_selected_tileset_blocks();
	this->update_selected_tileset_entity_groups();
}

void TilesetEditorLayout::update_selected_tileset_fields()
{
	LevelEditorDataManager &manager = LevelEditorDataManager::get_instance();
	const std::string tileset_name = manager.get_selected_tileset_name();
	this->selected_tileset_name_field.setText(tileset_name);
}

void TilesetEditorLayout::update_selected_tileset_tiles()
{
	LevelEditorDataManager &manager = LevelEditorDataManager::get_instance();
	const int tile_index = this->tileset_tile_select_box.getSelectedIndex();
	const bool should_update = manager.update_selected_tileset_tile(tile_index);
	if (should_update) {
		const std::string tile_name
			= this->tileset_tile_select_box.getItemAt(tile_index);
		this->selected_tile_name_field.setText(tile_name);
		ALLEGRO_BITMAP * tile_bitmap = manager.get_default_tile_bitmap(tile_index);
		this->tile_image_display.set_bitmap(tile_bitmap);
	}
}

void TilesetEditorLayout::update_selected_tileset_blocks()
{
	LevelEditorDataManager &manager = LevelEditorDataManager::get_instance();
	const int block_index = this->tileset_block_select_box.getSelectedIndex();
	const bool should_update = manager.update_selected_tileset_block(block_index);
	if (should_update) {
		const std::string block_name
			= this->tileset_block_select_box.getItemAt(block_index);
		this->selected_block_name_field.setText(block_name);
		ALLEGRO_BITMAP * block_bitmap = manager.get_default_block_bitmap(block_index);
		this->block_image_display.set_bitmap(block_bitmap);
	}
}

void TilesetEditorLayout::update_selected_tileset_entity_groups()
{
	LevelEditorDataManager &manager = LevelEditorDataManager::get_instance();
	const int eg_index = this->tileset_entity_group_select_box.getSelectedIndex();
	const bool should_update = manager.update_selected_tileset_entity_group(eg_index);
	if (should_update) {
		const std::string eg_name
			= this->tileset_entity_group_select_box.getItemAt(eg_index);
		this->selected_entity_group_name_field.setText(eg_name);
		ALLEGRO_BITMAP * eg_bitmap = manager.get_default_entity_group_bitmap(eg_index);
		this->entity_group_image_display.set_bitmap(eg_bitmap);
		const std::pair<float, float> eg_dimensions
			= manager.get_entity_group_image_dimensions_by_index(eg_index);
		this->entity_group_image_display.setSize(eg_dimensions.first, eg_dimensions.second);
	}
}

void TilesetEditorLayout::set_selected_tileset_index(int value)
{
	this->tileset_select_box.setSelectedIndex(value);
}

int TilesetEditorLayout::get_selected_tileset_index()
{
	return this->tileset_select_box.getSelectedIndex();
}

std::string TilesetEditorLayout::get_tileset_name(int index)
{
	return this->tileset_select_box.getItemAt(index);
}
