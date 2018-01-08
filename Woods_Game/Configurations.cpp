#include "Configurations.h"


//TODO: make serilizable and save
Configurations::Configurations()
{
	setClassName("Configurations");
	Register("screen_res_x", &screen_res_x);
	Register("screen_res_y", &screen_res_y);
	Register("screen_mode", &screen_mode);
}


Configurations::~Configurations()
{
}

void Configurations::set_screen_res_x(int x)
{
	this->screen_res_x = x;
}

void Configurations::set_screen_res_y(int y)
{
	this->screen_res_y = y;
}

void Configurations::set_screen_mode(std::string mode)
{
	this->screen_mode = mode;
}

std::string Configurations::get_screen_mode()
{
	return this->screen_mode.value();
}

int Configurations::get_screen_res_x()
{
	return this->screen_res_x.value();
}

int Configurations::get_screen_res_y()
{
	return this->screen_res_y.value();
}
