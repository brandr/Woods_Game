#include "Animation.h"
#include <allegro5\allegro_font.h>

const float DEFAULT_CHAR_OFF = 240.0f;

class MenuItem
{
private:
	Animation* animation;
	ALLEGRO_BITMAP* menu_image;
	std::string menu_text;
	std::pair<float,float> position;
	std::string selection_action_key;
	std::string confirm_action_key;
	std::vector<std::string> text_options;
	int options_index = 0;
	int stored_options_index = 0;
	ALLEGRO_BITMAP* left_bracket; 
	ALLEGRO_BITMAP* right_bracket;
public:
	MenuItem();
	~MenuItem();
	void unload_content();
	void draw(ALLEGRO_DISPLAY *display, float x_off, float y_off, float width, float height, ALLEGRO_FONT *font, std::string color_str, std::string selection_style, bool selected, bool option_selected);
	void draw_selected(ALLEGRO_DISPLAY * display, float x_off, float y_off, float width, float height, ALLEGRO_FONT *font, std::string color_str, std::string selection_style, bool option_selected);
	void reset();
	void save();
	void menu_left();
	void menu_right();
	void set_text(std::string);
	std::string get_text();
	void set_image(ALLEGRO_BITMAP *image);
	void set_position(std::pair<float, float> position);
	void set_selection_action_key(std::string key);
	void set_confirm_action_key(std::string key);
	void set_text_options(std::vector<std::string> options);
	void set_options_index(int index);
	void set_selection_brackets(ALLEGRO_BITMAP* left_bracket, ALLEGRO_BITMAP* right_bracket);
	std::pair<float, float> get_text_pos(float x_off, float y_off, float width, float height);
	std::pair<float, float> get_text_dimensions(ALLEGRO_FONT *font);
	std::pair<float, float> get_left_bracket_position(float x_off, float y_off, float width, float height);
	std::pair<float, float> get_right_bracket_position(float x_off, float y_off, float width, float height);
	std::pair<float, float> get_left_bracket_dimensions();
	std::pair<float, float> get_right_bracket_dimensions();
	std::pair<float, float> get_position();
	std::string get_selection_action_key();
	std::string get_confirm_action_key();
	std::string get_selected_text_option();
	std::vector<std::string> get_text_options();
	static std::vector<int> string_to_rgb(std::string color_str);
	static std::vector<int> brightened_color(std::vector<int> color, int brightness);
	static std::vector<int> darkened_color(std::vector<int> color, int darkness);
};

