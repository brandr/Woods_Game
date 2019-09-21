#ifndef LOCATION_MARKER_H
#define LOCATION_MARKER_H

#include "Entity.h"

#include "XMLSerialization.h"

struct LocationMarkerLevel : public xmls::Serializable
{
	xmls::xString level_key;
	LocationMarkerLevel();
	~LocationMarkerLevel();
};

class LocationMarker : public Entity, public xmls::Serializable
{
private:
	xmls::xString location_name;
	xmls::xString containing_level_key;
	xmls::Collection<LocationMarkerLevel> location_levels;
public:
	LocationMarker();
	~LocationMarker();
	virtual void set_content(std::string image_filename, Rect* image_subsection, std::pair<int, int> position);
	virtual Rect *get_bitmap_subsection();
	const std::string get_location_name();
	const std::string get_containing_level_key();
	const bool matches_level(const std::string level_key);
};

#endif