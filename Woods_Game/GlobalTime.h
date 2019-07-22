#ifndef GLOBAL_TIME_H
#define GLOBAL_TIME_H

#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define MIN_MONTH_INDEX 5;
#define MAX_MONTH_INDEX 8;

#define TIME_RATIO 1000 // number of time counter ticks per hour (probably want 5000-10000 for actual game, not sure though)

#define ORIGIN_YEAR 88
#define ORIGIN_MONTH 5
#define ORIGIN_DAY 18

class GlobalTime {
private:
	bool intialized = false;
	int day, time;
	struct std::tm origin_time = {0, 0, 0, ORIGIN_DAY, ORIGIN_MONTH, ORIGIN_YEAR};
	const time_t origin_timestamp = std::mktime(&origin_time);
	time_t current_timestamp;
	void update_timestamp();
public:
	GlobalTime(const int d, const int t);
	void copy(GlobalTime * other);
	void update();
	const std::string date_display_string();
	const std::string get_current_month_str();
	const std::string get_month_str(const int index);
	const int get_first_displayable_day_index();
	const int get_first_day_of_month_index();
	const int get_first_day_of_month_index(const int mon);
	const int get_last_day_of_month_index(const int mon);
	const int get_current_month_index();
	const int get_current_day_index();
	const int get_day_index(const int mon, const int day);
	const int get_num_days();
	const int get_num_days_in_month(const int index);
	const int get_current_minutes();
	const int get_day();
	void set_day(const int day);
	const int get_time();
	void set_time(const int time);

};
#endif