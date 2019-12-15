#include "FilterManager.h"

FilterManager & FilterManager::get_instance()
{
	static FilterManager manager;
	return manager;
}

FilterManager::~FilterManager()
{
}
