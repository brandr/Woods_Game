#ifndef CONFIGURATIONS_H
#define CONFIGURATIONS_H

#include "XMLSerialization.h"
//TODO: make serializable and save an XML config file


class Configurations: public xmls::Serializable
{

private:
	// video settings
	xmls::xInt screen_res_x;
	xmls::xInt screen_res_y;
	xmls::xString screen_mode;
public:
	static constexpr const char* SCREEN_STYLE_WINDOWED = "Windowed";
	static constexpr const char* SCREEN_STYLE_WINDOWED_FULLSCREEN = "Windowed Fullscreen";
	static constexpr const char* SCREEN_STYLE_FULLSCREEN = "Fullscreen";
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