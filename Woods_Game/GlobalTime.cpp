#include "GlobalTime.h"

void GlobalTime::update_timestamp()
{
	struct tm t = { 0, 0, 0, ORIGIN_DAY, ORIGIN_MONTH, ORIGIN_YEAR };
	t.tm_mday += (this->day - 1);
	this->current_timestamp = mktime(&t);
	this->intialized = true;
}

GlobalTime::GlobalTime(const int d, const int t)
{
	this->day = d, this->time = t;
	this->update_timestamp();
}

void GlobalTime::copy(GlobalTime * other)
{
	this->day = other->get_day();
	this->time = other->get_time();
	this->update_timestamp();
}

void GlobalTime::update()
{
	this->time++;
	if (this->time >= TIME_RATIO * 24) {
		this->time = 0;
		this->day++;
		this->update_timestamp();
	}
}

const std::string GlobalTime::date_display_string()
{
	if (!this->intialized) {
		return "";
	}
	struct tm timeinfo;
	gmtime_s(&timeinfo, &current_timestamp);
	std::stringstream buffer;
	buffer << std::put_time(&timeinfo, "%a, %b %d");
	return buffer.str();
}

const std::string GlobalTime::get_current_month_str()
{
	if (!this->intialized) {
		return "";
	}
	struct tm timeinfo;
	gmtime_s(&timeinfo, &current_timestamp);
	std::stringstream buffer;
	buffer << std::put_time(&timeinfo, "%b");
	return buffer.str();
}

const std::string GlobalTime::get_month_str(const int index)
{
	if (!this->intialized) {
		return "";
	}
	struct tm timeinfo;
	gmtime_s(&timeinfo, &current_timestamp);
	timeinfo.tm_mday = 1;
	timeinfo.tm_mon = index;
	time_t time = mktime(&timeinfo);
	gmtime_s(&timeinfo, &time);
	std::stringstream buffer;
	buffer << std::put_time(&timeinfo, "%b");
	return buffer.str();
}

const int GlobalTime::get_first_displayable_day_index()
{
	if (!this->intialized) {
		return 0;
	}
	struct tm timeinfo;
	gmtime_s(&timeinfo, &current_timestamp);
	timeinfo.tm_mday = 1;
	timeinfo.tm_mon = MIN_MONTH_INDEX;
	time_t time = mktime(&timeinfo);
	gmtime_s(&timeinfo, &time);
	return timeinfo.tm_wday;
}

const int GlobalTime::get_first_day_of_month_index()
{
	if (!this->intialized) {
		return 0;
	}
	struct tm timeinfo;
	gmtime_s(&timeinfo, &current_timestamp);
	timeinfo.tm_mday = 1;
	time_t time = mktime(&timeinfo);
	gmtime_s(&timeinfo, &time);
	return timeinfo.tm_wday;
}

const int GlobalTime::get_first_day_of_month_index(const int mon)
{
	if (!this->intialized) {
		return 0;
	}
	struct tm timeinfo;
	gmtime_s(&timeinfo, &current_timestamp);
	timeinfo.tm_mday = 1;
	timeinfo.tm_mon = mon;
	time_t time = mktime(&timeinfo);
	gmtime_s(&timeinfo, &time);
	return timeinfo.tm_wday;
}

const int GlobalTime::get_last_day_of_month_index(const int mon)
{
	if (!this->intialized) {
		return 0;
	}
	struct tm timeinfo;
	gmtime_s(&timeinfo, &current_timestamp);
	timeinfo.tm_mday = this->get_num_days_in_month(mon);
	timeinfo.tm_mon = mon;
	time_t time = mktime(&timeinfo);
	gmtime_s(&timeinfo, &time);
	return timeinfo.tm_wday;
}

const int GlobalTime::get_current_month_index()
{
	if (!this->intialized) {
		return 0;
	}
	struct tm timeinfo;
	gmtime_s(&timeinfo, &current_timestamp);
	return timeinfo.tm_mon;
}

const int GlobalTime::get_current_day_index()
{
	if (!this->intialized) {
		return 0;
	}
	struct tm timeinfo;
	gmtime_s(&timeinfo, &current_timestamp);
	return timeinfo.tm_mday;
}

const int GlobalTime::get_day_index(const int mon, const int day)
{
	if (!this->intialized) {
		return 0;
	}
	struct tm timeinfo;
	gmtime_s(&timeinfo, &current_timestamp);
	timeinfo.tm_mday = day;
	timeinfo.tm_mon = mon;
	time_t other_time = mktime(&timeinfo);
	int difference = std::difftime(this->current_timestamp, other_time) / (60 * 60 * 24);
	return this->day - (difference + 1);
}

const int GlobalTime::get_num_days()
{
	if (!this->intialized) {
		return 0;
	}
	struct tm timeinfo;
	gmtime_s(&timeinfo, &current_timestamp);
	const int month = timeinfo.tm_mon;
	int next_month = month;
	int last_day = timeinfo.tm_mday;
	while (next_month == month) {
		timeinfo.tm_mday += 1;
		time_t time = mktime(&timeinfo);
		gmtime_s(&timeinfo, &time);
		int next_day = timeinfo.tm_mday;
		if (next_day >= last_day) {
			last_day = next_day;
		}
		next_month = timeinfo.tm_mon;
	}
	return last_day;
}

const int GlobalTime::get_num_days_in_month(const int index)
{
	if (!this->intialized) {
		return 0;
	}
	struct tm timeinfo;
	gmtime_s(&timeinfo, &current_timestamp);
	timeinfo.tm_mday = 1;
	timeinfo.tm_mon = index;
	time_t time = mktime(&timeinfo);
	gmtime_s(&timeinfo, &time);
	const int month = timeinfo.tm_mon;
	int next_month = month;
	int last_day = timeinfo.tm_mday;
	while (next_month == month) {
		timeinfo.tm_mday += 1;
		time_t time = mktime(&timeinfo);
		gmtime_s(&timeinfo, &time);
		int next_day = timeinfo.tm_mday;
		if (next_day >= last_day) {
			last_day = next_day;
		}
		next_month = timeinfo.tm_mon;
	}
	return last_day;
}

const int GlobalTime::get_current_minutes()
{
	return (((this->time) * 60 / TIME_RATIO) % (TIME_RATIO * 60 * 24));
}

const int GlobalTime::get_day()
{
	return this->day;
}

void GlobalTime::set_day(const int d)
{
	this->day = d;
}

const int GlobalTime::get_time()
{
	return this->time;
}

void GlobalTime::set_time(const int t)
{
	this->time = t;
}