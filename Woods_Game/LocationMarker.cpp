#include "LocationMarker.h"



LocationMarker::LocationMarker()
{
	setClassName("LocationMarker");
	Register("LocationName", &location_name);
	Register("ContainingLevelKey", &containing_level_key);
	Register("LocationLevels", &location_levels);
	this->Register("EntityStartingPosX", &entity_starting_pos_x);
	this->Register("EntityStartingPosY", &entity_starting_pos_y);
	this->Register("EntityDataIndex", &entity_data_index);
	this->Register("EntitySheetCol", &entity_sheet_col);
	this->Register("EntitySheetRow", &entity_sheet_row);
	this->Register("EntityAttributes", &entity_attributes);
}


LocationMarker::~LocationMarker()
{
}

void LocationMarker::set_content(std::string image_filename, Rect * image_subsection, std::pair<int, int> position)
{
	this->entity_starting_pos_x = position.first;
	this->entity_starting_pos_y = position.second;
	GameImage::set_content(image_filename, image_subsection, position);
}

Rect * LocationMarker::get_bitmap_subsection()
{
	return new Rect(this->entity_sheet_col.value()*TILE_SIZE, this->entity_sheet_row.value()*TILE_SIZE, TILE_SIZE, TILE_SIZE);
}

const std::string LocationMarker::get_location_name()
{
	return this->location_name.value();
}

const std::string LocationMarker::get_containing_level_key()
{
	return this->containing_level_key.value();
}

const bool LocationMarker::matches_level(const std::string level_key)
{
	const int size = this->location_levels.size();
	for (int i = 0; i < size; i++) {
		LocationMarkerLevel * lml = this->location_levels.getItem(i);
		if (lml->level_key.value() == level_key) {
			return true;
		}
	}
	return false;
}

LocationMarkerLevel::LocationMarkerLevel()
{
	setClassName("LocationMarkerLevel");
	Register("LevelKey", &level_key);
}

LocationMarkerLevel::~LocationMarkerLevel()
{
}
