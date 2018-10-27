#ifndef FILE_DIALOG_MANAGER_H
#define FILE_DIALOG_MANAGER_H

#include "Dungeon.h"
#include "TileSet.h"
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_color.h>
#include <Agui/ActionEvent.hpp>


/* To communicate from a separate thread, we need a user event. */
#define ASYNC_DIALOG_EVENT1   ALLEGRO_GET_EVENT_TYPE('e', 'N', 'F', '1')
#define ASYNC_DIALOG_EVENT2   ALLEGRO_GET_EVENT_TYPE('e', 'N', 'F', '2')

typedef struct
{
	ALLEGRO_DISPLAY *display;
	ALLEGRO_FILECHOOSER *file_dialog;
	ALLEGRO_EVENT_SOURCE event_source;
	ALLEGRO_THREAD *thread;
} AsyncDialog;

class FileDialogManager
{
private:
	ALLEGRO_TEXTLOG *textlog;
	ALLEGRO_FILECHOOSER * file_chooser;
	bool message_log;
	AsyncDialog *old_dialog;
	AsyncDialog *cur_dialog;
	//static AsyncDialog *message_box;
public:
	FileDialogManager();
	void message(char const * format, ...);
	void process_action(ALLEGRO_DISPLAY *display);
	Dungeon * selected_dungeon();
	static void *async_file_dialog_thread_func(ALLEGRO_THREAD *thread, void *arg);
	static void *message_box_thread(ALLEGRO_THREAD *thread, void *arg);
	static AsyncDialog *spawn_async_file_dialog(ALLEGRO_DISPLAY *display, const char *initial_path);
	static void stop_async_dialog(AsyncDialog *data);
	static void show_files_list(ALLEGRO_FILECHOOSER *dialog,
		const ALLEGRO_FONT *font, ALLEGRO_COLOR info);
	
};

#endif
