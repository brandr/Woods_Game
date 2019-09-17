#include "FileDialogManager.h"

FileDialogManager::FileDialogManager()
{

}


void FileDialogManager::message(char const *format, ...)
{
	char str[1024];
	va_list args;
	va_start(args, format);
	vsnprintf(str, sizeof str, format, args);
	va_end(args);
	al_append_native_text_log(textlog, "%s", str);
}

void FileDialogManager::process_action(ALLEGRO_DISPLAY *display)
{
	file_chooser = al_create_native_file_dialog("resources/load/dungeon", "Select Dungeon", "*.*", ALLEGRO_FILECHOOSER_FILE_MUST_EXIST);
	if (al_get_native_file_dialog_count(file_chooser) < 1) {
		al_show_native_file_dialog(display, file_chooser);
	}
}

Dungeon * FileDialogManager::selected_dungeon()
{
	if (file_chooser) {
		std::string file_path = al_get_native_file_dialog_path(this->file_chooser, 0);
		size_t pos = 0;
		while ((pos = file_path.find("\\")) != std::string::npos) {
			file_path.erase(0, pos + 1);
		}
		std::string dungeon_name = file_path.substr(0, file_path.length() - 4);
		const int grid_width = STANDARD_LEVEL_GRID_WIDTH, grid_height = STANDARD_LEVEL_GRID_HEIGHT;
		return Dungeon::load_dungeon(dungeon_name, grid_width, grid_height);
	}
	return NULL;
}

void * FileDialogManager::async_file_dialog_thread_func(ALLEGRO_THREAD * thread, void * arg)
{
	AsyncDialog *data = static_cast<AsyncDialog*>(arg);
	ALLEGRO_EVENT event;
	(void)thread;

	al_show_native_file_dialog(data->display, data->file_dialog);

	event.user.type = ASYNC_DIALOG_EVENT1;
	al_emit_user_event(&data->event_source, &event, NULL);

	return NULL;
}


void * FileDialogManager::message_box_thread(ALLEGRO_THREAD * thread, void * arg)
{
	AsyncDialog *data = static_cast<AsyncDialog*>(arg);
	ALLEGRO_EVENT event;
	int button;

	(void)thread;

	button = al_show_native_message_box(data->display, "Warning",
		"Click Detected",
		"That does nothing. Stop clicking there.",
		"Oh no!|Don't press|Ok", ALLEGRO_MESSAGEBOX_WARN);
	if (button == 2) {
		button = al_show_native_message_box(data->display, "Error", "Hey!",
			"Stop it! I told you not to click there.",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
	}

	event.user.type = ASYNC_DIALOG_EVENT2;
	al_emit_user_event(&data->event_source, &event, NULL);

	return NULL;
}

AsyncDialog * FileDialogManager::spawn_async_file_dialog(ALLEGRO_DISPLAY * display, const char * initial_path)
{
	AsyncDialog *data = static_cast<AsyncDialog*>(calloc(1, sizeof *data));

	al_init_user_event_source(&data->event_source);
	data->display = display;
	data->thread = al_create_thread(message_box_thread, data);
	al_start_thread(data->thread);
	return data;
}


void FileDialogManager::stop_async_dialog(AsyncDialog * data)
{
	if (data) {
		al_destroy_thread(data->thread);
		al_destroy_user_event_source(&data->event_source);
		if (data->file_dialog)
			al_destroy_native_file_dialog(data->file_dialog);
		free(data);
	}
}

void FileDialogManager::show_files_list(ALLEGRO_FILECHOOSER * dialog, const ALLEGRO_FONT * font, ALLEGRO_COLOR info)
{
	ALLEGRO_BITMAP *target = al_get_target_bitmap();
	int count = al_get_native_file_dialog_count(dialog);
	int th = al_get_font_line_height(font);
	float x = al_get_bitmap_width(target) / 2;
	float y = al_get_bitmap_height(target) / 2 - (count * th) / 2;
	int i;

	for (i = 0; i < count; i++) {
		const char *name = al_get_native_file_dialog_path(dialog, i);
		al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);
		al_draw_textf(font, info, x, y + i * th, ALLEGRO_ALIGN_CENTRE, name, 0, 0);
	}
}
