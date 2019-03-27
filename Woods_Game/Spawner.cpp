#include "Spawner.h"



Spawner::Spawner()
{
	this->setClassName("Spawner");
	this->Register("SpawnerID", &spawner_id);
	this->Register("EntityStartingPosX", &entity_starting_pos_x);
	this->Register("EntityStartingPosY", &entity_starting_pos_y);
	this->Register("EntityDataIndex", &entity_data_index);
	this->Register("EntitySheetCol", &entity_sheet_col);
	this->Register("EntitySheetRow", &entity_sheet_row);
	this->Register("EntityAttributes", &entity_attributes);
	this->solid = false;
	this->visible = false;
}


Spawner::~Spawner()
{
}
void Spawner::set_content(std::string image_filename, Rect * image_subsection, std::pair<int, int> position)
{
	this->entity_starting_pos_x = position.first;
	this->entity_starting_pos_y = position.second;
	GameImage::set_content(image_filename, image_subsection, position);
}

const std::string Spawner::get_spawner_id()
{
	return this->spawner_id.value();
}

Rect * Spawner::get_bitmap_subsection()
{
	return new Rect(this->entity_sheet_col.value()*TILE_SIZE, this->entity_sheet_row.value()*TILE_SIZE, TILE_SIZE, TILE_SIZE);
}
