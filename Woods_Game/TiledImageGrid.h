#ifndef TILED_IMAGE_GRID_H
#define TILED_IMAGE_GRID_H

#include <Agui/Widget.hpp>
#include <Agui/Backends/Allegro5/Allegro5Image.hpp>
#include <Agui/Backends/Allegro5/Allegro5Graphics.hpp>

#include "LevelEditorDataManager.h"
#include "LevelGridListener.h"

class TiledImageGrid : public agui::Widget {
private:
	LevelGridListener grid_listener;
	void update_input();
	int topMargin;
	int leftMargin;
	int rightMargin;
	int bottomMargin;
	std::map<std::string, agui::Allegro5Image*> loaded_image_layers;
	agui::Allegro5Image *loaded_tiled_image_image(std::string prefix, std::string level_name);
	void set_loaded_tiled_image_image(agui::Allegro5Image *image, std::string prefix, std::string image_key);
	void load_image_layer(std::string layer);
	void update_image_layer(std::string layer);
	void clear_image_layers();
	void select_grid_pos(const std::pair<int, int> pos);
	bool has_tiled_image_key();
	std::string tiled_image_name();
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
	TiledImageGrid();
	TiledImageGrid(int, int, int, int);
	void update();
};

#endif // !TILED_IMAGE_GRID_H
