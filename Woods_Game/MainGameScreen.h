#include "InventoryScreen.h"
#include "Controls.h"

#define DEFAULT_SCREEN_WIDTH 800
#define DEFAULT_SCREEN_HEIGHT 600

class Level;

class MainGameScreen :
	public GameScreen
{
private:
	
	GameImageManager game_image_manager;
	PauseScreen pause_screen;
	InventoryScreen inventory_screen;
	Controls keyboard_controls;
	Controls controller_controls;
	//UI components
	ALLEGRO_BITMAP* hotbar_box;
	ALLEGRO_BITMAP* hotbar_box_selected;
public:
	MainGameScreen();
	~MainGameScreen();
	void set_default_controls();
	virtual void set_input_map();
	virtual void load_content();
	void load_ui_content();
	virtual void unload_content();
	void load_controls();
	virtual void update();
	virtual void draw(ALLEGRO_DISPLAY* display);
	void draw_ui(ALLEGRO_DISPLAY* display);
	void draw_ui_inventory(ALLEGRO_DISPLAY* display);
	void draw_ui_main_game(ALLEGRO_DISPLAY* display);
	void draw_hotbar(ALLEGRO_DISPLAY* display);
	void draw_ui_paused(ALLEGRO_DISPLAY* display);
	virtual int get_game_mode();
	virtual int get_camera_offset_x();
	virtual int get_camera_offset_y();
	virtual void refresh();
	// controls
	virtual void pause_game();
	virtual void cancel_menu();
	virtual void resume_game();
	virtual void menu_up();
	virtual void menu_down();
	virtual void menu_left();
	virtual void menu_right();
	virtual void confirm_selection();
	virtual void select();
	virtual void process_mouse_click_left(const int x, const int y);
	virtual void process_number_input(int num);
	virtual void a_button_action();
	virtual void b_button_action();
	virtual void x_button_action();
	virtual void y_button_action();
	virtual void left_bumper_action();
	virtual void right_bumper_action();
};

