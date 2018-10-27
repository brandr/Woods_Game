#ifndef GAME_IMAGE_DISPLAY_H
#define GAME_IMAGE_DISPLAY_H

#include "GameImage.h"
#include <Agui/Widget.hpp>
#include <Agui/Backends/Allegro5/Allegro5Image.hpp>

class GameImageDisplay : public agui::Widget {

	//TODO: decide how to bind to a gameimage
private:
	//GameImage * game_image;
	ALLEGRO_BITMAP * bitmap;
	int topMargin;
	int leftMargin;
	int rightMargin;
	int bottomMargin;
	agui::Allegro5Image *stored_game_image_bitmap;
	agui::Allegro5Image *loaded_game_image_bitmap();
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
	GameImageDisplay();
	GameImageDisplay(int, int, int, int);
	//void set_game_image(GameImage *game_image);
	void set_bitmap(ALLEGRO_BITMAP * bitmap);
};
#endif