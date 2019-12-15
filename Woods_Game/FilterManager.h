#ifndef FILTER_MANAGER_H
#define FILTER_MANAGER_H

class FilterManager
{
private:
	FilterManager() {}
public:
	FilterManager(FilterManager const&) = delete;
	void operator=(FilterManager const&) = delete;
	static FilterManager& get_instance();
	~FilterManager();
};
#endif
