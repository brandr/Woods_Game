#ifndef SPRITESHEETANIMATION_H
#define SPRITESHEETANIMATION_H

#include "ControlsMenuManager.h"
class SpriteSheetAnimation
{
private:
	int frame_counter;
public:
	SpriteSheetAnimation();
	~SpriteSheetAnimation();
	virtual void update(Animation& a, const bool should_adjust_duration, const float speed);
	bool animation_at_end(Animation& a);
	void reset();
};

#endif