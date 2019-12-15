#include "EntityEffect.h"
#include "ImageLoader.h"


void EntityEffect::set_animation_data(std::vector<AnimationData*> data)
{
	this->animation_data.Clear();
	for (AnimationData * ad : data) {
		this->animation_data.addItem(ad);
	}
}

const float EntityEffect::speed_for_anim_duration()
{
	return this->effect_anim_speed;
}

const bool EntityEffect::should_adjust_anim_duration()
{
	return this->effect_animated && this->effect_anim_speed > 0.0f;
}

const bool EntityEffect::should_update_animation()
{
	return this->effect_animated;
}

EntityEffect::EntityEffect()
{
}


EntityEffect::~EntityEffect()
{
}

void EntityEffect::load_content(const std::string filename, int row, std::pair<int, int> frame_dimensions, std::vector<AnimationData *> anim_data)
{
	this->ss_animation = new SpriteSheetAnimation();
	this->anim_state = ANIM_STATE_NEUTRAL;
	this->animation_spritesheet_key = filename;
	this->set_animation_data(anim_data);
	this->animation_row = row;
	std::vector<std::string> anim_keys;
	std::string anim_filename = this->animation_spritesheet_key.value();
	const int anim_data_count = this->animation_data.size();
	for (int i = 0; i < anim_data_count; i++) {
		Animation *anim = new Animation();
		AnimationData *anim_data = this->animation_data.getItem(i);
		std::string anim_key = anim_data->animation_key.value();
		int frame_count_x = anim_data->animation_frame_width.value();
		int frame_count_y = anim_data->animation_frame_height.value();
		std::pair<int, int> frame_count(frame_count_x, frame_count_y);
		anim->load_content(anim_filename, frame_count
			, std::pair<int, int>(0, 0), frame_dimensions, anim_data->animation_frame_duration.value(), anim_data->animation_pixel_step_size.value(),
			anim_data->animation_min_speed.value(), anim_data->animation_max_speed.value());
		ImageLoader::get_instance().load_spritesheet(*anim);
		animations[anim_key] = anim;
	}	
}

void EntityEffect::update(const float speed)
{
	this->effect_anim_speed = speed;
	GameImage::update();
	if (ss_animation->animation_at_end(*get_animation())) {
		set_effect_active(false);
	}
}

int EntityEffect::get_animation_row()
{
	return animation_row;
}

void EntityEffect::set_effect_active(const bool active)
{
	effect_active = active;
}

bool EntityEffect::get_effect_active()
{
	return effect_active;
}

void EntityEffect::set_effect_animated(const bool value)
{
	this->effect_animated = value;
}

EntityEffectData::EntityEffectData()
{
	setClassName("EntityEffectData");
	Register("effect_key", &effect_key);
	Register("animation_data", &animation_data);
}

EntityEffectData::~EntityEffectData()
{
}

const std::string EntityEffectData::get_effect_key()
{
	return this->effect_key.value();
}

std::vector<AnimationData*> EntityEffectData::get_animation_data()
{
	std::vector<AnimationData*> anim_data;
	const int size = this->animation_data.size();
	for (int i = 0; i < size; i++) {
		anim_data.push_back(this->animation_data.getItem(i));
	}
	return anim_data;
}
