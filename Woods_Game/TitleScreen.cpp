#include "TitleScreen.h"

InputManager title_input;

void TitleScreen::play_title_music()
{
	const std::string title_music_filename = "running_theme"; //temp
	if (!AudioManager::get_instance().is_playing_music()) {
		AudioManager::get_instance().play_music(title_music_filename);
	}
}

void TitleScreen::music_update()
{
	this->play_title_music();
}

TitleScreen::TitleScreen()
{
}


TitleScreen::~TitleScreen()
{
	font = NULL;
}

void TitleScreen::load_content()
{
	//main menu
	menus[TITLE_MAIN_MENU] = std::make_unique<MenuManager>();
	menus[TITLE_MAIN_MENU]->load_xml_content("title_main_menu");
	ImageLoader::get_instance().load_image("ui/title_main_backdrop");
	backdrop_filenames[TITLE_MAIN_MENU] = "ui/title_main_backdrop";
	
	//load menu
	menus[TITLE_LOAD_GAME_MENU] = std::make_unique<LoadGameMenuManager>();
	menus[TITLE_LOAD_GAME_MENU]->load_xml_content("title_load_game_menu");
	backdrop_filenames[TITLE_LOAD_GAME_MENU] = "ui/title_main_backdrop"; //TEMP
	
	//controls
	set_default_controls();
	//TODO: options, quit, etc
}

void TitleScreen::unload_content()
{
	al_destroy_font(font);
	font = NULL;
	GameScreen::unload_content();
}

void TitleScreen::initialize_load_game_menu()
{
	//TODO
}

// input-sensitive actions that may be passed along

void title_menu_cancel(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.screen_receiving_input().cancel_menu();
	}
}

void title_input_menu_up(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.screen_receiving_input().menu_up();		
	}
}

void title_input_menu_down(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.screen_receiving_input().menu_down();
	}
}

void title_input_menu_left(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.screen_receiving_input().menu_left();
	}
}

void title_input_menu_right(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.screen_receiving_input().menu_right();
	}
}

void title_input_confirm_selection(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.screen_receiving_input().confirm_selection();
	}
}

void title_move_mouse_pos(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.update_mouse_position(ev.mouse.x, ev.mouse.y, ev.mouse.z);
	}
}

void title_register_mouse_click_left(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.process_mouse_click_left(ev.mouse.x, ev.mouse.y);
	}
}

void title_input_select(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
if (toggle) {
	screen.screen_receiving_input().select();
}
}

void TitleScreen::set_default_controls()
{
	// paused controls	
	control_map[TITLE_SCREEN] = std::map<std::pair<int, int>, controlFunc>();

	// keyboard
	// select
	control_map[TITLE_SCREEN].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_UP), &title_input_menu_up);
	control_map[TITLE_SCREEN].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_DOWN), &title_input_menu_down);
	control_map[TITLE_SCREEN].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_LEFT), &title_input_menu_left);
	control_map[TITLE_SCREEN].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_RIGHT), &title_input_menu_right);
	// confirm/cancel selection
	control_map[TITLE_SCREEN].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_ENTER), &title_input_confirm_selection);
	control_map[TITLE_SCREEN].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_SPACE), &title_menu_cancel);
	control_map[TITLE_SCREEN].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_ESCAPE), &title_menu_cancel);
	// mouse 
	control_map[TITLE_SCREEN].emplace(std::pair<int, int>(ALLEGRO_EVENT_MOUSE_AXES, 0), &title_move_mouse_pos);
	control_map[TITLE_SCREEN].emplace(std::pair<int, int>(ALLEGRO_EVENT_MOUSE_BUTTON_DOWN, 1), &title_register_mouse_click_left);

	// controller
	// select
	control_map[TITLE_SCREEN].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_UP), &title_input_menu_up);
	control_map[TITLE_SCREEN].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_DOWN), &title_input_menu_down);
	control_map[TITLE_SCREEN].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_LEFT), &title_input_menu_left);
	control_map[TITLE_SCREEN].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_RIGHT), &title_input_menu_right);
	// confirm/cancel selection
	control_map[TITLE_SCREEN].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_A), &title_input_confirm_selection);
	control_map[TITLE_SCREEN].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_B), &title_menu_cancel);
}

int TitleScreen::get_game_mode()
{
	return TITLE_SCREEN;
}

void TitleScreen::update()
{
	GameScreen::update();
	this->mouse_cursor_update();
	this->music_update();
}

void TitleScreen::draw(ALLEGRO_DISPLAY * display)
{
	//TODO: draw background (animated?)
	//TODO: could put backdrop somewhere besides the center of the screen
	const float x = (al_get_display_width(display) - al_get_bitmap_width(current_backdrop())) / 2.0;
	const float y = (al_get_display_height(display) - al_get_bitmap_height(current_backdrop())) / 2.0;
	al_draw_bitmap(current_backdrop(), x, y, 0);
	current_menu_manager().draw(display, x, y,
		al_get_bitmap_width(current_backdrop()), al_get_bitmap_height(current_backdrop()));
	GameScreen::draw(display);
}

ALLEGRO_BITMAP * TitleScreen::current_backdrop()
{
	const std::string filename = this->backdrop_filenames[this->menu_key];
	return ImageLoader::get_instance().get_image(filename);
}

MenuManager & TitleScreen::current_menu_manager()
{
	return *menus[menu_key];
}

void TitleScreen::menu_up()
{
	current_menu_manager().menu_up();
	AudioManager::get_instance().play_sfx("menu_sounds/title_scroll", "" + SOUND_KEY_MENU);
}

void TitleScreen::menu_down()
{
	current_menu_manager().menu_down();
	AudioManager::get_instance().play_sfx("menu_sounds/title_scroll", "" + SOUND_KEY_MENU);
}

void TitleScreen::menu_left()
{
	current_menu_manager().menu_left();
	AudioManager::get_instance().play_sfx("menu_sounds/title_scroll", "" + SOUND_KEY_MENU);
}

void TitleScreen::menu_right()
{
	current_menu_manager().menu_right();
	AudioManager::get_instance().play_sfx("menu_sounds/title_scroll", "" + SOUND_KEY_MENU);
}

void TitleScreen::cancel_menu()
{
	if (this->menu_key == TITLE_LOAD_GAME_MENU) {
		this->menu_key = TITLE_MAIN_MENU;
		AudioManager::get_instance().play_sfx("menu_sounds/title_cancel", "" + SOUND_KEY_MENU);
	}
}

void TitleScreen::confirm_selection()
{
	std::string action_key = current_menu_manager().get_selected_action_key();
	if (action_key == MenuManager::SELECTION_KEY_START_NEW_GAME) {
		//TODO: more menu options before leaving title screen? (name character, select save file, etc.)
		screen_flag = FLAG_START_NEW_GAME;
	} else if (action_key == MenuManager::SELECTION_KEY_LOAD_GAME) {
		this->menu_key = TITLE_LOAD_GAME_MENU;
	} else if (action_key == MenuManager::SELECTION_KEY_CONFIRM_LOAD_GAME) {
		const std::string filename = this->menus[TITLE_LOAD_GAME_MENU]->selected_filepath();
		screen_flag = FLAG_LOAD_GAME;
		this->load_game_filepath = filename;
	}
	AudioManager::get_instance().play_sfx("menu_sounds/title_confirm", "" + SOUND_KEY_MENU);
}

void TitleScreen::process_mouse_click_left(const int x, const int y)
{
	//TODO: menu option select
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
			AudioManager::get_instance().play_sfx("menu_sounds/pause_menu_scroll", "" + SOUND_KEY_MENU);
			return;
		case 1:	//right bracket
			menu_right();
			AudioManager::get_instance().play_sfx("menu_sounds/pause_menu_scroll", "" + SOUND_KEY_MENU);
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

void TitleScreen::update_mouse_position(const int x, const int y, const int z)
{
	GameScreen::update_mouse_position(x, y, z);
}

void TitleScreen::mouse_cursor_update()
{
	const float x_off = (al_get_display_width(al_get_current_display()) - al_get_bitmap_width(current_backdrop())) / 2.0;
	const float y_off = (al_get_display_height(al_get_current_display()) - al_get_bitmap_height(current_backdrop())) / 2.0;
	current_menu_manager().mouse_cursor_update(mouse_pos.first, mouse_pos.second, x_off, y_off);
}


