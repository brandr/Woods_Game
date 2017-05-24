#include "Level.h"
#include "GameImageManager.h"

Level::Level()
{
	//TEMP
	width = 800;
	height = 600;
}

Level::Level(std::string filename, std::string id,  int grid_x, int grid_y, int grid_width, int grid_height)
{
	this->filename = filename;
	this->id = id;
	this->grid_x = grid_x;
	this->grid_y = grid_y;
	this->width = STANDARD_LEVEL_GRID_WIDTH * grid_width;
	this->height = STANDARD_LEVEL_GRID_HEIGHT * grid_height;
}

Level::Level(int grid_x, int grid_y, int grid_width, int grid_height)		
{
	this->grid_x = grid_x;
	this->grid_y = grid_y;
	this->width = STANDARD_LEVEL_GRID_WIDTH * grid_width;
	this->height = STANDARD_LEVEL_GRID_HEIGHT * grid_height;
}


Level::~Level()
{

}

void Level::update(std::map<int, bool> input_map)
{
	
}

// checks to see whether a space is passable
bool Level::passable_at(int x, int y)
{
	if (x < 0 || x > width || y < 0 || y > width) return false;
	//TODO: check for collisions
	return true;
}

std::string Level::get_filename()
{
	return filename;
}

std::string Level::get_id()
{
	return id;
}

std::pair<int, int> Level::get_dimensions()
{
	return std::make_pair(width, height);
}

int Level::get_width()
{
	return width;
}

int Level::get_height()
{
	return height;
}

int Level::get_grid_x()
{
	return this->grid_x;
}

int Level::get_grid_y()
{
	return this->grid_y;
}

int Level::grid_width()
{
	return width/STANDARD_LEVEL_GRID_WIDTH;	//TEMP
}

int Level::grid_height()
{
	return height/STANDARD_LEVEL_GRID_HEIGHT; //TEMP
}
