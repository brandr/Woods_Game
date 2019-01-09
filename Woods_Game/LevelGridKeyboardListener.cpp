#include "LevelGridKeyboardListener.h"

LevelGridKeyboardListener::LevelGridKeyboardListener()
{
}

LevelGridKeyboardListener::~LevelGridKeyboardListener()
{
}

void LevelGridKeyboardListener::keyDownCB(agui::KeyEvent & evt)
{
	agui::KeyEnum evtkey = evt.getKey();
	agui::ExtendedKeyEnum extendedKey = evt.getExtendedKey();
	this->keys_pressed[evtkey] = true;
	this->extended_keys_pressed[extendedKey] = true;
}

void LevelGridKeyboardListener::keyUpCB(agui::KeyEvent & evt)
{
	agui::KeyEnum evtkey = evt.getKey();
	agui::ExtendedKeyEnum extendedKey = evt.getExtendedKey();
	this->keys_pressed[evtkey] = false;
	this->extended_keys_pressed[extendedKey] = false;
}

void LevelGridKeyboardListener::keyRepeatCB(agui::KeyEvent & event)
{
}

bool LevelGridKeyboardListener::has_control_held()
{
	return this->extended_keys_pressed[agui::EXT_KEY_LEFT_CONTROL];
}
