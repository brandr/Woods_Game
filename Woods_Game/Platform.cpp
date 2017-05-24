#include "Platform.h"

Platform::Platform()
{
	solid = true;
}

Platform::Platform(int x, int y)
{
	//TEMP
	this->rect.x = x;
	this->rect.y = y;
	//std::string filename =
	//image_filename = "images/temp_platform.png";
	solid = true;
}


Platform::~Platform()
{
}

int Platform::get_type()
{
	return PLATFORM;
}
