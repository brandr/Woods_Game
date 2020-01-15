#include "EntityGroup.h"

EntityGroup::EntityGroup()
{
	this->setClassName("EntityGroup");
	this->Register("Solid", &(this->solid));
	this->Register("EntitySheetCol", &entity_sheet_col);
	this->Register("EntitySheetRow", &entity_sheet_row);
	this->Register("RootPosX", &root_pos_x);
	this->Register("RootPosY", &root_pos_y);
	this->Register("CollideXOffset", &collide_x_offset);
	this->Register("CollideYOffset", &collide_y_offset);
	this->Register("CollideWidth", &collide_width);
	this->Register("CollideHeight", &collide_height);
	this->Register("EntityGroupName", &entity_group_name);
	this->Register("ContactActions", &contact_actions);
	this->Register("InteractActions", &interact_actions);
	this->Register("ImageFilters", &image_filters);
}

EntityGroup::EntityGroup(std::pair<int, int> root_offset)
{
	this->setClassName("EntityGroup");
	this->Register("Solid", &(this->solid));
	this->Register("EntitySheetCol", &entity_sheet_col);
	this->Register("EntitySheetRow", &entity_sheet_row);
	this->Register("RootPosX", &root_pos_x);
	this->Register("RootPosY", &root_pos_y);
	this->Register("CollideXOffset", &collide_x_offset);
	this->Register("CollideYOffset", &collide_y_offset);
	this->Register("CollideWidth", &collide_width);
	this->Register("CollideHeight", &collide_height);
	this->Register("EntityGroupName", &entity_group_name);
	this->Register("ContactActions", &contact_actions);
	this->Register("InteractActions", &interact_actions);
	this->Register("ImageFilters", &image_filters);
	this->root_pos_x = root_offset.first, this->root_pos_y = root_offset.second;
}


EntityGroup::~EntityGroup()
{
	this->entities.clear();
}

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

void EntityGroup::set_collide_offset(const std::pair<int, int> offset)
{
	this->collide_x_offset = offset.first, this->collide_y_offset = offset.second;
	this->collide_rect.x = this->rect.x + offset.first, this->collide_rect.y = this->rect.y + offset.second;
}

void EntityGroup::set_collide_dimensions(const std::pair<int, int> dimensions)
{
	this->collide_width = dimensions.first, this->collide_height = dimensions.second;
	this->collide_rect.width = dimensions.first, this->collide_rect.height = dimensions.second;
}

//TODO: either call this less often and/or make it more efficient
const bool EntityGroup::intersects_area(Rect area)
{
	if (this->collide_rect.width > 0 && this->collide_rect.height > 0) {
		return this->collide_rect.intersects_rect(area);
	} else {
		return this->rect.intersects_rect(area);
	}
}

std::string EntityGroup::get_entity_group_name()
{
	return this->entity_group_name.value();
}

std::pair<int, int> EntityGroup::get_root_pos()
{
	return std::pair<int, int>(root_pos_x.value(), root_pos_y.value());
}

void EntityGroup::draw(ALLEGRO_DISPLAY * display, const int x_offset, const int y_offset, const int screen_w, const int screen_h)
{
	const int size = (int) entities.size();
	for (int i = 0; i < size; i++) {
		entities[i]->draw(display, x_offset, y_offset, screen_w, screen_h);
	}
}

const bool EntityGroup::should_draw(const int x_offset, const int y_offset, const int screen_w, const int screen_h)
{
	const int size = (int)entities.size();
	for (int i = 0; i < size; i++) {
		if (entities[i]->should_draw(x_offset, y_offset, screen_w, screen_h)) {
			return true;
		}
	}
	return false;
}
