#include "MainGameScreen.h"
#include "Level.h"
#include<iostream>

// functions to be passed along to the level, usually to the player
void keyboard_mappable_input(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	GameScreen& input_screen = screen.screen_receiving_input();
	input_screen.call_keyboard_mappable_input(ev, toggle);
}

void controller_mappable_input(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	GameScreen& input_screen = screen.screen_receiving_input();
	input_screen.call_controller_mappable_input(ev, toggle);
}

void cancel_mappable_input_keyboard(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	GameScreen& input_screen = screen.screen_receiving_input();
	input_screen.stop_taking_mappable_input_keyboard();
}

void cancel_mappable_input_controller(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	GameScreen& input_screen = screen.screen_receiving_input();
	input_screen.stop_taking_mappable_input_controller();
}

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

void interact(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.interact_action();
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

void toggle_run(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.toggle_run_action();
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

void advance_dialog(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.dialog_advance();
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

void input_tab_left(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.screen_receiving_input().tab_left();
	}
}

void input_tab_right(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.screen_receiving_input().tab_right();
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

void input_secondary_select(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.screen_receiving_input().secondary_select();
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
		screen.update_mouse_position(ev.mouse.x, ev.mouse.y, ev.mouse.z);
	}
}

void register_mouse_click_left(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.process_mouse_click_left(ev.mouse.x, ev.mouse.y);
	}
}

void register_mouse_click_right(GameScreen& screen, ALLEGRO_EVENT ev, bool toggle) {
	if (toggle) {
		screen.process_mouse_click_right(ev.mouse.x, ev.mouse.y);
	}
}

ALLEGRO_BITMAP * MainGameScreen::hotbar_box()
{
	return ImageLoader::get_instance().get_image("ui/item_box_1");
}

ALLEGRO_BITMAP * MainGameScreen::hotbar_box_selected()
{
	return ImageLoader::get_instance().get_image("ui/item_box_1_light");
}

ALLEGRO_BITMAP * MainGameScreen::dialog_backdrop_full_width()
{
	return ImageLoader::get_instance().get_image("ui/dialog_backdrop_full_width");
}

ALLEGRO_BITMAP * MainGameScreen::dialog_backdrop_partial_width()
{
	return ImageLoader::get_instance().get_image("ui/dialog_backdrop_partial_width");
}

ALLEGRO_BITMAP * MainGameScreen::dialog_portrait_backdrop()
{
	return ImageLoader::get_instance().get_image("ui/dialog_portrait_backdrop");
}

ALLEGRO_BITMAP * MainGameScreen::clock_backdrop()
{
	return ImageLoader::get_instance().get_image("ui/time_date_backdrop");
}

ALLEGRO_BITMAP * MainGameScreen::option_arrow()
{
	return ImageLoader::get_instance().get_image("ui/arrows/ui_arrow");
}

ALLEGRO_BITMAP * MainGameScreen::stamina_meter_top_full()
{
	return ImageLoader::get_instance().get_image("ui/stamina_meter_top_full");
}

ALLEGRO_BITMAP * MainGameScreen::stamina_meter_top_empty()
{
	return ImageLoader::get_instance().get_image("ui/stamina_meter_top_empty");
}

ALLEGRO_BITMAP * MainGameScreen::stamina_meter_middle_full()
{
	return ImageLoader::get_instance().get_image("ui/stamina_meter_middle_full");
}

ALLEGRO_BITMAP * MainGameScreen::stamina_meter_middle_empty()
{
	return ImageLoader::get_instance().get_image("ui/stamina_meter_middle_empty");
}

ALLEGRO_BITMAP * MainGameScreen::stamina_meter_bottom()
{
	return ImageLoader::get_instance().get_image("ui/stamina_meter_bottom");
}

void MainGameScreen::dialog_process_mouse_click_left(const int mouse_x, const int mouse_y)
{
	Dialog *dialog = game_image_manager.get_player()->get_open_dialog();
	if (dialog != NULL) {
		const int width = al_get_bitmap_width(dialog_backdrop_full_width());
		const int height = al_get_bitmap_height(dialog_backdrop_full_width());
		int x = (al_get_display_width(al_get_current_display()) - width) / 2;
		const int y = 10;
		if (!dialog->current_page_has_options() || (mouse_x >= x && mouse_y >= y && mouse_x < x + width && mouse_y < y + height)) {
			dialog->process_mouse_click_left(font_map[FONT_DIALOG], mouse_x, mouse_y, x, y);
			this->dialog_advance();
		}
	}
}

void MainGameScreen::dialog_mouse_cursor_update(const int mouse_x, const int mouse_y)
{
	Dialog *dialog = game_image_manager.get_player()->get_open_dialog();
	if (dialog != NULL) {
		const int width = al_get_bitmap_width(dialog_backdrop_full_width());
		const int height = al_get_bitmap_height(dialog_backdrop_full_width());
		int x = (al_get_display_width(al_get_current_display()) - width) / 2;
		const int y = 10;
		if (mouse_x >= x && mouse_y >= y && mouse_x < x + width && mouse_y < y + height) {
			if (dialog->current_page_has_options()) {
				dialog->mouse_cursor_update(font_map[FONT_DIALOG], mouse_x, mouse_y, x, y);
			}
		}
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
		// keyboard
			// movement
				// arrow keys
	//TODO: maybe these should be removed since WASD is already default
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
	control_map[TOP_DOWN].emplace(std::pair<int, int>(ALLEGRO_EVENT_MOUSE_AXES, 0), &move_mouse_pos);
	control_map[TOP_DOWN].emplace(std::pair<int, int>(ALLEGRO_EVENT_MOUSE_BUTTON_DOWN, 1), &register_mouse_click_left);
	control_map[TOP_DOWN].emplace(std::pair<int, int>(ALLEGRO_EVENT_MOUSE_BUTTON_DOWN, 2), &register_mouse_click_right);

		// controller
			// movement
	control_map[TOP_DOWN].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_AXIS, 0), &move_joystick_axis);

	// paused controls	
	control_map[MAIN_GAME_PAUSED] = std::map<std::pair<int, int>, controlFunc>();

		// keyboard
			// select
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_UP), &input_menu_up);
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_DOWN), &input_menu_down);
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_LEFT), &input_menu_left);
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_RIGHT), &input_menu_right);
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_W), &input_menu_up);
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_S), &input_menu_down);
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_A), &input_menu_left);
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_D), &input_menu_right);
		// confirm/cancel selection
		// mouse 
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_MOUSE_AXES, 0), &move_mouse_pos);
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_MOUSE_BUTTON_DOWN, 1), &register_mouse_click_left);

		// controller
			// select
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_UP), &input_menu_up);
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_DOWN), &input_menu_down);
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_LEFT), &input_menu_left);
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_RIGHT), &input_menu_right);
			// confirm/cancel selection
	control_map[MAIN_GAME_PAUSED].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_B), &menu_cancel);

	// inventory controls
	control_map[MAIN_GAME_INVENTORY] = std::map<std::pair<int, int>, controlFunc>();
		// keyboard
			// select
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_UP), &input_menu_up);
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_DOWN), &input_menu_down);
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_LEFT), &input_menu_left);
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_RIGHT), &input_menu_right);
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_W), &input_menu_up);
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_S), &input_menu_down);
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_A), &input_menu_left);
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_D), &input_menu_right);

	control_map[EXCHANGE_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_UP), &input_menu_up);
	control_map[EXCHANGE_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_DOWN), &input_menu_down);
	control_map[EXCHANGE_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_LEFT), &input_menu_left);
	control_map[EXCHANGE_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_RIGHT), &input_menu_right);
	control_map[EXCHANGE_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_W), &input_menu_up);
	control_map[EXCHANGE_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_S), &input_menu_down);
	control_map[EXCHANGE_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_A), &input_menu_left);
	control_map[EXCHANGE_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_D), &input_menu_right);

	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_Q), &input_tab_left);
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_E), &input_tab_right);

		// secondary select
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_X), &input_secondary_select);
		// resume
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_ESCAPE), &resume);
	control_map[EXCHANGE_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_ESCAPE), &resume);

	// mouse 
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_MOUSE_AXES, 0), &move_mouse_pos);
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_MOUSE_BUTTON_DOWN, 1), &register_mouse_click_left);
	control_map[EXCHANGE_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_MOUSE_AXES, 0), &move_mouse_pos);
	control_map[EXCHANGE_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_MOUSE_BUTTON_DOWN, 1), &register_mouse_click_left);

		// controller
			// select
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_UP), &input_menu_up);
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_DOWN), &input_menu_down);
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_LEFT), &input_menu_left);
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_RIGHT), &input_menu_right);
	control_map[EXCHANGE_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_UP), &input_menu_up);
	control_map[EXCHANGE_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_DOWN), &input_menu_down);
	control_map[EXCHANGE_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_LEFT), &input_menu_left);
	control_map[EXCHANGE_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_RIGHT), &input_menu_right);

	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_LEFT_SHOULDER), &input_tab_left);
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_RIGHT_SHOULDER), &input_tab_right);
			// secondary select
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_Y), &input_secondary_select);
			// resume
	control_map[MAIN_GAME_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_B), &resume);	
	control_map[EXCHANGE_INVENTORY].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_B), &resume);

	// dialog controls
		// keyboard
	control_map[MAIN_GAME_DIALOG].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_UP), &input_menu_up);
	control_map[MAIN_GAME_DIALOG].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_DOWN), &input_menu_down);
	control_map[MAIN_GAME_DIALOG].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_LEFT), &input_menu_left);
	control_map[MAIN_GAME_DIALOG].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_RIGHT), &input_menu_right);
	control_map[MAIN_GAME_DIALOG].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_W), &input_menu_up);
	control_map[MAIN_GAME_DIALOG].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_S), &input_menu_down);
	control_map[MAIN_GAME_DIALOG].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_A), &input_menu_left);
	control_map[MAIN_GAME_DIALOG].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_D), &input_menu_right);
		// controller
	control_map[MAIN_GAME_DIALOG].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_UP), &input_menu_up);
	control_map[MAIN_GAME_DIALOG].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_DOWN), &input_menu_down);
	control_map[MAIN_GAME_DIALOG].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_LEFT), &input_menu_left);
	control_map[MAIN_GAME_DIALOG].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_RIGHT), &input_menu_right);
	// mouse 
	control_map[MAIN_GAME_DIALOG].emplace(std::pair<int, int>(ALLEGRO_EVENT_MOUSE_AXES, 0), &move_mouse_pos);
	control_map[MAIN_GAME_DIALOG].emplace(std::pair<int, int>(ALLEGRO_EVENT_MOUSE_BUTTON_DOWN, 1), &register_mouse_click_left);

	// calendar controls
	// keyboard
	control_map[CALENDAR].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_UP), &input_menu_up);
	control_map[CALENDAR].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_DOWN), &input_menu_down);
	control_map[CALENDAR].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_LEFT), &input_menu_left);
	control_map[CALENDAR].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_RIGHT), &input_menu_right);
	control_map[CALENDAR].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_W), &input_menu_up);
	control_map[CALENDAR].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_S), &input_menu_down);
	control_map[CALENDAR].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_A), &input_menu_left);
	control_map[CALENDAR].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_D), &input_menu_right);
	// resume
	control_map[CALENDAR].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_ESCAPE), &resume);
	// mouse 
	control_map[CALENDAR].emplace(std::pair<int, int>(ALLEGRO_EVENT_MOUSE_AXES, 0), &move_mouse_pos);
	control_map[CALENDAR].emplace(std::pair<int, int>(ALLEGRO_EVENT_MOUSE_BUTTON_DOWN, 1), &register_mouse_click_left);
	// controller
	control_map[CALENDAR].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_UP), &input_menu_up);
	control_map[CALENDAR].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_DOWN), &input_menu_down);
	control_map[CALENDAR].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_LEFT), &input_menu_left);
	control_map[CALENDAR].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_PAD_RIGHT), &input_menu_right);
		// resume
	control_map[CALENDAR].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_B), &resume);
}

void MainGameScreen::set_mappable_controls()
{
	// keyboard
	this->map_keyboard_control_action(TOP_DOWN, "interact", &interact);
	this->map_keyboard_control_action(TOP_DOWN, "use_item", &use_item);
	this->map_keyboard_control_action(TOP_DOWN, "open_inventory", &open_inventory);
	this->map_keyboard_control_action(TOP_DOWN, "toggle_run", &toggle_run);
	this->map_keyboard_control_action(TOP_DOWN, "move_up", &move_up);
	this->map_keyboard_control_action(TOP_DOWN, "move_down", &move_down);
	this->map_keyboard_control_action(TOP_DOWN, "move_left", &move_left);
	this->map_keyboard_control_action(TOP_DOWN, "move_right", &move_right);
	this->map_keyboard_control_action(MAIN_GAME_INVENTORY, "open_inventory", &open_inventory);
	this->map_keyboard_control_action(EXCHANGE_INVENTORY, "open_inventory", &open_inventory);
	this->map_keyboard_control_action(MAIN_GAME_INVENTORY, "pause", &resume);
	this->map_keyboard_control_action(EXCHANGE_INVENTORY, "open_inventory", &open_inventory);
	this->map_keyboard_control_action(MAIN_GAME_PAUSED, "pause", &resume);
	this->map_keyboard_control_action(TOP_DOWN, "pause", &pause);
	this->map_keyboard_control_action(TOP_DOWN, "hotbar_left", &hotbar_left);	
	this->map_keyboard_control_action(TOP_DOWN, "hotbar_right", &hotbar_right);
	this->map_keyboard_control_action(MAIN_GAME_PAUSED, "menu_select", &input_confirm_selection);
	this->map_keyboard_control_action(MAIN_GAME_INVENTORY, "menu_select", &input_select);
	this->map_keyboard_control_action(EXCHANGE_INVENTORY, "menu_select", &input_select);
	this->map_keyboard_control_action(CALENDAR, "menu_select", &input_select);
	this->map_keyboard_control_action(MAIN_GAME_PAUSED, "menu_cancel", &menu_cancel);
	this->map_keyboard_control_action(MAIN_GAME_INVENTORY, "menu_cancel", &resume);
	this->map_keyboard_control_action(EXCHANGE_INVENTORY, "menu_cancel", &resume);
	this->map_keyboard_control_action(CALENDAR, "menu_cancel", &resume);
	this->map_keyboard_control_action(MAIN_GAME_DIALOG, "dialog_advance", &advance_dialog);
	// controller
	this->map_controller_control_action(TOP_DOWN, "interact", &interact);
	this->map_controller_control_action(TOP_DOWN, "use_item", &use_item);
	this->map_controller_control_action(TOP_DOWN, "open_inventory", &open_inventory);
	this->map_controller_control_action(TOP_DOWN, "toggle_run", &toggle_run);
	this->map_controller_control_action(MAIN_GAME_INVENTORY, "open_inventory", &open_inventory);
	this->map_controller_control_action(EXCHANGE_INVENTORY, "open_inventory", &open_inventory);
	this->map_controller_control_action(MAIN_GAME_INVENTORY, "pause", &resume);
	this->map_controller_control_action(EXCHANGE_INVENTORY, "pause", &resume);
	this->map_controller_control_action(MAIN_GAME_PAUSED, "pause", &resume);
	this->map_controller_control_action(TOP_DOWN, "pause", &pause);
	this->map_controller_control_action(TOP_DOWN, "hotbar_left", &hotbar_left);
	this->map_controller_control_action(TOP_DOWN, "hotbar_right", &hotbar_right);
	this->map_controller_control_action(MAIN_GAME_PAUSED, "menu_select", &input_confirm_selection);
	this->map_controller_control_action(MAIN_GAME_INVENTORY, "menu_select", &input_select);
	this->map_controller_control_action(EXCHANGE_INVENTORY, "menu_select", &input_select);
	this->map_controller_control_action(CALENDAR, "menu_select", &input_select);
	this->map_controller_control_action(MAIN_GAME_PAUSED, "menu_cancel", &menu_cancel);
	this->map_controller_control_action(MAIN_GAME_INVENTORY, "menu_cancel", &resume);
	this->map_controller_control_action(EXCHANGE_INVENTORY, "menu_cancel", &resume);
	this->map_controller_control_action(CALENDAR, "menu_cancel", &resume);
	this->map_controller_control_action(MAIN_GAME_DIALOG, "dialog_advance", &advance_dialog);
}

void MainGameScreen::unset_mappable_controls()
{
	//TODO: refactor this so we aren't copying the map method
	// keyboard
	this->unmap_keyboard_control_action(TOP_DOWN, "interact");
	this->unmap_keyboard_control_action(TOP_DOWN, "use_item");
	this->unmap_keyboard_control_action(TOP_DOWN, "open_inventory");
	this->unmap_keyboard_control_action(TOP_DOWN, "toggle_run");
	this->unmap_keyboard_control_action(TOP_DOWN, "move_up");
	this->unmap_keyboard_control_action(TOP_DOWN, "move_down");
	this->unmap_keyboard_control_action(TOP_DOWN, "move_left");
	this->unmap_keyboard_control_action(TOP_DOWN, "move_right");
	this->unmap_keyboard_control_action(MAIN_GAME_INVENTORY, "open_inventory");
	this->unmap_keyboard_control_action(MAIN_GAME_INVENTORY, "pause");
	this->unmap_keyboard_control_action(MAIN_GAME_PAUSED, "pause");
	this->unmap_keyboard_control_action(TOP_DOWN, "pause");
	this->unmap_keyboard_control_action(TOP_DOWN, "hotbar_left");
	this->unmap_keyboard_control_action(TOP_DOWN, "hotbar_right");
	this->unmap_keyboard_control_action(MAIN_GAME_PAUSED, "menu_select");
	this->unmap_keyboard_control_action(MAIN_GAME_INVENTORY, "menu_select");
	this->unmap_keyboard_control_action(CALENDAR, "menu_select");
	this->unmap_keyboard_control_action(MAIN_GAME_PAUSED, "menu_cancel");
	this->unmap_keyboard_control_action(MAIN_GAME_INVENTORY, "menu_cancel");
	this->unmap_keyboard_control_action(EXCHANGE_INVENTORY, "menu_cancel");
	this->unmap_keyboard_control_action(CALENDAR, "menu_cancel");
	this->unmap_keyboard_control_action(MAIN_GAME_DIALOG, "dialog_advance");
	// controller
	this->unmap_controller_control_action(TOP_DOWN, "interact");
	this->unmap_controller_control_action(TOP_DOWN, "use_item");
	this->unmap_controller_control_action(TOP_DOWN, "open_inventory");
	this->unmap_controller_control_action(TOP_DOWN, "toggle_run");
	this->unmap_controller_control_action(MAIN_GAME_INVENTORY, "open_inventory");
	this->unmap_controller_control_action(MAIN_GAME_INVENTORY, "pause");
	this->unmap_controller_control_action(MAIN_GAME_PAUSED, "pause");
	this->unmap_controller_control_action(TOP_DOWN, "pause");
	this->unmap_controller_control_action(TOP_DOWN, "hotbar_left");
	this->unmap_controller_control_action(TOP_DOWN, "hotbar_right");
	this->unmap_controller_control_action(MAIN_GAME_PAUSED, "menu_select");
	this->unmap_controller_control_action(MAIN_GAME_INVENTORY, "menu_select");
	this->unmap_controller_control_action(CALENDAR, "menu_select");
	this->unmap_controller_control_action(MAIN_GAME_PAUSED, "menu_cancel");
	this->unmap_controller_control_action(MAIN_GAME_INVENTORY, "menu_cancel");
	this->unmap_controller_control_action(EXCHANGE_INVENTORY, "menu_cancel");
	this->unmap_controller_control_action(CALENDAR, "menu_cancel");
	this->unmap_controller_control_action(MAIN_GAME_DIALOG, "dialog_advance");
}

void MainGameScreen::set_mappable_input_list()
{
	const std::vector<int> keyboard_mappable_nums = Controls::keyboard_mappable_nums();
	// set remappable controls
	for (int keycode : keyboard_mappable_nums) {
		control_map[TAKING_MAPPABLE_INPUT].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, keycode), &keyboard_mappable_input);
	}
	control_map[TAKING_MAPPABLE_INPUT].emplace(std::pair<int, int>(ALLEGRO_EVENT_KEY_DOWN, ALLEGRO_KEY_ESCAPE), &cancel_mappable_input_controller);
	const std::vector<int> controller_mappable_nums = Controls::controller_mappable_nums();
	for (int keycode : controller_mappable_nums) {
		control_map[TAKING_MAPPABLE_INPUT].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, keycode), &controller_mappable_input);
	}
	//TODO: why doesn't this work? (probably have something else in that spot)
	//control_map[TAKING_MAPPABLE_INPUT].emplace(std::pair<int, int>(ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN, XC_BUTTON_B), &cancel_mappable_input_keyboard);
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

void MainGameScreen::stop_taking_mappable_input_keyboard()
{
	switch (get_game_mode()) {
	case MAIN_GAME_PAUSED:
		pause_screen.stop_taking_mappable_input_keyboard();
		break;
	}
}

void MainGameScreen::stop_taking_mappable_input_controller()
{
	switch (get_game_mode()) {
	case MAIN_GAME_PAUSED:
		pause_screen.stop_taking_mappable_input_controller();
		break;
	}
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
	pause_screen.load_content();
	inventory_screen.load_content();
	inventory_screen.set_inventory(&(game_image_manager.get_player()->get_inventory()));
	inventory_screen.set_quest_data(game_image_manager.get_quest_data());
	inventory_screen.set_active_quests(game_image_manager.get_active_quests());
	inventory_screen.set_failed_quests(game_image_manager.get_failed_quests());
	inventory_screen.set_completed_quests(game_image_manager.get_completed_quests());
	inventory_screen.set_world_key(game_image_manager.get_world_key());
	inventory_screen.set_dungeon_key(game_image_manager.get_current_dungeon_key());
	this->load_location_markers();
	inventory_screen.set_encyclopedia(game_image_manager.get_encyclopedia());
	calendar_screen.load_content();

	GlobalTime * global_time = game_image_manager.get_current_global_time();
	calendar_screen.set_global_time(global_time);
	calendar_screen.set_player(game_image_manager.get_player());
	calendar_screen.set_world(game_image_manager.get_world());
	calendar_screen.set_level(game_image_manager.current_level);
	load_controls();
	set_default_controls();
	set_mappable_controls();
	set_mappable_input_list();

	load_ui_content();
}

//TODO: loading fonts causes problems if we load async. Look for a way to convert font bitmaps to video bitmaps after loading.
void MainGameScreen::load_fonts()
{

	// this is where we load fonts for the pause screen
	this->pause_screen.load_menus();
	this->inventory_screen.load_fonts();
	this->exchange_inventory_screen.load_fonts();
	this->calendar_screen.load_fonts();

	font_map[FONT_HOTBAR] = al_load_font("resources/fonts/OpenSans-Regular.ttf", 12, NULL);
	font_map[FONT_DIALOG] = al_load_font("resources/fonts/OpenSans-Regular.ttf", DIALOG_FONT_SIZE, NULL);
	font_map[FONT_CLOCK] = al_load_font("resources/fonts/OpenSans-Regular.ttf", 24, NULL);
}

void MainGameScreen::load_ui_content()
{
	ImageLoader::get_instance().load_image("ui/item_box_1");
	ImageLoader::get_instance().load_image("ui/item_box_1_light");
	ImageLoader::get_instance().load_image("ui/dialog_backdrop_full_width");
	ImageLoader::get_instance().load_image("ui/dialog_backdrop_partial_width");
	ImageLoader::get_instance().load_image("ui/dialog_portrait_backdrop");	
	ImageLoader::get_instance().load_image("ui/time_date_backdrop");
	ImageLoader::get_instance().load_image("ui/arrows/ui_arrow");
	ImageLoader::get_instance().load_image("ui/arrows/ui_arrow_small");
	ImageLoader::get_instance().load_image("ui/stamina_meter_top_empty");
	ImageLoader::get_instance().load_image("ui/stamina_meter_top_full");
	ImageLoader::get_instance().load_image("ui/stamina_meter_middle_empty");
	ImageLoader::get_instance().load_image("ui/stamina_meter_middle_full");
	ImageLoader::get_instance().load_image("ui/stamina_meter_bottom");
}

void MainGameScreen::load_location_markers()
{
	std::set<std::pair<int, int>> explored_map = this->game_image_manager.explored_map();
	std::vector<std::pair<std::string, std::pair<int, int>>> locations;
	std::vector<LocationMarker *> location_markers = game_image_manager.get_current_dungeon_location_markers();
	const int default_level_width = this->game_image_manager.default_level_width(), default_level_height = this->game_image_manager.default_level_height();
	for (LocationMarker * lm : location_markers) {
		const std::string containing_level_key = lm->get_containing_level_key();
		Level * containing_level = this->game_image_manager.get_level_with_key(containing_level_key);
		if (containing_level != NULL) {
			const int grid_x = containing_level->get_grid_x(), grid_y = containing_level->get_grid_y();
			const int lm_off_x = lm->get_x(), lm_off_y = lm->get_y();
			const std::pair<int, int> map_pos(grid_x + lm_off_x / default_level_width, grid_y + lm_off_y / default_level_height);
			if (explored_map.find(map_pos) != explored_map.end()) {
				const std::string lm_name = lm->get_location_name();
				locations.push_back(std::pair<std::string, std::pair<int, int>>(lm_name, std::pair<int, int>(grid_x * default_level_width + lm_off_x, grid_y * default_level_height + lm_off_y)));
			}
		}
	}
	inventory_screen.set_locations_for_display(locations);
	inventory_screen.set_default_level_dimensions(default_level_width, default_level_height);
	inventory_screen.set_explored_map(explored_map);
}

void MainGameScreen::start_new_game(const std::string world_key)
{
	this->game_image_manager.start_new_game(world_key);
}

void MainGameScreen::load_game(const std::string filepath)
{
	this->game_image_manager.full_load_game_from_save(filepath);
}

void MainGameScreen::unload_content()
{
	game_image_manager.unload_content();
	pause_screen.unload_content();
	inventory_screen.unload_content();
	exchange_inventory_screen.unload_content();
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
	this->mouse_cursor_update();
	this->mouse_scroll_update();
	game_image_manager.music_update();
	const int game_mode = game_image_manager.get_game_mode();
	GlobalTime * global_time = NULL;
	World * world = NULL;
	Level * current_level = NULL;
	switch (game_mode) {
		case TOP_DOWN:
			game_image_manager.update(input_map, joystick_pos_map);
			break;
		case MAIN_GAME_PAUSED:
			pause_screen_update();
			break;
		case MAIN_GAME_DIALOG:
			dialog_update();
			break;
		case MAIN_GAME_INVENTORY:
			inventory_update();
			break;
		case EXCHANGE_INVENTORY:
			exchange_inventory_update();
			break;
		case CUTSCENE:
			cutscene_update();
			break;
		case CALENDAR:
			world = this->game_image_manager.get_world();
			current_level = this->game_image_manager.current_level;
			global_time = game_image_manager.get_current_global_time();
			this->calendar_screen.set_global_time(global_time);
			this->calendar_screen.set_level(current_level);
			calendar_update();
			break;
		case TAKING_MAPPABLE_INPUT:
			pause_screen_update();
			break;
	}
	GameScreen::update();
}

void MainGameScreen::mouse_cursor_update()
{
	GameScreen::mouse_cursor_update();
	const int width = al_get_bitmap_width(dialog_backdrop_full_width());
	int x = (al_get_display_width(al_get_current_display()) - width) / 2;
	const int y = 10;
	switch (get_game_mode()) {
	case TOP_DOWN:
		//TODO: change cursor based on highlighted interactable
		break;
	case MAIN_GAME_PAUSED:
		// already handled on pause screen
		break;
	case MAIN_GAME_INVENTORY:
		// already handled on inventory screen
		break;
	case MAIN_GAME_DIALOG:
		this->dialog_mouse_cursor_update(this->mouse_pos.first, this->mouse_pos.second);
		break;
	case CUTSCENE:
		// it turns out this is already handled by the dialog update
		break;
	case CALENDAR:		
		this->calendar_screen.mouse_cursor_update(font_map[FONT_DIALOG], this->mouse_pos.first, this->mouse_pos.second, x, y);
		break;
	case EXCHANGE_INVENTORY:
		// already handled on inventory screen
		break;
	}
}

void MainGameScreen::mouse_scroll_update()
{
	switch (get_game_mode()) {
	case TOP_DOWN:
		this->game_image_manager.mouse_scroll_update(this->mouse_scroll);
		//TODO: item select
		break;
	case MAIN_GAME_INVENTORY:
		this->inventory_screen.mouse_scroll_update(this->mouse_scroll);
		break;
	}
}


void MainGameScreen::pause_screen_update()
{
	pause_screen.set_mouse_position(mouse_pos);
	pause_screen.update();
	const bool taking_mappable_input = this->pause_screen.taking_mappable_input();
	const int pause_screen_flag = this->pause_screen.get_screen_flag();
	if (pause_screen_flag == FLAG_RESUME_GAME) {
		resume_game();
	} else if (pause_screen_flag == FLAG_QUIT_GAME) {
		quit_game();
	} else if (taking_mappable_input) {
		game_image_manager.set_game_mode(TAKING_MAPPABLE_INPUT);
	} else {
		game_image_manager.set_game_mode(MAIN_GAME_PAUSED);
	}
}

void MainGameScreen::dialog_update()
{
	Player * player = game_image_manager.get_player();
	World * world = this->game_image_manager.get_world();
	player->dialog_update(world, this->game_image_manager.current_level);
	if (player->has_active_cutscene()) {
		this->game_image_manager.cutscene_animation_update();
	}
	if (!player->has_open_dialog()) {
		if (player->has_active_cutscene()) {
			this->game_image_manager.set_game_mode(CUTSCENE);
		} else {
			this->game_image_manager.update_quests();
			this->resume_game();
		}
	}
}

void MainGameScreen::inventory_update()
{
	this->inventory_screen.set_mouse_position(mouse_pos);
	this->inventory_screen.update();
}

void MainGameScreen::exchange_inventory_update()
{	
	if (this->exchange_inventory_screen.has_inventory() && this->exchange_inventory_screen.has_exchange_inventory()) {
		this->exchange_inventory_screen.set_mouse_position(mouse_pos);
		this->exchange_inventory_screen.update();
	} else {
		Inventory& inv = this->game_image_manager.get_player()->get_inventory();
		Inventory * exchange_inv = this->game_image_manager.get_exchange_inventory();
		this->exchange_inventory_screen.set_inventory(&inv);
		if (exchange_inv == NULL) {
			this->resume_game();
		} else {
			this->exchange_inventory_screen.set_exchange_inventory(exchange_inv);
		}
	}
}

void MainGameScreen::cutscene_update()
{
	Player * player = game_image_manager.get_player();
	player->cutscene_update(game_image_manager.get_world(), game_image_manager.current_level);
	game_image_manager.process_cutscene(player->get_active_cutscene());
	game_image_manager.cutscene_animation_update();
	if (!player->has_active_cutscene()) {
		//TODO: may want to trigger a different game mode if the cutscene leads to another one
		this->resume_game();
	}
}

void MainGameScreen::calendar_update()
{
	Player * player = game_image_manager.get_player();
	if (!player->get_should_open_calendar()) {
		this->resume_game();
	}
	else {
		this->calendar_screen.set_mouse_position(mouse_pos);
		this->calendar_screen.update();
	}
}

void MainGameScreen::draw(ALLEGRO_DISPLAY * display)
{
	game_image_manager.draw(display);
	draw_ui(display);
	GameScreen::draw(display);
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
	case EXCHANGE_INVENTORY:
		draw_ui_exchange_inventory(display);
		break;
	case CALENDAR:
		draw_ui_calendar(display);
		break;
	case TOP_DOWN:
		draw_ui_main_game(display);
		break;
	case MAIN_GAME_DIALOG:
		draw_ui_dialog(display);
		break;
	}
}

void MainGameScreen::draw_ui_inventory(ALLEGRO_DISPLAY * display)
{
	inventory_screen.draw(display);
}

void MainGameScreen::draw_ui_exchange_inventory(ALLEGRO_DISPLAY * display)
{
	if (exchange_inventory_screen.has_inventory()) {
		exchange_inventory_screen.draw(display);
	}
}

void MainGameScreen::draw_ui_calendar(ALLEGRO_DISPLAY * display)
{
	calendar_screen.draw(display);
}

void MainGameScreen::draw_ui_main_game(ALLEGRO_DISPLAY * display)
{
	draw_hotbar(display);
	draw_clock(display);
	draw_stamina(display);
}

void MainGameScreen::draw_hotbar(ALLEGRO_DISPLAY * display)
{
	Inventory& inventory = game_image_manager.get_player()->get_inventory();
	const int width = al_get_display_width(display);
	const int height = al_get_display_height(display);
	const int box_width = al_get_bitmap_width(hotbar_box());
	const int box_height = al_get_bitmap_height(hotbar_box());
	const int hotbar_index = inventory.get_hotbar_index();
	const int size = HOTBAR_SIZE;
	for (int i = 0; i < size; i++) {
		const float x = (width - box_width*size)/2 + i*box_width;
		const float y = height - box_height - 12;
		const int num = i < 9 ? i + 1 : 0;
		if (i == hotbar_index) {
			al_draw_bitmap(hotbar_box_selected(), x, y, 0);
		}
		else {
			al_draw_bitmap(hotbar_box(), x, y, 0);
		}
		const std::vector<int> rgb = MenuItem::string_to_rgb(FONT_COLOR_HOTBAR);
		al_draw_text(font_map[FONT_HOTBAR], al_map_rgb(rgb[0], rgb[1], rgb[2]), x + 8.0f, y + 5.0f, 0, std::to_string(num).c_str());
		Item* item = inventory.get_hotbar_item(i);
		if (item) item->draw(display, x, y);
	}
}

void MainGameScreen::draw_clock(ALLEGRO_DISPLAY * display)
{
	const int x = al_get_display_width(display) - al_get_bitmap_width(clock_backdrop()) - 8;
	const int y = 8;
	al_draw_bitmap(clock_backdrop(), x, y, NULL);
	const std::string date_str = game_image_manager.date_display_string();
	const std::string time_str = game_image_manager.time_display_string();
	al_draw_text(font_map[FONT_CLOCK], al_map_rgb(0, 0, 0), x + 20.0f, y + 16.0f, 0, date_str.c_str());
	al_draw_text(font_map[FONT_CLOCK], al_map_rgb(0, 0, 0), x + 200.0f, y + 16.0f, 0, time_str.c_str());
}

void MainGameScreen::draw_stamina(ALLEGRO_DISPLAY * display)
{
	Player * player = game_image_manager.get_player();	
	const int max_stamina = player->get_max_stamina();
	const int current_stamina = player->get_current_stamina();
	const int total_pixels = max_stamina / STAMINA_METER_SCALE;
	const int top_h = al_get_bitmap_height(this->stamina_meter_top_full());
	const float stamina_ratio = (float)current_stamina / (float)max_stamina;
	const float top_bar_ratio = 1.0f - (float)top_h / (float) total_pixels;
	ALLEGRO_BITMAP* top_bmp = stamina_ratio >= top_bar_ratio
		? this->stamina_meter_top_full() : this->stamina_meter_top_empty();	
	const int x_off = al_get_display_width(display) - al_get_bitmap_width(top_bmp) - 8;
	const int y_off = al_get_display_height(display) - total_pixels - al_get_bitmap_height(this->stamina_meter_bottom()) - 8;
	al_draw_bitmap(top_bmp, x_off, y_off, 0);
	const int middle_h = al_get_bitmap_height(this->stamina_meter_middle_full());
	
	const int num_middle_sections = (total_pixels - top_h) / middle_h;
	for (int i = 0; i < num_middle_sections; i++) {
		const int pixel_h = total_pixels - top_h - (i * middle_h);
		const int stamina_h = pixel_h * STAMINA_METER_SCALE;
		ALLEGRO_BITMAP* middle_bmp = current_stamina >= stamina_h
			? this->stamina_meter_middle_full() : this->stamina_meter_middle_empty();
		al_draw_bitmap(middle_bmp, x_off, y_off + top_h + i * middle_h, 0);
	}
	al_draw_bitmap(this->stamina_meter_bottom(), x_off, y_off + top_h + num_middle_sections * middle_h, 0);
}

void MainGameScreen::draw_ui_paused(ALLEGRO_DISPLAY* display)
{
	pause_screen.draw(display);
}

void MainGameScreen::draw_ui_dialog(ALLEGRO_DISPLAY * display)
{
	Dialog *dialog = game_image_manager.get_player()->get_open_dialog();
	if (dialog != NULL) {
		int x = (al_get_display_width(display) - al_get_bitmap_width(dialog_backdrop_full_width())) / 2;
		const int y = 10;
		ALLEGRO_BITMAP * backdrop_bitmap = dialog->has_portrait_image() ? this->dialog_backdrop_partial_width() : this->dialog_backdrop_full_width();
		if (dialog->has_portrait_image()) {
			al_draw_bitmap(this->dialog_portrait_backdrop(), x, y, 0);
			ALLEGRO_BITMAP * portrait_image = dialog->get_portrait_image();
			al_draw_bitmap(portrait_image, x + 5, y + 5, 0);
			x += al_get_bitmap_width(this->dialog_portrait_backdrop());
		}
		al_draw_bitmap(backdrop_bitmap, x, y, 0);
		dialog->draw(display, font_map[FONT_DIALOG], x, y);
		//TODO: font, text size, etc set in some config
	}
}

void MainGameScreen::wait(const int duration)
{
	this->game_image_manager.wait(duration);
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
		case EXCHANGE_INVENTORY:
			return exchange_inventory_screen;
		case CALENDAR:
			return calendar_screen;
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
	if (get_game_mode() == MAIN_GAME_PAUSED) {
		pause_screen.cancel_menu();
	}
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
	case EXCHANGE_INVENTORY:
		//TODO: do we need to reset anything else here?
		exchange_inventory_screen.reset();
		game_image_manager.get_player()->set_exchange_inventory_key("");
		break;
	case CALENDAR:
		if (calendar_screen.showing_dialog()) {
			calendar_screen.close_dialog();
		} else {
			calendar_screen.reset();
			game_image_manager.get_player()->close_calendar();
		}
		break;
	}
	//TODO: reset other screens as necessary for the given game mode
	clear_input();
	game_image_manager.resume();
	game_image_manager.set_game_mode(TOP_DOWN);	
	game_image_manager.update_quests();
	reset_controls();
}

void MainGameScreen::dialog_advance()
{
	Player * player = this->game_image_manager.get_player();
	player->advance_dialog();
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
	case EXCHANGE_INVENTORY:
		exchange_inventory_screen.menu_up();
		break;
	case MAIN_GAME_DIALOG:
		this->game_image_manager.decrement_dialog_option();
		break;
	case CALENDAR:
		this->calendar_screen.menu_up();
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
	case EXCHANGE_INVENTORY:
		exchange_inventory_screen.menu_down();
		break;
	case MAIN_GAME_DIALOG:
		this->game_image_manager.increment_dialog_option();
		break;
	case CALENDAR:
		this->calendar_screen.menu_down();
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
	case EXCHANGE_INVENTORY:
		exchange_inventory_screen.menu_left();
		break;
	case MAIN_GAME_DIALOG:
		this->game_image_manager.decrement_dialog_option();
		break;
	case CALENDAR:
		this->calendar_screen.menu_left();
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
	case MAIN_GAME_DIALOG:
		this->game_image_manager.increment_dialog_option();
		break;
	case EXCHANGE_INVENTORY:
		exchange_inventory_screen.menu_right();
		break;
	case CALENDAR:
		this->calendar_screen.menu_right();
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
	case EXCHANGE_INVENTORY:
		exchange_inventory_screen.select();
		break;
	case CALENDAR:
		calendar_screen.select();
		break;
	}
}

void MainGameScreen::process_mouse_click_left(const int x, const int y)
{
	switch (get_game_mode()) {
	case TOP_DOWN:
		this->game_image_manager.process_mouse_click_left(x, y);
		break;
	case MAIN_GAME_PAUSED:
		this->pause_screen.process_mouse_click_left(x, y);
		break;
	case MAIN_GAME_INVENTORY:
		this->inventory_screen.process_mouse_click_left(x, y);
		break;
	case MAIN_GAME_DIALOG:
		this->dialog_process_mouse_click_left(x, y);
		break;
	case CUTSCENE:
		// it turns out cutscenes are already handled by the dialog processing
		break;
	case CALENDAR:
		this->calendar_screen.process_mouse_click_left(x, y);
		break;
	case EXCHANGE_INVENTORY:
		//TODO
		this->exchange_inventory_screen.process_mouse_click_left(x, y);
		break;
	}
}

void MainGameScreen::process_mouse_click_right(const int x, const int y)
{
	switch (get_game_mode()) {
	case TOP_DOWN:
		this->game_image_manager.process_mouse_click_right(x, y);
		//TODO: other modes
		break;
	case MAIN_GAME_PAUSED:
		//this->pause_screen.process_mouse_click_left(x, y);
		break;
	case MAIN_GAME_INVENTORY:
		//this->inventory_screen.process_mouse_click_left(x, y);
		break;
	case MAIN_GAME_DIALOG:
		//this->dialog_process_mouse_click_left(x, y);
		break;
	case CUTSCENE:
		// it turns out cutscenes are already handled by the dialog processing
		break;
	case CALENDAR:
		//this->calendar_screen.process_mouse_click_left(x, y);
		break;
	case EXCHANGE_INVENTORY:
		//TODO
		//this->exchange_inventory_screen.process_mouse_click_left(x, y);
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
}

void MainGameScreen::y_button_action()
{
}

void MainGameScreen::left_bumper_action()
{
}

void MainGameScreen::right_bumper_action()
{
}

void MainGameScreen::open_inventory_action()
{
	if (game_image_manager.get_player()->get_current_action() != ACTION_NONE) return;
	else {
		std::pair<int, int> current_location;
		switch (get_game_mode()) {
		case TOP_DOWN:
			game_image_manager.set_game_mode(MAIN_GAME_INVENTORY);
			current_location = game_image_manager.current_player_location_for_map();
			this->inventory_screen.set_inventory(&(game_image_manager.get_player()->get_inventory()));
			this->inventory_screen.set_current_location(current_location.first, current_location.second);
			this->load_location_markers();
			this->inventory_screen.set_quest_data(game_image_manager.get_quest_data());
			this->inventory_screen.set_active_quests(game_image_manager.get_active_quests());
			this->inventory_screen.set_failed_quests(game_image_manager.get_failed_quests());
			this->inventory_screen.set_completed_quests(game_image_manager.get_completed_quests());
			this->inventory_screen.set_encyclopedia(game_image_manager.get_encyclopedia());
			break;
		case MAIN_GAME_INVENTORY:
			resume_game();
			game_image_manager.set_game_mode(TOP_DOWN);
			break;
		case EXCHANGE_INVENTORY:
			resume_game();
			game_image_manager.set_game_mode(TOP_DOWN);
			break;
		}
	}
}

void MainGameScreen::toggle_run_action()
{
	Player * player = game_image_manager.get_player();
	player->toggle_run();
	//TODO: toggle serializable run setting for player (not running caps max speed)
	//should serialize in config or world (someplace that persists across saves)
	// can serialize on player since people shouldn't care
	//		I think I want some new config file like "WorldConfig"
	//TODO: need controller button for this too? (might as well but need to pick a button and add it to xml, controls loading, etc)
}

void MainGameScreen::hotbar_left_action()
{
	game_image_manager.get_player()->hotbar_index_left();
}

void MainGameScreen::hotbar_right_action()
{
	game_image_manager.get_player()->hotbar_index_right();
}

void MainGameScreen::interact_action()
{
	game_image_manager.get_player()->interact();
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
}
