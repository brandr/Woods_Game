#ifndef SPRITESHEETANIMATION_H
#define SPRITESHEETANIMATION_H

#include "ControlsMenuManager.h"
class SpriteSheetAnimation //:public Animation
{
private:
	int frame_counter;//, switch_frame;
public:
	SpriteSheetAnimation();
	~SpriteSheetAnimation();
	virtual void update(Animation&);
	bool animation_at_end(Animation& a);
	void reset();
};

#endif