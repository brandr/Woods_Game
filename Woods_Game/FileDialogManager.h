#ifndef FILE_DIALOG_MANAGER_H
#define FILE_DIALOG_MANAGER_H

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_color.h>


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

ALLEGRO_TEXTLOG *textlog;

class FileDialogManager
{
private:
	static ALLEGRO_FILECHOOSER * file_chooser;
public:
	FileDialogManager();
	static void *async_file_dialog_thread_func(ALLEGRO_THREAD *thread, void *arg);
	static void *message_box_thread(ALLEGRO_THREAD *thread, void *arg);
	static AsyncDialog *spawn_async_file_dialog(ALLEGRO_DISPLAY *display, const char *initial_path);
	static void stop_async_dialog(AsyncDialog *data);
	static void show_files_list(ALLEGRO_FILECHOOSER *dialog,
		const ALLEGRO_FONT *font, ALLEGRO_COLOR info);
	
};

#endif
