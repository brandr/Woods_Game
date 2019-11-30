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
#include "Quest.h"
#include "TriggerStatus.h"

struct DialogLine {
	std::string text;
	std::string option_action_key;
	int option_page_num = -1;
	TriggerStatus * trigger_status;
	const bool has_option();
	const bool has_option_action();
	const bool has_option_page();
	const bool has_option_trigger_status();
};

struct DialogPage {
	std::vector<DialogLine*> lines;
	std::vector<QuestUpdate*> quest_updates;
	int next_page_number = -1;
	std::vector<InteractAction *> dialog_actions;
	std::string portrait_image_key;
	DialogPage();
	const int total_num_characters();
	const std::vector<std::string> get_text_lines(const int num_characters);
	const std::string option_action_key(const int index);
	const int option_next_page_num(const int index);
	TriggerStatus * trigger_status(const int index);
	const bool has_option(const int index);
	const bool has_options();
	const int options_count();
	const bool has_quest_updates();
	void set_quest_updates(std::vector<QuestUpdate *> updates);
	const bool has_portrait_image();
	ALLEGRO_BITMAP * get_portrait_image();
	void set_portrait_image_key(const std::string image_key);
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
	std::vector<InteractAction *> active_actions;
	TriggerStatus * active_trigger_status = NULL;
	std::vector<QuestUpdate *> pending_quest_updates;
	bool should_scroll_text = true;
	int character_counter = 0;
	const int current_num_characters();
	const bool is_showing_full_page();
	DialogPage * current_page();
	void set_quest_updates_for_page(const int page_num, std::vector<QuestUpdate*> quest_updates);
	void set_portrait_image_key_for_page(const int page_num, const std::string image_key);
	ALLEGRO_BITMAP * option_arrow;
	std::vector<ActionBinding*> action_bindings;
public:
	Dialog();
	~Dialog();
	void update();
	void draw(ALLEGRO_DISPLAY * display, ALLEGRO_FONT * font, const int x_off, const int y_off);
	void advance_dialog();
	const bool has_current_page();
	void add_line(const std::string line, const int page_num, const int next_page_num, const int line_num, 
		const std::string option_action_key, std::vector<InteractAction *> page_actions, DialogItemOption * option);
	void parse_text(const std::string text);
	void parse_dialog(DialogItem * dialog_item);
	const std::string get_active_action_key();
	void set_active_action_key(const std::string action_key);
	std::vector<InteractAction *> get_active_actions();
	void clear_active_actions();
	void add_active_action(InteractAction * action);
	TriggerStatus * get_active_trigger_status();
	void set_active_trigger_status(TriggerStatus * value);
	void decrement_option();
	void increment_option();
	void set_should_scroll_text(const bool value);
	void set_action_bindings(std::vector<ActionBinding *> bindings);
	std::vector<ActionBinding *> get_action_bindings();
	std::vector<QuestUpdate *> get_pending_quest_updates();
	void unset_pending_quest_updates();
	const bool has_portrait_image();
	ALLEGRO_BITMAP * get_portrait_image();
};

#endif