#ifndef WANDER_ZONE_H
#define WANDER_ZONE_H

#include "XMLSerialization.h"

class WanderZone : public xmls::Serializable
{
private:
	xmls::xBool should_wander;
	// these are relative to the center node's position, so they can be negative
	xmls::xInt min_x, min_y, max_x, max_y;
public:
	WanderZone();
	~WanderZone();
	const bool contains_object(const int center_x, const int center_y, const int object_x, const int object_y);
	const bool get_should_wander();
	const int get_min_x();
	const int get_min_y();
	const int get_max_x();
	const int get_max_y();
};

#endif