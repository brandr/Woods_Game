#include "LevelGridListener.h"

LevelGridListener::LevelGridListener()
{
}

void LevelGridListener::mouseClickCB(agui::MouseEvent & evt)
{
	this->last_event_pos = std::pair<float, float>((float) evt.getX(), (float) evt.getY());
	this->last_event_button = evt.getButton();
	this->update = true;
}

bool LevelGridListener::has_update()
{
	return this->update;
}

void LevelGridListener::unset_update()
{
	this->update = false;
}

std::pair<float, float> LevelGridListener::get_last_event_pos()
{
	return this->last_event_pos;
}

bool LevelGridListener::has_left_click()
{
	return this->last_event_button == 1;
}

bool LevelGridListener::has_right_click()
{
	return this->last_event_button == 2;
}
