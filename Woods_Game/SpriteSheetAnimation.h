#include "Animation.h"
class SpriteSheetAnimation //:public Animation
{
private:
	int frame_counter, switch_frame;
public:
	SpriteSheetAnimation();
	~SpriteSheetAnimation();
	virtual void update(Animation&);
};

