#include "GameImageDisplay.h"

//TODO: fix margins, make it so grass tile displays properly
//TODO: some way to display different columns from a tile's sheet (maybe all at once)

GameImageDisplay::GameImageDisplay()
{
	this->setMargins(0, 0, 0, 0);
}

GameImageDisplay::GameImageDisplay(int topMargin, int leftMargin, int rightMargin, int bottomMargin) {
	this->setMargins(topMargin, leftMargin, rightMargin, bottomMargin);
}

void GameImageDisplay::set_bitmap(ALLEGRO_BITMAP * bmp)
{
	this->bitmap = bmp;
	this->stored_game_image_bitmap = NULL;
}

agui::Allegro5Image * GameImageDisplay::loaded_game_image_bitmap()
{
	if (stored_game_image_bitmap == NULL && this->bitmap != NULL) {
		stored_game_image_bitmap = new agui::Allegro5Image();
		int width = al_get_bitmap_width(this->bitmap);
		int height = al_get_bitmap_height(this->bitmap);
		ALLEGRO_BITMAP* image_bitmap = al_create_bitmap(width, height);
		stored_game_image_bitmap->setBitmap(this->bitmap);
	}
	return stored_game_image_bitmap;
}

void GameImageDisplay::paintComponent(const agui::PaintEvent & paintEvent)
{
}

void GameImageDisplay::paintBackground(const agui::PaintEvent & paintEvent)
{
	if (this->bitmap != NULL) {
		agui::Allegro5Image *image_bitmap = this->loaded_game_image_bitmap();
		if (image_bitmap != NULL) {
			paintEvent.graphics()->drawImage(image_bitmap,
				agui::Point(this->getLeftMargin(),
					this->getTopMargin()));
		}
	}
}

int GameImageDisplay::getTopMargin() const
{
	return this->topMargin;
}

int GameImageDisplay::getLeftMargin() const
{
	return this->leftMargin;
}

int GameImageDisplay::getBottomMargin() const
{
	return this->bottomMargin;
}

int GameImageDisplay::getRightMargin() const
{
	return this->rightMargin;
}

void GameImageDisplay::setTopMargin(int margin)
{
	this->topMargin = margin;
}

void GameImageDisplay::setLeftMargin(int margin)
{
	this->leftMargin = margin;
}

void GameImageDisplay::setBottomMargin(int margin)
{
	this->bottomMargin = margin;
}

void GameImageDisplay::setRightMargin(int margin)
{
	this->rightMargin = margin;
}

void GameImageDisplay::setMargins(int t, int l, int b, int r)
{
	this->topMargin = t;
	this->leftMargin = l;
	this->bottomMargin = b;
	this->rightMargin = r;
}
