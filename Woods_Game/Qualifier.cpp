#include "Qualifier.h"
#include "World.h"

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
	Register("day_of_week", &day_of_week);
	Register("trigger_status", &trigger_status);
}

Qualifier::~Qualifier()
{
}

const bool Qualifier::evaluate(World * world)
{
	switch (this->evaluator.value()) {
	case EVALUATOR_KEY_VALUE:
		// we're not passing in a pair of values, so this always returns false
		return false;
	case EVALUATOR_AND:
		return this->and_evaluate(world);
	case EVALUATOR_OR:
		return this->or_evaluate(world);
	case EVALUATOR_TIME:
		return this->time_evaluate();
	case EVALUATOR_TRIGGER:
		return this->trigger_evaluate(world);
	case EVALUATOR_DAY_OF_WEEK:
		return this->day_of_week_evaluate();
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

const bool Qualifier::and_evaluate(World * world)
{
	const int size = this->qualifiers.size();
	for (int i = 0; i < size; i++) {
		Qualifier * q = this->qualifiers.getItem(i);
		q->set_other_time(this->other_time);
		const bool value = q->evaluate(world);
		if (!value) {
			return false;
		}
	}
	return true;
}

const bool Qualifier::or_evaluate(World * world)
{
	const int size = this->qualifiers.size();
	for (int i = 0; i < size; i++) {
		Qualifier * q = this->qualifiers.getItem(i);
		q->set_other_time(this->other_time);
		const bool value = q->evaluate(world);
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

const bool Qualifier::trigger_evaluate(World * world)
{
	const int trigger_state_1 = this->trigger_status.get_trigger_state();
	TriggerStatus * matching_status = world->matching_trigger_status(&(this->trigger_status));
	if (matching_status != NULL) {
		const int trigger_state_2 = matching_status->get_trigger_state();
		return this->evaluate(trigger_state_1, trigger_state_2);
	} else {
		return this->comparator.value() == COMPARATOR_NOT_EQUALS;
	}
}

// should only use equal and not equal because it's ambiguous what a greater day of week actually means
const bool Qualifier::day_of_week_evaluate()
{
	if (this->other_time == NULL) {
		return false;
	}
	const int day_of_week_val = this->day_of_week.value();
	const int other_val = this->other_time->get_current_day_of_week_index();
	return this->evaluate(other_val, day_of_week_val);
}

void Qualifier::set_other_time(GlobalTime * value)
{
	this->other_time = value;
}