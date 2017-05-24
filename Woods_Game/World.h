#include "Dungeon.h"
#include <memory>
class ImageLoader;

class World
{
private:
	//consider replacing with map
	//std::vector<Dungeon*> dungeons;
	
	//Player player;
	std::vector <std::shared_ptr<Dungeon>> dungeons;
public:
	
	Dungeon* current_dungeon;
	World();
	~World();
	void unload_content();
	void load_images(ImageLoader&);
	void add_dungeon(Dungeon*);
	Dungeon* get_current_dungeon();
	//Player* get_player();
	//Level* get_current_level();
	//void set_current_level(int, int);
	//std::pair<int, int> get_current_level_dimensions();
};

