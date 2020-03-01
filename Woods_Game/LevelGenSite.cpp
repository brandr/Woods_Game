#include "LevelGenSite.h"

LevelGenSite::LevelGenSite()
{
	setClassName("LevelGenSite");
	this->Register("site_key", &site_key);
	this->Register("min_x", &min_x);
	this->Register("max_x", &max_x);
	this->Register("min_y", &min_y);
	this->Register("max_y", &max_y);
	this->Register("min_w", &min_w);
	this->Register("max_w", &max_w);
	this->Register("min_h", &min_h);
	this->Register("max_h", &max_h);	
	this->Register("level_gen_data_key", &this->gen_data_key);
	this->Register("min_dist_from_trail", &this->min_dist_from_trail);
	this->Register("location_marker", &this->location_marker);
}

LevelGenSite::~LevelGenSite()
{
}

const std::string LevelGenSite::get_site_key()
{
	return this->site_key.value();
}

const int LevelGenSite::get_min_x()
{
	return this->min_x.value();
}

const int LevelGenSite::get_max_x()
{
	return this->max_x.value();
}

const int LevelGenSite::get_min_y()
{
	return this->min_y.value();
}

const int LevelGenSite::get_max_y()
{
	return this->max_y.value();
}

const int LevelGenSite::get_min_w()
{
	return this->min_w.value();
}

const int LevelGenSite::get_max_w()
{
	return this->max_w.value();
}

const int LevelGenSite::get_min_h()
{
	return this->min_h.value();
}

const int LevelGenSite::get_max_h()
{
	return this->max_h.value();
}

const std::string LevelGenSite::get_level_gen_data_key()
{
	return this->gen_data_key.value();
}

const int LevelGenSite::get_min_dist_from_trail()
{
	return this->min_dist_from_trail.value();
}

LocationMarker * LevelGenSite::get_location_marker()
{
	return &this->location_marker;
}

SiteRect::SiteRect(LevelGenSite * site, Rect site_rect)
{
	this->site_key = site->get_level_gen_data_key();
	this->rect = site_rect;
	this->min_dist = site->get_min_dist_from_trail();
}
