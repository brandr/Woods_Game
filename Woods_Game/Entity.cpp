#include "Entity.h"
#include "GlobalTime.h"
#include "Item.h"
#include "ItemManager.h"
#include "ItemPickup.h"
#include "Player.h"
#include "Level.h"
#include "TileSet.h"
#include "World.h"

const std::string Entity::image_filename_suffix()
{
	// plant suffix for growth
	if (this->has_entity_attribute(E_ATTR_PLANT_GROWTH_CURRENT_AGE)
		&& this->has_entity_attribute(E_ATTR_PLANT_GROWTH_MATURE_AGE)) {
		const int current = this->get_entity_attribute(E_ATTR_PLANT_GROWTH_CURRENT_AGE);
		const int mature = this->get_entity_attribute(E_ATTR_PLANT_GROWTH_MATURE_AGE);
		if (current >= mature) {
			return "";
		} else {
			const int num_stages = this->get_entity_attribute(E_ATTR_PLANT_GROWTH_NUM_STAGES);
			const int stage_index = current / (mature / num_stages) + 1;
			return "_stage_" + std::to_string(stage_index);
		}
	}
	// gate open sprite
	if (this->has_entity_attribute(E_ATTR_GATE_STATE) && this->get_entity_attribute(E_ATTR_GATE_STATE) == 1) {
		return "_open";
	}
	return GameImage::image_filename_suffix();
}

void Entity::emit_sound(const std::string filename, const int duration, const bool repeat)
{
	if (this->entity_sounds.find(filename) == this->entity_sounds.end()) {
		EntitySound * sound = new EntitySound(filename, duration);
		this->entity_sounds[filename] = sound;
	}
	EntitySound * sound = this->entity_sounds[filename];
	if (!sound->is_playing) {
		sound->is_playing = true;
	}
	sound->duration = duration;
	sound->source_x = this->get_center().first, sound->source_y = this->get_center().second;
	sound->repeats = repeat;
}

void Entity::emit_sound(const std::string filename, const int duration)
{
	this->emit_sound(filename, duration, true);
}

void Entity::stop_sound(const std::string filename)
{
	if (this->entity_sounds.find(filename) != this->entity_sounds.end()) {
		EntitySound * sound = this->entity_sounds[filename];
		if (sound->is_playing) {
			sound->is_playing = false;
		}
	}
}

void Entity::clear_sounds()
{
	for (auto const &it : this->entity_sounds) {
		EntitySound * sound = it.second;
		sound->is_playing = false;
	}
}

void Entity::drop_items(Level * level, Player * player)
{
	
	const int size = this->item_drops.size();
	for (int i = 0; i < size; i++) {
		ItemDrop * id = this->item_drops.getItem(i);
		srand(std::time(NULL) + i);
		bool should_drop = false;
		const float rate = id->drop_rate.value();
		if (rate == 1.0f) {
			should_drop = true;
		} else if (rate > 0.0f) {
			const int roll = rand() % 100;
			if (((float)roll) / 100.0f < rate) {
				should_drop = true;
			}
		}
		if (should_drop) {
			this->drop_item(level, player, id);
		}
	}
}

void Entity::drop_item(Level * level, Player * player, ItemDrop * drop)
{
	ItemPickup * pickup = this->spawn_item_pickup(level, drop);
	pickup->bounce_from_entity(player, 4);
	level->add_item_pickup(pickup);
}

ItemPickup * Entity::spawn_item_pickup(Level * level, ItemDrop * drop)
{
	std::pair<int, int> center = this->get_center();
	const int pickup_x = center.first, pickup_y = center.second;
	ItemPickup * pickup = new ItemPickup(drop->item_key.value(), pickup_x, pickup_y);
	return pickup;
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

void Entity::load_entity_effects(TileSet * tileset, const std::string entity_key, int row, std::pair<int, int> frame_dimensions)
{
	const std::string tileset_filename = tileset->get_tile_sheet_filename();
	const std::string filename_prefix = tileset_filename + "/effects/";
	const int size = this->entity_effect_data.size();
	for (int i = 0; i < size; i++) {
		EntityEffectData * effect_data = this->entity_effect_data.getItem(i);
		const std::string effect_key = effect_data->get_effect_key();
		std::string filename = filename_prefix + entity_key + "_" + effect_key;
		std::string full_filename = "resources/images/" + filename + ".png";
		if (!al_filename_exists(full_filename.c_str())) continue;		
		EntityEffect effect;
		std::vector<AnimationData *> anim_data = effect_data->get_animation_data();
		effect.load_content(filename, row, frame_dimensions, anim_data);
		effect.set_position(get_x(), get_y());
		entity_effects[effect_key] = effect;
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

void Entity::draw(ALLEGRO_DISPLAY * display, const int x_offset, const int y_offset, const int screen_w, const int screen_h)
{
	//TEMP. May eventually want to draw the entity's "broken" sprite, not sure.	
	if (get_entity_attribute(E_ATTR_BROKEN) != 1) {
		GameImage::draw(display, x_offset, y_offset, screen_w, screen_h);
	}	
	for (auto &it : entity_effects) {
		if (!it.second.get_effect_active()) continue;		
		it.second.draw(display, x_offset, y_offset, screen_w, screen_h);
	}	
}

void Entity::update()
{
	for (auto &it : entity_effects) {
		it.second.set_position(this->get_x(), this->get_y()); //TODO: offset?
		if (!it.second.get_effect_active()) continue;		
		it.second.update(this->speed_for_anim_duration());
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

const bool Entity::update_new_day(World * world, Level * level, Player * player)
{
	const InteractActionManager &manager = InteractActionManager::get_instance();
	const int size = this->load_day_actions.size();
	bool interacted = false;
	for (int i = 0; i < size; i++) {
		InteractAction * action = this->load_day_actions.getItem(i);
		if (manager.run_action(world, level, action, player, this)) {
			interacted = true;
		}
	}
	return interacted;
}

const bool Entity::contact_action(World * world, Level * level, Player * player)
{
	const InteractActionManager &manager = InteractActionManager::get_instance();
	const int size = this->contact_actions.size();
	bool interacted = false;
	for (int i = 0; i < size; i++) {
		InteractAction * action = this->contact_actions.getItem(i);
		if (manager.run_action(world, level, action, player, this)) {
			interacted = true;
		}
	}
	const std::string ee_category = this->encyclopedia_category_name(level);
	const std::string ee_name = this->encyclopedia_entry_name(level);
	if (!ee_category.empty() && !ee_name.empty()) {
		world->update_encyclopedia_state(ee_category, ee_name, ENTRY_KNOWN);
	}
	return interacted;
}

const bool Entity::interact_action(World * world, Level * level, GlobalTime * time, Player * player)
{
	const InteractActionManager &manager = InteractActionManager::get_instance();
	const int size = this->interact_actions.size();
	bool interacted = false;
	for (int i = 0; i < size; i++) {
		InteractAction * action = this->interact_actions.getItem(i);
		std::vector<ActionBinding*> bindings = action->get_bindings();
		if (manager.run_action(world, level, action, player, this)) {
			interacted = true;
		}
	}
	return interacted;
}

void Entity::entity_break(Level * level, Player * player)
{
	set_entity_attribute(E_ATTR_CURRENT_DURABILITY, 0);
	set_entity_attribute(E_ATTR_BROKEN, 1);
	auto it = entity_effects.find("break");
	if (it != entity_effects.end()) {
		entity_effects["break"].set_effect_active(true);
	}	
	this->drop_items(level, player);
}

void Entity::update_visibility(World * world, Level * level)
{
	const std::string ee_name = this->encyclopedia_entry_name(level);
	const std::string ee_category = this->encyclopedia_category_name(level);
	if (!ee_name.empty() && !ee_category.empty()) {
		const int state = world->get_encyclopedia_state(ee_category, ee_name);
		if (state == ENTRY_UNDISCOVERED) {
			world->update_encyclopedia_state(ee_category, ee_name, ENTRY_SEEN);
		}
	}
}

const bool Entity::has_visibility_actions(Level * level)
{
	if (!this->encyclopedia_entry_name(level).empty() && !this->encyclopedia_category_name(level).empty()) {
		return true;
	}
	return false;
}

const std::string Entity::encyclopedia_entry_name(Level * level)
{
	TileSet * tileset = level->get_tileset();
	return tileset->encyclopedia_entry_name_for_entity(this);
}

const std::string Entity::encyclopedia_category_name(Level * level)
{
	TileSet * tileset = level->get_tileset();
	return tileset->encyclopedia_category_for_entity(this);
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

void Entity::copy_contact_actions(std::vector<InteractAction*> actions)
{
	for (InteractAction * ia : actions) {
		InteractAction *action = new InteractAction(ia->get_interact_action_key(), ia->get_function_name());
		std::vector<ActionBinding*> bindings = ia->get_bindings();
		std::vector<ActionBinding *> copy_bindings;
		for (ActionBinding * b : bindings) {
			ActionBinding * copy_binding = new ActionBinding(b->binding_key.value(), b->binding_value.value());
			copy_bindings.push_back(copy_binding);
		}
		std::vector<Qualifier*> quals = ia->get_qualifiers();
		std::vector<Qualifier *> copy_qualifiers;
		for (Qualifier * q : quals) {
			copy_qualifiers.push_back(q);
		}
		action->set_bindings(copy_bindings);
		action->set_qualifiers(copy_qualifiers);
		this->contact_actions.addItem(action);
	}
}

std::vector<InteractAction*> Entity::get_contact_actions()
{
	std::vector<InteractAction*> actions;
	const int size = this->contact_actions.size();
	for (int i = 0; i < size; i++) {
		actions.push_back(this->contact_actions.getItem(i));
	}
	return actions;
}

void Entity::set_interact_actions(const std::vector<std::pair<std::string, std::string>> actions)
{
	this->interact_actions.Clear();
	for (std::pair<std::string, std::string> action_data : actions) {
		InteractAction *action = new InteractAction(action_data.first, action_data.second);
		this->interact_actions.addItem(action);
	}
}

void Entity::copy_interact_actions(std::vector<InteractAction*> actions)
{
	for (InteractAction * ia : actions) {
		InteractAction *action = new InteractAction(ia->get_interact_action_key(), ia->get_function_name());
		std::vector<ActionBinding*> bindings = ia->get_bindings();
		std::vector<ActionBinding *> copy_bindings;
		for (ActionBinding * b : bindings) {
			ActionBinding * copy_binding = new ActionBinding(b->binding_key.value(), b->binding_value.value());
			copy_bindings.push_back(b);
		}
		std::vector<Qualifier*> quals = ia->get_qualifiers();
		std::vector<Qualifier *> copy_qualifiers;
		for (Qualifier * q : quals) {
			copy_qualifiers.push_back(q);
		}
		action->set_bindings(copy_bindings);
		action->set_qualifiers(copy_qualifiers);
		action->set_bindings(copy_bindings);
		this->interact_actions.addItem(action);
	}
}

std::vector<InteractAction*> Entity::get_interact_actions()
{
	std::vector<InteractAction*> actions;
	const int size = this->interact_actions.size();
	for (int i = 0; i < size; i++) {
		actions.push_back(this->interact_actions.getItem(i));
	}
	return actions;
}

void Entity::set_load_day_actions(const std::vector<std::pair<std::string, std::string>> actions)
{
	this->load_day_actions.Clear();
	for (std::pair<std::string, std::string> action_data : actions) {
		InteractAction *action = new InteractAction(action_data.first, action_data.second);
		this->load_day_actions.addItem(action);
	}
}

void Entity::copy_load_day_actions(std::vector<InteractAction*> actions)
{
	for (InteractAction * ia : actions) {
		InteractAction *action = new InteractAction(ia->get_interact_action_key(), ia->get_function_name());
		std::vector<ActionBinding*> bindings = ia->get_bindings();
		std::vector<ActionBinding *> copy_bindings;
		for (ActionBinding * b : bindings) {
			ActionBinding * copy_binding = new ActionBinding(b->binding_key.value(), b->binding_value.value());
			copy_bindings.push_back(b);
		}
		std::vector<Qualifier*> quals = ia->get_qualifiers();
		std::vector<Qualifier *> copy_qualifiers;
		for (Qualifier * q : quals) {
			copy_qualifiers.push_back(q);
		}
		action->set_bindings(copy_bindings);
		action->set_qualifiers(copy_qualifiers);
		action->set_bindings(copy_bindings);
		this->load_day_actions.addItem(action);
	}
}

void Entity::copy_entity_effect_data(std::vector<EntityEffectData*> effect_datas)
{
	this->entity_effect_data.Clear();
	for (EntityEffectData * eed : effect_datas) {
		EntityEffectData * copy_data = new EntityEffectData();
		copy_data->effect_key = eed->effect_key.value();
		copy_data->animation_data = eed->animation_data;
		this->entity_effect_data.addItem(copy_data);
	}
}

std::vector<InteractAction*> Entity::get_load_day_actions()
{
	std::vector<InteractAction*> actions;
	const int size = this->load_day_actions.size();
	for (int i = 0; i < size; i++) {
		actions.push_back(this->load_day_actions.getItem(i));
	}
	return actions;
}

void Entity::set_item_drops(std::vector<ItemDrop*> item_drops)
{
	this->item_drops.Clear();
	for (ItemDrop * id : item_drops) {
		this->item_drops.addItem(id);
	}
}

std::vector<ItemDrop*> Entity::get_item_drops()
{
	std::vector<ItemDrop*> drops;
	const int size = this->item_drops.size();
	for (int i = 0; i < size; i++) {
		drops.push_back(this->item_drops.getItem(i));
	}
	return drops;
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

void Entity::set_entity_starting_pos_x(const int x)
{
	this->entity_starting_pos_x = x;
}

void Entity::set_entity_starting_pos_y(const int y)
{
	this->entity_starting_pos_y = y;
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

void Entity::take_durability_damage(Level * level, Player * player, const int damage)
{
	if (!has_entity_attribute(E_ATTR_DURABILITY)) return;
	if (!has_entity_attribute(E_ATTR_CURRENT_DURABILITY)) set_entity_attribute(E_ATTR_CURRENT_DURABILITY, get_entity_attribute(E_ATTR_DURABILITY));
	const int current_durability = get_entity_attribute(E_ATTR_CURRENT_DURABILITY);
	if (current_durability - damage <= 0) {
		entity_break(level, player);
		return;
	}
	set_entity_attribute(E_ATTR_CURRENT_DURABILITY, current_durability - damage);
	auto it = entity_effects.find("hit");
	if (it == entity_effects.end()) return;
	entity_effects["hit"].set_effect_active(true);
}

void Entity::take_damage(const int damage)
{
	// override in subclasses unless this should do something for all entities
}

const bool Entity::get_should_push_others()
{
	return false;
}

void Entity::push_back(Level * level, const float xvel, const float yvel)
{
	//override in subclasse	
}

void Entity::set_entity_effect_data(std::vector<EntityEffectData *> effect_data)
{
	this->entity_effect_data.Clear();
	for (EntityEffectData * ed : effect_data) {
		this->entity_effect_data.addItem(ed);
	}
}

std::vector<EntityEffectData*> Entity::get_entity_effect_data()
{
	std::vector<EntityEffectData*> effects;
	const int size = this->entity_effect_data.size();
	for (int i = 0; i < size; i++) {
		EntityEffectData * eed = this->entity_effect_data.getItem(i);
		effects.push_back(eed);
	}
	return effects;
}

void Entity::load_image_filters()
{
	const int size = this->image_filters.size();
	for (int i = 0; i < size; i++) {
		ImageFilter * f = this->image_filters.getItem(i);
		ImageLoader::get_instance().load_image("filters/" + f->get_filename(), "", false, "resources/images/", true);
		ALLEGRO_BITMAP * filter_bmp = ImageLoader::get_instance().get_image("filters/" + f->get_filename());
		const int w = al_get_bitmap_width(filter_bmp), h = al_get_bitmap_height(filter_bmp);
		f->set_image_dimensions(w, h);
	}
}

void Entity::set_image_filters(std::vector<ImageFilter*> filters)
{
	this->image_filters.Clear();
	for (ImageFilter * f : filters) {
		this->image_filters.addItem(f);
	}
}

std::vector<ImageFilter*> Entity::get_image_filters()
{
	std::vector<ImageFilter*> filters;
	const int size = this->image_filters.size();
	for (int i = 0; i < size; i++) {
		filters.push_back(this->image_filters.getItem(i));
	}
	return filters;
}

void Entity::toggle_light_filter(const int filter_id)
{
	const int size = this->image_filters.size();
	for (int i = 0; i < size; i++) {
		ImageFilter * f = this->image_filters.getItem(i);
		if (f->get_filter_id() == filter_id) {
			f->toggle();
			break;
		}
	}
}

std::vector<ImageFilter*> Entity::get_active_image_filters()
{
	std::vector<ImageFilter*> active_filters;
	const int size = this->image_filters.size();
	for (int i = 0; i < size; i++) {
		ImageFilter * f = this->image_filters.getItem(i);
		if (f->get_is_toggled()) {
			active_filters.push_back(f);
		}
	}
	return active_filters;
}

const std::string Entity::get_sound_key()
{
	return "" + SOUND_KEY_DEFAULT;
}

std::vector<EntitySound*> Entity::get_active_entity_sounds()
{
	std::vector<EntitySound*> active_sounds;
	for (auto const &it : this->entity_sounds) {
		EntitySound * s = it.second;
		if (s->is_playing) {
			const int duration = std::max(1, s->duration);
			s->duration_counter = (s->duration_counter + 1) % duration;
			if (s->duration_counter == 0) {
				active_sounds.push_back(s);
				if (!s->repeats) {
					s->is_playing = false;
				}
			}
		} else {
			s->duration_counter = 0;
		}
		this->entity_sounds[it.first] = s;
	}
	return active_sounds;
}

Item * Entity::get_plant_gather_item()
{
	const int item_key = this->get_entity_attribute(E_ATTR_PLANT_GATHER_ITEM_KEY);
	if (item_key >= 0) {
		return ItemManager::get_instance().create_item(item_key);
	}
	return NULL;
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
	Register("item_drops", &item_drops);
	Register("spawn_tile_rules", &spawn_tile_rules);
	Register("entity_effect_data", &entity_effect_data);
	Register("image_filters", &image_filters);
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
	Register("encyclopedia_category_key", &encyclopedia_category_key);
	Register("encyclopedia_entry_key", &encyclopedia_entry_key);
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
	const int size = (int)components.size();
	for (int i = 0; i < size; i++) {
		this->components.addItem(components[i]);
	}
}

std::vector<std::pair<std::string, std::string>> EntityData::get_block_contact_action_data()
{
	std::vector<std::pair<std::string, std::string>> data;
	const int size = (int)this->contact_actions.size();
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
	const int size = (int)this->interact_actions.size();
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
	const int size = (int) this->load_day_actions.size();
	for (int i = 0; i < size; i++) {
		data.push_back(std::pair<std::string, std::string>(
			this->load_day_actions.getItem(i)->get_interact_action_key(),
			this->load_day_actions.getItem(i)->get_function_name()));
	}
	return data;
}

std::vector<InteractAction*> EntityData::get_block_interact_actions()
{
	std::vector<InteractAction*> actions;
	const int size = (int)this->interact_actions.size();
	for (int i = 0; i < size; i++) {
		actions.push_back(this->interact_actions.getItem(i));
	}
	return actions;
}

std::vector<InteractAction*> EntityData::get_block_contact_actions()
{
	std::vector<InteractAction*> actions;
	const int size = (int)this->contact_actions.size();
	for (int i = 0; i < size; i++) {
		actions.push_back(this->contact_actions.getItem(i));
	}
	return actions;
}

std::vector<InteractAction*> EntityData::get_block_load_day_actions()
{
	std::vector<InteractAction*> actions;
	const int size = (int)this->load_day_actions.size();
	for (int i = 0; i < size; i++) {
		actions.push_back(this->load_day_actions.getItem(i));
	}
	return actions;
}

std::vector<ItemDrop*> EntityData::get_item_drops()
{
	std::vector<ItemDrop*> drops;
	const int size = (int)this->item_drops.size();
	for (int i = 0; i < size; i++) {
		ItemDrop * id1 = this->item_drops.getItem(i);
		ItemDrop * id2 = new ItemDrop();
		id2->item_key = id1->item_key.value();
		id2->drop_rate = id1->drop_rate.value();
		drops.push_back(id2);
	}
	return drops;
}

const std::vector<EntitySpawnTileRule *> EntityData::get_block_spawn_tile_rules()
{
	std::vector<EntitySpawnTileRule *> data;
	const int size = (int)this->spawn_tile_rules.size();
	for (int i = 0; i < size; i++) {
		data.push_back(this->spawn_tile_rules.getItem(i));
	}
	return data;
}

std::vector<EntityEffectData*> EntityData::get_entity_effect_data()
{
	std::vector<EntityEffectData*> effect_data;
	const int size = this->entity_effect_data.size();
	for (int i = 0; i < size; i++) {
		effect_data.push_back(this->entity_effect_data.getItem(i));
	}
	return effect_data;
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
	const int size = (int)this->components.size();
	for (int i = 0; i < size; i++) {
		EntityComponentData *comp = this->components.getItem(i);
		components.push_back(comp);
	}
	return components;
}

const bool EntityData::has_entity_attribute(const std::string attr_key)
{
	const int size = (int)this->attributes.size();
	for (int i = 0; i < size; i++) {
		EntityAttribute *attr = this->attributes.getItem(i);
		if (attr->attribute_key.value() == attr_key) {
			return true;
		}
	}
	return false;
}

int EntityData::get_entity_attribute(const std::string attr_key)
{
	const int size = (int)this->attributes.size();
	for (int i = 0; i < size; i++) {
		EntityAttribute *attr = this->attributes.getItem(i);
		if (attr->attribute_key.value() == attr_key) {
			return attr->attribute_value.value();
		}
	}
	return 0;
}

const std::vector<std::string> EntityData::image_filename_suffixes()
{
	std::vector<std::string> suffixes;
	if (this->has_entity_attribute(Entity::E_ATTR_PLANT_GROWTH_MATURE_AGE)) {
		const int mature = this->get_entity_attribute(Entity::E_ATTR_PLANT_GROWTH_MATURE_AGE);
		if (mature <= 0) {
			return suffixes;
		} else {
			const int num_stages = this->get_entity_attribute(Entity::E_ATTR_PLANT_GROWTH_NUM_STAGES);
			int stage_index = 1;
			for (int index = 0; index < mature; index += mature / num_stages) {
				suffixes.push_back("_stage_" + std::to_string(stage_index));
				stage_index++;
			}
		}
	}
	// gate open sprite
	if (this->has_entity_attribute(GameImage::E_ATTR_GATE_STATE)) {
		suffixes.push_back("_open");
	}
	return suffixes;
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
	const int size = (int)attributes.size();
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
	const int size = (int)this->attributes.size();
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

EntitySound::EntitySound()
{
}

EntitySound::EntitySound(const std::string name, const int duration)
{
	this->filename = name;
	this->duration = duration;
}
