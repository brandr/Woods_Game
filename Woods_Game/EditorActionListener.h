#ifndef EDITOR_ACTION_LISTENER_H
#define EDITOR_ACTION_LISTENER_H

#include <Agui/ActionListener.hpp>

enum ListenerTypes
{
	SELECT_DUNGEON
};

class EditorActionListener : public agui::ActionListener
{
private:
	int listener_type;
public:
	virtual void actionPerformed(const agui::ActionEvent &evt);
	void set_listener_type(int value);
	int get_listener_type();
};

#endif