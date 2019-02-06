#include "Tile.h"



Tile::Tile()
{
	setClassName("Tile");
	Register("TilePosX", &tile_pos_x);
	Register("TilePosY", &tile_pos_y);
	Register("TileTypeIndex", &tile_type_index);
	Register("TileSheetCol", &tile_sheet_col);
	Register("TileSheetRow", &tile_sheet_row);
	Register("TileEdges", &tile_edges);
	Register("Block", &block);
}

Tile::Tile(TileSet *tileset, int tile_x, int tile_y, int type_index, int sheet_col, int sheet_row)
{
	setClassName("Tile");
	Register("TilePosX", &tile_pos_x);
	Register("TilePosY", &tile_pos_y);
	Register("TileTypeIndex", &tile_type_index);
	Register("TileSheetCol", &tile_sheet_col);
	Register("TileSheetRow", &tile_sheet_row);
	Register("TileEdges", &tile_edges);
	Register("Block", &block);
	this->tile_pos_x = tile_x;
	this->tile_pos_y = tile_y;
	this->tile_type_index = type_index;
	this->tile_sheet_col = sheet_col;
	this->tile_sheet_row = sheet_row;
	std::string filename = tileset->get_full_tile_sheet_filename(type_index);
	Rect* offset_rect = new Rect(sheet_col*TILE_SIZE, sheet_row*TILE_SIZE, TILE_SIZE, TILE_SIZE);
	std::pair<int, int> position(tile_x*TILE_SIZE, tile_y*TILE_SIZE);
	this->set_content(filename, offset_rect, position);
	this->set_edge_priority(tileset->get_edge_priority(type_index));
	this->set_speed_mod(tileset->get_tile_speed_mod(type_index));
	this->set_bitmap(ImageLoader::get_instance().get_current_image(this));
}

Tile::~Tile()
{
}

Tile * Tile::null_tile(TileSet *tileset, int tile_x, int tile_y)
{
	Tile *tile = new Tile();
	tile->set_tile_pos_x(tile_x);
	tile->set_tile_pos_y(tile_y);
	tile->set_tile_type_index(0);
	tile->set_tile_sheet_col(0);
	tile->set_tile_sheet_row(0);
	std::string filename = tileset->get_full_tile_sheet_filename(0);
	Rect* offset_rect = new Rect(0, 0, TILE_SIZE, TILE_SIZE);
	std::pair<int, int> position(tile_x*TILE_SIZE, tile_y*TILE_SIZE);
	tile->set_content(filename, offset_rect, position);
	tile->set_edge_priority(0);
	tile->set_speed_mod(1.0f);
	tile->set_bitmap(ImageLoader::get_instance().get_current_image(tile));
	return tile;
}

void Tile::set_content(std::string image_filename, Rect * image_subsection, std::pair<int, int> position)
{
	GameImage::set_content(image_filename, image_subsection, position);
	refresh_mask();
}

void Tile::unload_content()
{
	/*
	if (block) {
		block->unload_content();
		delete block;
	}
	block = NULL;
	*/
	GameImage::unload_content();
}

void Tile::reset(TileSet *tileset, Tile * t)
{
	const int tile_x = t->get_tile_pos_x();
	const int tile_y = t->get_tile_pos_y();
	const int sheet_col = t->tile_sheet_col.value();
	const int sheet_row = t->tile_sheet_row.value();
	const int type_index = t->get_tile_type_index();
	this->tile_pos_x = tile_x;
	this->tile_pos_y = tile_y;
	this->tile_type_index = type_index;
	this->tile_sheet_col = t->tile_sheet_col;
	this->tile_sheet_row = sheet_row;
	std::string filename = tileset->get_full_tile_sheet_filename(t->get_tile_type_index());
	Rect* offset_rect = new Rect(sheet_col*TILE_SIZE, sheet_row*TILE_SIZE, TILE_SIZE, TILE_SIZE);
	std::pair<int, int> position(tile_x*TILE_SIZE, tile_y*TILE_SIZE);
	this->set_content(filename, offset_rect, position);
	this->set_edge_priority(tileset->get_edge_priority(type_index));
	this->set_speed_mod(tileset->get_tile_speed_mod(type_index));
	this->set_bitmap(ImageLoader::get_instance().get_current_image(this));
}

void Tile::draw(ALLEGRO_DISPLAY *display, int x_offset, int y_offset)
{
	GameImage::draw(display, x_offset, y_offset);
}

void Tile::draw_block(ALLEGRO_DISPLAY *display, int x_offset, int y_offset)
{
	if (!block.is_empty() && block.is_visible()) {
		block.draw(display, x_offset, y_offset);
	}
}

void Tile::initialize_block()
{
	this->block.set_entity_data_index(0);
}

void Tile::replace_block(TileSet * tileset, int block_index, std::pair<int, int> ss_pos, std::pair<int, int> pos)
{
	Rect* offset_rect = new Rect(ss_pos.first*TILE_SIZE, ss_pos.second*TILE_SIZE, TILE_SIZE, TILE_SIZE);
	const std::string filename = tileset->get_full_block_sheet_filename(block_index);
	const std::pair<int, int> pixel_pos(pos.first*TILE_SIZE, pos.second*TILE_SIZE);
	std::map<std::string, int> block_attributes = tileset->get_block_attributes(block_index);
	const std::vector<std::pair<std::string, std::string>> interact_action_data 
		= tileset->get_block_interact_action_data(block_index);
	const std::vector<std::pair<std::string, std::string>> contact_action_data
		= tileset->get_block_contact_action_data(block_index);
	const bool solid = tileset->get_block_solid(block_index);
	const bool visible = tileset->get_block_visible(block_index);
	this->block.set_entity_data_index(block_index);
	this->block.set_content(filename, offset_rect, pixel_pos);
	this->block.set_starting_pos(pixel_pos.first, pixel_pos.second);
	this->block.set_entity_sheet_offset(ss_pos.first, ss_pos.second);
	this->block.set_bitmap(ImageLoader::get_instance().get_current_image(&block));
	this->block.set_solid(solid);							//will be serialized
	this->block.set_visible(visible);						//will be serialized
	this->block.set_entity_attributes(block_attributes);	//will be serialized
	this->block.set_interact_actions(interact_action_data);	//will be serialized
	this->block.set_contact_actions(contact_action_data);	//will be serialized
	this->block.load_entity_effects(filename, ss_pos.second, std::pair<int, int>(TILE_SIZE, TILE_SIZE));
	this->block.refresh_mask();
	//TODO: interact actions
}

void Tile::remove_block()
{
	//TODO: delete other stuff from block if necessary
	this->block.set_empty();
}

void Tile::remove_edges()
{
	this->tile_edges.Clear();
	//TODO: do we need to reload image?
}

Rect * Tile::get_bitmap_subsection()
{
	return new Rect(this->tile_sheet_col.value()*TILE_SIZE, this->tile_sheet_row.value()*TILE_SIZE, TILE_SIZE, TILE_SIZE);
}

Block * Tile::get_block()
{
	if (this->block.is_empty()) {
		return NULL;
	}
	return &(this->block);
}

void Tile::set_tile_type_index(int index)
{
	tile_type_index = index;
}

void Tile::set_sheet_row(int sheet_row)
{
	this->sheet_row = sheet_row;
}

int Tile::get_tile_type_index()
{
	return this->tile_type_index.value();
}

int Tile::get_sheet_row()
{
	return this->tile_sheet_row.value();
}

int Tile::get_sheet_col()
{
	return tile_sheet_col.value();
}

void Tile::set_speed_mod(float sm)
{
	speed_mod = sm;
}

float Tile::get_speed_mod()
{
	return speed_mod;
}

void Tile::add_edge(int row, int dir, std::string tile_key)
{
	TileEdge *edge = new TileEdge(row, dir, tile_key);
	this->tile_edges.addItem(edge);
}

std::vector<TileEdge*> Tile::get_tile_edges()
{
	std::vector<TileEdge*> edges;
	const int size = this->tile_edges.size();
	for (int i = 0; i < size; i++) {
		edges.push_back(this->tile_edges.getItem(i));
	}
	return edges;
}

bool Tile::has_edges()
{
	return this->tile_edges.size() > 0;
}

void Tile::set_edge_priority(int priority)
{
	this->edge_priority = priority;
}

int Tile::get_edge_priority()
{
	return edge_priority;
}

void Tile::set_tile_pos_x(int x)
{
	this->tile_pos_x = x;
}

void Tile::set_tile_pos_y(int y)
{
	this->tile_pos_y = y;
}

void Tile::set_tile_sheet_col(int col)
{
	this->tile_sheet_col = col;
}

void Tile::set_tile_sheet_row(int row)
{
	this->tile_sheet_row = row;
}

int Tile::get_tile_pos_x()
{
	return this->tile_pos_x.value();
}

int Tile::get_tile_pos_y()
{
	return this->tile_pos_y.value();
}

TileEdge::TileEdge()
{
	setClassName("TileEdge");
	//Register("Filename", &filename);
	Register("RowIndex", &row_index);
	Register("DirectionIndex", &direction_index);
	Register("TileKey", &tile_key);
}

TileEdge::TileEdge(int row, int direction, std::string key)
{
	setClassName("TileEdge");
	//Register("Filename", &filename);
	Register("RowIndex", &row_index);
	Register("DirectionIndex", &direction_index);
	Register("TileKey", &tile_key);
	//this->filename = edge_filename;
	this->row_index = row;
	this->direction_index = direction;
	this->tile_key = key;
}
