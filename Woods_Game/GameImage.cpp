#include "GameImage.h"
#include "ImageLoader.h"

const std::string GameImage::image_filename_suffix()
{
	return "";
}

GameImage::GameImage(std::string filename)
{
}

GameImage::GameImage()
{
	rect = Rect(0.0f, 0.0f, 64.0f, 64.0f);
}

GameImage::GameImage(float x, float y, float width, float height)
{
	rect = Rect(x, y, width, height);
}

GameImage::~GameImage()
{
	//TOOD: how should we delete things? (shouldn't need to use al_destroy_bitmap, thanks to imageloader)
	/*
	delete animation;
	delete ss_animation;
	if(image_subsection)
		delete image_subsection;
	animation = NULL;
	ss_animation = NULL;
	image_subsection = NULL;
	*/
}

int GameImage::get_type()
{
	return GAME_IMAGE;
}

std::string GameImage::get_image_filename()
{
	return image_filename + this->image_filename_suffix();
}

void GameImage::load_content_from_attributes() {

	std::vector<std::string> anim_keys;
	std::string anim_filename = "sprite_sheets/" + this->animation_spritesheet_key.value();
	ss_animation = new SpriteSheetAnimation();
	const int anim_data_count = this->animation_data.size();
	for (int i = 0; i < anim_data_count; i++) {
		Animation *anim = new Animation();
		AnimationData *anim_data = this->animation_data.getItem(i);
		std::string anim_key = anim_data->animation_key.value();
		int frame_count_x = anim_data->animation_frame_width.value();
		int frame_count_y = anim_data->animation_frame_height.value();
		std::pair<int, int> frame_count(frame_count_x, frame_count_y);
		std::pair<int, int> ss_frame_dimensions(this->spritesheet_frame_width.value()
			, this->spritesheet_frame_height.value());
		anim->load_content(anim_filename + "_" + anim_key, frame_count
			, std::pair<int, int>(0, 0), ss_frame_dimensions, anim_data->animation_frame_duration.value());
		ImageLoader::get_instance().load_spritesheet(*anim);
		animations[anim_key] = anim;
	}
	load_mask(anim_filename);
}

//TODO: consider changing the way we mantain Rects (could key/store them in ImageLoader?)
void GameImage::set_content(std::string image_filename, Rect* image_subsection, std::pair<int, int> position)
{
	this->image_filename = image_filename;
	this->image_subsection = image_subsection;
	rect.x = position.first, rect.y = position.second;
}

void GameImage::load_mask(std::string base_filename)
{
	this->mask_filename = base_filename;
	ImageLoader::get_instance().load_mask(base_filename, "");
}

void GameImage::load_additional_masks_from_attributes(std::string prefix)
{
	const int additional_mask_count = this->additional_mask_data.size();
	for (int i = 0; i < additional_mask_count; i++) {
		MaskData *mask_data = this->additional_mask_data.getItem(i);
		const std::string mask_key = mask_data->mask_key.value();
		int mask_width = mask_data->mask_width.value();
		int mask_height = mask_data->mask_height.value();
		const std::string filename = "sprite_sheets/" + prefix + "_" + mask_key + "_spritesheet_mask";
		const int frame_count = mask_data->mask_frame_count.value();
		std::pair<int, int> frame_dimensions(mask_width, mask_height);
		for (int row = 0; row < frame_count; row++) {
			Rect sub(0.0f, frame_dimensions.second*row, frame_dimensions.first, frame_dimensions.second);
			const std::string rect_string = ImageLoader::get_instance().rect_to_string(sub);
			ImageLoader::get_instance().load_mask(filename, rect_string, false);
		}
	}
}

mask_t * GameImage::get_additional_mask(const std::string mask_key, const std::string prefix, const int row)
{
	const int additional_mask_count = this->additional_mask_data.size();
	for (int i = 0; i < additional_mask_count; i++) {
		MaskData *mask_data = this->additional_mask_data.getItem(i);
		const std::string mk = mask_data->mask_key.value();
		if (mask_key != mk) {
			continue;
		}
		const int mask_width = mask_data->mask_width.value();
		const int mask_height = mask_data->mask_height.value();
		const int frame_count = mask_data->mask_frame_count.value();
		std::pair<int, int> frame_dimensions(mask_width, mask_height);
		Rect sub(0.0f, frame_dimensions.second*row, frame_dimensions.first, frame_dimensions.second);
		const std::string rect_string = ImageLoader::get_instance().rect_to_string(sub);
		const std::string filename = "sprite_sheets/" + prefix + "_" + mask_key + "_spritesheet_mask";
		return ImageLoader::get_instance().get_mask(filename, rect_string);
	}
	return NULL;
}


void GameImage::unload_content()
{
	if (image_subsection) {
		delete image_subsection;
	}
	if(ss_animation)
		delete ss_animation;
	for (auto const &it : animations) {
		it.second->unload_content();
		delete it.second;
	}
	animations.clear();
	animations.swap(std::map<std::string, Animation*>());
	image_subsection = NULL;
	ss_animation = NULL;
}

void GameImage::draw(ALLEGRO_DISPLAY *display, int x_offset, int y_offset)
{
	ALLEGRO_BITMAP* draw_bitmap = ImageLoader::get_instance().get_current_image(this);
	const int image_width = std::max(this->spritesheet_frame_width.value(), (int) this->get_width());
	const int image_height = std::max(this->spritesheet_frame_height.value(), (int) this->get_height());
	if (draw_bitmap 
		&& rect.x + x_offset < al_get_display_width(display) 
		&& rect.x + image_width + x_offset >= 0 
		&& rect.y + y_offset < al_get_display_height(display) 
		&& rect.y + image_height + y_offset >= 0) {
		al_draw_bitmap(draw_bitmap, rect.x + x_offset, rect.y + y_offset, 0);
		int size = additional_image_layer_data.size();
		for (int i = 0; i < size; i++) {
			const std::pair<std::string, Rect> additional_data = this->additional_image_layer_data[i];
			ALLEGRO_BITMAP * additional_bmp = ImageLoader::get_instance().get_image(additional_data.first, additional_data.second);
			al_draw_bitmap(additional_bmp, rect.x + x_offset, rect.y + y_offset, 0);
		}
	}
}

void GameImage::update()
{
	if (ss_animation) {
		Animation* anim = get_animation();
		if (anim) {
			anim->set_row(get_animation_row());
			ss_animation->update(*anim);
		}
	}
}

void GameImage::set_position(int x, int y)
{
	rect.x = x;
	rect.y = y;
}

void GameImage::set_rect(int x, int y, int width, int height)
{
	rect.x = x, rect.y = y, rect.width = width, rect.height = height;
}

void GameImage::set_center_offset(std::pair<int, int> offset)
{
	this->center_offset_x = offset.first;
	this->center_offset_y = offset.second;
}

const std::pair<int, int> GameImage::get_center()
{
	int offset_x = this->center_offset_x.value();
	int offset_y = this->center_offset_y.value();
	return std::pair<int, int>(static_cast<int>(rect.x) + offset_x
		, static_cast<int>(rect.y) + offset_y);
}

void GameImage::add_additional_image_layer(const std::string filename, Rect subsection)
{
	additional_image_layer_data.push_back(std::pair<std::string, Rect>(filename, subsection));
}

void GameImage::refresh_mask()
{
	this->mask_filename = this->image_filename;
	Rect* subsection = this->get_image_subsection();
	std::string rect_string = "";
	if (subsection) {
		rect_string = ImageLoader::get_instance().rect_to_string(*subsection);
	}
	ImageLoader::get_instance().load_mask(this->mask_filename, rect_string, false);
}

mask_t * GameImage::get_mask()
{
	Rect* subsection = this->get_image_subsection();
	std::string rect_string = "";
	if (subsection) {
		rect_string = ImageLoader::get_instance().rect_to_string(*subsection);
	}
	return ImageLoader::get_instance().get_mask(this->mask_filename, rect_string);
}

std::string GameImage::get_anim_state_key()
{
	return get_anim_state_key(anim_state);
	
}

std::string GameImage::get_anim_state_key(int anim_state)
{
	switch (anim_state) {
	case ANIM_STATE_NEUTRAL:
		return "neutral";
	case ANIM_STATE_WALKING:
		return "walking";
	case ANIM_STATE_SHEARING:
		return "shearing";
	}
	return std::string();
}

int GameImage::get_animation_row()
{
	return get_animation_direction();
}

int GameImage::get_animation_direction()
{
	switch (direction) {
	case DIR_UP: return ANIM_UP;
	case DIR_DOWN: return ANIM_DOWN;
	case DIR_LEFT: return ANIM_LEFT;
	case DIR_RIGHT: return ANIM_RIGHT;
	case DIR_NEUTRAL: return ANIM_NEUTRAL;
	}
	return ANIM_NEUTRAL;
}

Animation * GameImage::get_animation()
{
	//TODO: don't accidentally create a null pointer here
	auto it = animations.find(get_anim_state_key());
	if (it == animations.end()) {
		return NULL;
	}
	return it->second;
}

SpriteSheetAnimation* GameImage::get_ss_animation()
{
	return ss_animation;
}

Rect * GameImage::get_image_subsection()
{
	return image_subsection;
}

std::vector<ALLEGRO_BITMAP*> GameImage::get_additional_image_layers()
{
	std::vector<ALLEGRO_BITMAP*> layers;
	for (std::pair<std::string, Rect> layer_data : this->additional_image_layer_data) {
		layers.push_back(ImageLoader::get_instance().get_image(layer_data.first, layer_data.second));
	}
	return layers;
}

std::pair<float, float> GameImage::get_rect_center()
{
	return std::pair<float, float>(rect.x + rect.width/2.0f, rect.y + rect.height/2.0f);
}

float GameImage::get_x()
{
	return rect.x;
}

float GameImage::get_y()
{
	return rect.y;
}

float GameImage::get_width()
{
	return rect.width;
}

float GameImage::get_height()
{
	return rect.height;
}

bool GameImage::contains_point(int x, int y)
{
	return x >= this->rect.x && y >= this->rect.y && x < this->get_x() + this->rect.width && y < this->rect.y + this->rect.height;
}

const int GameImage::calculate_direction(GameImage * other)
{
	const std::pair<int, int> pos_1 = this->get_center(), pos_2 = other->get_center();
	const int x_dist = std::abs(pos_1.first - pos_2.first), 
		y_dist = std::abs(pos_1.second - pos_2.second);
	if (x_dist > y_dist) {
		return pos_1.first > pos_2.first ? DIR_LEFT : DIR_RIGHT;
	} else {
		return pos_1.second > pos_2.second ? DIR_UP : DIR_DOWN;
	}
	return DIR_NEUTRAL;
}

const bool GameImage::intersects_area(Rect area)
{
	return rect.intersects_rect(area);
}

bool GameImage::outside_level(std::pair<int, int> level_dimensions)
{
	return rect.right() < 0 || rect.x > level_dimensions.first || rect.bottom() < 0 || rect.y > level_dimensions.second;
}

Animation * GameImage::load_animation_single_row(std::string filename, int row, std::pair<int, int> frame_dimensions)
{
	Animation *anim = new Animation();
	ImageLoader::get_instance().load_image(filename);	//load full spritesheet
	ALLEGRO_BITMAP *full_spritesheet = ImageLoader::get_instance().get_image(filename);
	std::pair<int, int> frame_count(al_get_bitmap_width(full_spritesheet)/frame_dimensions.first, 
		al_get_bitmap_height(full_spritesheet) / frame_dimensions.second);
	const int frame_duration = DEFAULT_EFFECT_FRAME_DURATION;
	anim->load_content(filename, frame_count, std::pair<int, int>(0, 0), frame_dimensions, frame_duration);
	ImageLoader::get_instance().load_spritesheet(*anim);
	return anim;
}