#ifndef MENUITEM_H
#define MENUITEM_H

#include "Animation.h"
#include <allegro5\allegro_font.h>
#include "allegro5/bitmap.h"        // for ALLEGRO_BITMAP
#include "allegro5/display.h"       // for ALLEGRO_DISPLAY
#include "utility"                  // for pair
#include "vector"                   // for vector
#include "xstring"                  // for string
#include <algorithm>
#include <memory>                   // for allocator
#include "XMLSerialization.h"  // for xString, Serializable, Collection

const float DEFAULT_CHAR_OFF = 240.0f;

struct TextOption : public xmls::Serializable {
	xmls::xString Option;
public:
	TextOption();
	~TextOption();
	TextOption(std::string option);
};

class MenuItem: public xmls::Serializable
{
private:
	Animation* animation;
	ALLEGRO_BITMAP* menu_image;
	xmls::xString menu_text;
	xmls::xString image_filename = "";
	xmls::xFloat x_pos;
	xmls::xFloat y_pos;
	std::pair<float,float> position;
	xmls::xString selection_action_key;
	xmls::xString confirm_action_key;
	//std::vector<std::string> text_options;
	xmls::Collection<TextOption> text_options;
	int options_index = 0;
	xmls::xInt stored_options_index = 0;
	xmls::xString controls_action_key = "";
	xmls::xString controls_input_key = "";
	xmls::xString controls_input_label = "";
	xmls::xString left_bracket_filename = "";
	xmls::xString right_bracket_filename = "";
	ALLEGRO_BITMAP* left_bracket; 
	ALLEGRO_BITMAP* right_bracket;
public:
	MenuItem();
	~MenuItem();
	void load_additional_content();
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
	void set_controls_action_key(std::string action_key);
	void set_controls_input_key(std::string input_key);
	void set_controls_input_label(std::string input_label);
	void set_selection_brackets(ALLEGRO_BITMAP* left_bracket, ALLEGRO_BITMAP* right_bracket);
	void set_selection_bracket_filenames(std::string left_filename, std::string right_filename);
	void set_image_filename(std::string filename);
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
	xmls::Collection<TextOption> get_text_options();
	std::string get_controls_action_key();
	std::string get_controls_input_key();
	std::string get_controls_input_label();
	static std::vector<int> string_to_rgb(std::string color_str);
	static std::vector<int> brightened_color(std::vector<int> color, int brightness);
	static std::vector<int> darkened_color(std::vector<int> color, int darkness);
};

#endif