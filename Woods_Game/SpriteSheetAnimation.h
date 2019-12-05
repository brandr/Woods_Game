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
	virtual void update(Animation&);
	bool animation_at_end(Animation& a);
	void reset();
};

#endif