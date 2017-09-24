#include "Menuitem.h"

// pause menu options
const static std::string SELECTION_KEY_RETURN_TO_MAIN_PAUSE = "return_to_main_pause";
const static std::string SELECTION_KEY_SELECT_OPTIONS = "select_options";
	// main pause menu
const static std::string SELECTION_KEY_RESUME_GAME = "resume_game";
const static std::string SELECTION_KEY_QUIT_GAME = "quit_game";
const static std::string SELECTION_KEY_OPEN_VIDEO_SETTINGS = "open_video_settings";
const static std::string SELECTION_KEY_OPEN_CONTROL_SETTINGS = "open_control_settings";
	// pause video settings	
const static std::string SELECTION_KEY_SET_RESOLUTION = "set_resolution";
const static std::string SELECTION_KEY_SET_SCREEN_STYLE = "set_screen_style";
	// pause controls settings
const static std::string SELECTION_KEY_OPEN_KEYBOARD_CONTROLS = "open_keyboard_controls";
const static std::string SELECTION_KEY_OPEN_CONTROLLER_CONTROLS = "open_controller_controls";
const static std::string SELECTION_KEY_RETURN_TO_MAIN_CONTROLS = "return_to_main_controls";

const int MENU_ITEM_SPACING = 30;

class MenuManager
{
private:
	std::vector<MenuItem*> menu_items;
	int item_index;
	int selected_index = -1;
	FileManager file_manager;
	std::vector<std::vector<std::string>> attributes;
	std::vector<std::vector<std::string>> contents;
	float width;
	float height;
	float position[2];
	int axis;
	std::string align;
	std::string selection_style;
	//std::vector<std::string> animation_types;
	ALLEGRO_FONT *font;
	std::string color_str;
	//std::vector<Animation*> temp_animations;
	void set_menu_items();
	void set_animations();
public:
	MenuManager();
	~MenuManager();
	virtual void load_content(std::string id);
	void unload_content();
	void update();
	void draw(ALLEGRO_DISPLAY *display, float x_off, float y_off, float width, float height);
	void menu_up();
	void menu_down();
	void menu_left();
	void menu_right();
	void reset();
	void process_mouse_click_left(float x, float y, float x_off, float y_off);
	void mouse_cursor_update(float x_pos, float y_pos, float x_off, float y_off);
	const int mouse_selected_index(float x_pos, float y_pos, float x_off, float y_off);
	const int mouse_selected_options_direction_index(float x_pos, float y_pos, 
		float x_off, float y_off, 
		float back_width, float back_height);
	bool mouse_selecting_current_option(float x_pos, float y_pos, float x_off, float y_off, float back_width, float back_height);
	void select_options();
	void cancel_option_select();
	void confirm_option_select();
	bool is_selecting_options();
	void set_item_index(const int index);
	std::vector<MenuItem*>& get_menu_items();
	std::string get_selected_action_key();
	std::string get_confirm_action_key();
};

