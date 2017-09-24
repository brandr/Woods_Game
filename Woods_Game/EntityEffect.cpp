#include "EntityEffect.h"



EntityEffect::EntityEffect()
{
}


EntityEffect::~EntityEffect()
{
}

void EntityEffect::load_content(std::string filename, int row, std::pair<int, int> frame_dimensions)
{
	ss_animation = new SpriteSheetAnimation();
	Animation *anim = load_animation_single_row(filename, row, frame_dimensions);
	animations[get_anim_state_key(ANIM_STATE_NEUTRAL)] = anim;
	anim_state = ANIM_STATE_NEUTRAL;
	animation_row = row;
}

void EntityEffect::update()
{
	//TODO: update animation and deactivate when we reach the end of the animation.
	GameImage::update();
	if (ss_animation->animation_at_end(*get_animation())) set_effect_active(false);
}

int EntityEffect::get_animation_row()
{
	//return 0; //temp
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