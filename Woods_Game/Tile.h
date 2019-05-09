#ifndef TILE_H
#define TILE_H

#include "GameImage.h"
#include "Block.h"
#include "XMLSerialization.h"
#include "TileSet.h"
#include "ImageLoader.h"

enum TILE_DIRS { TILE_UP_LEFT = 0, TILE_UP = 1, TILE_UP_RIGHT = 2, TILE_LEFT = 3, TILE_CENTER = 4,  TILE_RIGHT = 5, TILE_DOWN_LEFT = 6, TILE_DOWN = 7, TILE_DOWN_RIGHT = 8 };

struct TileEdge : xmls::Serializable {
	TileEdge();
	TileEdge(int row, int direction, std::string tile_key);
	xmls::xInt row_index;
	xmls::xInt direction_index;
	xmls::xString tile_key;
};

class Tile:
	public GameImage, public xmls::Serializable
{
private:
	Block block;
	int sheet_row = 0;
	int edge_priority = 0;
	std::string id;
	xmls::xInt tile_pos_x;
	xmls::xInt tile_pos_y;
	xmls::xInt tile_type_index;
	xmls::xInt tile_sheet_row;
	xmls::xInt tile_sheet_col;
	xmls::Collection<TileEdge> tile_edges;
	float speed_mod = 1.0f;
public:
	Tile();
	Tile(TileSet *tileset, int tile_x, int tile_y, int tile_type, int sheet_col, int sheet_row);
	~Tile();
	static Tile *null_tile(TileSet *tileset, int tile_x, int tile_y);
	virtual void set_content(std::string image_filename, Rect* image_subsection, std::pair<int, int> position);
	virtual void unload_content();
	void reset(TileSet *tileset, Tile *t);
	void reset_for_reload();
	virtual void draw(ALLEGRO_DISPLAY*, int, int);
	void draw_block(ALLEGRO_DISPLAY*, int, int);
	void initialize_block();
	void replace_block(TileSet * tileset, int block_index, std::pair<int, int> ss_pos, std::pair<int, int> pos);
	void remove_block();
	void remove_edges();
	Rect *get_bitmap_subsection();
	Block *get_block();
	void set_tile_type_index(int index);
	void set_sheet_row(int sheet_row);
	int get_tile_type_index();
	int get_sheet_row();
	int get_sheet_col();
	void set_speed_mod(float sm);
	float get_speed_mod();
	void add_edge(int row, int dir, std::string tile_key);
	std::vector<TileEdge*> get_tile_edges();
	bool has_edges();
	void set_edge_priority(int priority);
	int get_edge_priority();
	void set_tile_pos_x(int x);
	void set_tile_pos_y(int y);
	void set_tile_sheet_col(int col);
	void set_tile_sheet_row(int row);
	int get_tile_pos_x();
	int get_tile_pos_y();
};

#endif