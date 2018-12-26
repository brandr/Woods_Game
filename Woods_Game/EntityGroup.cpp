#include "EntityGroup.h"



EntityGroup::EntityGroup()
{
	this->setClassName("EntityGroup");
	this->Register("Solid", &(this->solid));
	this->Register("EntitySheetCol", &entity_sheet_col);
	this->Register("EntitySheetRow", &entity_sheet_row);	
	this->Register("RootPosX", &root_pos_x);
	this->Register("RootPosY", &root_pos_y);
	this->Register("EntityGroupName", &entity_group_name);
}

EntityGroup::EntityGroup(std::pair<int, int> root_offset)
{
	this->setClassName("EntityGroup");
	this->Register("Solid", &(this->solid));
	this->Register("EntitySheetCol", &entity_sheet_col);
	this->Register("EntitySheetRow", &entity_sheet_row);
	this->Register("RootPosX", &root_pos_x);
	this->Register("RootPosY", &root_pos_y);
	this->Register("EntityGroupName", &entity_group_name);
	this->root_pos_x = root_offset.first, this->root_pos_y = root_offset.second;
}


EntityGroup::~EntityGroup()
{
	this->entities.clear();
}
/*
void EntityGroup::set_content(std::string image_filename, Rect * image_subsection, std::pair<int, int> position)
{
	//TODO
}
*/
int EntityGroup::get_type()
{
	return ENTITY_GROUP;
}

void EntityGroup::set_entity_group_name(std::string name)
{
	this->entity_group_name = name;
}

void EntityGroup::set_sheet_pos(int col, int row)
{
	this->entity_sheet_col = col;
	this->entity_sheet_row = row;
}

std::vector<Entity*> EntityGroup::get_entities()
{
	return this->entities;
}

void EntityGroup::set_entities(std::vector<Entity*> entities)
{
	this->entities = entities;
}

void EntityGroup::set_root_pos(std::pair<int, int> root_pos)
{
	this->root_pos_x = root_pos.first, this->root_pos_y = root_pos.second;
}

std::string EntityGroup::get_entity_group_name()
{
	return this->entity_group_name.value();
}

std::pair<int, int> EntityGroup::get_root_pos()
{
	return std::pair<int, int>(root_pos_x.value(), root_pos_y.value());
}

void EntityGroup::draw(ALLEGRO_DISPLAY * display, int x_offset, int y_offset)
{
	int size = entities.size();
	for (int i = 0; i < size; i++) {
		entities[i]->draw(display, x_offset, y_offset);
	}
	//temp
	//Mask_Draw(mask, rect.x + x_offset, rect.y + y_offset);
		//al_draw_bitmap(temp_mask_image, rect.x + x_offset, rect.y + y_offset, 0);
	//temp
}