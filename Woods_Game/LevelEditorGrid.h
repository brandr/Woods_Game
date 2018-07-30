#ifndef LEVEL_EDITOR_GRID_H
#define LEVEL_EDITOR_GRID_H

#include <Agui/Widget.hpp>
#include <Agui/Backends/Allegro5/Allegro5Image.hpp>
#include "Level.h"

const std::string TILE_LAYER = "tile_layer";
const std::string BLOCK_LAYER = "block_layer";
const std::string ENTITY_GROUP_LAYER = "entity_group_layer";
const std::vector<std::string> LEVEL_LAYERS{ TILE_LAYER, BLOCK_LAYER, ENTITY_GROUP_LAYER};

class LevelEditorGrid : public agui::Widget {
private:
	// level properties
	Level * level;
	// component properties
	// TODO: consider making a class that contains this layer
	std::map<std::string, agui::Allegro5Image*> loaded_image_layers;
	std::map<std::string, bool> layer_visibility_map;
	int topMargin;
	int leftMargin;
	int rightMargin;
	int bottomMargin;
	void clear_image_layers();
	void load_image_layer(std::string layer);
	void update_image_layer(std::string layer);
	agui::Allegro5Image *loaded_level_image(std::string prefix, std::string level_name);
	void set_loaded_level_image(agui::Allegro5Image *image, std::string prefix, std::string level_name);
	bool is_layer_visible(std::string layer);
protected:
	// component methods
	virtual void paintComponent(const agui::PaintEvent &paintEvent);
	virtual void paintBackground(const agui::PaintEvent &paintEvent);
	int getTopMargin() const;
	int getLeftMargin() const;
	int getBottomMargin() const;
	int getRightMargin() const;
	void setTopMargin(int margin);
	void setLeftMargin(int margin);
	void setBottomMargin(int margin);
	void setRightMargin(int margin);
	void setMargins(int t, int l, int b, int r);
public:
	LevelEditorGrid();
	LevelEditorGrid(int, int, int, int);
	void update();
	void set_layer_visible(std::string layer, bool visible);
	void set_level(Level *level);
};

#endif