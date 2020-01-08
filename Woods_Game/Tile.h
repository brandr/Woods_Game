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
	void reset(TileSet *tileset, Tile *t, const int tile_x, const int tile_y);
	void reset(TileSet *tileset, const int tile_index);
	void reset_for_reload();
	virtual void draw(ALLEGRO_DISPLAY*, const int, const int, const int screen_w, const int screen_h);
	void draw_block(ALLEGRO_DISPLAY*, const int, const int, const int screen_w, const int screen_h);
	void replace_block(TileSet * tileset, const int block_index, std::pair<int, int> ss_pos, std::pair<int, int> pos);
	void replace_block(TileSet* tileset, Block * other_block, std::pair<int, int> ss_pos, std::pair<int, int> pos);
	void remove_block();
	void remove_edges();
	Rect *get_bitmap_subsection();
	Block *get_block();
	void set_tile_type_index(const int index);
	void set_sheet_row(const int sheet_row);
	const int get_tile_type_index();
	const int get_sheet_row();
	const int get_sheet_col();
	void add_edge(const int row, const int dir, std::string tile_key);
	std::vector<TileEdge*> get_tile_edges();
	const bool has_edges();
	void set_tile_pos_x(const int x);
	void set_tile_pos_y(const int y);
	void set_tile_sheet_col(const int col);
	void set_tile_sheet_row(const int row);
	int get_tile_pos_x();
	int get_tile_pos_y();
	const bool get_can_grow_plants();
};

#endif