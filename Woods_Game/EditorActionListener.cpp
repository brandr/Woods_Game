#include "EditorActionListener.h"

EditorActionListener::EditorActionListener()
{
	this->dialog_manager = new FileDialogManager();
}

void EditorActionListener::actionPerformed(const agui::ActionEvent & evt)
{
	switch (get_listener_type()) {
	case LOAD_DUNGEON:
		dialog_manager->process_action(this->display);
		break;
	case SAVE_DUNGEON:
		this->flags[SHOULD_SAVE] = 1;
		//TODO: save dungeon with changes to level, file, etc
		break;
	default:
		break;
	}
}

void EditorActionListener::set_listener_type(int value)
{
	this->listener_type = value;
}

int EditorActionListener::get_listener_type()
{
	return this->listener_type;
}

void EditorActionListener::set_display(ALLEGRO_DISPLAY * value)
{
	this->display = value;
}

void EditorActionListener::set_dialog_manager(FileDialogManager * value)
{
	this->dialog_manager = value;
}

Dungeon * EditorActionListener::selected_dungeon()
{
	if (LOAD_DUNGEON == this->get_listener_type()) {
		return this->dialog_manager->selected_dungeon();
	}
	return NULL;
}

void EditorActionListener::unset_flag(int flag)
{
	this->flags[flag] = 0;
}

bool EditorActionListener::has_flag(int flag)
{
	auto it = flags.find(flag);
	if (it == flags.end()) {
		return false;
	}
	return this->flags[flag];
}

