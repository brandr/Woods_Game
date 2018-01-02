#include "MenuItem.h"



MenuItem::MenuItem()
{
	setClassName("MenuItem");
	Register("MenuText", &menu_text);
	Register("xPos", &x_pos);
	Register("yPos", &y_pos);
	Register("SelectionActionKey", &selection_action_key);
	Register("ConfirmActionKey", &confirm_action_key);
	Register("TextOptions", &text_options);
	Register("LeftBracketFilename", &left_bracket_filename);
	Register("RightBracketFilename", &right_bracket_filename);
	Register("StoredOptionsIndex", &stored_options_index);
	Register("ControlsActionKey", &controls_action_key);
	Register("ControlsInputKey", &controls_input_key);
	Register("ControlsInputLabel", &controls_input_label);
	Register("ImageFilename", &image_filename);
}


MenuItem::~MenuItem()
{
	this->text_options.Clear();
}

void MenuItem::load_additional_content()
{
	const std::string prefix = "resources/images/";
	if (this->image_filename.c_str() != NULL && this->image_filename.value() != "") {
		std::string filename = prefix + this->image_filename.value();
		this->set_image(al_load_bitmap(filename.c_str()));
	}
	if (this->left_bracket_filename.c_str() != NULL && this->left_bracket_filename.value() != ""
		&& this->right_bracket_filename.c_str() != NULL && this->right_bracket_filename.value() != "") {
		const std::string left_bracket_filepath = prefix + left_bracket_filename.value();
		const std::string right_bracket_filepath = prefix + right_bracket_filename.value();
		ALLEGRO_BITMAP* bracket_L = al_load_bitmap(left_bracket_filepath.c_str());
		ALLEGRO_BITMAP* bracket_R = al_load_bitmap(right_bracket_filepath.c_str());
		al_convert_mask_to_alpha(bracket_L, al_map_rgb(255, 0, 255));
		al_convert_mask_to_alpha(bracket_R, al_map_rgb(255, 0, 255));
		this->set_selection_brackets(bracket_L, bracket_R);
	}
	this->options_index = stored_options_index.value();
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
		if (menu_image) al_draw_bitmap(menu_image, x_off + x_pos.value(), y_off + y_pos.value(), NULL);
		else if (menu_text.value() != "") {
			al_draw_text(font, al_map_rgb(rgb[0], rgb[1], rgb[2]), x_off + x_pos.value(), y_off + y_pos.value(), NULL, menu_text.c_str());
		}
		if (text_options.size() > 0) {
			std::string current_option = text_options.getItem(options_index)->Option.value();
			const int char_count = menu_text.value().length();
			const int char_off = 240;	//temp
			al_draw_text(font, al_map_rgb(rgb[0], rgb[1], rgb[2]), x_off + width - char_off, y_off + y_pos.value(), NULL, current_option.c_str());
		}
		if (controls_input_label.value() != "") {
			const int char_count = menu_text.value().length();
			const int char_off = 240;	//temp
			al_draw_text(font, al_map_rgb(rgb[0], rgb[1], rgb[2]), x_off + width - char_off, y_off + y_pos.value(), NULL, controls_input_label.c_str());
		}
	}
}

void MenuItem::draw_selected(ALLEGRO_DISPLAY * display, float x_off, float y_off, float width, float height, 
	ALLEGRO_FONT * font, std::string color_str, std::string selection_style, bool option_selected)
{
	std::vector<int> rgb;
	if (selection_style == "brighten") rgb = brightened_color(string_to_rgb(color_str), 80);
	else if (selection_style == "darken") rgb = darkened_color(string_to_rgb(color_str), 80);
	if (menu_image) al_draw_bitmap(menu_image, x_off + x_pos.value(), y_off + y_pos.value(), 0);
	else if (menu_text.value() != "") al_draw_text(font, al_map_rgb(rgb[0], rgb[1], rgb[2]), 
		x_off + x_pos.value(), y_off + y_pos.value(), 
		NULL, menu_text.c_str());
	if (!option_selected) rgb = string_to_rgb(color_str);
	
	if (text_options.size() > 0) {
		std::string current_option = text_options.getItem(options_index)->Option.value();
		std::pair<float, float> text_pos = get_text_pos(x_off, y_off, width, height);
		std::pair<float, float> left_bracket_pos = get_left_bracket_position(x_off, y_off, width, height);
		std::pair<float, float> right_bracket_pos = get_right_bracket_position(x_off, y_off, width, height);
		al_draw_text(font, al_map_rgb(rgb[0], rgb[1], rgb[2]), text_pos.first, text_pos.second,
			NULL, current_option.c_str());
		if (option_selected) {
			al_draw_bitmap(left_bracket, left_bracket_pos.first, left_bracket_pos.second, 0);
			al_draw_bitmap(right_bracket, right_bracket_pos.first, right_bracket_pos.second, 0);
		}
	}
	if (controls_input_label.value() != "") {
		const int char_count = menu_text.value().length();
		const int char_off = 240;	//temp
		al_draw_text(font, al_map_rgb(rgb[0], rgb[1], rgb[2]), x_off + width - char_off, y_off + y_pos.value(), NULL, controls_input_label.c_str());
	}
}

void MenuItem::reset()
{
	options_index = stored_options_index.value();
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
	return menu_text.value();
}

void MenuItem::set_image(ALLEGRO_BITMAP * image)
{
	menu_image = image;
}

void MenuItem::set_position(std::pair<float, float> position)
{
	this->position = position;
	this->x_pos = position.first;
	this->y_pos = position.second;
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
	text_options.Clear();
	Register("TextOptions", &text_options);
	for (int i = 0; i < options.size(); i++) {
		text_options.addItem(new TextOption(options[i]));
	}
	//text_options = options;
}

void MenuItem::set_options_index(int index)
{
	options_index = index;
}

void MenuItem::set_controls_action_key(std::string action_key)
{
	this->controls_action_key = action_key;
}

void MenuItem::set_controls_input_key(std::string input_key)
{
	this->controls_input_key = input_key;
}

void MenuItem::set_controls_input_label(std::string input_label)
{
	this->controls_input_label = input_label;
}

void MenuItem::set_selection_brackets(ALLEGRO_BITMAP * left_bracket, ALLEGRO_BITMAP * right_bracket)
{
	this->left_bracket = left_bracket, this->right_bracket = right_bracket;
}

void MenuItem::set_selection_bracket_filenames(std::string left_filename, std::string right_filename)
{
	this->left_bracket_filename = left_filename;
	this->right_bracket_filename = right_filename;
}

void MenuItem::set_image_filename(std::string filename)
{
	this->image_filename = filename;
}

std::pair<float, float> MenuItem::get_text_pos(float x_off, float y_off, float width, float height)
{
	return std::pair<float, float>(x_off + width - DEFAULT_CHAR_OFF, y_off + y_pos.value());
}

std::pair<float, float> MenuItem::get_text_dimensions(ALLEGRO_FONT *font)
{
	if (options_index < 0) return std::pair<float, float>(0.0f, 0.0f);
	std::string text = text_options.getItem(options_index)->Option.value();
	return std::pair<float, float>(
		float(al_get_text_width(font, text.c_str())), 
		float(al_get_font_line_height(font)));
}

std::pair<float, float> MenuItem::get_left_bracket_position(float x_off, float y_off, float width, float height)
{
	return std::pair<float, float>(x_off + width - DEFAULT_CHAR_OFF - 24.0f, y_off + y_pos.value());
}

std::pair<float, float> MenuItem::get_right_bracket_position(float x_off, float y_off, float width, float height)
{
	return std::pair<float, float>(x_off + width - 64.0f, y_off + y_pos.value());
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
	return selection_action_key.value();
}

std::string MenuItem::get_confirm_action_key()
{
	return confirm_action_key.value();
}

std::string MenuItem::get_selected_text_option()
{
	return text_options.getItem(options_index)->Option.value();
}

xmls::Collection<TextOption> MenuItem::get_text_options()
{
	return text_options;
}

std::string MenuItem::get_controls_action_key()
{
	return this->controls_action_key.value();
}

std::string MenuItem::get_controls_input_key()
{
	return this->controls_input_key.value();
}

std::string MenuItem::get_controls_input_label()
{
	return this->controls_input_label.value();
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

TextOption::TextOption()
{
	setClassName("TextOption");
	Register("Option", &Option);
}

TextOption::TextOption(std::string option)
{
	setClassName("TextOption");
	Register("Option", &Option);
	Option = option;
}

TextOption::~TextOption()
{
}