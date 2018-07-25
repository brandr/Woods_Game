#include "EditorActionListener.h"

void EditorActionListener::actionPerformed(const agui::ActionEvent & evt)
{
	
}

void EditorActionListener::set_listener_type(int value)
{
	this->listener_type = value;
}

int EditorActionListener::get_listener_type()
{
	return this->listener_type;
}
