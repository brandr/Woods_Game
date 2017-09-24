#include "Block.h"

enum TILE_DIRS { TILE_UP_LEFT = 0, TILE_UP = 1, TILE_UP_RIGHT = 2, TILE_LEFT = 3, TILE_CENTER = 4,  TILE_RIGHT = 5, TILE_DOWN_LEFT = 6, TILE_DOWN = 7, TILE_DOWN_RIGHT = 8 };

class Tile:
	public GameImage
{
private:
	Block* block = NULL;
	int sheet_row = 0;
	float speed_mod = 1.0f;
	int edge_priority = 0;
	//temp
	//ALLEGRO_BITMAP *bitmap;

	//temp
public:
	Tile();
	~Tile();
	virtual void set_content(std::string image_filename, Rect* image_subsection, std::pair<int, int> position);
	virtual void unload_content();
	virtual void draw(ALLEGRO_DISPLAY*, int, int);
	Block *get_block();
	void set_block(Block *b);
	void set_sheet_row(int sheet_row);
	int get_sheet_row();
	void set_speed_mod(float sm);
	float get_speed_mod();
	void set_edge_priority(int priority);
	int get_edge_priority();
};

