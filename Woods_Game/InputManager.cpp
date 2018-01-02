#include "InputManager.h"

InputManager::InputManager()
{

}


InputManager::~InputManager()
{
}

bool InputManager::is_key_pressed(ALLEGRO_EVENT ev, int key)
{
	return ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == key;
}

bool InputManager::is_key_pressed(ALLEGRO_EVENT ev, std::vector<int> keys)
{
	return is_input_pressed(ev, ALLEGRO_EVENT_KEY_UP, keys);
}

bool InputManager::is_key_released(ALLEGRO_EVENT ev, int key)
{
	return ev.type == ALLEGRO_EVENT_KEY_UP && ev.keyboard.keycode == key;
}

bool InputManager::is_key_released(ALLEGRO_EVENT ev, std::vector<int> keys)
{
	if (ev.type == ALLEGRO_EVENT_KEY_UP) {
		int size = keys.size();
		for (int i = 0; i < size; i++) {
			if (ev.keyboard.keycode == keys[i]) return true;
		}
	}
	return false;
}

bool InputManager::is_input_pressed(ALLEGRO_EVENT ev, int evtype, std::vector<int> keys)
{
	if (ev.type == ALLEGRO_EVENT_KEY_UP) return false;
	if (ev.type == evtype) {
		int size = keys.size();
		for (int i = 0; i < size; i++) {
			if (ev.keyboard.keycode == keys[i]) return true;
		}
	}
	return false;
}

bool InputManager::is_input_pressed(ALLEGRO_EVENT ev, int evtype, int key)
{
	if (ev.type == ALLEGRO_EVENT_JOYSTICK_CONFIGURATION) {
		//TODO
		return false;
	}
	else if (ev.type == ALLEGRO_EVENT_JOYSTICK_AXIS) {
		return evtype == ALLEGRO_EVENT_JOYSTICK_AXIS;
	}
	else if (ev.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN) {
		return evtype == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN && ev.joystick.button == key;
	}
	else if (ev.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_UP) {
		//TODO
		return false;
	}
	else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
		return evtype == ALLEGRO_EVENT_MOUSE_AXES;
	}
	else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
		return evtype == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && ev.mouse.button == key;
	}
	else if (ev.type == ALLEGRO_EVENT_KEY_UP && ev.keyboard.keycode == key) return false;
	else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) return ev.type == evtype && ev.keyboard.keycode == key;
	return false;
}

bool InputManager::is_input_released(ALLEGRO_EVENT ev, int evtype, int key)
{
	//temp. use evtype if necessary for special cases
	return ev.type == ALLEGRO_EVENT_KEY_UP && is_key_released(ev, key);
}
