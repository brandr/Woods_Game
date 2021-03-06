#ifndef LEVEL_EDITOR_GRID_H
#define LEVEL_EDITOR_GRID_H

#include <Agui/Widget.hpp>
#include <Agui/Backends/Allegro5/Allegro5Image.hpp>
#include <Agui/Backends/Allegro5/Allegro5Graphics.hpp>

#include "LevelEditorDataManager.h"
#include "LevelGridListener.h"
#include "LevelGridKeyboardListener.h"

class LevelEditorGrid : public agui::Widget {
private:
	int select_mode = 0;
	LevelGridListener grid_listener;
	void update_input();
	// component properties
	std::map<std::string, std::vector<std::pair<agui::Allegro5Image *, std::string>>> loaded_image_layers;
	std::map<std::string, bool> layer_visibility_map;
	int topMargin;
	int leftMargin;
	int rightMargin;
	int bottomMargin;
	void add_object(const std::pair<float, float> pos);
	void delete_object(const std::pair<float, float> pos);
	void select_object(const std::pair<float, float> pos);
	void clear_image_layers();
	void load_image_layer(const std::string layer, const std::string level_name);
	void update_image_layer(const std::string layer);
	std::vector<std::pair<agui::Allegro5Image *, std::string>> loaded_level_images(const std::string prefix, const std::string level_name);
	void set_loaded_level_image(agui::Allegro5Image *image, const std::string prefix, const std::string level_name, const std::string rect_key);
	const std::vector<std::string> image_layer_rect_keys();
	bool is_layer_visible(const std::string layer);
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
	void set_layer_visible(const std::string layer, const bool visible);
	void set_select_mode(const int value);
	void reset_all_grid_image_layers();
	void reset_image_layer(const std::string layer, const std::string level_name);
	void reset_image_layer(const int index, const std::string level_name);
};

#endif