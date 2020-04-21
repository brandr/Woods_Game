#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

#include "MenuManager.h"

MenuManager::MenuManager()
{
	setClassName("MenuManager");
	Register("MenuItems", &menu_items);
	Register("xPos", &x_pos);
	Register("yPos", &y_pos);
	Register("Axis", &axis);
	Register("SelectionStyle", &selection_style);
	Register("FontFilename", &font_filename);
	Register("FontSize", &font_size);
	Register("ColorStr", &color_str);
}


MenuManager::~MenuManager()
{
}

void MenuManager::set_menu_items()
{
}

void MenuManager::set_animations()
{
	float pos[2];
	pos[0] = x_pos.value(), pos[1] = y_pos.value();
	const int size = menu_items.size();
	for (int i = 0; i < size; i++) {
		//menu_items[i].;		//TODO: set animation
		if (axis.value() == 2) {
			*(pos + 1) += MENU_ITEM_SPACING;
		}
		else if (axis.value() == 1) *(pos) += al_get_text_width(font, menu_items.getItem(i)->get_text().c_str());
		std::pair<float, float> item_pos = std::pair<float, float>(pos[0], pos[1]);
		menu_items.getItem(i)->set_position(item_pos);
	}
}

void MenuManager::load_xml_content(std::string menu_key)
{
	this->menu_items.Clear();
	this->menu_key = menu_key;
	file_manager.load_xml_content(this, "resources/load/menus",
		"SerializableClass", "MenuKey", menu_key);
	position[0] = x_pos.value();
	position[1] = y_pos.value();
	std::string font_full_filename = "resources/fonts/" + this->font_filename.value() + ".ttf";
	font = al_load_font(font_full_filename.c_str(), this->font_size.value(), NULL);
	const int size = this->menu_items.size();
	for (int i = 0; i < size; i++) {
		menu_items.getItem(i)->load_additional_content();
	}
}

void MenuManager::unload_content()
{
	al_destroy_font(font);
	menu_items.Clear();
	attributes.clear();
	contents.clear();
}

void MenuManager::update()
{
}

void MenuManager::set_keyboard_mappable_input(int keycode)
{
	std::cout << "wrong keyboard mappable input method\n";
}

void MenuManager::set_controller_mappable_input(int keycode)
{
}



void MenuManager::draw(ALLEGRO_DISPLAY * display, float x_off, float y_off, float width, float height)
{
	const int size = menu_items.size();
	for (int i = 0; i < size; i++) {
		menu_items.getItem(i)->draw(display, x_off, y_off, width, height, font, color_str.value(), selection_style.value(), i == item_index, 
			(i == selected_index || (this->is_selecting_input() && i == item_index)));
	}
}

void MenuManager::menu_up()
{
	if (selected_index < 0 && !this->is_selecting_input()) {
		if (axis.value() == 2) item_index = item_index > 0 ? item_index - 1 : menu_items.size() - 1;
		return;
	}
}

void MenuManager::menu_down()
{
	if (selected_index < 0 && !this->is_selecting_input()) {
		if (axis.value() == 2) item_index = (item_index + 1) % menu_items.size();
		return;
	}
}

void MenuManager::menu_left()
{
	if (selected_index >= 0 && !this->is_selecting_input()) {
		menu_items.getItem(selected_index)->menu_left();
	}
}

void MenuManager::menu_right()
{
	if (selected_index >= 0 && !this->is_selecting_input()) {
		menu_items.getItem(selected_index)->menu_right();
	}
}

void MenuManager::reset()
{
	item_index = 0;
	selected_index = -1;
}

void MenuManager::process_mouse_click_left(float x_pos, float y_pos, float x_off, float y_off)
{

}

void MenuManager::mouse_cursor_update(float x_pos, float y_pos, float x_off, float y_off)
{
	if (is_selecting_options()) {
		//TODO
		return;
	}
	const int index = mouse_selected_index(x_pos, y_pos, x_off, y_off);
	if (index >= 0) item_index = index;
}

const int MenuManager::mouse_selected_index(float x_pos, float y_pos, float x_off, float y_off)
{
	const int size = menu_items.size();
	std::pair<float, float> mouse_pos(x_pos - x_off - position[0], y_pos - y_off - position[1]);
	for (int i = 0; i < size; i++) {
		MenuItem * item = menu_items.getItem(i);
		const std::string text = item->get_text();
		if (text == "") continue;
		std::pair<float, float> menu_pos = item->get_position();
		std::pair<float, float> adjusted_menu_pos(menu_pos.first - position[0], menu_pos.second - position[1]);
		std::pair<float, float> menu_item_dim(
			float(al_get_text_width(font, text.c_str())), 
			float(al_get_font_line_height(font)));
		if (mouse_pos.first > adjusted_menu_pos.first
			&& mouse_pos.second > adjusted_menu_pos.second
			&& mouse_pos.first < adjusted_menu_pos.first + menu_item_dim.first
			&& mouse_pos.second < adjusted_menu_pos.second + menu_item_dim.second) {
			return i;
		}
	}
	return -1;
}

const int MenuManager::mouse_selected_options_direction_index(float x_pos, float y_pos, float x_off, float y_off, float back_width, float back_height)
{
	if(!is_selecting_options()) return -1;
	std::pair<float, float> mouse_pos(x_pos, y_pos);
	std::pair<float, float> left_bracket_pos = menu_items.getItem(selected_index)->get_left_bracket_position(x_off, y_off, back_width, back_height);
	std::pair<float, float> right_bracket_pos = menu_items.getItem(selected_index)->get_right_bracket_position(x_off, y_off, back_width, back_height);
	std::pair<float, float> left_bracket_dim = menu_items.getItem(selected_index)->get_left_bracket_dimensions();
	std::pair<float, float> right_bracket_dim = menu_items.getItem(selected_index)->get_right_bracket_dimensions();
	if(mouse_pos.first > left_bracket_pos.first 
		&& mouse_pos.second > left_bracket_pos.second
		&& mouse_pos.first < left_bracket_pos.first + left_bracket_dim.first
		&& mouse_pos.second < left_bracket_pos.second + left_bracket_dim.second){
		return 0;
	}
	else if(mouse_pos.first > right_bracket_pos.first
		&& mouse_pos.second > right_bracket_pos.second
		&& mouse_pos.first < right_bracket_pos.first + right_bracket_dim.first
		&& mouse_pos.second < right_bracket_pos.second + right_bracket_dim.second){
		return 1;
	}
	return -1;
}

bool MenuManager::mouse_selecting_current_option(float x_pos, float y_pos, 
	float x_off, float y_off, 
	float back_width, float back_height)
{
	if (!is_selecting_options()) return false;
	std::pair<float, float> mouse_pos(x_pos, y_pos);
	std::pair<float, float> text_pos = menu_items.getItem(selected_index)->get_text_pos(x_off, y_off, back_width, back_height);
	std::pair<float, float> text_dim = menu_items.getItem(selected_index)->get_text_dimensions(font);
	return mouse_pos.first > text_pos.first
		&& mouse_pos.second > text_pos.second
		&& mouse_pos.first < text_pos.first + text_dim.first
		&& mouse_pos.second < text_pos.second + text_dim.second;
}

void MenuManager::save_controls()
{
}

void MenuManager::revert_controls_default()
{
}

void MenuManager::select_options()
{
	selected_index = item_index;
}

void MenuManager::cancel_option_select()
{
	menu_items.getItem(selected_index)->reset();
	selected_index = -1;
}

void MenuManager::confirm_option_select()
{
	menu_items.getItem(selected_index)->save();
	selected_index = -1;
	std::string xml_string = this->toXML();
	this->file_manager.replace_xml_content("resources/load/menus", "SerializableClass", "MenuKey", this->menu_key, xml_string);
}

void MenuManager::set_option_for_menu_item(const std::string menu_item_name, const std::string option_name, const bool should_save)
{
	const int size = this->menu_items.size();
	for (int i = 0; i < size; i++) {
		MenuItem * item = this->menu_items.getItem(i);
		if (item->get_text() == menu_item_name) {
			item->set_option_with_name(option_name);
			if (should_save) {
				item->save();
			}
			break;
		}
	}
}

bool MenuManager::is_selecting_options()
{
	return selected_index >= 0;
}

void MenuManager::set_item_index(const int index)
{
	item_index = index;
}

bool MenuManager::is_selecting_input()
{
	return selecting_input;
}

void MenuManager::set_selecting_input(bool selecting)
{
	this->selecting_input = selecting;
}

const std::string MenuManager::selected_filepath()
{
	return "";
}

xmls::Collection<MenuItem> &MenuManager::get_menu_items()
{
	return menu_items;
}

std::string MenuManager::get_selected_action_key()
{
	const int size = menu_items.size();
	if (size < item_index + 1) return "";
	return menu_items.getItem(item_index)->get_selection_action_key();
}

std::string MenuManager::get_confirm_action_key()
{
	return menu_items.getItem(item_index)->get_confirm_action_key();
}
