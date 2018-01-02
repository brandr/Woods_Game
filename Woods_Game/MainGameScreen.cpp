#include "MainGameScreen.h"
#include "Level.h"
#include<iostream>

//InputManager main_input;

// functions to be passed along to the level, usually to the player
void keyboard_mappable_input(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	//TODO: figure out how to check this input against our controls map
	//TODO: in some cases, use this for a default input
	//TODO: since remapping enter to another key can cause problems
	/*
	GameScreen& input_screen = screen.screen_receiving_input();
	if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER
		&& !input_screen.taking_mappable_input()) {
		if (toggle) {
			input_screen.confirm_selection();
		}
		return;
	} else {
		input_screen.call_keyboard_mappable_input(ev, toggle);
	}
	*/
	GameScreen& input_screen = screen.screen_receiving_input();
	input_screen.call_keyboard_mappable_input(ev, toggle);
}

void controller_mappable_input(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	//TODO: figure out how to check this input against our controls map
	//TODO: in some cases, use this for a default input
	GameScreen& input_screen = screen.screen_receiving_input();
	if (ev.joystick.button == XC_BUTTON_A
		&& !input_screen.taking_mappable_input()) {
		if (toggle) {
			input_screen.confirm_selection();
		}
		return;
	}
	else {
		input_screen.call_controller_mappable_input(ev, toggle);
	}
}

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

void use_item(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.use_item_action();
	}
}

void open_inventory(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.open_inventory_action();
	}
}

void hotbar_left(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.hotbar_left_action();
	}
}

void hotbar_right(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.hotbar_right_action();
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

// input-sensitive actions that may be passed along

void menu_cancel(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.screen_receiving_input().cancel_menu();
	}
}

void input_menu_up(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.screen_receiving_input().menu_up();
	}
}

void input_menu_down(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.screen_receiving_input().menu_down();
	}
}

void input_menu_left(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.screen_receiving_input().menu_left();
	}
}

void input_menu_right(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.screen_receiving_input().menu_right();
	}
}

void input_confirm_selection(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.screen_receiving_input().confirm_selection();
	}
}

void input_select(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.screen_receiving_input().select();
	}
}

//TODO: consider doing this to main game screen when paused to avoid that glitch that can happen
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
	//control_map[TOP_DOWN].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_START), &pause);
	// paused controls	
	control_map[MAIN_GAME_PAUSED] = std::map<std::pair<int, int>, controlFunc>();

		// key presses
			// select
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_UP), &input_menu_up);
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_DOWN), &input_menu_down);
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_LEFT), &input_menu_left);
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_RIGHT), &input_menu_right);
		// confirm/cancel selection
	//control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_ENTER), &input_confirm_selection);
		// mouse 
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_MOUSE_AXES, 0), &move_mouse_pos);
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_MOUSE_BUTTON_DOWN, 1), &register_mouse_click_left);

		// joystick
			// select
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_UP), &input_menu_up);
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_DOWN), &input_menu_down);
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_LEFT), &input_menu_left);
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_RIGHT), &input_menu_right);
			// confirm/cancel selection
	//control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_A), &input_confirm_selection);
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_B), &menu_cancel);

	// inventory controls
	control_map[MAIN_GAME_INVENTORY] = std::map<std::pair<int, int>, controlFunc>();
		// key presses
			// select
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_UP), &input_menu_up);
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_DOWN), &input_menu_down);
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_LEFT), &input_menu_left);
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_RIGHT), &input_menu_right);
			// resume
	//control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_ESCAPE), &resume);
		// mouse
		// joystick
			// select
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_UP), &input_menu_up);
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_DOWN), &input_menu_down);
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_LEFT), &input_menu_left);
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_RIGHT), &input_menu_right);
			// move items
	//control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_A), &input_select);
			// resume
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_B), &resume);	
}

void MainGameScreen::set_mappable_controls()
{
	// keyboard
	this->map_keyboard_control_action(TOP_DOWN, "use_item", &use_item);
	this->map_keyboard_control_action(TOP_DOWN, "open_inventory", &open_inventory);
	this->map_keyboard_control_action(MAIN_GAME_INVENTORY, "open_inventory", &open_inventory);
	this->map_keyboard_control_action(MAIN_GAME_INVENTORY, "pause", &resume);
	this->map_keyboard_control_action(MAIN_GAME_PAUSED, "pause", &resume);
	this->map_keyboard_control_action(TOP_DOWN, "pause", &pause);
	this->map_keyboard_control_action(TOP_DOWN, "hotbar_left", &hotbar_left);	
	this->map_keyboard_control_action(TOP_DOWN, "hotbar_right", &hotbar_right);
	this->map_keyboard_control_action(MAIN_GAME_PAUSED, "menu_select", &input_confirm_selection);
	this->map_keyboard_control_action(MAIN_GAME_INVENTORY, "menu_select", &input_select);
	this->map_keyboard_control_action(MAIN_GAME_PAUSED, "menu_cancel", &menu_cancel);
	this->map_keyboard_control_action(MAIN_GAME_INVENTORY, "menu_cancel", &resume);
	// controller
	this->map_controller_control_action(TOP_DOWN, "use_item", &use_item);
	this->map_controller_control_action(TOP_DOWN, "open_inventory", &open_inventory);
	this->map_controller_control_action(MAIN_GAME_INVENTORY, "open_inventory", &open_inventory);
	this->map_controller_control_action(MAIN_GAME_INVENTORY, "pause", &resume);
	this->map_controller_control_action(MAIN_GAME_PAUSED, "pause", &resume);
	this->map_controller_control_action(TOP_DOWN, "pause", &pause);
	this->map_controller_control_action(TOP_DOWN, "hotbar_left", &hotbar_left);
	this->map_controller_control_action(TOP_DOWN, "hotbar_right", &hotbar_right);
	this->map_controller_control_action(MAIN_GAME_PAUSED, "menu_select", &input_confirm_selection);
	this->map_controller_control_action(MAIN_GAME_INVENTORY, "menu_select", &input_select);
	this->map_controller_control_action(MAIN_GAME_PAUSED, "menu_cancel", &menu_cancel);
	this->map_controller_control_action(MAIN_GAME_INVENTORY, "menu_cancel", &resume);
}

void MainGameScreen::unset_mappable_controls()
{
	//TODO: refactor this so we aren't copying the map method
	// keyboard
	this->unmap_keyboard_control_action(TOP_DOWN, "use_item");
	this->unmap_keyboard_control_action(TOP_DOWN, "open_inventory");
	this->unmap_keyboard_control_action(MAIN_GAME_INVENTORY, "open_inventory");
	this->unmap_keyboard_control_action(MAIN_GAME_INVENTORY, "pause");
	this->unmap_keyboard_control_action(MAIN_GAME_PAUSED, "pause");
	this->unmap_keyboard_control_action(TOP_DOWN, "pause");
	this->unmap_keyboard_control_action(TOP_DOWN, "hotbar_left");
	this->unmap_keyboard_control_action(TOP_DOWN, "hotbar_right");
	this->unmap_keyboard_control_action(MAIN_GAME_PAUSED, "menu_select");
	this->unmap_keyboard_control_action(MAIN_GAME_INVENTORY, "menu_select");
	this->unmap_keyboard_control_action(MAIN_GAME_PAUSED, "menu_cancel");
	this->unmap_keyboard_control_action(MAIN_GAME_INVENTORY, "menu_cancel");
	// controller
	this->unmap_controller_control_action(TOP_DOWN, "use_item");
	this->unmap_controller_control_action(TOP_DOWN, "open_inventory");
	this->unmap_controller_control_action(MAIN_GAME_INVENTORY, "open_inventory");
	this->unmap_controller_control_action(MAIN_GAME_INVENTORY, "pause");
	this->unmap_controller_control_action(MAIN_GAME_PAUSED, "pause");
	this->unmap_controller_control_action(TOP_DOWN, "pause");
	this->unmap_controller_control_action(TOP_DOWN, "hotbar_left");
	this->unmap_controller_control_action(TOP_DOWN, "hotbar_right");
	this->unmap_controller_control_action(MAIN_GAME_PAUSED, "menu_select");
	this->unmap_controller_control_action(MAIN_GAME_INVENTORY, "menu_select");
	this->unmap_controller_control_action(MAIN_GAME_PAUSED, "menu_cancel");
	this->unmap_controller_control_action(MAIN_GAME_INVENTORY, "menu_cancel");
}

void MainGameScreen::set_mappable_input_list()
{
	std::vector<int> keyboard_mappable_nums = Controls::keyboard_mappable_nums();
	// set remappable controls
	for (int keycode : keyboard_mappable_nums) {
		control_map[TAKING_MAPPABLE_INPUT].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, keycode), &keyboard_mappable_input);
	}
	std::vector<int> controller_mappable_nums = Controls::controller_mappable_nums();
	for (int keycode : controller_mappable_nums) {
		control_map[TAKING_MAPPABLE_INPUT].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, keycode), &controller_mappable_input);
	}
}

void MainGameScreen::map_keyboard_control_action(int game_mode, std::string action_key, controlFunc action_func)
{
	const int input_num = this->keyboard_controls.keyboard_input_num_for_action_key(action_key);
	control_map[game_mode].emplace(std::pair<int,int>(ALLEGRO_EVENT_KEY_DOWN, input_num), action_func);
}

void MainGameScreen::map_controller_control_action(int game_mode, std::string action_key, controlFunc action_func)
{
	const int input_num = this->controller_controls.controller_input_num_for_action_key(action_key);
	control_map[game_mode].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, input_num), action_func);	
}

void MainGameScreen::unmap_keyboard_control_action(int game_mode, std::string action_key)
{
	const int input_num = this->keyboard_controls.keyboard_input_num_for_action_key(action_key);
	auto it = control_map[game_mode].find(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, input_num));
	if (it == control_map[game_mode].end()) return;
	control_map[game_mode].erase(it);
}

void MainGameScreen::unmap_controller_control_action(int game_mode, std::string action_key)
{
	const int input_num = this->controller_controls.controller_input_num_for_action_key(action_key);
	auto it = control_map[game_mode].find(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, input_num));
	if (it == control_map[game_mode].end()) return;
	control_map[game_mode].erase(it);
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
	set_default_controls();
	set_mappable_controls();
	set_mappable_input_list();
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
	file_manager.load_xml_content(&this->keyboard_controls, "resources/load/controls",
		"SerializableClass", "ControlsKey", "current_controls_keyboard");
	file_manager.load_xml_content(&this->controller_controls, "resources/load/controls",
		"SerializableClass", "ControlsKey", "current_controls_controller");
}

void MainGameScreen::reset_controls()
{
	this->unset_mappable_controls();
	this->keyboard_controls.clear();
	this->controller_controls.clear();
	this->load_controls();
	this->set_mappable_controls();
	this->set_mappable_input_list();
}

void MainGameScreen::update()
{
	const int game_mode = game_image_manager.get_game_mode();
	switch (game_mode) {
		case TOP_DOWN:
			game_image_manager.update(input_map, joystick_pos_map);
			break;
		case MAIN_GAME_PAUSED:
			pause_screen_update();
			break;
		case TAKING_MAPPABLE_INPUT:
			pause_screen_update();
			break;
	}
	
	GameScreen::update();
}

void MainGameScreen::pause_screen_update()
{
	pause_screen.set_mouse_position(mouse_pos);
	pause_screen.update();
	if (pause_screen.get_screen_flag() == FLAG_RESUME_GAME) {
		resume_game();
	} else if (pause_screen.get_screen_flag() == FLAG_QUIT_GAME) {
		quit_game();
	} else if (pause_screen.taking_mappable_input()) {
		game_image_manager.set_game_mode(TAKING_MAPPABLE_INPUT);
	} else {
		game_image_manager.set_game_mode(MAIN_GAME_PAUSED);
	}
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
	case TAKING_MAPPABLE_INPUT:
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

GameScreen & MainGameScreen::screen_receiving_input()
{
	switch (get_game_mode()) {
		case MAIN_GAME_PAUSED:
			return pause_screen;
		case TAKING_MAPPABLE_INPUT:
			return pause_screen;
		case MAIN_GAME_INVENTORY:
			return inventory_screen;
		
		default:
			return *this;
	}
}

void MainGameScreen::load_keyboard_input(std::string action_key)
{
	//TODO
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
	reset_controls();
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
	/*
	Item* item = game_image_manager.get_player()->get_selected_item();
	if (item) {
		switch (get_game_mode()) {
		case TOP_DOWN:
			game_image_manager.get_player()->use_selected_item();
			break;
		}
	}
	*/
}

void MainGameScreen::y_button_action()
{
	/*
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
	*/
}

void MainGameScreen::left_bumper_action()
{
	//game_image_manager.get_player()->hotbar_index_left();
}

void MainGameScreen::right_bumper_action()
{
	//game_image_manager.get_player()->hotbar_index_right();
}

void MainGameScreen::open_inventory_action()
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

void MainGameScreen::hotbar_left_action()
{
	game_image_manager.get_player()->hotbar_index_left();
}

void MainGameScreen::hotbar_right_action()
{
	game_image_manager.get_player()->hotbar_index_right();
}

void MainGameScreen::use_item_action()
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

void MainGameScreen::call_keyboard_mappable_input(ALLEGRO_EVENT ev, bool toggle)
{
	switch (get_game_mode()) {
	case MAIN_GAME_PAUSED:
		pause_screen.call_keyboard_mappable_input(ev, toggle);
		break;
	}
	//TODO: if not paused, use whatever action the key is mapped to, if any
}
