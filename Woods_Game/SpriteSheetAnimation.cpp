#include "SpriteSheetAnimation.h"



SpriteSheetAnimation::SpriteSheetAnimation()
{
	frame_counter = 0;
}


SpriteSheetAnimation::~SpriteSheetAnimation()
{
}

void SpriteSheetAnimation::update(Animation &a, const bool should_adjust_duration, const float speed)
{
	if (a.is_active()) {
		frame_counter++;
		int frame_duration = a.get_frame_duration();
		std::pair<int, int> fdim = a.get_frame_dimensions();
		std::pair<int, int> dim = a.get_image_dimensions();
		const float step_size = a.get_pixel_step_size();
		if (should_adjust_duration && speed > 0.0f && step_size > 0.0f) {
			// 2d/sf
			frame_duration = std::max(1, (int)(2.0f * step_size / (speed * ((float)a.get_frame_count().first))));
		}
		if (frame_counter >= frame_duration){
			frame_counter = 0;
			a.get_current_frame().first++;			
			if (a.get_current_frame().first * fdim.first >= dim.first)
				a.get_current_frame().first = 0;
		}
	}
	else {
		frame_counter = 0;
		a.get_current_frame().first = 1;
	}
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