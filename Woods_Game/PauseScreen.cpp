#include "PauseScreen.h"



PauseScreen::PauseScreen()
{
	
}


PauseScreen::~PauseScreen()
{
}

void PauseScreen::reset()
{
	current_menu_manager().reset();
	screen_flag = FLAG_NONE;
	menu_key = PAUSE_MAIN_MENU;	
}

void PauseScreen::set_menus()
{
}

ALLEGRO_BITMAP * PauseScreen::current_backdrop()
{
	return backdrops[menu_key];
}

MenuManager& PauseScreen::current_menu_manager()
{
	return *menus[menu_key];
}

void PauseScreen::load_content()
{
	//load_config_settings();
	//main menu
	menus[PAUSE_MAIN_MENU] = std::make_unique<MenuManager>();
	//menus[PAUSE_MAIN_MENU]->load_content("pause_main_menu");
	menus[PAUSE_MAIN_MENU]->load_xml_content("pause_main_menu");
	ImageLoader::get_instance().load_image("ui/pause_main_backdrop");
	backdrops[PAUSE_MAIN_MENU] = ImageLoader::get_instance().get_image("ui/pause_main_backdrop");
	//video settings
	menus[PAUSE_VIDEO_MENU] = std::make_unique<MenuManager>();
	//menus[PAUSE_VIDEO_MENU]->load_content("pause_video_menu");
	menus[PAUSE_VIDEO_MENU]->load_xml_content("pause_video_menu");
	ImageLoader::get_instance().load_image("ui/pause_video_backdrop");
	backdrops[PAUSE_VIDEO_MENU] = ImageLoader::get_instance().get_image("ui/pause_video_backdrop");
	//std::vector<MenuItem*> video_items = menus[PAUSE_VIDEO_MENU]->get_menu_items();
	
	//xmls::Collection<MenuItem> video_items = menus[PAUSE_VIDEO_MENU]->get_menu_items();
	//xmls::Collection<TextOption> &resolutions = video_items.getItem(0)->get_text_options();
	/*
	for (int i = 0; i < resolutions.size(); i++) {
		if (resolutions.getItem(i)->Option.value() == config_settings[CONFIG_SCREEN_RESOLUTION]) {
			video_items.getItem(0)->set_options_index(i);
		}
	}
	xmls::Collection<TextOption> &screen_styles = video_items.getItem(1)->get_text_options();
	for (int i = 0; i < screen_styles.size(); i++) {
		if (screen_styles.getItem(i)->Option.value() == config_settings[CONFIG_SCREEN_STYLE]) {
			video_items.getItem(1)->set_options_index(i);
		}
	}
	
	
	//temp
	std::string xmlString = menus[PAUSE_VIDEO_MENU]->toXML();
	std::ofstream out("resources/load/pause_video_menu.xml");
	out << xmlString;
	out.close();
	//temp
	*/
	// controls settings
	menus[PAUSE_CONTROLS_MENU] = std::make_unique<MenuManager>();
	//menus[PAUSE_CONTROLS_MENU]->load_content("pause_controls_menu");
	menus[PAUSE_CONTROLS_MENU]->load_xml_content("pause_controls_menu");
	ImageLoader::get_instance().load_image("ui/pause_controls_backdrop");
	backdrops[PAUSE_CONTROLS_MENU] = ImageLoader::get_instance().get_image("ui/pause_controls_backdrop");
	//temp
	/*
	std::string xmlString1 = menus[PAUSE_CONTROLS_MENU]->toXML();
	std::ofstream out1("resources/load/pause_controls_menu.xml");
	out1 << xmlString1;
	out1.close();
	*/
	//temp
	// keyboard controls settings
	menus[PAUSE_CONTROLS_MENU_KEYBOARD] = std::make_unique<ControlsMenuManager>();
	//menus[PAUSE_CONTROLS_MENU_KEYBOARD]->load_content("pause_controls_menu_keyboard");
	menus[PAUSE_CONTROLS_MENU_KEYBOARD]->load_xml_content("pause_controls_menu_keyboard");
	ImageLoader::get_instance().load_image("ui/pause_controls_keyboard_backdrop");
	backdrops[PAUSE_CONTROLS_MENU_KEYBOARD] = ImageLoader::get_instance().get_image("ui/pause_controls_keyboard_backdrop");
	//temp
	/*
	std::string xmlString2 = menus[PAUSE_CONTROLS_MENU_KEYBOARD]->toXML();
	std::ofstream out2("resources/load/pause_controls_menu_keyboard.xml");
	out2 << xmlString2;
	out2.close();
	*/
	//temp
	// controller controls settings
	menus[PAUSE_CONTROLS_MENU_CONTROLLER] = std::make_unique<ControlsMenuManager>();
	//menus[PAUSE_CONTROLS_MENU_CONTROLLER]->load_content("pause_controls_menu_controller");
	menus[PAUSE_CONTROLS_MENU_CONTROLLER]->load_xml_content("pause_controls_menu_controller");
	ImageLoader::get_instance().load_image("ui/pause_controls_controller_backdrop");
	backdrops[PAUSE_CONTROLS_MENU_CONTROLLER] = ImageLoader::get_instance().get_image("ui/pause_controls_controller_backdrop");
	/*
	//temp
	std::string xmlString3 = menus[PAUSE_CONTROLS_MENU_CONTROLLER]->toXML();
	std::ofstream out3("resources/load/pause_controls_menu_controller.xml");
	out3 << xmlString3;
	out3.close();
	//temp
	*/
}

void PauseScreen::load_config_settings()
{
	//TODO: change to new xml loading
	FileManager filemanager;
	std::string filename = "resources/config";
	filemanager.load_xml_content(config.get(), filename, "SerializableClass", "ConfigurationsKey", "current_configurations");
	//std::vector<std::vector<std::string>> attributes, contents;
	//filemanager.load_content("resources/config.ini", attributes, contents);

	//TODO: handle config settings here
	/*
	for (int i = 0; i < attributes.size(); i++) {
		for (int j = 0; j < attributes[i].size(); j++) {
			if (attributes[i][j] == "screen_resolution") {
				config_settings[CONFIG_SCREEN_RESOLUTION] = contents[i][j];
			}
			else if (attributes[i][j] == "screen_style") {
				config_settings[CONFIG_SCREEN_STYLE] = contents[i][j];
			}
		}
	}
	*/
}

void PauseScreen::unload_content()
{
	
	backdrops.clear();
	//main_backdrop = NULL;
	GameScreen::unload_content();
}

void PauseScreen::update()
{
	mouse_cursor_update();
	GameScreen::update();
}

void PauseScreen::draw(ALLEGRO_DISPLAY * display)
{
	const float x = (al_get_display_width(display) - al_get_bitmap_width(current_backdrop())) / 2.0;
	const float y = (al_get_display_height(display) - al_get_bitmap_height(current_backdrop())) / 2.0;
	al_draw_bitmap(current_backdrop(), x, y, 0);
	current_menu_manager().draw(display, x, y, 
		al_get_bitmap_width(current_backdrop()), al_get_bitmap_height(current_backdrop()));
	GameScreen::draw(display);
}

void PauseScreen::process_mouse_click_left(const int x, const int y)
{
	const float x_off = (al_get_display_width(al_get_current_display()) - al_get_bitmap_width(current_backdrop())) / 2.0;
	const float y_off = (al_get_display_height(al_get_current_display()) - al_get_bitmap_height(current_backdrop())) / 2.0;
	if (current_menu_manager().is_selecting_input()) {
		return;
	}
	if (current_menu_manager().is_selecting_options()) {
		const int direction_index = current_menu_manager().mouse_selected_options_direction_index(x, y,
			x_off, y_off,
			al_get_bitmap_width(current_backdrop()),
			al_get_bitmap_height(current_backdrop()));
		switch (direction_index) {
		case 0: //left bracket
			menu_left();
			return;
		case 1:	//right bracket
			menu_right();
			return;
		}
		if (current_menu_manager().mouse_selecting_current_option(x, y, x_off, y_off,
			al_get_bitmap_width(current_backdrop()),
			al_get_bitmap_height(current_backdrop()))) {
			confirm_selection();
		}
		return;
	}
	const int index = current_menu_manager().mouse_selected_index(x, y, x_off, y_off);
	if (index >= 0) {
		current_menu_manager().set_item_index(index);
		confirm_selection();
	}
}

void PauseScreen::mouse_cursor_update()
{
	const float x_off = (al_get_display_width(al_get_current_display()) - al_get_bitmap_width(current_backdrop())) / 2.0;
	const float y_off = (al_get_display_height(al_get_current_display()) - al_get_bitmap_height(current_backdrop())) / 2.0;
	current_menu_manager().mouse_cursor_update(mouse_pos.first, mouse_pos.second, x_off, y_off);
}

void PauseScreen::cancel_menu()
{
	if (current_menu_manager().is_selecting_options()) {
		current_menu_manager().cancel_option_select();
		return;
	}
	current_menu_manager().reset();
	switch (menu_key) {
	case PAUSE_MAIN_MENU:
		screen_flag = FLAG_RESUME_GAME;
		break;
	case PAUSE_VIDEO_MENU:
		menu_key = PAUSE_MAIN_MENU;
		break;
	case PAUSE_CONTROLS_MENU:
		menu_key = PAUSE_MAIN_MENU;
		break;
	case PAUSE_CONTROLS_MENU_KEYBOARD:
		menu_key = PAUSE_CONTROLS_MENU;
		break;
	case PAUSE_CONTROLS_MENU_CONTROLLER:
		menu_key = PAUSE_CONTROLS_MENU;
		break;
	}
	
}

void PauseScreen::menu_up()
{
	current_menu_manager().menu_up();
}

void PauseScreen::menu_down()
{
	current_menu_manager().menu_down();
}

void PauseScreen::menu_left()
{
	current_menu_manager().menu_left();
}

void PauseScreen::menu_right()
{
	current_menu_manager().menu_right();
}

void PauseScreen::confirm_selection()
{
std::string action_key = current_menu_manager().get_selected_action_key();
if (action_key == MenuManager::SELECTION_KEY_RETURN_TO_MAIN_PAUSE) {
	current_menu_manager().reset();
	menu_key = PAUSE_MAIN_MENU;
}
// confirm actions (for options selected from some scrollable list)
else if (action_key == MenuManager::SELECTION_KEY_SELECT_OPTIONS) {
	if (current_menu_manager().is_selecting_options()) {
		const std::string confirm_key = current_menu_manager().get_confirm_action_key();
		// video settings confirm actions
		if (confirm_key == MenuManager::SELECTION_KEY_SET_RESOLUTION) {
			//TODO: write the new resolution to the config xml file
			const std::string res_string = current_menu_manager().get_menu_items().getItem(0)->get_selected_text_option();
			const std::pair<std::string, std::string> res_parts = FileManager::string_to_pair(res_string, "x");
			al_resize_display(al_get_current_display(), ::atoi(res_parts.first.c_str()), ::atoi(res_parts.second.c_str()));
			this->config->set_screen_res_x(::atoi(res_parts.first.c_str()));
			this->config->set_screen_res_y(::atoi(res_parts.second.c_str()));
			this->save_configurations();
		}
		else if (confirm_key == MenuManager::SELECTION_KEY_SET_SCREEN_STYLE) {
			const std::string style_string = current_menu_manager().get_menu_items().getItem(1)->get_selected_text_option();
			if (style_string == Configurations::SCREEN_STYLE_FULLSCREEN) {
				ALLEGRO_DISPLAY_MODE   disp_data;
				al_get_display_mode(al_get_num_display_modes() - 1, &disp_data);
				//al_set_display_flag(al_get_current_display(), ALLEGRO_FULLSCREEN_WINDOW, false);
				al_set_display_flag(al_get_current_display(), ALLEGRO_FULLSCREEN, true);
				al_resize_display(al_get_current_display(), disp_data.width, disp_data.height);
				
			} else if (style_string == Configurations::SCREEN_STYLE_WINDOWED_FULLSCREEN) {
				//std::cout << "setting windowed fullscreen\n";
				ALLEGRO_DISPLAY_MODE   disp_data;
				al_get_display_mode(al_get_num_display_modes() - 1, &disp_data);
				//al_set_display_flag(al_get_current_display(), ALLEGRO_FULLSCREEN, false);
				al_set_display_flag(al_get_current_display(), ALLEGRO_FULLSCREEN_WINDOW, true);
				al_resize_display(al_get_current_display(), disp_data.width, disp_data.height);
			}
			else if (style_string == Configurations::SCREEN_STYLE_WINDOWED) {
				const std::string res_string = current_menu_manager().get_menu_items().getItem(0)->get_selected_text_option();
				std::pair<std::string, std::string> res_parts = FileManager::string_to_pair(res_string, "x");
				al_set_display_flag(al_get_current_display(), ALLEGRO_FULLSCREEN_WINDOW, false);
				al_set_display_flag(al_get_current_display(), ALLEGRO_FULLSCREEN, false);
				al_resize_display(al_get_current_display(), ::atoi(res_parts.first.c_str()), ::atoi(res_parts.second.c_str()));
			}
			this->config->set_screen_mode(style_string);
			this->save_configurations();
			//TODO: set screen styles properly
		}
		current_menu_manager().confirm_option_select();
	}
	else {
		current_menu_manager().select_options();
	}
}
// main pause menu actions
else if (action_key == MenuManager::SELECTION_KEY_RESUME_GAME) {
	current_menu_manager().reset();
	screen_flag = FLAG_RESUME_GAME;
}
else if (action_key == MenuManager::SELECTION_KEY_QUIT_GAME) {
	current_menu_manager().reset();
	screen_flag = FLAG_QUIT_GAME;
}
else if (action_key == MenuManager::SELECTION_KEY_OPEN_VIDEO_SETTINGS) {
	menu_key = PAUSE_VIDEO_MENU;
}
else if (action_key == MenuManager::SELECTION_KEY_OPEN_CONTROL_SETTINGS) {
	menu_key = PAUSE_CONTROLS_MENU;
}
else if (action_key == MenuManager::SELECTION_KEY_OPEN_KEYBOARD_CONTROLS) {
	menu_key = PAUSE_CONTROLS_MENU_KEYBOARD;
}
else if (action_key == MenuManager::SELECTION_KEY_OPEN_CONTROLLER_CONTROLS) {
	menu_key = PAUSE_CONTROLS_MENU_CONTROLLER;
}
else if (action_key == MenuManager::SELECTION_KEY_RETURN_TO_MAIN_CONTROLS) {
	current_menu_manager().reset();
	menu_key = PAUSE_CONTROLS_MENU;
}
else if (action_key == MenuManager::SELECTION_KEY_SET_CONTROL_INPUT) {
	if (!current_menu_manager().is_selecting_input()) {
		current_menu_manager().set_selecting_input(true);
	} else {
		current_menu_manager().set_selecting_input(false);
	}
}
else if (action_key == MenuManager::SELECTION_KEY_CANCEL_CONTROLS_CHANGES) {
	current_menu_manager().reset();
	menu_key = PAUSE_CONTROLS_MENU;
}
else if (action_key == MenuManager::SELECTION_KEY_REVERT_CONTROLS_DEFAULT) {
	current_menu_manager().revert_controls_default();
}
else if (action_key == MenuManager::SELECTION_KEY_SAVE_CONTROLS_CHANGES) {
	current_menu_manager().save_controls();
	current_menu_manager().reset();
	menu_key = PAUSE_CONTROLS_MENU;
	screen_flag = FLAG_RESUME_GAME;
}
}

void PauseScreen::call_keyboard_mappable_input(ALLEGRO_EVENT ev, bool toggle)
{
	if (!toggle || this->menu_key != PAUSE_CONTROLS_MENU_KEYBOARD) return;
	current_menu_manager().set_keyboard_mappable_input(ev.keyboard.keycode);
}

void PauseScreen::call_controller_mappable_input(ALLEGRO_EVENT ev, bool toggle)
{
	if (!toggle || this->menu_key != PAUSE_CONTROLS_MENU_CONTROLLER) return;
	current_menu_manager().set_controller_mappable_input(ev.joystick.button);
}

bool PauseScreen::taking_mappable_input()
{
	return (this->menu_key == PAUSE_CONTROLS_MENU_KEYBOARD || this->menu_key == PAUSE_CONTROLS_MENU_CONTROLLER) 
		&& this->current_menu_manager().is_selecting_input();
}
