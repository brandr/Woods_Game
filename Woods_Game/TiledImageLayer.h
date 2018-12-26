#ifndef TILED_IMAGE_LAYER_H
#define TILED_IMAGE_LAYER_H

#include "XMLSerialization.h"
#include "TiledImage.h"

class TiledImageLayer :
	public xmls::Serializable
{
private:
	xmls::Collection<TiledImage> tiled_images;
	xmls::xInt starting_pos_x;
	xmls::xInt starting_pos_y;
	xmls::xInt tiled_image_key;
	xmls::xInt tiled_image_sheet_col;
	xmls::xInt tiled_image_sheet_row;
public:
	TiledImageLayer();
	~TiledImageLayer();
};

#endif
//TODO: store tiledImages here

