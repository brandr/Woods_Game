#include "InputManager.h"
#include <iostream>

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
	if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
		int size = keys.size();
		for (int i = 0; i < size; i++) {
			if (ev.keyboard.keycode == keys[i]) return true;
		}
	}
	return false;
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
