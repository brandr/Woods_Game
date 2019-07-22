#include "CalendarScreen.h"

ALLEGRO_BITMAP * CalendarScreen::calendar_backdrop()
{
	return ImageLoader::get_instance().get_image("ui/calendar_backdrop");
}

ALLEGRO_BITMAP * CalendarScreen::calendar_backdrop_dialog()
{
	return ImageLoader::get_instance().get_image("ui/calendar_backdrop_dialog");
}

ALLEGRO_BITMAP * CalendarScreen::arrow_left()
{
	return ImageLoader::get_instance().get_image("ui/arrows/ui_arrow_calendar_left");
}

ALLEGRO_BITMAP * CalendarScreen::arrow_right()
{
	return ImageLoader::get_instance().get_image("ui/arrows/ui_arrow_calendar_right");
}

ALLEGRO_BITMAP * CalendarScreen::arrow_left_disabled()
{
	return ImageLoader::get_instance().get_image("ui/arrows/ui_arrow_calendar_left_disabled");
}

ALLEGRO_BITMAP * CalendarScreen::arrow_right_disabled()
{
	return ImageLoader::get_instance().get_image("ui/arrows/ui_arrow_calendar_right_disabled");
}

ALLEGRO_BITMAP * CalendarScreen::check_mark()
{
	return ImageLoader::get_instance().get_image("ui/calendar/check_mark");
}

const int CalendarScreen::selected_month_index()
{
	if (this->month_index < 0 && this->global_time != NULL) {
		this->month_index = this->global_time->get_current_month_index();
	}
	return this->month_index;
}

const std::string CalendarScreen::month_str()
{
	if (this->global_time != NULL) {
		return this->global_time->get_month_str(this->month_index);
	}
	return "";
}

const int CalendarScreen::num_days_in_month()
{
	return this->global_time->get_num_days_in_month(this->month_index);
}

const int CalendarScreen::first_day_index()
{
	return this->global_time->get_first_day_of_month_index(this->month_index);
}

const int CalendarScreen::last_day_index()
{
	return this->global_time->get_last_day_of_month_index(this->month_index);
}

const int CalendarScreen::day_at_pos(const int x, const int y)
{
	if (x < 0 || y < 0 || y > 5 || x > 6) {
		return -1;
	}
	const int first_index = this->first_day_index();
	const int num_days = this->num_days_in_month();
	if (y == 0) {
		if (x < first_index) {
			return -1;
		}
		return x - first_index + 1;
	} else {
		return (x - first_index) + (y * 7) + 1;
	}
}

const bool CalendarScreen::may_select_day_at_pos(const int x, const int y)
{
	const int current_mon = this->global_time->get_current_month_index();
	const int current_day = this->global_time->get_current_day_index();
	const int day_num = this->day_at_pos(x, y);
	if (day_num < 1 || day_num > this->num_days_in_month()) {
		return false;
	}
	const bool b1 = this->month_index < MIN_MONTH_INDEX;
	const bool b2 = this->month_index > MAX_MONTH_INDEX;
	if (b1 || b2 || this->month_index > current_mon ) {
		return false;
	}

	// day is before the game starts
	if (this->month_index == ORIGIN_MONTH && day_num < ORIGIN_DAY) {
		return false;
	}

	// day is in the today or in the future
	if (this->month_index == current_mon && day_num >= current_day) {
		return false;
	}

	return true;
}

CalendarScreen::CalendarScreen()
{
}

CalendarScreen::~CalendarScreen()
{
}

void CalendarScreen::load_content()
{
	// ui icons
	ImageLoader::get_instance().load_image("ui/calendar_backdrop");
	ImageLoader::get_instance().load_image("ui/calendar_backdrop_dialog");
	ImageLoader::get_instance().load_image("ui/arrows/ui_arrow_calendar_left");
	ImageLoader::get_instance().load_image("ui/arrows/ui_arrow_calendar_right");
	ImageLoader::get_instance().load_image("ui/arrows/ui_arrow_calendar_left_disabled");
	ImageLoader::get_instance().load_image("ui/arrows/ui_arrow_calendar_right_disabled");
	ImageLoader::get_instance().load_image("ui/calendar/check_mark");
}

void CalendarScreen::load_fonts()
{
	// font for day numbers and marked days
	font_map[FONT_CALENDAR_MONTH] = al_load_font("resources/fonts/ArialBlack.ttf", 36, NULL);
	font_map[FONT_CALENDAR_DAYS] = al_load_font("resources/fonts/ArialBlack.ttf", 12, NULL);
}

void CalendarScreen::draw(ALLEGRO_DISPLAY * display)
{
	if (this->global_time == NULL) {
		return;
	}
	const int x1 = (al_get_display_width(display) - al_get_bitmap_width(calendar_backdrop())) / 2;
	const int y1 = (al_get_display_height(display) - al_get_bitmap_height(calendar_backdrop())) / 2;
	al_draw_bitmap(calendar_backdrop(), x1, y1, NULL);

	const int x2 = (al_get_display_width(display)) / 2 - 40;
	const int y2 = (al_get_display_height(display) - al_get_bitmap_height(calendar_backdrop())) / 2 + 12;
	const std::vector<int> rgb = MenuItem::string_to_rgb(FONT_COLOR_CALENDAR_MONTH);
	al_draw_text(font_map[FONT_CALENDAR_MONTH], al_map_rgb(rgb[0], rgb[1], rgb[2]), x2, y2, 0, this->month_str().c_str());

	this->draw_day_labels(display);
	this->draw_month_arrows(display);
	this->draw_selection_ui(display);
	this->draw_symbols_ui(display);
	if (this->showing_dialog()) {
		this->draw_confirm_dialog_ui(display);
	}
}

void CalendarScreen::draw_day_labels(ALLEGRO_DISPLAY * display)
{
	int day_num = 1;
	int col = this->first_day_index();
	for (int row = 0; row < 6; row++ && day_num <= this->num_days_in_month()) {
		col = col % 7;
		while (col < 7 && day_num <= this->num_days_in_month()) {
			const int x = (al_get_display_width(display) - al_get_bitmap_width(calendar_backdrop())) / 2 + (col * 70) + 26;
			const int y = (al_get_display_height(display) - al_get_bitmap_height(calendar_backdrop())) / 2 + (row * 53) + 100;
			const std::vector<int> rgb = MenuItem::string_to_rgb(FONT_COLOR_CALENDAR_DAYS);
			al_draw_text(font_map[FONT_CALENDAR_DAYS], al_map_rgb(rgb[0], rgb[1], rgb[2]), x, y, 0, std::to_string(day_num).c_str());
			day_num++;
			col++;
		}
	}
}

void CalendarScreen::draw_month_arrows(ALLEGRO_DISPLAY * display)
{
	// left arrow
	ALLEGRO_BITMAP * left = this->may_decrement_month() ? this->arrow_left() : this->arrow_left_disabled();
	const int x1 = (al_get_display_width(display) - al_get_bitmap_width(calendar_backdrop())) / 2 + 16;
	const int y1 = (al_get_display_height(display) - al_get_bitmap_height(calendar_backdrop())) / 2 + 22;
	al_draw_bitmap(left, x1, y1, 0);

	// right arrow
	ALLEGRO_BITMAP * right = this->may_increment_month() ? this->arrow_right() : this->arrow_right_disabled();
	const int x2 = (al_get_display_width(display) - al_get_bitmap_width(calendar_backdrop())) / 2 + al_get_bitmap_width(calendar_backdrop()) - 48;
	const int y2 = (al_get_display_height(display) - al_get_bitmap_height(calendar_backdrop())) / 2 + 22;
	al_draw_bitmap(right, x2, y2, 0);
}

void CalendarScreen::draw_selection_ui(ALLEGRO_DISPLAY * display)
{
	const int select_x = this->select_pos.first, select_y = this->select_pos.second;
	if (select_y < 0 || select_x < 0) {
		const int x1 = (al_get_display_width(display) - al_get_bitmap_width(calendar_backdrop())) / 2 + 13;
		const int y1 = (al_get_display_height(display) - al_get_bitmap_height(calendar_backdrop())) / 2 + 13;
		const int x2 = x1 + 508;
		const int y2 = y1 + 49;

		al_draw_line(x1, y1, x1, y2, al_map_rgb(220, 0, 0), 2.0);
		al_draw_line(x2, y1, x2, y2, al_map_rgb(220, 0, 0), 2.0);
		al_draw_line(x1, y1, x2, y1, al_map_rgb(220, 0, 0), 2.0);
		al_draw_line(x1, y2, x2, y2, al_map_rgb(220, 0, 0), 2.0);
	} else {
		const int x1 = (al_get_display_width(display) - al_get_bitmap_width(calendar_backdrop())) / 2 + (select_x * 70) + 20;
		const int y1 = (al_get_display_height(display) - al_get_bitmap_height(calendar_backdrop())) / 2 + (select_y * 53) + 96;
		const int x2 = x1 + 70;
		const int y2 = y1 + 53;

		al_draw_line(x1, y1, x1, y2, al_map_rgb(220, 0, 0), 2.0);
		al_draw_line(x2, y1, x2, y2, al_map_rgb(220, 0, 0), 2.0);
		al_draw_line(x1, y1, x2, y1, al_map_rgb(220, 0, 0), 2.0);
		al_draw_line(x1, y2, x2, y2, al_map_rgb(220, 0, 0), 2.0);
	}
}

void CalendarScreen::draw_symbols_ui(ALLEGRO_DISPLAY * display)
{
	const bool b1 = this->selected_month_index() >= MIN_MONTH_INDEX;
	const bool b2 = this->selected_month_index() <= MAX_MONTH_INDEX;
	if (b1 && b2) {
		const int mon = this->global_time->get_current_month_index();
		const int day = this->global_time->get_current_day_index();
		if (this->selected_month_index() <= mon) {
			int day_num = 1;
			int col = this->first_day_index();
			for (int row = 0; row < 6; row++ && day_num <= this->num_days_in_month()) {
				col = col % 7;
				while (col < 7 && day_num <= this->num_days_in_month() && (day_num < day || this->selected_month_index() < mon)) {
					const int x = (al_get_display_width(display) - al_get_bitmap_width(calendar_backdrop())) / 2 + (col * 70) + 46;
					const int y = (al_get_display_height(display) - al_get_bitmap_height(calendar_backdrop())) / 2 + (row * 53) + 120;
					if (this->may_select_day_at_pos(col, row)) {
						al_draw_bitmap(this->check_mark(), x, y, 0);
					}
					day_num++;
					col++;
				}
			}
		}
	}
}

void CalendarScreen::draw_confirm_dialog_ui(ALLEGRO_DISPLAY * display)
{
	const int x1 = (al_get_display_width(display) - al_get_bitmap_width(calendar_backdrop_dialog())) / 2;
	const int y1 = (al_get_display_height(display) - al_get_bitmap_height(calendar_backdrop_dialog())) / 2;
	al_draw_bitmap(calendar_backdrop_dialog(), x1, y1, NULL);
	this->player->get_open_dialog()->draw(display, font_map[FONT_CALENDAR_DAYS], x1 + 4, y1 + 4);
}

void CalendarScreen::update()
{
	if (this->player != NULL && this->showing_dialog()) {
		this->player->dialog_update();
	}
}

void CalendarScreen::reset()
{
}

void CalendarScreen::menu_up()
{
	if (this->showing_dialog()) {
		this->player->get_open_dialog()->decrement_option();
		return;
	}
	int x = this->select_pos.first, y = this->select_pos.second;
	if (y < 0) {
		y = 5;
		x = this->last_day_index();
		if (this->day_at_pos(x, y) > this->num_days_in_month()) {
			y = 4;
		}
	} else {
		y -= 1;
	}
	if (y < 0) {
		x = -1;
	}
	this->select_pos = std::pair<int, int>(x, y);
}

void CalendarScreen::menu_down()
{
	if (this->showing_dialog()) {
		this->player->get_open_dialog()->increment_option();
		return;
	}
	int x = this->select_pos.first, y = this->select_pos.second;
	if (y >= 5) {
		y = -1;
		x = -1;
	} else if (y < 0) {
		y = 0;
		x = this->first_day_index();
	} else {
		y += 1;
	}
	if (this->day_at_pos(x, y) > this->num_days_in_month()) {
		x = -1;
		y = -1;
	}
	this->select_pos = std::pair<int, int>(x, y);
}

void CalendarScreen::menu_left()
{
	if (this->showing_dialog()) {
		this->player->get_open_dialog()->decrement_option();
		return;
	}
	bool go_up = false;
	int x = this->select_pos.first, y = this->select_pos.second;
	if (y == 0 && x == this->first_day_index()) {
		y = -1;
		x = -1;
	} else if (y < 0) {
		if (this->may_decrement_month()) {
			this->month_index--;
		}
		return;
	}
	else {
		x = x > 0 ? x - 1 : 6;
		if (x == 6) {
			go_up = true;
		}
	}
	this->select_pos = std::pair<int, int>(x, y);
	if (go_up) {
		this->menu_up();
	}
}

void CalendarScreen::menu_right()
{
	if (this->showing_dialog()) {
		this->player->get_open_dialog()->increment_option();
		return;
	}
	bool go_down = false;
	int x = this->select_pos.first, y = this->select_pos.second;
	if (y < 0) {
		if (this->may_increment_month()) {
			this->month_index++;
		}
		return;
	} else {
		x = (x + 1) % 7;
		if (x == 0) {
			go_down = true;
		}
		if (this->day_at_pos(x, y) > this->num_days_in_month()) {
			x = -1;
			y = -1;
		}
	}
	this->select_pos = std::pair<int, int>(x, y);
	if (go_down) {
		menu_down();
	}
}

void CalendarScreen::select()
{
	if (this->showing_dialog()) {
		this->player->get_open_dialog()->advance_dialog();
	} else {
		const int x = this->select_pos.first, y = this->select_pos.second;
		if (this->may_select_day_at_pos(x, y)) {
			Dialog * select_day_dialog = new Dialog();
			const std::string dialog_text
				= "[p][l]Go back to the selected day?[/l][l][o][ot]Yes[/ot][oa]load_selected_day[/oa][/o][/l][l][o][ot]No[/ot][oa]close_dialog[/oa][/o][/l][/p]";
			select_day_dialog->parse_text(dialog_text);
			select_day_dialog->set_should_scroll_text(false);
			std::vector<ActionBinding*> bindings;
			const int day_index = this->global_time->get_day_index(this->month_index, this->day_at_pos(x, y));
			bindings.push_back(new ActionBinding("load_day_index", std::to_string(day_index)));
			select_day_dialog->set_action_bindings(bindings);
			this->player->set_open_dialog(select_day_dialog);
		}
	}
}

const bool CalendarScreen::may_decrement_month()
{
	return this->selected_month_index() > MIN_MONTH_INDEX;
}

const bool CalendarScreen::may_increment_month()
{
	return this->selected_month_index() < MAX_MONTH_INDEX;
}

void CalendarScreen::set_global_time(GlobalTime * time)
{
	if (this->global_time != NULL && (this->global_time->get_day() != time->get_day())) {
		//delete this->global_time;
		this->global_time = NULL;
	}
	if (this->global_time == NULL) {
		this->global_time = time;
		this->month_index = time->get_current_month_index();
	}
}

const bool CalendarScreen::showing_dialog()
{
	return this->player != NULL && this->player->has_open_dialog();
}

void CalendarScreen::close_dialog()
{
	if (this->player != NULL && this->player->has_open_dialog()) {
		this->player->close_dialog();
	}
}

void CalendarScreen::set_player(Player * p)
{
	this->player = p;
}
