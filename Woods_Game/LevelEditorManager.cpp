#include "LevelEditorManager.h"

LevelEditorManager::LevelEditorManager()
{
	
}

LevelEditorManager::LevelEditorManager(agui::Gui * guiInstance, ALLEGRO_DISPLAY * display)
{
	editor_gui = guiInstance;
	this->dungeon_edit_layout = new DungeonEditorLayout(display);
	this->level_edit_layout = new LevelEditorLayout(display);
	this->tileset_edit_layout = new TilesetEditorLayout(display);

	// tabs
	tabbed_pane.setSize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
	main_layout.add(&tabbed_pane);

	tabs[0].setText("Dungeon");
	tabs[1].setText("Level");
	tabs[2].setText("Tileset");
	tabbed_pane.addTab(&tabs[0], dungeon_edit_layout);
	tabbed_pane.addTab(&tabs[1], level_edit_layout);
	tabbed_pane.addTab(&tabs[2], tileset_edit_layout);
	editor_gui->add(&main_layout);
	editor_gui->addKeyPreviewListener(&level_grid_keyboard_listener);

	// load
	this->load_all_tilesets();
}

LevelEditorManager::~LevelEditorManager()
{
}

void LevelEditorManager::reset_fields()
{
	this->dungeon_edit_layout->reset_fields();
}

void LevelEditorManager::save_dungeon()
{
	this->update_attributes();
	const std::string dungeon_name = this->dungeon_edit_layout->get_input_dungeon_name();
	LevelEditorDataManager::get_instance().save_dungeon(dungeon_name);
	this->dungeon_edit_layout->reset_fields();
}

void LevelEditorManager::update_attributes()
{
	this->update_level_attributes();
	//TODO: changes to level grid objects, tilesets, etc.
}

void LevelEditorManager::update_level_attributes()
{
	const std::string input_level_name = this->dungeon_edit_layout->get_input_level_name();
	const std::pair<int, int> level_pos = this->dungeon_edit_layout->get_input_level_grid_pos();
	const std::pair<int, int> level_dimensions = this->dungeon_edit_layout->get_input_level_dimensions();
	LevelEditorDataManager::get_instance().set_active_level_attributes(input_level_name, level_pos, level_dimensions);
}

void LevelEditorManager::load_all_tilesets()
{
	LevelEditorDataManager::get_instance().load_all_tilesets();
	this->tileset_edit_layout->load_tilesets();
}

void LevelEditorManager::update()
{
	this->dungeon_update();
	this->level_update();
	this->tileset_update();
}

void LevelEditorManager::dungeon_update()
{
	this->update_load_dungeon();
	this->update_add_dungeon();
	this->update_delete_dungeon();
	this->update_save_dungeon();
}

void LevelEditorManager::level_update()
{
	// dungeon view
	this->update_selected_level();
	this->update_add_level();
	this->update_delete_level();
	// level view
	this->update_reset_level();
	this->update_reset_tile_edges();
	this->update_sheet_col();
	this->update_level_grid();
	this->update_object_instance();
}

void LevelEditorManager::tileset_update()
{
	this->update_selected_tileset();
}

void LevelEditorManager::update_load_dungeon()
{
	this->dungeon_edit_layout->update_load_dungeon();
}

void LevelEditorManager::update_selected_level()
{
	const bool should_update = this->dungeon_edit_layout->update_selected_level();
	if (should_update) {
		const int tileset_index = LevelEditorDataManager::get_instance().selected_level_tileset_index();
		LevelEditorDataManager::get_instance().set_selected_tileset_index(tileset_index);
		// level edit layout
		this->level_edit_layout->load_selected_tileset_tiles();
		this->level_edit_layout->load_selected_tileset_blocks();
		this->level_edit_layout->load_selected_tileset_entity_groups();
		this->level_edit_layout->load_selected_tileset_spawners();
		this->level_edit_layout->load_selected_tileset_path_nodes();
		this->level_edit_layout->load_selected_tileset_tiled_images();
		// tileset layout
		this->tileset_edit_layout->set_selected_tileset_index(tileset_index);
		this->tileset_edit_layout->update_selected_tileset_fields();
		this->tileset_edit_layout->load_selected_tileset_tiles();
		this->tileset_edit_layout->load_selected_tileset_blocks();
		this->tileset_edit_layout->load_selected_tileset_entity_groups();
	}
}

void LevelEditorManager::update_level_grid()
{
	this->level_edit_layout->set_selecting_objects(this->level_grid_keyboard_listener.has_control_held());
	this->level_edit_layout->update();
}

void LevelEditorManager::update_object_instance()
{
	if (this->level_edit_layout->should_save_instance_attributes()) {
		this->level_edit_layout->save_instance_attributes();
	}
}

void LevelEditorManager::update_save_dungeon()
{
	if (this->dungeon_edit_layout->should_save_dungeon()) {
		this->save_dungeon();
	}
}

void LevelEditorManager::update_add_dungeon()
{
	if (this->dungeon_edit_layout->should_add_dungeon()) {
		const std::string dungeon_name = this->dungeon_edit_layout->get_input_dungeon_name();
		LevelEditorDataManager::get_instance().add_dungeon(dungeon_name);
		this->dungeon_edit_layout->reset_dungeon_list();
	}
}

void LevelEditorManager::update_delete_dungeon()
{
	if (this->dungeon_edit_layout->should_delete_dungeon()) {
		const std::string dungeon_name = this->dungeon_edit_layout->listbox_selected_dungeon_name();
		LevelEditorDataManager::get_instance().delete_dungeon(dungeon_name);
		this->dungeon_edit_layout->reset_fields();
	}
}

void LevelEditorManager::update_add_level()
{
	if (this->dungeon_edit_layout->should_add_level()) {
		const std::string level_name = this->dungeon_edit_layout->get_input_level_name();
		LevelEditorDataManager::get_instance().add_level(level_name);
		this->dungeon_edit_layout->reset_level_list();
	}
}

void LevelEditorManager::update_delete_level()
{
	if (this->dungeon_edit_layout->should_delete_level()) {
		const std::string level_name = this->dungeon_edit_layout->listbox_selected_level_name();
		LevelEditorDataManager::get_instance().delete_level(level_name);
		this->dungeon_edit_layout->reset_level_list();
	}
}

void LevelEditorManager::update_selected_tileset()
{
	const int tileset_index = this->tileset_edit_layout->get_selected_tileset_index();
	const bool needs_update 
		= LevelEditorDataManager::get_instance().update_selected_tileset(tileset_index);
	if (needs_update) {
		const std::string tileset_name = this->tileset_edit_layout->get_tileset_name(tileset_index);
		this->load_selected_tileset_tiles();
		this->load_selected_tileset_blocks();
		this->load_selected_tileset_entity_groups();
		this->load_selected_tileset_spawners();
		this->load_selected_tileset_path_nodes();

		this->tileset_edit_layout->update_selected_tileset_fields();

		this->level_edit_layout->load_selected_tileset_tiles();
		this->level_edit_layout->load_selected_tileset_blocks();
		this->level_edit_layout->load_selected_tileset_entity_groups();
		this->level_edit_layout->load_selected_tileset_spawners();
		this->level_edit_layout->load_selected_tileset_path_nodes();
		this->level_edit_layout->load_selected_tileset_tiled_images();

		this->tileset_edit_layout->set_selected_tileset_index(tileset_index);
		this->tileset_edit_layout->update_selected_tileset_fields();
		this->tileset_edit_layout->load_selected_tileset_tiles();
		this->tileset_edit_layout->load_selected_tileset_blocks();
		this->tileset_edit_layout->load_selected_tileset_entity_groups();
		LevelEditorDataManager::get_instance().reset_tileset_object_selection();
	}
	this->tileset_edit_layout->update_selected_tileset();
}

void LevelEditorManager::update_reset_level()
{
	if (this->level_edit_layout->should_reset_level()) {
		LevelEditorDataManager::get_instance().reset_level();
		this->level_edit_layout->reset_all_grid_image_layers();
	}
}

void LevelEditorManager::update_reset_tile_edges()
{
	if (this->level_edit_layout->should_reset_tile_edges()) {
		LevelEditorDataManager::get_instance().reset_tile_edges();
		this->level_edit_layout->reset_grid_image_layer(LevelEditorDataManager::TILE_LAYER);
	}
}

void LevelEditorManager::update_sheet_col()
{
	bool update = false;
	if (this->level_edit_layout->should_decrement_sheet_col()) {
		update = LevelEditorDataManager::get_instance().decrement_sheet_col();
	} else if (this->level_edit_layout->should_increment_sheet_col()) {
		update = LevelEditorDataManager::get_instance().increment_sheet_col();
	}
	if (update) {
		this->level_edit_layout->update_sheet_col();
	}
}

void LevelEditorManager::load_selected_tileset_tiles()
{
	this->level_edit_layout->load_selected_tileset_tiles();
	this->tileset_edit_layout->load_selected_tileset_tiles();
}

void LevelEditorManager::load_selected_tileset_blocks()
{
	this->level_edit_layout->load_selected_tileset_blocks();
	this->tileset_edit_layout->load_selected_tileset_blocks();
}

void LevelEditorManager::load_selected_tileset_entity_groups()
{
	this->level_edit_layout->load_selected_tileset_entity_groups();
	this->tileset_edit_layout->load_selected_tileset_entity_groups();
}

void LevelEditorManager::load_selected_tileset_tiled_images()
{
	this->level_edit_layout->load_selected_tileset_tiled_images();
	this->tileset_edit_layout->load_selected_tileset_tiled_images();
}

void LevelEditorManager::load_selected_tileset_spawners()
{
	this->level_edit_layout->load_selected_tileset_spawners();
	//this->tileset_edit_layout->load_selected_tileset_spawners(); //TODO
}

void LevelEditorManager::load_selected_tileset_path_nodes()
{
	this->level_edit_layout->load_selected_tileset_path_nodes();
	//this->tileset_edit_layout->load_selected_tileset_path_nodes(); //TODO
}



void LevelEditorManager::draw(ALLEGRO_DISPLAY * display)
{
	this->editor_gui->logic();
	this->editor_gui->render();
}
