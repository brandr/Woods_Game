#include "MainGameScreen.h"
#include "Level.h"
#include<iostream>

//InputManager main_input;

// functions to be passed along to the level, usually to the player
void move_up(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	screen.set_input(INPUT_UP, toggle);
}

void move_down(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	screen.set_input(INPUT_DOWN, toggle);
}

void move_left(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	screen.set_input(INPUT_LEFT, toggle);
}

void move_right(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	screen.set_input(INPUT_RIGHT, toggle);
}

void number_entry(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if(toggle){
		screen.process_number_input(ev.keyboard.keycode);
	}
}

void input_a_button(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.a_button_action();
	}
}

void input_b_button(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.b_button_action();
	}
}

void input_x_button(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.x_button_action();
	}
}

void input_y_button(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.y_button_action();
	}
}

void input_left_bumper(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.left_bumper_action();
	}
}

void input_right_bumper(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.right_bumper_action();
	}
}

void pause(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.pause_game();
	}
}

void resume(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.resume_game();
	}
}

void menu_cancel(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.cancel_menu();
	}
}

void input_menu_up(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.menu_up();
	}
}

void input_menu_down(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.menu_down();
	}
}

void input_menu_left(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.menu_left();
	}
}

void input_menu_right(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.menu_right();
	}
}

void input_confirm_selection(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.confirm_selection();
	}
}

void input_select(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.select();
	}
}

void move_joystick_axis(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		switch (ev.joystick.axis) {
		case 0:	// x axis
			screen.set_joystick_pos(ev.joystick.stick, ev.joystick.pos, screen.get_joystick_pos(ev.joystick.stick).second);
			break;
		case 1: // y axis
			screen.set_joystick_pos(ev.joystick.stick,  screen.get_joystick_pos(ev.joystick.stick).first, ev.joystick.pos);
			break;
		
		}
	}
}

void move_mouse_pos(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.update_mouse_position(ev.mouse.x, ev.mouse.y);
	}
}

void register_mouse_click_left(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.process_mouse_click_left(ev.mouse.x, ev.mouse.y);
	}
}

MainGameScreen::MainGameScreen()
{
	set_default_controls();
	set_input_map();
}

MainGameScreen::~MainGameScreen()
{

}

void MainGameScreen::set_default_controls()
{
	// main controls
	control_map[TOP_DOWN] = std::map<std::pair<int, int>, controlFunc>();
		// key presses
			// movement
	control_map[TOP_DOWN].emplace(std::pair<int,int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_UP), &move_up);
	control_map[TOP_DOWN].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_DOWN), &move_down);
	control_map[TOP_DOWN].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_LEFT), &move_left);
	control_map[TOP_DOWN].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_RIGHT), &move_right);
			// pause
	control_map[TOP_DOWN].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_ESCAPE), &pause);
			// hotbar hotkeys
	control_map[TOP_DOWN].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_1), &number_entry);
	control_map[TOP_DOWN].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_2), &number_entry);
	control_map[TOP_DOWN].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_3), &number_entry);
	control_map[TOP_DOWN].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_4), &number_entry);
	control_map[TOP_DOWN].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_5), &number_entry);
	control_map[TOP_DOWN].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_6), &number_entry);
	control_map[TOP_DOWN].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_7), &number_entry);
	control_map[TOP_DOWN].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_8), &number_entry);
	control_map[TOP_DOWN].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_9), &number_entry);
	control_map[TOP_DOWN].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_0), &number_entry);

		// mouse
	//control_map[TOP_DOWN].emplace(std::pair<int, int>(ALLEGRO_EVENT_MOUSE_AXES, 0), &move_mouse_axis);

		// joystick
			// movement
	control_map[TOP_DOWN].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_AXIS, 0), &move_joystick_axis);
			// pause
	control_map[TOP_DOWN].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_START), &pause);
			// A/B/X/Y button
	control_map[TOP_DOWN].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_A), &input_a_button);
	control_map[TOP_DOWN].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_B), &input_b_button);
	control_map[TOP_DOWN].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_X), &input_x_button);
	control_map[TOP_DOWN].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_Y), &input_y_button);
			// hotbar selection
	control_map[TOP_DOWN].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_LEFT_SHOULDER), &input_left_bumper);
	control_map[TOP_DOWN].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_RIGHT_SHOULDER), &input_right_bumper);
	
	
	// paused controls	
	control_map[MAIN_GAME_PAUSED] = std::map<std::pair<int, int>, controlFunc>();

		// key presses
			// select
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_UP), &input_menu_up);
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_DOWN), &input_menu_down);
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_LEFT), &input_menu_left);
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_RIGHT), &input_menu_right);
			// resume
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_ESCAPE), &resume);
		// confirm selection
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_ENTER), &input_confirm_selection);

		// mouse 
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_MOUSE_AXES, 0), &move_mouse_pos);
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_MOUSE_BUTTON_DOWN, 1), &register_mouse_click_left);

		// joystick
			// select
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_UP), &input_menu_up);
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_DOWN), &input_menu_down);
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_LEFT), &input_menu_left);
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_RIGHT), &input_menu_right);
			// resume
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_START), &resume);
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_B), &menu_cancel);
			// confirm selection
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_A), &input_confirm_selection);

	// inventory controls
		// key presses
			// select
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_UP), &input_menu_up);
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_DOWN), &input_menu_down);
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_LEFT), &input_menu_left);
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_RIGHT), &input_select);
			// resume
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_ESCAPE), &resume);
		// mouse
		// joystick
			// select
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_UP), &input_menu_up);
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_DOWN), &input_menu_down);
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_LEFT), &input_menu_left);
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_RIGHT), &input_menu_right);
			// move items
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_A), &input_select);
			// resume
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_Y), &resume);
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_B), &resume);
	
}

void MainGameScreen::set_input_map()
{
	input_map[INPUT_UP] = false;
	input_map[INPUT_DOWN] = false;
	input_map[INPUT_LEFT] = false;
	input_map[INPUT_RIGHT] = false;
}

void MainGameScreen::load_content()
{
	game_image_manager.load_content();
	pause_screen.load_content();
	inventory_screen.load_content();
	inventory_screen.set_inventory(&(game_image_manager.get_player()->get_inventory()));
	load_controls();
	load_ui_content();
}

void MainGameScreen::load_ui_content()
{
	ImageLoader::get_instance().load_image("ui/item_box_1");
	ImageLoader::get_instance().load_image("ui/item_box_1_light");
	hotbar_box = ImageLoader::get_instance().get_image("ui/item_box_1");
	hotbar_box_selected = ImageLoader::get_instance().get_image("ui/item_box_1_light");
	font_map[FONT_HOTBAR] = al_load_font("resources/fonts/OpenSans-Regular.ttf", 12, NULL); 
	//TODO: other UI components like stamina/time
}

void MainGameScreen::unload_content()
{
	game_image_manager.unload_content();
	pause_screen.unload_content();
	inventory_screen.unload_content();
	GameScreen::unload_content();
	
	//TODO: unload everything that was loaded in
}

void MainGameScreen::load_controls()
{
	FileManager file_manager;
	std::vector<std::vector<std::string>> keyboard_attributes;
	std::vector<std::vector<std::string>> keyboard_contents;	
	file_manager.load_content("resources/load/controls", keyboard_attributes, keyboard_contents, "current_controls_keyboard");
	this->keyboard_controls.load_content(keyboard_attributes, keyboard_contents);
	std::vector<std::vector<std::string>> controller_attributes;
	std::vector<std::vector<std::string>> controller_contents;
	file_manager.load_content("resources/load/controls", controller_attributes, controller_contents, "current_controls_controller");
	this->controller_controls.load_content(controller_attributes, controller_contents);
}

void MainGameScreen::update()
{
	const int game_mode = game_image_manager.get_game_mode();
	switch (game_mode) {
		case TOP_DOWN:
			game_image_manager.update(input_map, joystick_pos_map);
			break;
		case MAIN_GAME_PAUSED:
			pause_screen.set_mouse_position(mouse_pos);
			pause_screen.update();
			if (pause_screen.get_screen_flag() == FLAG_RESUME_GAME) {
				resume_game();
			}
			else if (pause_screen.get_screen_flag() == FLAG_QUIT_GAME) {
				quit_game();
			}
			break;
	}
	
	GameScreen::update();
}

void MainGameScreen::draw(ALLEGRO_DISPLAY * display)
{
	game_image_manager.draw(display);
	draw_ui(display);
}

void MainGameScreen::draw_ui(ALLEGRO_DISPLAY * display)
{
	switch (get_game_mode()) {
	case MAIN_GAME_PAUSED:
		draw_ui_paused(display);
		break;
	case MAIN_GAME_INVENTORY:
		draw_ui_inventory(display);
		break;
	case TOP_DOWN:
		draw_ui_main_game(display);
		break;
	}
}

void MainGameScreen::draw_ui_inventory(ALLEGRO_DISPLAY * display)
{
	inventory_screen.draw(display);
}

void MainGameScreen::draw_ui_main_game(ALLEGRO_DISPLAY * display)
{
	draw_hotbar(display);
	//TODO: draw other HUD components like stamina
}

void MainGameScreen::draw_hotbar(ALLEGRO_DISPLAY * display)
{
	Inventory& inventory = game_image_manager.get_player()->get_inventory();
	std::vector<Item*> hotbar = inventory.get_hotbar();
	const int width = al_get_display_width(display);
	const int height = al_get_display_height(display);
	const int box_width = al_get_bitmap_width(hotbar_box);
	const int box_height = al_get_bitmap_height(hotbar_box);
	const int hotbar_index = inventory.get_hotbar_index();
	const int size = hotbar.size();
	for (int i = 0; i < size; i++) {
		const float x = (width - box_width*size)/2 + i*box_width;
		const float y = height - box_height - 12;
		const int num = i < 9 ? i + 1 : 0;
		if (i == hotbar_index) {
			al_draw_bitmap(hotbar_box_selected, x, y, 0);
		}
		else {
			al_draw_bitmap(hotbar_box, x, y, 0);
		}
		const std::vector<int> rgb = MenuItem::string_to_rgb(FONT_COLOR_HOTBAR);
		al_draw_text(font_map[FONT_HOTBAR], al_map_rgb(rgb[0], rgb[1], rgb[2]), x + 8.0f, y + 5.0f, 0, std::to_string(num).c_str());
		Item* item = hotbar[i];
		if (item) item->draw(display, x, y);
		//TODO: draw the item here if the index of the hotbar is not empty
	}
}

void MainGameScreen::draw_ui_paused(ALLEGRO_DISPLAY* display)
{
	pause_screen.draw(display);
}

int MainGameScreen::get_game_mode()
{
	return game_image_manager.get_game_mode();
}

int MainGameScreen::get_camera_offset_x()
{
	return 0;
}

int MainGameScreen::get_camera_offset_y()
{
	return 0;
}

void MainGameScreen::refresh()
{

}

void MainGameScreen::pause_game()
{
	if (game_image_manager.get_player()->get_current_action() != ACTION_NONE) return;
	switch (get_game_mode()) {
	case TOP_DOWN:
		game_image_manager.set_game_mode(MAIN_GAME_PAUSED);
		pause_screen.reset();
		break;
	}
}

void MainGameScreen::cancel_menu()
{
	if (get_game_mode() != MAIN_GAME_PAUSED) return;
	pause_screen.cancel_menu();
}

void MainGameScreen::resume_game()
{
	switch (get_game_mode()) {
	case MAIN_GAME_PAUSED:
		pause_screen.reset();
		break;
	case MAIN_GAME_INVENTORY:
		inventory_screen.reset();
		break;
	}
	//TODO: reset other screens as necessary for the given game mode
	clear_input();
	game_image_manager.resume();
	game_image_manager.set_game_mode(TOP_DOWN);	
}

void MainGameScreen::menu_up()
{
	switch (get_game_mode()) {
	case MAIN_GAME_PAUSED:
		pause_screen.menu_up();
		break;
	case MAIN_GAME_INVENTORY:
		inventory_screen.menu_up();
		break;
	}
}

void MainGameScreen::menu_down()
{
	switch (get_game_mode()) {
	case MAIN_GAME_PAUSED:
		pause_screen.menu_down();
		break;
	case MAIN_GAME_INVENTORY:
		inventory_screen.menu_down();
		break;
	}
}

void MainGameScreen::menu_left()
{
	switch (get_game_mode()) {
	case MAIN_GAME_PAUSED:
		pause_screen.menu_left();
		break;
	case MAIN_GAME_INVENTORY:
		inventory_screen.menu_left();
		break;
	}
}

void MainGameScreen::menu_right()
{
	switch (get_game_mode()) {
	case MAIN_GAME_PAUSED:
		pause_screen.menu_right();
		break;
	case MAIN_GAME_INVENTORY:
		inventory_screen.menu_right();
		break;
	}
}

void MainGameScreen::confirm_selection()
{
	switch (get_game_mode()) {
	case MAIN_GAME_PAUSED:
		pause_screen.confirm_selection();
		break;
	}
}

void MainGameScreen::select()
{
	switch (get_game_mode()) {
	case MAIN_GAME_INVENTORY:
		inventory_screen.select();
		break;
	}
}

void MainGameScreen::process_mouse_click_left(const int x, const int y)
{
	switch (get_game_mode()) {
	case MAIN_GAME_PAUSED:
		pause_screen.process_mouse_click_left(x, y);
		break;
	}
}

void MainGameScreen::process_number_input(int num)
{
	int index = 0;
	switch (num) {
	case NUM_1:
		index = 0;
		break;
	case NUM_2:
		index = 1;
		break;
	case NUM_3:
		index = 2;
		break;
	case NUM_4:
		index = 3;
		break;
	case NUM_5:
		index = 4;
		break;
	case NUM_6:
		index = 5;
		break;
	case NUM_7:
		index = 6;
		break;
	case NUM_8:
		index = 7;
		break;
	case NUM_9:
		index = 8;
		break;
	case NUM_0:
		index = 9;
		break;
	}
	game_image_manager.get_player()->get_inventory().set_hotbar_index(index);
}

void MainGameScreen::a_button_action()
{
	//TODO
}

void MainGameScreen::b_button_action()
{
	//TODO
}

void MainGameScreen::x_button_action()
{
	Item* item = game_image_manager.get_player()->get_selected_item();
	if (item) {
		switch (get_game_mode()) {
		case TOP_DOWN:
			game_image_manager.get_player()->use_selected_item();
			break;
		}
	}
}

void MainGameScreen::y_button_action()
{
	if (game_image_manager.get_player()->get_current_action() != ACTION_NONE) return;
	else {
		switch (get_game_mode()) {
		case TOP_DOWN:
			game_image_manager.set_game_mode(MAIN_GAME_INVENTORY);
			//TODO: any action necessary regarding the inventory screen
			break;
		case MAIN_GAME_INVENTORY:
			resume_game();
			game_image_manager.set_game_mode(TOP_DOWN);
			break;
		}
	}
}

void MainGameScreen::left_bumper_action()
{
	game_image_manager.get_player()->hotbar_index_left();//get_inventory().hotbar_index_left();
}

void MainGameScreen::right_bumper_action()
{
	game_image_manager.get_player()->hotbar_index_right();//get_inventory().hotbar_index_right();
}
