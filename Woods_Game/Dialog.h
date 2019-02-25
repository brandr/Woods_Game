#ifndef DIALOG_H
#define DIALOG_H

#define DIALOG_LINE_SPACING 24
#define DIALOG_FONT_SIZE 24
#define DIALOG_OFFSET_X 16
#define DIALOG_OFFSET_Y 16
#define DIALOG_TEXT_SCROLL_FRAMES 4

#include <string>
#include <vector>
#include <allegro5/display.h>
#include <allegro5/allegro_font.h>
#include <algorithm>
#include "FileManager.h"

struct DialogLine {
	std::string text;
};

struct DialogPage {
	std::vector<DialogLine*> lines;
	DialogPage();
	const int total_num_characters();
	const std::vector<std::string> get_text_lines(const int num_characters);
};

class Dialog
{
private:
	std::vector<DialogPage*> pages;
	int page_num = 0;
	bool should_scroll_text = true;
	int character_counter = 0;
	const int current_num_characters();
	const bool is_showing_full_page();
	DialogPage * current_page();
public:
	Dialog();
	~Dialog();
	void update();
	void draw(ALLEGRO_DISPLAY * display, ALLEGRO_FONT * font, const int x_off, const int y_off);
	void advance_dialog();
	const bool has_current_page();
	void add_line(const std::string line, const int page_num, const int line_num);
	void parse_text(const std::string text);
};

#endif