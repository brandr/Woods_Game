#ifndef TILESET_EDITOR_LAYOUT_H
#define TILESET_EDITOR_LAYOUT_H

#include <allegro5/display.h>

#include <Agui/FlowLayout.hpp>
#include <Agui/Widgets/Label/Label.hpp>
#include <Agui/Widgets/Frame/Frame.hpp>
#include <Agui/Widgets/TextField/TextField.hpp>
#include <Agui/Widgets/ListBox/ListBox.hpp>
#include <Agui/EmptyWidget.hpp>

#include "GameImageDisplay.h"
#include "EditorActionListener.h"
#include "LevelEditorDataManager.h"

class TilesetEditorLayout : public agui::FlowLayout {

private:
	//main components 
	// action listeners
	EditorActionListener load_tileset_select_listener;
	EditorActionListener save_tileset_select_listener;

	// tileset name
	agui::Label tileset_label;
	agui::TextField selected_tileset_name_field;

	// listbox
	agui::ListBox tileset_select_box;

	// tiles
	agui::Frame tileset_tile_frame;
	agui::FlowLayout tileset_tile_layout;
	agui::Label tileset_tile_label;
	agui::ListBox tileset_tile_select_box;
	agui::Label selected_tile_name_label;
	agui::TextField selected_tile_name_field;
	GameImageDisplay tile_image_display;

	// blocks
	agui::Frame tileset_block_frame;
	agui::FlowLayout tileset_block_layout;
	agui::Label tileset_block_label;
	agui::ListBox tileset_block_select_box;
	agui::Label selected_block_name_label;
	agui::TextField selected_block_name_field;
	GameImageDisplay block_image_display;

	// entity groups
	agui::Frame tileset_entity_group_frame;
	agui::FlowLayout tileset_entity_group_layout;
	agui::Label tileset_entity_group_label;
	agui::ListBox tileset_entity_group_select_box;
	agui::Label selected_entity_group_name_label;
	agui::TextField selected_entity_group_name_field;
	GameImageDisplay entity_group_image_display;

	// tiled images
	agui::Frame tileset_tiled_image_frame;
	agui::FlowLayout tileset_tiled_image_layout;
	agui::Label tileset_tiled_image_label;
	//agui::ListBox tileset_tiled_image_select_box;
	agui::Label selected_tiled_image_name_label;
	agui::TextField selected_tiled_image_name_field;
	GameImageDisplay tiled_image_image_display;
public:
	TilesetEditorLayout(ALLEGRO_DISPLAY *display);
	void load_tilesets();
	void load_selected_tileset_tiles();
	void load_selected_tileset_blocks();
	void load_selected_tileset_entity_groups();
	void load_selected_tileset_tiled_images();
	void update_selected_tileset();
	void update_selected_tileset_fields();
	void update_selected_tileset_tiles();
	void update_selected_tileset_blocks();
	void update_selected_tileset_entity_groups();
	void update_selected_tileset_tiled_images();
	void set_selected_tileset_index(int value);
	int get_selected_tileset_index();
	std::string get_tileset_name(int index);
};

#endif