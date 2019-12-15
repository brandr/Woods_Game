#ifndef ENTITY_EFFECT_H
#define ENTITY_EFFECT_H

#include "Animation.h"
#include "GameImage.h"
#include "utility"      // for pair
#include "xstring"      // for string

struct EntityEffectData :
	public xmls::Serializable {
	xmls::xString effect_key;
	xmls::Collection<AnimationData> animation_data;
	EntityEffectData();
	~EntityEffectData();
	const std::string get_effect_key();
	std::vector<AnimationData*> get_animation_data();
};

class EntityEffect :
	public GameImage
{
private:
	bool effect_active = false;
	bool effect_animated = true;
	int animation_row = 0;
	float effect_anim_speed = 0.0f;
	void set_animation_data(std::vector<AnimationData *> data);
protected:
	virtual const float speed_for_anim_duration();
	virtual const bool should_adjust_anim_duration();
	virtual const bool should_update_animation();
public:
	EntityEffect();
	~EntityEffect();
	virtual void load_content(const std::string filename, int row, std::pair<int,int> frame_dimensions, 
		std::vector<AnimationData*> anim_data);
	virtual void update(const float speed);
	virtual int get_animation_row();
	void set_effect_active(const bool active);
	bool get_effect_active();
	void set_effect_animated(const bool value);
};

#endif