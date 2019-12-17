#ifndef FILTER_MANAGER_H
#define FILTER_MANAGER_H

#define SUNRISE_START_MINUTES 240
#define SUNRISE_END_MINUTES 360
#define DAY_START_MINUTES 480
#define DAY_END_MINUTES 1020
#define SUNSET_START_MINUTES 1080
#define SUNSET_END_MINUTES 1200

#define NIGHT_B 80
#define NIGHT_A 140

#define SUNRISE_END_R 160
#define SUNRISE_END_G 120
#define SUNRISE_END_B 0
#define SUNRISE_END_A 70

#define DAY_START_R 200
#define DAY_START_G 200
#define DAY_START_A 40

#define DAY_END_R 200
#define DAY_END_G 200
#define DAY_END_A 40

#define SUNSET_START_R 160
#define SUNSET_START_G 120
#define SUNSET_START_B 0
#define SUNSET_START_A 70

#define SUNSET_END_R 0
#define SUNSET_END_G 0
#define SUNSET_END_B 80
#define SUNSET_END_A 140

#include "allegro5/allegro_color.h"

#include "ImageLoader.h"

class FilterManager {
private:
	FilterManager() {}
public:
	FilterManager(FilterManager const&) = delete;
	void operator=(FilterManager const&) = delete;
	static FilterManager& get_instance();
	~FilterManager();
	ALLEGRO_COLOR time_light_filter_color(const float minutes);
	ALLEGRO_BITMAP * time_light_filter(ALLEGRO_DISPLAY * display, const float minutes);
};

#endif