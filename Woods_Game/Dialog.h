#ifndef DIALOG_H
#define DIALOG_H

#define DIALOG_LINE_SPACING 24
#define DIALOG_FONT_SIZE 24
#define DIALOG_OFFSET_X 16
#define DIALOG_OFFSET_Y 16
#define DIALOG_TEXT_SCROLL_FRAMES 4

#define DIALOG_PAGE_TOKEN "p";
#define DIALOG_LINE_TOKEN "l";
#define DIALOG_OPTION_TOKEN "o";
#define DIALOG_OPTION_TEXT_TOKEN "ot";
#define DIALOG_OPTION_ACTION_TOKEN "oa";

#include <string>
#include <vector>
#include <allegro5/display.h>
#include <allegro5/allegro_font.h>
#include <algorithm>
#include "FileManager.h"
#include "InteractAction.h"

struct DialogLine {
	std::string text;
	std::string option_action_key;
	int option_page_num = -1;
	const bool has_option();
	const bool has_option_action();
	const bool has_option_page();
};

struct DialogPage {
	std::vector<DialogLine*> lines;
	int next_page_number = -1;
	DialogPage();
	const int total_num_characters();
	const std::vector<std::string> get_text_lines(const int num_characters);
	const std::string option_action_key(const int index);
	const int option_next_page_num(const int index);
	const bool has_option(const int index);
	const bool has_options();
	const int options_count();
};

struct DialogItem;
struct DialogItemOption;
class ImageLoader;
class Dialog
{
private:
	std::map<int, DialogPage *> page_map;
	int page_num = 0;
	int selected_option_index = 0;
	std::string active_action_key = "";
	bool should_scroll_text = true;
	int character_counter = 0;
	const int current_num_characters();
	const bool is_showing_full_page();
	DialogPage * current_page();
	ALLEGRO_BITMAP * option_arrow;
	std::vector<ActionBinding*> action_bindings;
public:
	Dialog();
	~Dialog();
	void update();
	void draw(ALLEGRO_DISPLAY * display, ALLEGRO_FONT * font, const int x_off, const int y_off);
	void advance_dialog();
	const bool has_current_page();
	void add_line(const std::string line, const int page_num, const int next_page_num, const int line_num, const std::string option_action_key, DialogItemOption * option);
	void parse_text(const std::string text);
	void parse_dialog(DialogItem * dialog_item);
	const std::string get_active_action_key();
	void decrement_option();
	void increment_option();
	void set_should_scroll_text(const bool value);
	void set_action_bindings(std::vector<ActionBinding *> bindings);
	std::vector<ActionBinding *> get_action_bindings();
};

#endif