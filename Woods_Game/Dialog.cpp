#include "Dialog.h"
#include "DialogTree.h"
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
	return this->page_map.find(this->page_num) != this->page_map.end();
}

DialogPage * Dialog::current_page()
{
	return this->has_current_page() ? this->page_map[this->page_num] : NULL;
}

void Dialog::set_quest_updates_for_page(const int page_num, std::vector<QuestUpdate*> quest_updates)
{
	if (this->page_map.find(page_num) == this->page_map.end()) {
		this->page_map[page_num] = new DialogPage();
	}
	this->page_map[page_num]->set_quest_updates(quest_updates);
}

void Dialog::set_portrait_image_key_for_page(const int page_num, const std::string image_key)
{
	if (this->page_map.find(page_num) == this->page_map.end()) {
		this->page_map[page_num] = new DialogPage();
	}
	this->page_map[page_num]->set_portrait_image_key(image_key);
}

Dialog::Dialog()
{
	this->option_arrow = ImageLoader::get_instance().get_image("ui/arrows/ui_arrow_small");
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

void Dialog::add_line(const std::string line_text, const int page_num, const int next_page_num, const int line_num, const std::string option_action_key, 
	std::vector<InteractAction *> page_actions, DialogItemOption * option)
{
	if (this->page_map.find(page_num) == this->page_map.end()) {
		this->page_map[page_num] = new DialogPage();
		this->page_map[page_num]->next_page_number = next_page_num;
	}
	if (!page_actions.empty()) {
		this->page_map[page_num]->dialog_actions = page_actions;
	}
	DialogPage * page = this->page_map[page_num];
	const int size = page->lines.size();
	while (line_num >= size) {
		page->lines.push_back(new DialogLine());
	}
	page->lines[line_num]->text = line_text;
	if (option != NULL) {
		page->lines[line_num]->option_action_key = option->option_action_key.value();
		page->lines[line_num]->option_page_num = option->next_page_index.value();
		if (option->has_trigger_status()) {
			page->lines[line_num]->trigger_status = &(option->trigger_status);
		}
	} else if (option_action_key != "") {
		page->lines[line_num]->option_action_key = option_action_key;
	}
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
				this->add_line(line_text, page_index, -1, line_index, option_action_key, std::vector<InteractAction*>(), NULL);
				line_index++;
				empty_page = false;
			}
		}
		if (!empty_page) {
			page_index++;
		}
	}
}

void Dialog::parse_dialog(DialogItem * dialog_item)
{
	FileManager manager;
	std::string page_token = DIALOG_PAGE_TOKEN;
	std::string line_token = DIALOG_LINE_TOKEN;
	std::string option_token = DIALOG_OPTION_TOKEN;
	std::string option_text_token = DIALOG_OPTION_TEXT_TOKEN;
	std::string option_action_token = DIALOG_OPTION_ACTION_TOKEN;
	std::vector<DialogItemPage *> pages = dialog_item->get_pages();
	const int page_count = pages.size();
	for (int p = 0; p < page_count; p++) {
		int line_index = 0;
		DialogItemPage * page = pages[p];
		const int page_index = page->get_page_number();
		const int next_page_index = page->next_page_index.value();
		std::vector<InteractAction *> page_actions = page->get_dialog_actions();
		const std::vector<std::string> lines = manager.parse_tags(page->dialog_text.value(), line_token);
		const int line_count = lines.size();
		for (int l = 0; l < line_count; l++) {
			const std::string line = lines[l];
			if (line.size() > 0) {
				std::string line_text = line;
				this->add_line(line_text, page_index, next_page_index, line_index, "", page_actions, NULL);
				line_index++;
			}
		}
		std::vector<DialogItemOption *> options = page->get_options();
		const int option_size = options.size();
		for (int i = 0; i < option_size; i++) {
			DialogItemOption * dio = options[i];
			this->add_line(dio->option_text.value(), page_index, next_page_index, line_index, "", page_actions, dio);
			line_index++;
		}
		std::vector<QuestUpdate *> quest_updates = page->get_quest_updates();
		this->set_quest_updates_for_page(page_index, quest_updates);
		this->set_portrait_image_key_for_page(page_index, page->get_portrait_image_key());
	}
}

const std::string Dialog::get_active_action_key()
{
	return this->active_action_key;
}

void Dialog::set_active_action_key(const std::string action_key)
{
	this->active_action_key = action_key;
}

std::vector<InteractAction *> Dialog::get_active_actions() {

	return this->active_actions;
}

void Dialog::clear_active_actions()
{
	this->active_actions.clear();
}

void Dialog::add_active_action(InteractAction * action)
{
	this->active_actions.push_back(action);
}

TriggerStatus * Dialog::get_active_trigger_status() {

	return this->active_trigger_status;
}

void Dialog::set_active_trigger_status(TriggerStatus * value)
{
	this->active_trigger_status = value;
}

void Dialog::decrement_option()
{
	DialogPage * page = this->page_map[this->page_num];
	if (page != NULL && page->has_options()) {
		const int option_count = page->options_count();
		this->selected_option_index = (this->selected_option_index - 1) % option_count;
	}
}

void Dialog::increment_option()
{
	DialogPage * page = this->page_map[this->page_num];
	if (page != NULL && page->has_options()) {
		const int option_count = page->options_count();
		this->selected_option_index = (this->selected_option_index + 1) % option_count;
	}
}

void Dialog::set_should_scroll_text(const bool value)
{
	this->should_scroll_text = value;
}

void Dialog::set_action_bindings(std::vector<ActionBinding*> bindings)
{
	this->action_bindings = bindings;
}

std::vector<ActionBinding*> Dialog::get_action_bindings()
{
	return this->action_bindings;
}

std::vector<QuestUpdate*> Dialog::get_pending_quest_updates()
{
	return this->pending_quest_updates;
}

void Dialog::unset_pending_quest_updates()
{
	// don't use clear() because I think that deletes the objects the pointers point to
	this->pending_quest_updates = std::vector<QuestUpdate *>();
}

const bool Dialog::has_portrait_image()
{
	if (this->has_current_page()) {
		DialogPage * page = this->page_map[this->page_num];
		return page->has_portrait_image();
	}
	return false;
}

ALLEGRO_BITMAP * Dialog::get_portrait_image()
{
	if (this->has_current_page()) {
		DialogPage * page = this->page_map[this->page_num];
		return page->get_portrait_image();
	}
	return NULL;
}

void Dialog::draw(ALLEGRO_DISPLAY * display, ALLEGRO_FONT * font, const int x_off, const int y_off)
{
	if (this->has_current_page()) {
		DialogPage * page = this->page_map[this->page_num];
		const bool page_has_options = page->has_options();
		const int num_characters = this->should_scroll_text ? this->current_num_characters() : page->total_num_characters();
		const std::vector<std::string> text_lines = page->get_text_lines(num_characters);
		const int line_count = text_lines.size();
		int option_index = 0;
		int text_x_off = 0;
		for (int l = 0; l < line_count; l++) {
			const std::string text = text_lines[l];
			const bool line_has_option = page->has_option(l);
			const int opt_off = line_has_option ? 34 : 0;
			const int x = x_off + DIALOG_OFFSET_X, y = y_off + DIALOG_OFFSET_Y + (l * DIALOG_LINE_SPACING);
			if (line_has_option) {
				if (this->selected_option_index == option_index) {
					al_draw_bitmap(this->option_arrow, x + text_x_off, y, 0);
				}
				option_index++;
			}
			al_draw_text(font, al_map_rgb(0, 0, 0), x + opt_off + text_x_off, y, 0, text.c_str());
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
		int next_page_num = -1;
		bool should_advance = true;
		DialogPage * page = this->current_page();
		if (page != NULL) {
			if (page->has_options()) {
				next_page_num = page->option_next_page_num(this->selected_option_index);
				const std::string action_key = page->option_action_key(this->selected_option_index);
				if (action_key.length() > 0) {
					this->active_action_key = action_key;
					should_advance = false;
				}
				TriggerStatus * trigger_status = page->trigger_status(this->selected_option_index);
				if (trigger_status != NULL) {
					this->active_trigger_status = trigger_status;
				}
			}
			if (page->has_quest_updates()) {
				std::vector<QuestUpdate *> updates = page->quest_updates;
				this->pending_quest_updates = updates;
			}
			if (!page->dialog_actions.empty()) {
				this->active_actions = page->dialog_actions;
				//this->active_action_key = page->dialog_action->get_interact_action_key();
				//this->active_action = page->dialog_action;
				page->dialog_actions.clear();
			}
		}
		if (!should_advance) {
			return;
		}
		if (next_page_num < 0) {
			next_page_num = page->next_page_number;
		}
		// use -2 to say the dialog should end
		if (next_page_num == -2) {
			this->page_num = -1;
		} else if (next_page_num == -1) {
			this->page_num = this->page_num + 1;
		} else {
			this->page_num = next_page_num;
		}
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
			if (this->lines[i]->has_option_action() && option_index == index) {
				return this->lines[i]->option_action_key;
			}
			option_index++;
		}
	}
	return "";
}

const int DialogPage::option_next_page_num(const int index)
{
	const int line_count = this->lines.size();
	int option_index = 0;
	for (int i = 0; i < line_count; i++) {
		if (this->lines[i]->has_option()) {
			if (this->lines[i]->has_option_page() && option_index == index) {
				return this->lines[i]->option_page_num;
			}
			option_index++;
		}
	}
	return -1;
}

TriggerStatus * DialogPage::trigger_status(const int index)
{
	const int line_count = this->lines.size();
	int option_index = 0;
	for (int i = 0; i < line_count; i++) {
		if (this->lines[i]->has_option()) {
			if (this->lines[i]->has_option_trigger_status() && option_index == index) {
				return this->lines[i]->trigger_status;
			}
			option_index++;
		}
	}
	return NULL;
}

const bool DialogPage::has_option(const int index)
{
	const int size = this->lines.size();
	return index >= 0 && index < size && this->lines[index]->has_option();
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

const bool DialogPage::has_quest_updates()
{
	return this->quest_updates.size() > 0;
}

void DialogPage::set_quest_updates(std::vector<QuestUpdate*> updates)
{
	this->quest_updates.clear();
	this->quest_updates = updates;
}

const bool DialogPage::has_portrait_image()
{
	return !this->portrait_image_key.empty();
}

ALLEGRO_BITMAP * DialogPage::get_portrait_image()
{
	if (this->has_portrait_image()) {
		return ImageLoader::get_instance().get_image("portraits/" + this->portrait_image_key);
	}
	return NULL;
}

void DialogPage::set_portrait_image_key(const std::string image_key)
{
	this->portrait_image_key = image_key;
	if (!image_key.empty()) {
		ImageLoader::get_instance().load_image("portraits/" + this->portrait_image_key);
	}
}

const bool DialogLine::has_option()
{
	return this->has_option_action() || this->has_option_page() || this->has_option_trigger_status();
}

const bool DialogLine::has_option_action()
{
	return this->option_action_key.length() > 0;
}

const bool DialogLine::has_option_page()
{
	return this->option_page_num >= 0;
}

const bool DialogLine::has_option_trigger_status()
{
	return this->trigger_status != NULL && !this->trigger_status->is_empty();
}
