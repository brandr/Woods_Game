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
	int get_image_data_index();
};

class TiledImage :
	public GameImage, public xmls::Serializable
{
private:
	xmls::xInt starting_pos_x;
	xmls::xInt starting_pos_y;
	xmls::xInt tiled_image_key;
	xmls::xString tiled_image_name;
	xmls::xInt tiled_image_sheet_col;
	xmls::xInt tiled_image_sheet_row;
	xmls::xInt empty_flag;
public: 
		TiledImage(); 
		~TiledImage();
		void set_empty();
		void set_not_empty();
		bool is_empty();
		Rect * get_bitmap_subsection();
		void set_starting_pos(const int x, const int y);
		const std::pair<int, int> get_starting_pos();
		void set_sheet_pos(const int x, const int y);
		void set_tiled_image_key(const int key);
		void set_tiled_image_name(const std::string name);
		const std::string get_tiled_image_name();
};

#endif // !1