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

	bool is_key_pressed(ALLEGRO_EVENT ev, int key);
	bool is_key_pressed(ALLEGRO_EVENT ev, std::vector<int> keys);
	bool is_key_released(ALLEGRO_EVENT ev, int key);
	bool is_key_released(ALLEGRO_EVENT ev, std::vector<int> keys);
};

