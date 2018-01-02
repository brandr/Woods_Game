#ifndef ENTITY_EFFECT_H
#define ENTITY_EFFECT_H

#include "GameImage.h"
#include "utility"      // for pair
#include "xstring"      // for string

class EntityEffect :
	public GameImage
{
private:
	bool effect_active = false;
	int animation_row = 0;
public:
	EntityEffect();
	~EntityEffect();
	virtual void load_content(std::string filename, int row, std::pair<int,int> frame_dimensions);
	virtual void update();
	virtual int get_animation_row();
	void set_effect_active(const bool active);
	bool get_effect_active();
};

#endif