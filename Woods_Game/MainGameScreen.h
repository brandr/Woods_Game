


//#include "GameScreen.h"
#include "GameImageManager.h"

#define DEFAULT_SCREEN_WIDTH 800
#define DEFAULT_SCREEN_HEIGHT 600

class Level;

class MainGameScreen :
	public GameScreen
{
private:
	
	GameImageManager game_image_manager;
	//Level current_level;
	//Dungeon* dungeon;
	//Level* current_level;
	//Player* player;
public:
	MainGameScreen();
	~MainGameScreen();
	void set_default_controls();
	virtual void set_input_map();
	virtual void load_content();
	virtual void unload_content();
	virtual void update();
	virtual void draw(ALLEGRO_DISPLAY* display);
	//void set_world(World);
	//void set_current_level(int, int);
	//Level* get_current_level();
	//std::list<GameImage> get_images();
	virtual int get_camera_offset_x();
	virtual int get_camera_offset_y();
	//std::pair<int, int> get_current_level_dimensions();
	//virtual void load_images(ImageLoader&);
	virtual void refresh();
	//void change_player_level();
};

