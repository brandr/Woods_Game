#include "Dialog.h"
#include "ImageLoader.h"

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
	this->option_arrow = ImageLoader::get_instance().get_image("ui/arrows/ui_arrow");
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

void Dialog::add_line(const std::string line_text, const int page_num, const int line_num, const std::string option_action_key)
{
	while (page_num >= this->pages.size()) {
		this->pages.push_back(new DialogPage());
	}
	DialogPage * page = this->pages[page_num];
	while (line_num >= page->lines.size()) {
		page->lines.push_back(new DialogLine());
	}
	page->lines[line_num]->text = line_text;
	page->lines[line_num]->option_action_key = option_action_key;
}

void Dialog::parse_text(const std::string text)
{
	FileManager manager;
	std::string page_token = DIALOG_PAGE_TOKEN;
	std::string line_token = DIALOG_LINE_TOKEN;
	std::string option_token = DIALOG_OPTION_TOKEN;
	std::string option_text_token = DIALOG_OPTION_TEXT_TOKEN;
	std::string option_action_token = DIALOG_OPTION_ACTION_TOKEN;
	const std::vector< std::string> pages = manager.parse_tags(text, page_token);
	const int page_count = pages.size();
	int page_index = 0;
	for (int p = 0; p < page_count; p++) {
		bool empty_page = true;
		int line_index = 0;
		const std::string page = pages[p];
		const std::vector<std::string> lines = manager.parse_tags(page, line_token);
		const int line_count = lines.size();
		for (int l = 0; l < line_count; l++) {
			const std::string line = lines[l];
			if (line.size() > 0) {
				std::string line_text = line;
				std::string option_action_key = "";
				const std::vector<std::string> options = manager.parse_tags(line, option_token);
				if (options.size() > 0) {
					// only one option per line
					const std::string option = options[0];
					line_text = manager.parse_tags(option, option_text_token)[0];
					option_action_key = manager.parse_tags(option, option_action_token)[0];
					
				}
				this->add_line(line_text, page_index, line_index, option_action_key);
				line_index++;
				empty_page = false;
			}
		}
		if (!empty_page) {
			page_index++;
		}
	}
}

const std::string Dialog::get_active_action_key()
{
	return this->active_action_key;
}

void Dialog::decrement_option()
{
	DialogPage * page = this->pages[this->page_num];
	if (page != NULL && page->has_options()) {
		const int option_count = page->options_count();
		this->selected_option_index = (this->selected_option_index - 1) % option_count;
	}
}

void Dialog::increment_option()
{
	DialogPage * page = this->pages[this->page_num];
	if (page != NULL && page->has_options()) {
		const int option_count = page->options_count();
		this->selected_option_index = (this->selected_option_index + 1) % option_count;
	}
}

void Dialog::draw(ALLEGRO_DISPLAY * display, ALLEGRO_FONT * font, const int x_off, const int y_off)
{
	//TODO: draw options arrow
	if (this->pages.size() > this->page_num) {
		DialogPage * page = this->pages[this->page_num];
		const bool page_has_options = page->has_options();
		const int num_characters = this->should_scroll_text ? this->current_num_characters() : page->total_num_characters();
		const std::vector<std::string> text_lines = page->get_text_lines(num_characters);
		const int line_count = text_lines.size();
		int option_index = 0;
		for (int l = 0; l < line_count; l++) {
			const std::string text = text_lines[l];
			const bool line_has_option = page->has_option(l);
			const int opt_off = line_has_option ? 34 : 0;
			const int x = x_off + DIALOG_OFFSET_X, y = y_off + DIALOG_OFFSET_Y + (l * DIALOG_LINE_SPACING);
			if (line_has_option) {
				if (this->selected_option_index == option_index) {
					al_draw_bitmap(this->option_arrow, x, y, 0);
				}
				option_index++;
			}
			al_draw_text(font, al_map_rgb(0, 0, 0), x + opt_off, y, 0, text.c_str());
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
		DialogPage * page = this->current_page();
		if (page != NULL && page->has_options()) {
			const std::string action_key = page->option_action_key(this->selected_option_index);
			if (action_key.length() > 0) {
				this->active_action_key = action_key;
				return;
			}
		}
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

const std::string DialogPage::option_action_key(const int index)
{
	const int line_count = this->lines.size();
	int option_index = 0;
	for (int i = 0; i < line_count; i++) {
		if (this->lines[i]->has_option()) {
			if (option_index == index) {
				return this->lines[i]->option_action_key;
			}
			option_index++;
		}
	}
	return "";
}

const bool DialogPage::has_option(const int index)
{
	return index >= 0 && index < this->lines.size() && this->lines[index]->has_option();
}

const bool DialogPage::has_options()
{
	const int size = this->lines.size();
	for (int i = 0; i < size; i++) {
		if (this->lines[i]->has_option()) {
			return true;
		}
	}
	return false;
}

const int DialogPage::options_count()
{
	int count = 0;
	const int size = this->lines.size();
	for (int i = 0; i < size; i++) {
		if (this->lines[i]->has_option()) {
			count++;
		}
	}
	return count;
}

const bool DialogLine::has_option()
{
	return this->option_action_key.length() > 0;
}
