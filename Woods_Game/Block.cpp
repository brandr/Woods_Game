#include "Block.h"

Block::Block()
{
	this->setClassName("Block");
	this->Register("Solid", &(this->solid));
	this->Register("EntityStartingPosX", &entity_starting_pos_x);
	this->Register("EntityStartingPosY", &entity_starting_pos_y);
	this->Register("EntityDataIndex", &entity_data_index);
	this->Register("EntitySheetCol", &entity_sheet_col);
	this->Register("EntitySheetRow", &entity_sheet_row);
	this->Register("EntityAttributes", &entity_attributes);
	this->entity_data_index = -1;
	//TODO: what to serialize? (probably things from entity)
	//TODO: consider serializing entity instead if that works better, also see how inheritance works with serializing
}

Block::Block(int x, int y)
{
	this->setClassName("Block");
	this->Register("Solid", &(this->solid));
	this->Register("EntityStartingPosX", &entity_starting_pos_x);
	this->Register("EntityStartingPosY", &entity_starting_pos_y);
	this->Register("EntityDataIndex", &entity_data_index);
	this->Register("EntitySheetCol", &entity_sheet_col);
	this->Register("EntitySheetRow", &entity_sheet_row);
	this->Register("EntityAttributes", &entity_attributes);
	this->entity_data_index = -1;
	this->rect.x = x;
	this->rect.y = y;
	this->entity_starting_pos_x = x;
	this->entity_starting_pos_y = y;
}


Block::~Block()
{
}

bool Block::is_empty() 
{
	return this->get_entity_data_index() < 0;
}

void Block::set_empty()
{
	this->entity_data_index = -1;
	this->entity_sheet_col = 0;
	this->entity_sheet_row = 0;
	this->entity_attributes.Clear();
}

void Block::set_content(std::string image_filename, Rect * image_subsection, std::pair<int, int> position)
{
	this->entity_data_index = entity_data_index;
	GameImage::set_content(image_filename, image_subsection, position);
	refresh_mask();
}

int Block::get_type()
{
	return BLOCK;
}

Rect * Block::get_bitmap_subsection()
{
	return new Rect(this->entity_sheet_col.value()*TILE_SIZE, this->entity_sheet_row.value()*TILE_SIZE, TILE_SIZE, TILE_SIZE);
}
