#include<allegro5/allegro.h>
#include<allegro5/keyboard.h>
#include<vector>

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

