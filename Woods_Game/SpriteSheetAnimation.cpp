#include "SpriteSheetAnimation.h"



SpriteSheetAnimation::SpriteSheetAnimation()
{
	frame_counter = 0;
	//switch_frame = 8; //temp
}


SpriteSheetAnimation::~SpriteSheetAnimation()
{
}

void SpriteSheetAnimation::update(Animation &a)
{
	if (a.is_active()) {
		frame_counter++;
		if (frame_counter >= a.get_frame_duration()){
			frame_counter = 0;
			a.get_current_frame().first++;
			std::pair<int, int> fdim = a.get_frame_dimensions();
			std::pair<int,int> dim = a.get_image_dimensions();
			if (a.get_current_frame().first * fdim.first >= dim.first)
				a.get_current_frame().first = 0;
		}
	}
	else {
		frame_counter = 0;
		a.get_current_frame().first = 1;
	}
	//a.image, a.current_frame().first*a.frame_dimensions.first
}

bool SpriteSheetAnimation::animation_at_end(Animation & a)
{
	if (!a.is_active()) return true;
	if (frame_counter + 1 >= a.get_frame_duration()) {
		std::pair<int, int> fdim = a.get_frame_dimensions();
		std::pair<int, int> dim = a.get_image_dimensions();
		return ((a.get_current_frame().first + 1) * fdim.first >= dim.first);
	}
	return false;
}

void SpriteSheetAnimation::reset()
{
	frame_counter = 0;
}
