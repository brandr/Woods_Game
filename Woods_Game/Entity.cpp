#include "Entity.h"
#include "Player.h"


const std::string Entity::image_filename_suffix()
{
	if (this->has_entity_attribute(E_ATTR_PLANT_GROWTH_CURRENT_AGE)
		&& this->has_entity_attribute(E_ATTR_PLANT_GROWTH_MATURE_AGE)) {
		const int current = this->get_entity_attribute(E_ATTR_PLANT_GROWTH_CURRENT_AGE);
		const int mature = this->get_entity_attribute(E_ATTR_PLANT_GROWTH_MATURE_AGE);
		if (current >= mature) {
			return "";
		}
		else {
			const int num_stages = this->get_entity_attribute(E_ATTR_PLANT_GROWTH_NUM_STAGES);
			const int stage_index = current / (mature / num_stages) + 1;
			return "_stage_" + std::to_string(stage_index);
		}
	}
	return GameImage::image_filename_suffix();
}

Entity::Entity()
{
	// NOTE: we do not register xml attributes here, only in subclass constructors
	solid = false;
}


Entity::~Entity()
{
}

void Entity::copy_entity_attributes(Entity * other)
{
	this->entity_attributes.Clear();
	if (other != NULL) {
		this->set_entity_attributes(other->get_entity_attributes());
	}
}

Rect * Entity::get_bitmap_subsection()
{
	//override this for subclasses
	return NULL;
}

void Entity::set_solid(const bool solid)
{
	this->solid = solid;
}

void Entity::set_visible(const bool visible)
{
	this->visible = visible;
}

bool Entity::is_solid()
{
	if (get_entity_attribute(E_ATTR_BROKEN) > 0) {
		return false;
	}
	return solid.value();
}

bool Entity::is_visible()
{
	return visible.value();
}

void Entity::load_entity_effects(std::string filename, const std::string entity_key, int row, std::pair<int, int> frame_dimensions)
{
	//TODO: consider including animation speed if we decide we want different animation speeds
	const std::string filename_prefix = filename + "/effects/";
	const std::vector<std::string> entity_effect_names = get_entity_effect_names(); //FIXME: effect names should be set in tileset, or should be a constant
	const int size = entity_effect_names.size();
	for (int i = 0; i < size; i++) {
		std::string filename = filename_prefix + entity_key + "_" + entity_effect_names[i];
		std::string full_filename = "resources/images/" + filename + ".png";
		if (!al_filename_exists(full_filename.c_str())) continue;
		EntityEffect effect;
		effect.load_content(filename, row, frame_dimensions);
		effect.set_position(get_x(), get_y());
		entity_effects[entity_effect_names[i]] = effect;
	}
}

void Entity::load_content_from_attributes()
{
	GameImage::load_content_from_attributes();
	this->rect.x = this->entity_starting_pos_x.value();
	this->rect.y = this->entity_starting_pos_y.value();
	this->rect.width = std::max((int)this->rect.width, this->spritesheet_frame_width.value());
	this->rect.height = std::max((int)this->rect.height, this->spritesheet_frame_height.value());
	this->collide_rect.x = this->rect.x + this->collide_x_offset.value();
	this->collide_rect.y = this->rect.y + this->collide_y_offset.value();
	this->collide_rect.width = this->collide_width.value();
	this->collide_rect.height = this->collide_height.value();
}

void Entity::unload_content()
{
	this->entity_attributes.Clear();
	GameImage::unload_content();
}

void Entity::draw(ALLEGRO_DISPLAY * display, int x_offset, int y_offset)
{
	for (auto &it : entity_effects) {
		if (!it.second.get_effect_active()) continue;
		it.second.draw(display, x_offset, y_offset);
	}
	if (get_entity_attribute(E_ATTR_BROKEN) == 1) return; //TEMP. May eventually want to draw the entity's "broken" sprite, not sure.	
	GameImage::draw(display, x_offset, y_offset);
}

void Entity::update()
{
	for (auto &it : entity_effects) {
		if (!it.second.get_effect_active()) continue;
		it.second.update();
	}
	counter_update();
	GameImage::update();
}

void Entity::counter_update()
{
	for (auto const &it : counters) {
		counters[it.first] = std::max(0, counters[it.first] - 1);
	}
}

void Entity::durability_update()
{

}

void Entity::mark_needs_plant_day_update()
{
	this->counters[COUNTER_PLANT_DAY_UPDATE] = 1;
}

void Entity::unmark_needs_plant_day_update()
{
	this->counters[COUNTER_PLANT_DAY_UPDATE] = 0;
}

const bool Entity::needs_plant_day_update()
{
	return this->counters[COUNTER_PLANT_DAY_UPDATE] > 0;
}

const int Entity::get_plant_growth_spread_range()
{
	return this->get_entity_attribute(E_ATTR_PLANT_GROWTH_SPREAD_RANGE);
}

const bool Entity::update_new_day(Player * player)
{
	const InteractActionManager &manager = InteractActionManager::get_instance();
	const int size = this->load_day_actions.size();
	bool interacted = false;
	for (int i = 0; i < size; i++) {
		InteractAction * action = this->load_day_actions.getItem(i);
		if (manager.run_action(action, player, this)) {
			interacted = true;
		}
	}
	return interacted;
}

const bool Entity::contact_action(Player * player)
{
	const InteractActionManager &manager = InteractActionManager::get_instance();
	const int size = this->contact_actions.size();
	bool interacted = false;
	for (int i = 0; i < size; i++) {
		InteractAction * action = this->contact_actions.getItem(i);
		if (manager.run_action(action, player)) {
			interacted = true;
		}
	}
	return interacted;
}

const bool Entity::interact_action(Player * player)
{
	const InteractActionManager &manager = InteractActionManager::get_instance();
	const int size = this->interact_actions.size();
	bool interacted = false;
	for (int i = 0; i < size; i++) {
		InteractAction * action = this->interact_actions.getItem(i);
		if (manager.run_action(action, player)) {
			interacted = true;
		}
	}
	return interacted;
}

void Entity::entity_break()
{
	set_entity_attribute(E_ATTR_CURRENT_DURABILITY, 0);
	set_entity_attribute(E_ATTR_BROKEN, 1);
	auto it = entity_effects.find("break");
	if (it == entity_effects.end()) return;
	entity_effects["break"].set_effect_active(true);
}

void Entity::set_entity_data_index(int index)
{
	this->entity_data_index = index;
}

void Entity::set_entity_sheet_offset(int col, int row)
{
	this->entity_sheet_col = col;
	this->entity_sheet_row = row;
}

void Entity::set_entity_attributes(std::map<std::string, int> attributes)
{
	this->entity_attributes.Clear();
	for (std::pair<std::string, int> attr_pair : attributes) {
		EntityAttribute *attr = new EntityAttribute(attr_pair.first, attr_pair.second);
		this->entity_attributes.addItem(attr);
	}
}

void Entity::set_entity_attributes(std::vector<std::string> attributes)
{
	this->entity_attributes.Clear();
	for (std::string attr_key : attributes) {
		EntityAttribute *attr = new EntityAttribute(attr_key, 1);
		this->entity_attributes.addItem(attr);
	}
}

void Entity::set_entity_attribute(std::string attr_key, int val)
{
	const int size = this->entity_attributes.size();
	for (int i = 0; i < size; i++) {
		EntityAttribute *attr = this->entity_attributes.getItem(i);
		if (attr->attribute_key.value() == attr_key) {
			attr->attribute_value = val;
			return;
		}
	}
	entity_attributes.addItem(new EntityAttribute(attr_key, val));
}

void Entity::set_contact_actions(const std::vector<std::pair<std::string, std::string>> actions)
{
	this->contact_actions.Clear();
	for (std::pair<std::string, std::string> action_data : actions) {
		InteractAction *action = new InteractAction(action_data.first, action_data.second);
		this->contact_actions.addItem(action);
	}
}

void Entity::set_interact_actions(const std::vector<std::pair<std::string, std::string>> actions)
{
	this->interact_actions.Clear();
	for (std::pair<std::string, std::string> action_data : actions) {
		InteractAction *action = new InteractAction(action_data.first, action_data.second);
		this->interact_actions.addItem(action);
	}
}

void Entity::set_load_day_actions(const std::vector<std::pair<std::string, std::string>> actions)
{
	this->load_day_actions.Clear();
	for (std::pair<std::string, std::string> action_data : actions) {
		InteractAction *action = new InteractAction(action_data.first, action_data.second);
		this->load_day_actions.addItem(action);
	}
}

void Entity::set_spawn_tile_rules(const std::vector<EntitySpawnTileRule*> rules)
{
	this->spawn_tile_rules.Clear();
	for (EntitySpawnTileRule * rule : rules) {
		this->spawn_tile_rules.addItem(rule);
	}
}

void Entity::set_starting_pos(int x, int y)
{
	this->entity_starting_pos_x = x;
	this->entity_starting_pos_y = y;
}

int Entity::get_entity_starting_pos_x()
{
	return this->entity_starting_pos_x.value();
}

int Entity::get_entity_starting_pos_y()
{
	return this->entity_starting_pos_y.value();
}

const std::vector<int> Entity::get_allowed_spawn_tile_types()
{
	std::vector<int> types;
	const int size = this->spawn_tile_rules.size();
	for (int i = 0; i < size; i++) {
		types.push_back(this->spawn_tile_rules.getItem(i)->tile_type_index.value());
	}
	return types;
}

int Entity::get_entity_data_index()
{
	return this->entity_data_index.value();
}

int Entity::get_entity_attribute(std::string attr_key)
{
	const int size = this->entity_attributes.size();
	for (int i = 0; i < size; i++) {
		EntityAttribute *attr = this->entity_attributes.getItem(i);
		if (attr->attribute_key.value() == attr_key) {
			return attr->attribute_value.value();
		}
	}
	return 0;
}

std::map<std::string, int> Entity::get_entity_attributes()
{
	std::map<std::string, int> attributes;
	const int size = this->entity_attributes.size();
	for (int i = 0; i < size; i++) {
		EntityAttribute *attr = this->entity_attributes.getItem(i);
		attributes[attr->attribute_key.value()] = attr->attribute_value.value();
	}
	return attributes;
}

bool Entity::has_entity_attribute(std::string attr_key)
{
	const int size = this->entity_attributes.size();
	for (int i = 0; i < size; i++) {
		EntityAttribute *attr = this->entity_attributes.getItem(i);
		if (attr->attribute_key.value() == attr_key) {
			return true;
		}
	}
	return false;
}
int Entity::get_entity_sheet_col()
{
	return this->entity_sheet_row.value();
}
int Entity::get_entity_sheet_row()
{
	return this->entity_sheet_row.value();
}

void Entity::take_durability_damage(const int damage)
{
	if (!has_entity_attribute(E_ATTR_DURABILITY)) return;
	if (!has_entity_attribute(E_ATTR_CURRENT_DURABILITY)) set_entity_attribute(E_ATTR_CURRENT_DURABILITY, get_entity_attribute(E_ATTR_DURABILITY));
	const int current_durability = get_entity_attribute(E_ATTR_CURRENT_DURABILITY);
	if (current_durability - damage <= 0) {
		entity_break();
		return;
	}
	set_entity_attribute(E_ATTR_CURRENT_DURABILITY, current_durability - damage);
	auto it = entity_effects.find("hit");
	if (it == entity_effects.end()) return;
	entity_effects["hit"].set_effect_active(true);
}

//TODO: do this part better
std::vector<std::string> Entity::get_entity_effect_names()
{
	std::vector<std::string> names{ "break", "hit" };
	return names;
}

// EntityData

EntityData::EntityData()
{

	setClassName("EntityData");
	Register("entity_data_key", &entity_data_key);
	Register("entity_data_index", &entity_data_index);
	Register("interact_actions", &interact_actions);
	Register("contact_actions", &contact_actions);
	Register("load_day_actions", &load_day_actions);
	Register("spawn_tile_rules", &spawn_tile_rules);
	Register("attributes", &attributes);
	Register("components", &components);
	Register("root_offset_x", &root_offset_x);
	Register("root_offset_y", &root_offset_y);
	Register("collide_offset_x", &collide_offset_x);
	Register("collide_offset_y", &collide_offset_y);
	Register("collide_width", &collide_width);
	Register("collide_height", &collide_height);
	Register("center_offset_x", &center_offset_x);
	Register("center_offset_y", &center_offset_y);
	Register("solid", &solid);
	Register("visible", &visible);
	solid = false;
	visible = true;
}

void EntityData::set_attributes(std::map<std::string, int> attributes)
{
	this->attributes.Clear();
	for (auto const &it : attributes) {
		EntityAttribute *attr = new EntityAttribute();
		attr->attribute_key = it.first;
		attr->attribute_value = it.second;
		this->attributes.addItem(attr);
	}
}

std::map<std::string, int> EntityData::attribute_map()
{
	std::map<std::string, int> attr_map;
	const int size = this->attributes.size();
	for (int i = 0; i < size; i++) {
		EntityAttribute *attr = this->attributes.getItem(i);
		attr_map[attr->attribute_key.value()] = attr->attribute_value.value();
	}
	return attr_map;
}

void EntityData::set_root_offset(std::pair<int, int> offset)
{
	this->root_offset_x = offset.first;
	this->root_offset_y = offset.second;
}

void EntityData::set_center_offset(std::pair<int, int> offset)
{
	this->center_offset_x = offset.first;
	this->center_offset_y = offset.second;
}

void EntityData::set_components(std::vector<EntityComponentData*> components)
{
	this->components.Clear();
	const int size = components.size();
	for (int i = 0; i < size; i++) {
		this->components.addItem(components[i]);
	}
}

std::vector<std::pair<std::string, std::string>> EntityData::get_block_contact_action_data()
{
	std::vector<std::pair<std::string, std::string>> data;
	const int size = this->contact_actions.size();
	for (int i = 0; i < size; i++) {
		data.push_back(std::pair<std::string, std::string>(
			this->contact_actions.getItem(i)->get_interact_action_key(),
			this->contact_actions.getItem(i)->get_function_name()));
	}
	return data;
}

std::vector<std::pair<std::string, std::string>> EntityData::get_block_interact_action_data()
{
	std::vector<std::pair<std::string, std::string>> data;
	const int size = this->interact_actions.size();
	for (int i = 0; i < size; i++) {
		data.push_back(std::pair<std::string, std::string>(
			this->interact_actions.getItem(i)->get_interact_action_key(),
			this->interact_actions.getItem(i)->get_function_name()));
	}
	return data;
}

std::vector<std::pair<std::string, std::string>> EntityData::get_block_load_day_action_data()
{
	std::vector<std::pair<std::string, std::string>> data;
	const int size = this->load_day_actions.size();
	for (int i = 0; i < size; i++) {
		data.push_back(std::pair<std::string, std::string>(
			this->load_day_actions.getItem(i)->get_interact_action_key(),
			this->load_day_actions.getItem(i)->get_function_name()));
	}
	return data;
}

const std::vector<EntitySpawnTileRule *> EntityData::get_block_spawn_tile_rules()
{
	std::vector<EntitySpawnTileRule *> data;
	const int size = this->spawn_tile_rules.size();
	for (int i = 0; i < size; i++) {
		data.push_back(this->spawn_tile_rules.getItem(i));
	}
	return data;
}

bool EntityData::is_empty()
{
	return this->entity_data_index.value() > -1;
}

std::string EntityData::get_entity_data_key()
{
	return this->entity_data_key.value();
}

std::pair<int, int> EntityData::get_root_offset()
{
	return std::pair<int, int>(this->root_offset_x.value(), this->root_offset_y.value());
}

const std::pair<int, int> EntityData::get_collide_offset()
{
	return std::pair<int, int>(this->collide_offset_x.value(), this->collide_offset_y.value());
}

const std::pair<int, int> EntityData::get_collide_dimensions()
{
	return std::pair<int, int>(this->collide_width.value(), this->collide_height.value());
}

std::pair<int, int> EntityData::get_center_offset()
{
	return std::pair<int, int>(this->center_offset_x.value(), this->center_offset_y.value());
}

std::vector<EntityComponentData*> EntityData::get_components()
{
	std::vector<EntityComponentData*> components;
	const int size = this->components.size();
	for (int i = 0; i < size; i++) {
		EntityComponentData *comp = this->components.getItem(i);
		components.push_back(comp);
	}
	return components;
}

EntityAttribute::EntityAttribute()
{
	setClassName("EntityAttribute");
	Register("attribute_key", &attribute_key);
	Register("attribute_value", &attribute_value);
}

EntityAttribute::EntityAttribute(std::string key, int value)
{
	setClassName("EntityAttribute");
	Register("attribute_key", &attribute_key);
	Register("attribute_value", &attribute_value);
	this->attribute_key = key, this->attribute_value = value;
}

EntityComponentData::EntityComponentData()
{
	this->setClassName("EntityComponentAttribute");
	this->Register("name", &(this->name));
	this->Register("attributes", &(this->attributes));
}

EntityComponentData::EntityComponentData(std::string name, std::vector<std::string> attributes)
{
	this->setClassName("EntityComponentAttribute");
	this->Register("name", &(this->name));
	this->Register("attributes", &(this->attributes));
	this->name = name;
	this->set_attributes(attributes);
}

void EntityComponentData::set_attributes(std::vector<std::string> attributes)
{
	this->attributes.Clear();
	const int size = attributes.size();
	for (int i = 0; i < size; i++) {
		EntityComponentAttribute *attr = new EntityComponentAttribute(attributes[i]);
		this->attributes.addItem(attr);
	}
}

std::string EntityComponentData::get_name()
{
	return this->name.value();
}

std::vector<std::string> EntityComponentData::get_attributes()
{

	std::vector<std::string> attributes;
	const int size = this->attributes.size();
	for (int i = 0; i < size; i++) {
		attributes.push_back(this->attributes.getItem(i)->name.value());
	}
	return attributes;
}

EntityComponentAttribute::EntityComponentAttribute()
{
	this->setClassName("EntityComponentAttribute");
	this->Register("name", &(this->name));
}


EntityComponentAttribute::EntityComponentAttribute(std::string name)
{
	this->setClassName("EntityComponentAttribute");
	this->Register("name", &(this->name));
	this->name = name;
}

EntityGroupData::EntityGroupData()
{
	setClassName("EntityData");
	Register("entity_group_name", &entity_group_name);
	Register("entity_group_index", &entity_group_index);
	Register("entity_group_image_width", &entity_group_image_width);
	Register("entity_group_image_height", &entity_group_image_height);
	solid = false;
	visible = true;
}

std::string EntityGroupData::get_entity_group_name()
{
	return entity_group_name.value();
}

int EntityGroupData::get_entity_group_index()
{
	return this->entity_group_index.value();
}

void EntityGroupData::set_entity_group_image_dimensions(std::pair<int, int> dimensions)
{
	this->entity_group_image_width = dimensions.first, this->entity_group_image_height = dimensions.second;
}

std::pair<int, int> EntityGroupData::get_entity_group_image_dimensions()
{
	return std::pair<int, int>(this->entity_group_image_width.value(), this->entity_group_image_height.value());
}

EntitySpawnTileRule::EntitySpawnTileRule()
{
	this->setClassName("EntitySpawnTileRule");
	this->Register("tile_type_index", &tile_type_index);
}

//TODO: store this as a variable like rect instead of setting it like this
/*
Rect Entity::collide_rect()
{
return Rect(
this->get_x() + this->get_collide_x_offset(),
this->get_y() + this->get_collide_y_offset(),
this->get_collide_width(), this->get_collide_height());
}
*/

const int Entity::get_collide_x_offset()
{
	return this->collide_x_offset.value();
}

const int Entity::get_collide_y_offset()
{
	return this->collide_y_offset.value();
}

const int Entity::get_collide_width()
{
	return this->collide_width.value();
}

const int Entity::get_collide_height()
{
	return this->collide_height.value();
}