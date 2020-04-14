#include "Qualifier.h"
#include "Quest.h"
#include "Level.h"
#include "World.h"

Qualifier::Qualifier()
{
	setClassName("Qualifier");
	Register("comparator", &comparator);
	Register("evaluator", &evaluator);
	Register("qualifiers", &qualifiers);
	Register("bindings", &bindings);
	Register("month", &month);
	Register("day", &day);
	Register("hour", &hour);
	Register("minute", &minute);
	Register("day_of_week", &day_of_week);
	Register("int_val", &int_val);
	Register("string_val", &string_val);
	Register("trigger_status", &trigger_status);
	Register("npc_key", &npc_key);
}

Qualifier::~Qualifier()
{
}

const bool Qualifier::evaluate(World * world, Level * current_level)
{
	switch (this->evaluator.value()) {
	case EVALUATOR_KEY_VALUE:
		// we're not passing in a pair of values, so this always returns false
		return false;
	case EVALUATOR_AND:
		return this->and_evaluate(world, current_level);
	case EVALUATOR_OR:
		return this->or_evaluate(world, current_level);
	case EVALUATOR_TIME:
		return this->time_evaluate();
	case EVALUATOR_TRIGGER:
		return this->trigger_evaluate(world);
	case EVALUATOR_DAY_OF_WEEK:
		return this->day_of_week_evaluate();
	case EVALUATOR_CURRENT_LEVEL:
		return this->current_level_evaluate(world, current_level);
	case EVALUATOR_NPC_PLACEMENT:
		return this->npc_placement_evaluate(world, current_level);
	case EVALUATOR_HAS_ITEM:
		return this->has_item_evaluate(world, current_level);
	case EVALUATOR_QUEST_STATE:
		return this->quest_state_evaluate(world, current_level);
	case EVALUATOR_INVENTORY_FULL:
		return this->inventory_full_evaluate(world, current_level);
	case EVALUATOR_BLOCK_ATTRIBUTE:
		return this->block_attribute_evaluate(world, current_level);
	default:
		return false;
	}
	return false;
}

const bool Qualifier::evaluate(World * world, Level * current_level, GlobalTime * time)
{
	this->set_other_time(time);
	return this->evaluate(world, current_level);
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

const bool Qualifier::has_binding(const std::string attr_key)
{
	const int size = this->bindings.size();
	for (int i = 0; i < size; i++) {
		QualifierBinding * qb = this->bindings.getItem(i);
		if (qb->binding_key.value() == attr_key) {
			return true;
		}
	}
	return false;
}

const std::string Qualifier::get_binding(const std::string attr_key)
{
	const int size = this->bindings.size();
	for (int i = 0; i < size; i++) {
		QualifierBinding * qb = this->bindings.getItem(i);
		if (qb->binding_key.value() == attr_key) {
			return qb->binding_value.value();
		}
	}
	return "";
}

const bool Qualifier::and_evaluate(World * world, Level * level)
{
	const int size = this->qualifiers.size();
	for (int i = 0; i < size; i++) {
		Qualifier * q = this->qualifiers.getItem(i);
		q->set_other_time(this->other_time);
		const bool value = q->evaluate(world, level);
		if (!value) {
			return false;
		}
	}
	return true;
}

const bool Qualifier::or_evaluate(World * world, Level * level)
{
	const int size = this->qualifiers.size();
	for (int i = 0; i < size; i++) {
		Qualifier * q = this->qualifiers.getItem(i);
		q->set_other_time(this->other_time);
		const bool value = q->evaluate(world, level);
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

const bool Qualifier::current_level_evaluate(World * world, Level * level)
{
	if (COMPARATOR_EQUALS == this->comparator.value()) {
		return level != NULL && level->get_filename() == this->string_val.value();
	} else if (COMPARATOR_NOT_EQUALS == this->comparator.value()) {
		return level == NULL || level->get_filename() != this->string_val.value();
	}
	return false;
}

const bool Qualifier::npc_placement_evaluate(World * world, Level * level)
{
	const bool placement_matches = world->npc_is_on_node(this->npc_key.value(), this->string_val.value());
	if (COMPARATOR_EQUALS == this->comparator.value()) {
		return placement_matches;
	}
	else if (COMPARATOR_NOT_EQUALS == this->comparator.value()) {
		return !placement_matches;
	}
	return false;
}

const bool Qualifier::has_item_evaluate(World * world, Level * level)
{
	const int item_key = this->int_val.value();
	Player * player = world->get_player();
	if (player != NULL && item_key >= 0) {
		const bool has_item = player->has_item_with_key(item_key);
		if (COMPARATOR_EQUALS == this->comparator.value()) {
			return has_item;
		}
		else if (COMPARATOR_NOT_EQUALS == this->comparator.value()) {
			return !has_item;
		}
	}
	return false;
}

const bool Qualifier::quest_state_evaluate(World * world, Level * level)
{
	const std::string quest_key = this->string_val.value();
	const int quest_state = this->int_val.value();
	if (!quest_key.empty() && quest_state >= 0) {
		Quest * quest = world->quest_for_key(quest_key);
		if (quest != NULL) {
			const bool matches_state = quest->get_quest_state() == quest_state;
			if (COMPARATOR_EQUALS == this->comparator.value()) {
				return matches_state;
			}
			else if (COMPARATOR_NOT_EQUALS == this->comparator.value()) {
				return !matches_state;
			}
		}
	}
	return false;
}

const bool Qualifier::inventory_full_evaluate(World * world, Level * level)
{
	Player * player = world->get_player();
	if (player != NULL) {
		const bool inventory_full = player->get_inventory().is_full();
		if (COMPARATOR_EQUALS == this->comparator.value()) {
			return inventory_full;
		}
		else if (COMPARATOR_NOT_EQUALS == this->comparator.value()) {
			return !inventory_full;
		}
	}
	return false;
}

const bool Qualifier::block_attribute_evaluate(World * world, Level * level)
{
	if (this->has_binding(Q_BINDING_BLOCK_ATTR_KEY) && this->has_binding(Q_BINDING_BLOCK_ATTR_VALUE)
		&& this->has_binding(Q_BINDING_BLOCK_TX) && this->has_binding(Q_BINDING_BLOCK_TY)) {
		const std::string b_attr_key = this->get_binding(Q_BINDING_BLOCK_ATTR_KEY),
			b_attr_value = this->get_binding(Q_BINDING_BLOCK_ATTR_VALUE);
		const std::string b_tx_str = this->get_binding(Q_BINDING_BLOCK_TX),
			b_ty_str = this->get_binding(Q_BINDING_BLOCK_TY);
		Level * check_level = level;
		if (this->has_binding(Q_BINDING_BLOCK_LEVEL_KEY)) {
			const std::string level_key = this->get_binding(Q_BINDING_BLOCK_LEVEL_KEY);
			check_level = world->get_level_with_key(level_key);
		}
		Tile * t = check_level->get_tile(::atoi(b_tx_str.c_str()), ::atoi(b_ty_str.c_str()));
		if (t != NULL) {
			Block * b = t->get_block();
			if (b != NULL && !b->is_empty()) {
				// this is close enough to what we're checking that we can count false as true for "not equals"
				const bool attribute_matches = b->has_entity_attribute(b_attr_key)
					&& b->get_entity_attribute(b_attr_key) == ::atoi(b_attr_value.c_str());
				if (COMPARATOR_EQUALS == this->comparator.value()) {
					return attribute_matches;
				} else if (COMPARATOR_NOT_EQUALS == this->comparator.value()) {
					return !attribute_matches;
				}
			}
		}
	}
	return false;
}

void Qualifier::set_other_time(GlobalTime * value)
{
	this->other_time = value;
}

QualifierBinding::QualifierBinding()
{
	setClassName("QualifierBinding");
	Register("binding_key", &binding_key);
	Register("binding_value", &binding_value);
}
