#include<allegro5/allegro.h>
#include<allegro5/keyboard.h>
#include<vector>

//copied from example at https://github.com/dradtke/Allegro-360/blob/master/src/xc.h
// TODO: figure out a more general way to determine controller inputs
enum XC_BUTTONS {
	XC_BUTTON_A = 0,
	XC_BUTTON_B = 1,
	XC_BUTTON_X = 2,
	XC_BUTTON_Y = 3,
	XC_BUTTON_RIGHT_SHOULDER = 4,
	XC_BUTTON_LEFT_SHOULDER = 5,
	XC_BUTTON_RIGHT_STICK = 6,
	XC_BUTTON_LEFT_STICK = 7,
	XC_BUTTON_BACK = 8,
	XC_BUTTON_START = 9,
	XC_BUTTON_PAD_RIGHT = 10,
	XC_BUTTON_PAD_LEFT = 11,
	XC_BUTTON_PAD_DOWN = 12,
	XC_BUTTON_PAD_UP = 13
	
};
/*
typedef struct XC_STATE {
	// axis values
	float left_stick_x;
	float left_stick_y;
	float left_trigger;
	float right_stick_x;
	float right_stick_y;
	float right_trigger;
	float dpad_x;
	float dpad_y;
	// buttons pressed
	bool button_a;
	bool button_b;
	bool button_x;
	bool button_y;
	bool button_left_stick;
	bool button_right_stick;
	bool button_left_shoulder;
	bool button_right_shoulder;
	bool button_start;
	bool button_back;
	bool button_xbox;
	// reference to the underlying joystick object
	ALLEGRO_JOYSTICK *joy;
}*/

class InputManager
{
private:
	ALLEGRO_KEYBOARD_STATE key_state;
public:
	InputManager();
	~InputManager();

	//key presses
	bool is_key_pressed(ALLEGRO_EVENT ev, int key);
	bool is_key_pressed(ALLEGRO_EVENT ev, std::vector<int> keys);	

	//key releases
	bool is_key_released(ALLEGRO_EVENT ev, int key);
	bool is_key_released(ALLEGRO_EVENT ev, std::vector<int> keys);

	//other inputs
	bool is_input_pressed(ALLEGRO_EVENT ev, int evtype, std::vector<int> keys);
	bool is_input_pressed(ALLEGRO_EVENT ev, int evtype, int key);
	bool is_input_released(ALLEGRO_EVENT ev, int evtype, int key);
};

