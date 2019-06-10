#include "TiledImage.h"

TiledImage::TiledImage()
{
	setClassName("TiledImage");
	this->Register("StartingPosX", &starting_pos_x);
	this->Register("StartingPosY", &starting_pos_y);
	this->Register("TiledImageKey", &tiled_image_key);
	this->Register("TiledImageName", &tiled_image_name);
	this->Register("TiledImageSheetCol", &tiled_image_sheet_col);
	this->Register("TiledImageSheetRow", &tiled_image_sheet_row);
	this->Register("EmptyFlag", &empty_flag);
	this->empty_flag = 1;
}

TiledImage::~TiledImage()
{
}

void TiledImage::set_empty()
{
	this->starting_pos_x = 0;
	this->starting_pos_y = 0;
	this->tiled_image_key = -1;
	this->tiled_image_name = "";
	this->tiled_image_sheet_col = -1;
	this->tiled_image_sheet_row = -1;
	this->empty_flag = 1;
}

void TiledImage::set_not_empty()
{
	this->empty_flag = 0;
}

bool TiledImage::is_empty()
{
	return this->empty_flag.value();
}

Rect * TiledImage::get_bitmap_subsection()
{
	return new Rect(this->tiled_image_sheet_col.value() * TILE_SIZE,
		this->tiled_image_sheet_row.value() * TILE_SIZE,
		TILE_SIZE,
		TILE_SIZE);
}

void TiledImage::set_starting_pos(const int x, const int y)
{
	this->starting_pos_x = x;
	this->starting_pos_y = y;
}

const std::pair<int, int> TiledImage::get_starting_pos()
{
	return std::pair<int, int>(this->starting_pos_x.value(), this->starting_pos_y.value());
}

void TiledImage::set_sheet_pos(const int x, const int y)
{
	this->tiled_image_sheet_col = x;
	this->tiled_image_sheet_row = y;
}

void TiledImage::set_tiled_image_key(const int key)
{
	this->tiled_image_key = key;
}

void TiledImage::set_tiled_image_name(const std::string name)
{
	this->tiled_image_name = name.c_str();
}

const std::string TiledImage::get_tiled_image_name()
{
	return this->tiled_image_name.value();
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

int TiledImageData::get_image_data_index()
{
	return this->image_data_index.value();
}
