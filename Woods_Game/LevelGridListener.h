#ifndef LEVEL_GRID_LISTENER_H
#define LEVEL_GRID_LISTENER_H

#include <Agui/MouseListener.hpp>

class LevelGridListener : public agui::MouseListener {
private:
	std::pair<float, float> last_event_pos;
	int last_event_button = -1;
	bool update = false;

public:
	LevelGridListener();
	virtual void mouseClickCB(agui::MouseEvent &event);
	bool has_update();
	void unset_update();
	std::pair<float, float> get_last_event_pos();
	bool has_left_click();
	bool has_right_click();
};

#endif