#include "Block.h"
//TODO
class Tile:
	public GameImage
{
private:
	Block* block = NULL;
	float speed_mod = 1.0f;
	//temp
	//ALLEGRO_BITMAP *bitmap;

	//temp
public:
	Tile();
	~Tile();
	virtual void set_content(std::string image_filename, Rect* image_subsection, std::pair<int, int> position);
	virtual void unload_content();
	virtual void draw(ALLEGRO_DISPLAY*, int, int);
	//void set_bitmap(ALLEGRO_BITMAP *bitmap);	//TEMP
	Block *get_block();
	void set_block(Block *b);
	void set_speed_mod(float sm);
	float get_speed_mod();
};

