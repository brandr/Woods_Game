#include "MenuManager.h"

MenuManager::MenuManager()
{
	setClassName("MenuManager");
	Register("MenuItems", &menu_items);
	Register("xPos", &x_pos);
	Register("yPos", &y_pos);
	Register("Axis", &axis);
	//Register("Align", &align);
	Register("SelectionStyle", &selection_style);
	Register("FontFilename", &font_filename);
	Register("FontSize", &font_size);
	Register("ColorStr", &color_str);
	//TODO: what else to register?
}


MenuManager::~MenuManager()
{
}

void MenuManager::set_menu_items()
{
	/*
	for (int i = 0; i < menu_items.size(); i++) {
		if (menu_images.size() == i)
			menu_images.push_back(NULL);
	}
	for (int i = 0; i < menu_images.size(); i++) {
		if (menu_items.size() == i)
			menu_items.push_back("");
	}
	*/
}

void MenuManager::set_animations()
{
	float pos[2];
	pos[0] = x_pos.value(), pos[1] = y_pos.value();
	for (int i = 0; i < menu_items.size(); i++) {
		//menu_items[i].;		//TODO: set animation
		if (axis.value() == 2) {
			*(pos + 1) += MENU_ITEM_SPACING;
		}
		else if (axis.value() == 1) *(pos) += al_get_text_width(font, menu_items.getItem(i)->get_text().c_str());
		std::pair<float, float> item_pos = std::pair<float, float>(pos[0], pos[1]);
		menu_items.getItem(i)->set_position(item_pos);
	}
}

//temp. get rid of this method once load_xml_content works for all menus
void MenuManager::load_content(std::string id)
{
	file_manager.load_content("resources/load/menus.cme", attributes, contents, id);
	for (int i = 0; i < attributes.size(); i++) {
		for (int j = 0; j < attributes[i].size(); j++) {
			if (attributes[i][j] == "text_item") {
				std::pair<std::string, std::string> text_item_data = FileManager::string_to_pair(contents[i][j], ",");
				MenuItem *item = new MenuItem();
				item -> set_text(text_item_data.first);
				item -> set_selection_action_key(text_item_data.second);
				menu_items.addItem(item);
			}
			else if (attributes[i][j] == "image_item") {
				MenuItem *item = new MenuItem();
				std::pair<std::string, std::string> image_item_data = FileManager::string_to_pair(contents[i][j], ",");
				std::string filename = "resources/images/ui/menu_image_items/" + image_item_data.first + ".png";
				item->set_image_filename("ui/menu_image_items/" + image_item_data.first + ".png");
				item->set_image(al_load_bitmap(filename.c_str()));
				item->set_selection_action_key(image_item_data.second);
				menu_items.addItem(item);
			}
			else if (attributes[i][j] == "text_item_options") {
				std::pair<std::string, std::string> text_options_data = FileManager::string_to_pair(contents[i][j], ":");
				std::pair<std::string, std::string> text_item_data = FileManager::string_to_pair(text_options_data.first, ",");
				MenuItem *item = new MenuItem();
				item->set_text(text_item_data.first);
				item->set_selection_action_key(SELECTION_KEY_SELECT_OPTIONS);
				item->set_confirm_action_key(text_item_data.second);
				std::vector<std::string> options_list = FileManager::string_to_parts(text_options_data.second, ",");
				item->set_text_options(options_list);
				std::string filename1 = "resources/images/ui/brackets/bracket_1L.png";
				std::string filename2 = "resources/images/ui/brackets/bracket_1R.png";
				ALLEGRO_BITMAP* bracket_L = al_load_bitmap(filename1.c_str());
				ALLEGRO_BITMAP* bracket_R = al_load_bitmap(filename2.c_str());
				al_convert_mask_to_alpha(bracket_L, al_map_rgb(255, 0, 255));
				al_convert_mask_to_alpha(bracket_R, al_map_rgb(255, 0, 255));
				item->set_selection_brackets(bracket_L, bracket_R);
				item->set_selection_bracket_filenames("ui/brackets/bracket_1L.png", "ui/brackets/bracket_1R.png");
				menu_items.addItem(item);
			}
			else if (attributes[i][j] == "controls_menu_item") {
				MenuItem *item = new MenuItem();
				std::pair<std::string, std::string> text_item_data = FileManager::string_to_pair(contents[i][j], ",");
				item->set_text(text_item_data.first);
				item->set_selection_action_key(SELECTION_KEY_SET_CONTROL_INPUT);
				item->set_controls_action_key(text_item_data.second);
				menu_items.addItem(item);
			}
			else if (attributes[i][j] == "font") {
				std::pair<std::string, std::string> font_pair = FileManager::string_to_pair(contents[i][j], ",");
				std::string filename = "resources/fonts/" + font_pair.first + ".ttf";
				this->font_filename = font_pair.first;
				this->font_size = ::atoi(font_pair.second.c_str());
				font = al_load_font(filename.c_str(), ::atoi(font_pair.second.c_str()), NULL);
			}
			else if (attributes[i][j] == "color") {
				color_str = contents[i][j];
			}
			else if (attributes[i][j] == "align") {
				align = contents[i][j];
			}
			else if (attributes[i][j] == "selection_style") {
				selection_style = contents[i][j];
			}
			else if (attributes[i][j] == "animation") {
				//TODO: replace animation with "transformation" or something that will make it clear that a given element is selected
				//TODO
				//animation_types.push_back(contents[i][j]);
				/*
				const int contents_size = contents[i].size();
				for (int k = 0; k < contents_size; k++) {
					//animation_types.push_back(contents[i][k]);
				}
				*/
			}
			else if (attributes[i][j] == "axis") {
				axis = ::atoi(contents[i][j].c_str());
			}
			else if (attributes[i][j] == "position") {
				std::pair<std::string, std::string> pos_pair = FileManager::string_to_pair(contents[i][j], ",");
				position[0] = ::atoi(pos_pair.first.c_str());
				position[1] = ::atoi(pos_pair.second.c_str());
				x_pos = ::atoi(pos_pair.first.c_str());
				y_pos = ::atoi(pos_pair.second.c_str());
			}
		}
	}
	set_menu_items();
	set_animations();
}

void MenuManager::load_xml_content(std::string menu_key)
{
	this->menu_key = menu_key;
	file_manager.load_xml_content(this, "resources/load/menus",
		"SerializableClass", "MenuKey", menu_key);
	position[0] = x_pos.value();
	position[1] = y_pos.value(); //TODO: replace position with the two positions and remove this part
	std::string font_full_filename = "resources/fonts/" + this->font_filename.value() + ".ttf";
	font = al_load_font(font_full_filename.c_str(), this->font_size.value(), NULL);
	const int size = this->menu_items.size();
	for (int i = 0; i < size; i++) {
		menu_items.getItem(i)->load_additional_content();
	}
	//set_menu_items();
	//set_animations();
	//TODO: anything that isn't serialzed
}

void MenuManager::unload_content()
{
	al_destroy_font(font);
	//TODO:figure out what memory leaks  we need to catch here
	// necessary since we didn't use ImageLoader to get  the menu images
	/*
	for (int i = 0; i < menu_items.size(); i++) {
		menu_items[i]->unload_content();
		delete menu_items[i];
	}
	*/
	menu_items.Clear();
	attributes.clear();
	contents.clear();
}

void MenuManager::update()
{
	//for (int i = 0; i < menu_items.size(); i++) {
		//TODO: update as necessary
	//}
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
		std::pair<float, float> adjusted_menu_pos(menu_pos.first - position[0], menu_pos.second);
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
	//std::pair<float, float> mouse_pos(x_pos - x_off - position[0], y_pos - y_off - position[1]);
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

xmls::Collection<MenuItem> &MenuManager::get_menu_items()
{
	return menu_items;
}

std::string MenuManager::get_selected_action_key()
{
	if (menu_items.size() < item_index + 1) return "";
	return menu_items.getItem(item_index)->get_selection_action_key();
}

std::string MenuManager::get_confirm_action_key()
{
	return menu_items.getItem(item_index)->get_confirm_action_key();
}
