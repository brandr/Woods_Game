#include "TiledImage.h"

TiledImage::TiledImage()
{
	setClassName("TiledImage");
	this->Register("StartingPosX", &starting_pos_x);
	this->Register("StartingPosY", &starting_pos_y);
	this->Register("TiledImageKey", &tiled_image_key);
	this->Register("TiledImageSheetCol", &tiled_image_sheet_col);
	this->Register("TiledImageSheetRow", &tiled_image_sheet_row);
}

TiledImage::~TiledImage()
{
}

TiledImageData::TiledImageData()
{
	setClassName("TiledImageData");
	Register("image_data_key", &image_data_key);
	Register("image_data_index", &image_data_index);
}

std::string TiledImageData::get_image_data_key()
{
	return this->image_data_key.value();
}
