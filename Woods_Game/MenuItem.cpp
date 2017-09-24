#include "MenuItem.h"



MenuItem::MenuItem()
{
}


MenuItem::~MenuItem()
{
}


void MenuItem::unload_content()
{
	if (menu_image) {
		al_destroy_bitmap(menu_image);
		menu_image = NULL;
	}
	if (animation) {
		animation->unload_content();
		delete animation;
		animation = NULL;
	}
	if (left_bracket) {
		al_destroy_bitmap(left_bracket);
		left_bracket = NULL;
	}
	if (right_bracket) {
		al_destroy_bitmap(right_bracket);
		right_bracket = NULL;
	}
}

void MenuItem::draw(ALLEGRO_DISPLAY * display, float x_off, float y_off, float width, float height, 
	ALLEGRO_FONT *font, std::string color_str, std::string selection_style, bool selected, bool option_selected)
{
	//TODO: account for animation
	const std::vector<int> rgb = string_to_rgb(color_str);
	if (selected) draw_selected(display, x_off, y_off, width, height, font, color_str, selection_style, option_selected);
	else {
		if (menu_image) al_draw_bitmap(menu_image, x_off + position.first, y_off + position.second, NULL);
		else if (menu_text != "") {
			al_draw_text(font, al_map_rgb(rgb[0], rgb[1], rgb[2]), x_off + position.first, y_off + position.second, NULL, menu_text.c_str());
		}
		if (text_options.size() > 0) {
			std::string current_option = text_options[options_index];
			const int char_count = menu_text.length();
			const int char_off = 240;	//temp
			al_draw_text(font, al_map_rgb(rgb[0], rgb[1], rgb[2]), x_off + width - char_off, y_off + position.second, NULL, current_option.c_str());
		}
	}
}

void MenuItem::draw_selected(ALLEGRO_DISPLAY * display, float x_off, float y_off, float width, float height, 
	ALLEGRO_FONT * font, std::string color_str, std::string selection_style, bool option_selected)
{
	std::vector<int> rgb;
	if (selection_style == "brighten") rgb = brightened_color(string_to_rgb(color_str), 80);
	else if (selection_style == "darken") rgb = darkened_color(string_to_rgb(color_str), 80);
	if (menu_image) al_draw_bitmap(menu_image, x_off + position.first, y_off + position.second, 0);
	else if (menu_text != "") al_draw_text(font, al_map_rgb(rgb[0], rgb[1], rgb[2]), 
		x_off + position.first, y_off + position.second, 
		NULL, menu_text.c_str());
	if (!option_selected) rgb = string_to_rgb(color_str);
	
	if (text_options.size() > 0) {
		std::string current_option = text_options[options_index];
		std::pair<float, float> text_pos = get_text_pos(x_off, y_off, width, height);
		std::pair<float, float> left_bracket_pos = get_left_bracket_position(x_off, y_off, width, height);
		std::pair<float, float> right_bracket_pos = get_right_bracket_position(x_off, y_off, width, height);
		//const int char_count = menu_text.length();
		//const int char_off = 240;	//temp
		//al_draw_text(font, al_map_rgb(rgb[0], rgb[1], rgb[2]), x_off + width - DEFAULT_CHAR_OFF, 
		//	y_off + position.second, NULL, current_option.c_str());
		al_draw_text(font, al_map_rgb(rgb[0], rgb[1], rgb[2]), text_pos.first, text_pos.second,
			NULL, current_option.c_str());
		if (option_selected) {
			al_draw_bitmap(left_bracket, left_bracket_pos.first, left_bracket_pos.second, 0);
			al_draw_bitmap(right_bracket, right_bracket_pos.first, right_bracket_pos.second, 0);
			//al_draw_bitmap(left_bracket, x_off + width - char_off - 24, y_off + position.second, 0);
			//al_draw_bitmap(right_bracket, x_off + width - 64, y_off + position.second, 0);
		}
	}
}

void MenuItem::reset()
{
	options_index = stored_options_index;
}

void MenuItem::save()
{
	stored_options_index = options_index;
}

void MenuItem::menu_left()
{
	options_index = options_index > 0 ? options_index - 1 : text_options.size() - 1;
}

void MenuItem::menu_right()
{
	options_index = (options_index + 1) % text_options.size();
}

void MenuItem::set_text(std::string text)
{
	menu_text = text;
}

std::string MenuItem::get_text()
{
	return menu_text;
}

void MenuItem::set_image(ALLEGRO_BITMAP * image)
{
	menu_image = image;
}

void MenuItem::set_position(std::pair<float, float> position)
{
	this->position = position;
}

void MenuItem::set_selection_action_key(std::string key)
{
	selection_action_key = key;
}

void MenuItem::set_confirm_action_key(std::string key)
{
	confirm_action_key = key;
}

void MenuItem::set_text_options(std::vector<std::string> options)
{
	text_options = options;
}

void MenuItem::set_options_index(int index)
{
	options_index = index;
}

void MenuItem::set_selection_brackets(ALLEGRO_BITMAP * left_bracket, ALLEGRO_BITMAP * right_bracket)
{
	this->left_bracket = left_bracket, this->right_bracket = right_bracket;
}

std::pair<float, float> MenuItem::get_text_pos(float x_off, float y_off, float width, float height)
{
	return std::pair<float, float>(x_off + width - DEFAULT_CHAR_OFF, y_off + position.second);
}

std::pair<float, float> MenuItem::get_text_dimensions(ALLEGRO_FONT *font)
{
	if (options_index < 0) return std::pair<float, float>(0.0f, 0.0f);
	return std::pair<float, float>(
		float(al_get_text_width(font, text_options[options_index].c_str())), 
		float(al_get_font_line_height(font)));
}

std::pair<float, float> MenuItem::get_left_bracket_position(float x_off, float y_off, float width, float height)
{
	return std::pair<float, float>(x_off + width - DEFAULT_CHAR_OFF - 24.0f, y_off + position.second);
}

std::pair<float, float> MenuItem::get_right_bracket_position(float x_off, float y_off, float width, float height)
{
	return std::pair<float, float>(x_off + width - 64.0f, y_off + position.second);
}

std::pair<float, float> MenuItem::get_left_bracket_dimensions()
{
	return std::pair<float, float>(al_get_bitmap_width(left_bracket), al_get_bitmap_height(left_bracket));
}

std::pair<float, float> MenuItem::get_right_bracket_dimensions()
{
	return std::pair<float, float>(al_get_bitmap_width(right_bracket), al_get_bitmap_height(right_bracket));
}

std::pair<float, float> MenuItem::get_position()
{
	return position;
}

std::string MenuItem::get_selection_action_key()
{
	return selection_action_key;
}

std::string MenuItem::get_confirm_action_key()
{
	return confirm_action_key;
}

std::string MenuItem::get_selected_text_option()
{
	return text_options[options_index];
}

std::vector<std::string> MenuItem::get_text_options()
{
	return text_options;
}

std::vector<int> MenuItem::string_to_rgb(std::string color_str)
{
	std::vector<int> colors;
	std::string r_str = color_str.substr(0, 2);
	std::string g_str = color_str.substr(2, 2);
	std::string b_str = color_str.substr(4, 2);

	int r = (int)strtol(r_str.c_str(), NULL, 16);
	int g = (int)strtol(g_str.c_str(), NULL, 16);
	int b = (int)strtol(b_str.c_str(), NULL, 16);
	colors.push_back(r), colors.push_back(g), colors.push_back(b);
	return colors;
}

std::vector<int> MenuItem::brightened_color(std::vector<int> rgb, int brightness)
{
	std::vector<int> brightened;
	brightened.push_back(std::min(255, rgb[0] + brightness));
	brightened.push_back(std::min(255, rgb[1] + brightness));
	brightened.push_back(std::min(255, rgb[2] + brightness));
	return brightened;
}

std::vector<int> MenuItem::darkened_color(std::vector<int> rgb, int darkness)
{
	std::vector<int> darkened;
	darkened.push_back(std::max(0, rgb[0] - darkness));
	darkened.push_back(std::max(0, rgb[1] - darkness));
	darkened.push_back(std::max(0, rgb[2] - darkness));
	return darkened;
}
