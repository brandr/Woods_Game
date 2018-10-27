#ifndef LEVEL_EDITOR_GRID_H
#define LEVEL_EDITOR_GRID_H

#include <Agui/Widget.hpp>
#include <Agui/Backends/Allegro5/Allegro5Image.hpp>
#include <Agui/Backends/Allegro5/Allegro5Graphics.hpp>

#include "LevelEditorDataManager.h"
#include "LevelGridListener.h"

class LevelEditorGrid : public agui::Widget {
private:
	LevelGridListener grid_listener;
	void update_input();
	// component properties
	std::map<std::string, agui::Allegro5Image*> loaded_image_layers;
	std::map<std::string, bool> layer_visibility_map;
	int topMargin;
	int leftMargin;
	int rightMargin;
	int bottomMargin;
	void add_object(std::pair<float, float> pos);
	void delete_object(std::pair<float, float> pos);
	void clear_image_layers();
	void load_image_layer(std::string layer);
	void reset_image_layer(int index);
	void update_image_layer(std::string layer);
	agui::Allegro5Image *loaded_level_image(std::string prefix, std::string level_name);
	void set_loaded_level_image(agui::Allegro5Image *image, std::string prefix, std::string level_name);
	bool is_layer_visible(std::string layer);
	std::string level_name();
	bool has_level();
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
};

#endif