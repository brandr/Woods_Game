#ifndef LEVEL_GRID_KEYBOARD_LISTENER_H
#define LEVEL_GRID_KEYBOARD_LISTENER_H

#include <map>
#include <Agui/KeyboardListener.hpp>

class LevelGridKeyboardListener : public agui::KeyboardListener {
private:
	std::map<agui::KeyEnum, bool> keys_pressed;
	std::map<agui::ExtendedKeyEnum, bool> extended_keys_pressed;
	//std::pair<float, float> last_event_pos;
	//int last_event_button = -1;
	//bool update = false;

public:
	LevelGridKeyboardListener();
	~LevelGridKeyboardListener();
	virtual void keyDownCB(agui::KeyEvent &event);
	virtual void keyUpCB(agui::KeyEvent &event);
	virtual void keyRepeatCB(agui::KeyEvent &event);
	bool has_control_held();
};

#endif