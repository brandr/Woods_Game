#include "Dialog.h"

const int Dialog::current_num_characters()
{
	return this->character_counter/DIALOG_TEXT_SCROLL_FRAMES;
}

const bool Dialog::is_showing_full_page()
{
	DialogPage * page = this->current_page();
	if (page) {
		return this->current_num_characters() >= page->total_num_characters();
	}
	return false;
}

const bool Dialog::has_current_page()
{
	return this->pages.size() > this->page_num;
}

DialogPage * Dialog::current_page()
{
	return this->has_current_page() ? this->pages[this->page_num] : NULL;
}

Dialog::Dialog()
{
}


Dialog::~Dialog()
{
}

void Dialog::update()
{
	if (this->should_scroll_text) {
		DialogPage * page = this->current_page();
		if (page && this->current_num_characters() < page->total_num_characters()) {
			this->character_counter++;
		}
	}
}

void Dialog::add_line(const std::string line_text, const int page_num, const int line_num)
{
	while (page_num >= this->pages.size()) {
		this->pages.push_back(new DialogPage());
	}
	DialogPage * page = this->pages[page_num];
	while (line_num >= page->lines.size()) {
		page->lines.push_back(new DialogLine());
	}
	page->lines[line_num]->text = line_text;
}

void Dialog::parse_text(const std::string text)
{
	FileManager manager;
	const std::string page_delimiter = "[p]";
	const std::string line_delimiter = "[l]";
	const std::vector< std::string> pages = manager.string_to_parts(text, page_delimiter);
	const int page_count = pages.size();
	int page_index = 0;
	for (int p = 0; p < page_count; p++) {
		bool empty_page = true;
		int line_index = 0;
		const std::string page = pages[p];
		const std::vector<std::string> lines = manager.string_to_parts(page, line_delimiter);
		const int line_count = lines.size();
		for (int l = 0; l < line_count; l++) {
			const std::string line = lines[l];
			if (line.size() > 0) {
				this->add_line(line, page_index, line_index);
				line_index++;
				empty_page = false;
			}
		}
		if (!empty_page) {
			page_index++;
		}
	}
}

void Dialog::draw(ALLEGRO_DISPLAY * display, ALLEGRO_FONT * font, const int x_off, const int y_off)
{
	if (this->pages.size() > this->page_num) {
		DialogPage * page = this->pages[this->page_num];
		const int num_characters = this->should_scroll_text ? this->current_num_characters() : page->total_num_characters();
		const std::vector<std::string> text_lines = page->get_text_lines(num_characters);
		const int line_count = text_lines.size();
		for (int l = 0; l < line_count; l++) {
			const std::string text = text_lines[l];
			const int x = x_off + DIALOG_OFFSET_X, y = y_off + DIALOG_OFFSET_Y + (l * DIALOG_LINE_SPACING);
			al_draw_text(font, al_map_rgb(0, 0, 0), x, y, 0, text.c_str());
		}
	}
}

DialogPage::DialogPage()
{
}

void Dialog::advance_dialog()
{
	if (this->should_scroll_text && !this->is_showing_full_page()) {
		DialogPage * page = this->current_page();
		if (page) {
			this->character_counter = page->total_num_characters()*DIALOG_TEXT_SCROLL_FRAMES;
		}
	} else {
		this->page_num++;
	}
}

const int DialogPage::total_num_characters()
{
	int num_characters = 0;
	const int line_count = this->lines.size();
	for (int l = 0; l < line_count; l++) {
		DialogLine * line = this->lines[l];
		const std::string text = line->text;
		num_characters += text.size();
	}
	return num_characters;
}

const std::vector<std::string> DialogPage::get_text_lines(const int num_characters)
{
	std::vector<std::string> text_lines;
	int characters_remaining = num_characters;
	const int total_line_count = this->lines.size();
	for (int l = 0; l < total_line_count && characters_remaining > 0; l++) {
		const std::string full_text_line = this->lines[l]->text;
		const int full_line_length = full_text_line.size();
		const int line_length = std::min(full_line_length, characters_remaining);
		text_lines.push_back(full_text_line.substr(0, line_length));
		characters_remaining -= line_length;
	}
	return text_lines;
}
