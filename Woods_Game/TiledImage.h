#ifndef TILED_IMAGE_H
#define TILED_IMAGE_H

#include "GameImage.h"
#include "XMLSerialization.h"

//TODO: building walls, roof, etc.
//TODO: layerable image that takes up only one tile, is drawn over level tiles, and exists on one of many layers


struct TiledImageData : public xmls::Serializable {
	xmls::xString image_data_key;
	xmls::xInt image_data_index;
	TiledImageData();
	std::string get_image_data_key();
};

class TiledImage :
	public GameImage, public xmls::Serializable
{
private:
	xmls::xInt starting_pos_x;
	xmls::xInt starting_pos_y;
	xmls::xInt tiled_image_key;
	xmls::xInt tiled_image_sheet_col;
	xmls::xInt tiled_image_sheet_row;
public: 
		TiledImage(); 
		~TiledImage();
};

#endif // !1