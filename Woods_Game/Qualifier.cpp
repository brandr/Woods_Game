#include "Qualifier.h"

Qualifier::Qualifier()
{
	setClassName("Qualifier");
	Register("comparator", &comparator);
	Register("evaluator", &evaluator);
	Register("qualifiers", &qualifiers);
	Register("month", &month);
	Register("day", &day);
	Register("hour", &hour);
	Register("minute", &minute);
}

Qualifier::~Qualifier()
{
}

// override in subclasses
const bool Qualifier::evaluate()
{
	switch (this->evaluator.value()) {
	case EVALUATOR_KEY_VALUE:
		// we're not passing in a pair of values, so this always returns false
		return false;
	case EVALUATOR_AND:
		return this->and_evaluate();
	case EVALUATOR_OR:
		return this->or_evaluate();
	case EVALUATOR_TIME:
		return this->time_evaluate();
	default:
		return false;
	}
	return false;
}

const bool Qualifier::evaluate(const int a, const int b)
{
	const int comp = this->comparator.value();
	switch (comp) {
	case COMPARATOR_EQUALS:
		return a == b;
	case COMPARATOR_NOT_EQUALS:
		return a != b;
	case COMPARATOR_LESS_THAN:
		return a < b;
	case COMPARATOR_LESS_THAN_EQUAL:
		return a <= b;
	case COMPARATOR_GREATER_THAN:
		return a > b;
	case COMPARATOR_GREATER_THAN_EQUAL:
		return a >= b;
	default:
		return false;
	}
}

const bool Qualifier::and_evaluate()
{
	const int size = this->qualifiers.size();
	for (int i = 0; i < size; i++) {
		Qualifier * q = this->qualifiers.getItem(i);
		q->set_other_time(this->other_time);
		const bool value = q->evaluate();
		if (!value) {
			return false;
		}
	}
	return true;
}

const bool Qualifier::or_evaluate()
{
	const int size = this->qualifiers.size();
	for (int i = 0; i < size; i++) {
		Qualifier * q = this->qualifiers.getItem(i);
		q->set_other_time(this->other_time);
		const bool value = q->evaluate();
		if (value) {
			return true;
		}
	}
	return false;
}

const bool Qualifier::time_evaluate()
{
	if (this->other_time == NULL) {
		return false;
	}
	// check month
	const int month_val = this->month.value();
	if (month_val >= 0) {
		const int other_month = this->other_time->get_current_month_index();
		return Qualifier::evaluate(other_month, month_val);
	}

	// check day
	const int day_val = this->day.value();
	if (day_val >= 0) {
		const int other_day = this->other_time->get_current_day_index();
		return Qualifier::evaluate(other_day, day_val);
	}

	// check hour
	const int hour_val = this->hour.value();
	if (hour_val >= 0) {
		const int other_hour = this->other_time->get_total_minutes() / 60;
		return Qualifier::evaluate(other_hour, hour_val);
	}

	// check minute
	const int minute_val = this->minute.value();
	if (minute_val >= 0) {
		const int other_minute = this->other_time->get_current_minutes();
		return Qualifier::evaluate(other_minute, minute_val);
	}
	return true;
}

void Qualifier::set_other_time(GlobalTime * value)
{
	this->other_time = value;
}