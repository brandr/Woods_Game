#ifndef LEVEL_GEN_SITE_H
#define LEVEL_GEN_SITE_H

#include "Animation.h"
#include "LocationMarker.h"
#include "XMLSerialization.h"

class LevelGenSite : public xmls::Serializable {
private:
	xmls::xString site_key;
	xmls::xInt min_x, max_x, min_y, max_y;
	xmls::xInt min_w, max_w, min_h, max_h;
	xmls::xString gen_data_key;
	xmls::xInt min_dist_from_trail;
	LocationMarker location_marker;
public:
	LevelGenSite();
	~LevelGenSite();
	const std::string get_site_key();
	const int get_min_x();
	const int get_max_x();
	const int get_min_y();
	const int get_max_y();
	const int get_min_w();
	const int get_max_w();
	const int get_min_h();
	const int get_max_h();
	const std::string get_level_gen_data_key();
	const int get_min_dist_from_trail();
	LocationMarker * get_location_marker();
};

struct SiteRect {
	std::string site_key;
	Rect rect;
	int min_dist;
	SiteRect(LevelGenSite * site, Rect rect);
};

#endif