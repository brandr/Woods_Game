#include "FilterManager.h"

FilterManager & FilterManager::get_instance()
{
	static FilterManager manager;
	return manager;
}

FilterManager::~FilterManager()
{
}

ALLEGRO_COLOR FilterManager::time_light_filter_color(const float minutes)
{
	// (4 AM) blue, darkest -> (6 AM) orange, lighter -> (8 AM) yellow, ligthest -> orange, lighter (6 PM) -> blue, darkest (8 PM)
	int r = 0;
	int g = 0;
	int b = NIGHT_B;
	int a = NIGHT_A;
	if (minutes >= SUNRISE_START_MINUTES && minutes < SUNRISE_END_MINUTES) {
		const float m = ((minutes - SUNRISE_START_MINUTES) / (SUNRISE_END_MINUTES - SUNRISE_START_MINUTES));
		r = SUNRISE_END_R * m;
		g = SUNRISE_END_G * m;
		b = NIGHT_B - ((NIGHT_B - SUNRISE_END_B) * m);
		a = NIGHT_A - ((NIGHT_A - SUNRISE_END_A) * m);
	}
	else if (minutes >= SUNRISE_END_MINUTES && minutes < DAY_START_MINUTES) {
		const float m = ((minutes - SUNRISE_END_MINUTES) / (DAY_START_MINUTES - SUNRISE_END_MINUTES));
		r = SUNRISE_END_R + ((DAY_START_R - SUNRISE_END_R) * m);
		g = SUNRISE_END_G + ((DAY_START_G - SUNRISE_END_G) * m);
		b = SUNRISE_END_B;
		a = SUNRISE_END_A - ((SUNRISE_END_A - DAY_START_A) * m);
	}
	else if (minutes >= DAY_START_MINUTES && minutes < DAY_END_MINUTES) {
		const float m = ((minutes - DAY_START_MINUTES) / (DAY_END_MINUTES - DAY_START_MINUTES));
		r = DAY_START_R;
		g = DAY_START_G;
		b = 0;
		a = DAY_START_A - ((DAY_END_A - DAY_START_A) * m);
	}
	else if (minutes >= DAY_END_MINUTES && minutes < SUNSET_START_MINUTES) {
		const float m = ((minutes - DAY_END_MINUTES) / (SUNSET_START_MINUTES - DAY_END_MINUTES));
		r = DAY_END_R - ((DAY_END_R - SUNSET_START_R) * m);
		g = DAY_END_G - ((DAY_END_G - SUNSET_START_G) * m);
		b = 0;
		a = DAY_END_A + ((SUNSET_START_A - DAY_END_A) * m);
	}
	else if (minutes >= SUNSET_START_MINUTES && minutes < SUNSET_END_MINUTES) {
		const float m = ((minutes - SUNSET_START_MINUTES) / (SUNSET_END_MINUTES - SUNSET_START_MINUTES));
		r = SUNSET_START_R - ((SUNSET_START_R - SUNSET_END_R) * m);
		g = SUNSET_START_G - ((SUNSET_START_G - SUNSET_END_G) * m);
		b = (SUNSET_END_B - SUNSET_START_B) * m;
		a = SUNSET_START_A + ((SUNSET_END_A - SUNSET_START_A) * m);
	} else if (minutes >= SUNSET_END_MINUTES) {
		r = SUNSET_END_R;
		g = SUNSET_END_G;
		b = SUNSET_END_B;
		a = SUNSET_END_A;
	}
	return al_map_rgba(r, g, b, a);
}

ALLEGRO_BITMAP * FilterManager::time_light_filter(ALLEGRO_DISPLAY * display, const float minutes)
{
	const int width = al_get_display_width(display);
	const int height = al_get_display_height(display);
	ALLEGRO_BITMAP * filter = NULL;
	const std::string suffix = "";
	if (!ImageLoader::get_instance().keyed_image_exists(IMAGE_KEY_FILLED_RECT, width, height, suffix)) {
		filter = al_create_bitmap(width, height);
		ImageLoader::get_instance().set_keyed_image(filter, IMAGE_KEY_FILLED_RECT, width, height, suffix);
	}
	else {
		filter = ImageLoader::get_instance().get_keyed_image(IMAGE_KEY_FILLED_RECT, width, height, suffix);
	}

	al_set_target_bitmap(filter);

	ALLEGRO_COLOR color = FilterManager::get_instance().time_light_filter_color(minutes);
	al_clear_to_color(color);

	al_set_target_bitmap(al_get_backbuffer(display));
	return filter;
}
