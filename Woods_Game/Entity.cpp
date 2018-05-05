#include "Entity.h"

Entity::Entity()
{
	// NOTE: we do not register xml attributes here, only in subclass constructors
	solid = false;
}


Entity::~Entity()
{
	//currentLevel = NULL;
}

Rect * Entity::get_bitmap_subsection()
{
	//override this for subclasses
	return nullptr;
}

void Entity::set_solid(bool solid)
{
	this->solid = solid;
}

bool Entity::is_solid()
{
	if (get_entity_attribute(E_ATTR_BROKEN) > 0) return false;
	return solid.value();
}

void Entity::load_entity_effects(std::string filename, int row, std::pair<int,int> frame_dimensions)
{
	//TODO: consider including animation speed if we decide we want different animation speeds
	std::string filename_prefix = filename;//"effects/" + filename;
	std::vector<std::string> entity_effect_names = get_entity_effect_names();
	const int size = entity_effect_names.size();
	for (int i = 0; i < size; i++) {
		std::string filename = filename_prefix + "_" + entity_effect_names[i];
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
}

void Entity::draw(ALLEGRO_DISPLAY * display, int x_offset, int y_offset)
{
	for (auto &it : entity_effects) {
		if (!it.second.get_effect_active()) continue;
		//std::cout << "drawing effect with row: " << it.second.get_animation_row() << "\n";
		it.second.draw(display, x_offset, y_offset);
	}
	if (get_entity_attribute(E_ATTR_BROKEN) == 1) return; //TEMP. May eventually want to draw the entity's "broken" sprite, not sure.
	//const int effect_count = entity_effects.size();
	
	GameImage::draw(display, x_offset, y_offset);
}

void Entity::update()
{
	//if (has_entity_attribute(E_ATTR_DURABILITY)) durability_update();
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
	//this->entity_attributes = attributes;
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
	//entity_attributes[attr] = val;
}

void Entity::set_starting_pos(int x, int y)
{
	this->entity_starting_pos_x = x;
	this->entity_starting_pos_y = y;
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
	/*
	auto it = entity_attributes.find(attribute);
	if (it != entity_attributes.end()) return it->second;
	*/
	return 0;
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
	//auto it = entity_attributes.find(attribute);
	//return (it != entity_attributes.end());
}
int Entity::get_entity_sheet_col()
{
	return this->entity_sheet_row.value();
}
int Entity::get_entity_sheet_row()
{
	return this->entity_sheet_row.value();
}
/*
bool Entity::get_entity_flag(int flag_key)
{
	return false;
}
*/
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
	Register("attributes", &attributes);
	Register("components", &components);
	Register("root_offset_x", &root_offset_x);
	Register("root_offset_y", &root_offset_y);
	Register("center_offset_x", &center_offset_x);
	Register("center_offset_y", &center_offset_y);
	Register("solid", &solid);
	solid = false;
	//this->entity_data_index = -1;
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
	Register("attributes", &attributes);
	Register("components", &components);
	Register("root_offset_x", &root_offset_x);
	Register("root_offset_y", &root_offset_y);
	Register("center_offset_x", &center_offset_x);
	Register("center_offset_y", &center_offset_y);
	Register("entity_group_image_width", &entity_group_image_width);
	Register("entity_group_image_height", &entity_group_image_height);
	Register("solid", &solid);
	solid = false;
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
