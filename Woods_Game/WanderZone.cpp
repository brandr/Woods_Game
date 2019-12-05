#include "WanderZone.h"



WanderZone::WanderZone()
{
	setClassName("WanderZone");
	Register("should_wander", &should_wander);
	Register("min_x", &min_x);
	Register("min_y", &min_y);
	Register("max_x", &max_x);
	Register("max_y", &max_y);
}


WanderZone::~WanderZone()
{
}

const bool WanderZone::contains_object(const int center_x, const int center_y, const int object_x, const int object_y)
{
	const int adj_min_x = center_x + this->min_x.value(), adj_min_y = center_y + this->min_y.value(), adj_max_x = center_x + this->max_x.value(), adj_max_y = center_y + this->max_y.value();
	return object_x >= adj_min_x && object_x < adj_max_x && object_y >= adj_min_y && object_y < adj_max_y;
}

const bool WanderZone::get_should_wander()
{
	return this->should_wander.value();
}

const int WanderZone::get_min_x()
{
	return this->min_x.value();
}

const int WanderZone::get_min_y()
{
	return this->min_y.value();
}

const int WanderZone::get_max_x()
{
	return this->max_x.value();
}

const int WanderZone::get_max_y()
{
	return this->max_y.value();
}
