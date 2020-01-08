#include "Configurations.h"


//TODO: make serilizable and save
Configurations::Configurations()
{
	setClassName("Configurations");
	Register("screen_res_x", &screen_res_x);
	Register("screen_res_y", &screen_res_y);
	Register("screen_mode", &screen_mode);
	Register("master_volume", &master_volume);
	Register("music_volume", &music_volume);
	Register("sfx_volume", &sfx_volume);
	Register("text_scroll_speed", &text_scroll_speed);
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

void Configurations::set_master_volume(const float value)
{
	this->master_volume = value;
}

void Configurations::set_music_volume(const float value)
{
	this->music_volume = value;
}

void Configurations::set_sfx_volume(const float value)
{
	this->sfx_volume = value;
}

const float Configurations::get_master_volume()
{
	return this->master_volume.value();
}

const float Configurations::get_music_volume()
{
	return this->music_volume.value();
}

const float Configurations::get_sfx_volume()
{
	return this->sfx_volume.value();
}

const int Configurations::get_text_scroll_speed()
{
	return this->text_scroll_speed.value();
}

void Configurations::set_text_scroll_speed(const int value)
{
	this->text_scroll_speed = value;
}
