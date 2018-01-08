#ifndef CONFIGURATIONS_H
#define CONFIGURATIONS_H

#include "XMLSerialization.h"
//TODO: make serializable and save an XML config file
const static std::string SCREEN_STYLE_WINDOWED = "Windowed";
const static std::string SCREEN_STYLE_WINDOWED_FULLSCREEN = "Windowed Fullscreen";
const static std::string SCREEN_STYLE_FULLSCREEN = "Fullscreen";

class Configurations: public xmls::Serializable
{

private:
	// video settings
	xmls::xInt screen_res_x;
	xmls::xInt screen_res_y;
	xmls::xString screen_mode;
public:
	Configurations();
	~Configurations();
	void set_screen_res_x(int x);
	void set_screen_res_y(int y);
	void set_screen_mode(std::string mode);
	std::string get_screen_mode();
	int get_screen_res_x();
	int get_screen_res_y();
};

#endif