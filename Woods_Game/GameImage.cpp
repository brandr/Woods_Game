#include "GameImage.h"
#include "ImageLoader.h"

GameImage::GameImage(std::string filename)
{
	//this->image_filename = filename;
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
	return image_filename;
}

void GameImage::load_content(std::vector<std::string> attributes, std::vector<std::string> contents)
{
	std::string anim_filename = "";
	std::pair<int, int> ss_frame_dimensions;
	std::vector<std::string> anim_keys;
	int size = attributes.size();
	std::map<std::string, std::pair<int, int>> frame_counts;
	std::map<std::string, int> frame_durations;
	//TODO: some way to check whether a mask image exists
	for (int i = 0; i < size; i++) {
		if (attributes[i] == "image") {
			image_filename = contents[i];
			ImageLoader::get_instance().load_image(contents[i]);
		}
		else if (attributes[i] == "position") {
			std::string tile_string = contents[i];
			rect.x = atoi(tile_string.substr(0, tile_string.find(',')).c_str());
			rect.y = atoi(tile_string.substr(tile_string.find(',') + 1).c_str());
		}	
		else if (attributes[i] == "spritesheet") {
			anim_filename = "sprite_sheets/" + contents[i];
			ss_animation = new SpriteSheetAnimation();
		}
		else if (attributes[i] == "center_offset") {
			std::pair<int,int> center_offset = FileManager::string_to_pair(contents[i]);
			this->center_offset_x = center_offset.first;
			this->center_offset_y = center_offset.second;
		}
		else if (attributes[i] == "anim_keys") {
			std::string keys_string = contents[i];
			std::string delimiter = ",";
			size_t pos = 0;
			std::string key;
			while ((pos = keys_string.find(delimiter)) != std::string::npos) {
				key = keys_string.substr(0, pos);
				anim_keys.push_back(key);
				keys_string.erase(0, pos + delimiter.length());
			}
			//last iteration
			anim_keys.push_back(keys_string);
		}
		else if (attributes[i] == "ss_frame_count") {
			std::string frame_count_list_str = contents[i];
			std::string delimiter = ";";

			size_t pos = 0;
			std::string frame_count_str;
			while ((pos = frame_count_list_str.find(delimiter)) != std::string::npos) {
				frame_count_str = frame_count_list_str.substr(0, pos);
				std::string anim_key_str = frame_count_str.substr(0, frame_count_str.find(":"));
				std::string pair_str = frame_count_str.substr(frame_count_str.find(":") + 1).c_str();
				int x = atoi(pair_str.substr(0, pair_str.find(',')).c_str());
				int y = atoi(pair_str.substr(pair_str.find(',') + 1).c_str());
				frame_counts[anim_key_str] = std::pair<int, int>(x, y);
				frame_count_list_str.erase(0, pos + delimiter.length());
			}

			//last iteration
			frame_count_str = frame_count_list_str;
			std::string anim_key_str = frame_count_str.substr(0, frame_count_str.find(":"));
			std::string pair_str = frame_count_str.substr(frame_count_str.find(":") + 1).c_str();
			int x = atoi(pair_str.substr(0, pair_str.find(',')).c_str());
			int y = atoi(pair_str.substr(pair_str.find(',') + 1).c_str());
			frame_counts[anim_key_str] = std::pair<int, int>(x, y);
			frame_count_list_str.erase(0, pos + delimiter.length());
		}
		else if (attributes[i] == "ss_frame_duration") {
			std::vector<std::string> dur_datas = FileManager::string_to_parts(contents[i], ",");
			for (int i = 0; i < dur_datas.size(); i++) {
				std::pair<std::string, std::string> dur_parts = FileManager::string_to_pair(dur_datas[i], ":");
				frame_durations[dur_parts.first] = ::atoi(dur_parts.second.c_str());
			}
		}
		else if (attributes[i] == "ss_frame_dimensions") {
			std::string str = contents[i];
			int x = atoi(str.substr(0, str.find(',')).c_str());
			int y = atoi(str.substr(str.find(',') + 1).c_str());
			ss_frame_dimensions = std::pair<int, int>(x, y);
		}
	}
	if (ss_animation) {
		int keys_size = anim_keys.size();
		
		for (int i = 0; i < keys_size; i++) {
			std::string key = anim_keys[i];
			Animation *anim = new Animation();
			std::pair<int, int> frame_count = frame_counts[key]; 
			int frame_duration = frame_durations[key];
			anim->load_content(anim_filename + "_" + key, frame_count, std::pair<int, int>(0, 0), ss_frame_dimensions, frame_duration);
			ImageLoader::get_instance().load_spritesheet(*anim);
			animations[key] = anim;
		}
	}
	mask = NULL; 
	if (anim_filename != "") {
		load_mask(anim_filename);
	}
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
	this->mask = NULL;
	load_mask(anim_filename);
}

void GameImage::set_content(std::string image_filename, Rect* image_subsection, std::pair<int, int> position)
{
	this->image_filename = image_filename;
	this->image_subsection = image_subsection;
	rect.x = position.first, rect.y = position.second;
}

void GameImage::load_mask(std::string base_filename)
{
	ALLEGRO_BITMAP *mask_image = ImageLoader::get_instance().get_image(base_filename + "_mask");
	if (!mask_image) {
		ImageLoader::get_instance().load_image(base_filename + "_mask");
		mask_image = ImageLoader::get_instance().get_image(base_filename + "_mask");
	}
	if (mask_image) {
		mask = Mask_New(mask_image);
	}
	//TODO: consider loading mask from sprite by default if a mask image is not found.
	mask_image = NULL;
}


void GameImage::load_additional_masks(std::vector<std::string> attributes, std::vector<std::string> contents, std::string prefix)
{
	int size = attributes.size();
	std::string mask_key = "";
	std::pair<int,int> frame_dimensions;
	int frame_count = 0;
	for (int i = 0; i < size; i++) {
		if (attributes[i] == "filename") {
			mask_key = contents[i];
		}
		else if (attributes[i] == "ss_frame_dimensions") {
			frame_dimensions = FileManager::string_to_pair(contents[i]);
		}
		else if (attributes[i] == "ss_frame_count") {
			frame_count = ::atoi(contents[i].c_str());
		}
	}
	ALLEGRO_BITMAP* mask_image;
	std::string filename = "sprite_sheets/" + prefix + "_" + mask_key + "_spritesheet_mask";
	for (int row = 0; row < frame_count; row++) {
		Rect sub(0.0f, frame_dimensions.second*row, frame_dimensions.first, frame_dimensions.second);
		ImageLoader::get_instance().load_image(filename, sub);
		mask_image = ImageLoader::get_instance().get_image(filename, sub);
		additional_masks[std::pair<std::string, int>(mask_key, row)] = Mask_New(mask_image);
	}
	mask_image = NULL;
}

void GameImage::load_additional_masks_from_attributes(std::string prefix)
{
	const int additional_mask_count = this->additional_mask_data.size();
	for (int i = 0; i < additional_mask_count; i++) {
		MaskData *mask_data = this->additional_mask_data.getItem(i);
		std::string mask_key = mask_data->mask_key.value();
		int mask_width = mask_data->mask_width.value();
		int mask_height = mask_data->mask_height.value();
		ALLEGRO_BITMAP* mask_image;
		std::string filename = "sprite_sheets/" + prefix + "_" + mask_key + "_spritesheet_mask";
		int frame_count = mask_data->mask_frame_count.value();
		std::pair<int, int> frame_dimensions(mask_width, mask_height);
		for (int row = 0; row < frame_count; row++) {
			Rect sub(0.0f, frame_dimensions.second*row, frame_dimensions.first, frame_dimensions.second);
			ImageLoader::get_instance().load_image(filename, sub);
			mask_image = ImageLoader::get_instance().get_image(filename, sub);
			additional_masks[std::pair<std::string, int>(mask_key, row)] = Mask_New(mask_image);
		}
		mask_image = NULL;
	}
}


void GameImage::unload_content()
{
	if (image_subsection) {
		delete image_subsection;
	}
	if(ss_animation)
		delete ss_animation;
	if(mask)
		Mask_Delete(mask);
	for (auto const &it : animations) {
		it.second->unload_content();
		delete it.second;
	}
	for (auto const &it : additional_masks) {
		Mask_Delete(it.second);
	}
	animations.clear();
	animations.swap(std::map<std::string, Animation*>());
	image_subsection = NULL;
	ss_animation = NULL;
	mask = NULL;
}

void GameImage::draw(ALLEGRO_DISPLAY *display, int x_offset, int y_offset)
{
	ALLEGRO_BITMAP* draw_bitmap = this->bitmap;
	if (ss_animation) {
		draw_bitmap = ImageLoader::get_instance().get_current_image(this);
	}
	//bitmap = ImageLoader::get_instance().get_current_image(this);
	if (draw_bitmap && rect.x + x_offset < al_get_display_width(display) && rect.right() + x_offset > 0 && 
		rect.y + y_offset < al_get_display_height(display) && rect.bottom() + y_offset > 0) {
		al_draw_bitmap(draw_bitmap, rect.x + x_offset, rect.y + y_offset, 0);
		int size = additional_image_layers.size();
		for (int i = 0; i < size; i++) {
			al_draw_bitmap(additional_image_layers[i], rect.x + x_offset, rect.y + y_offset, 0);
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

ALLEGRO_BITMAP* GameImage::get_bitmap()
{
	return this->bitmap;
}

void GameImage::set_bitmap(ALLEGRO_BITMAP * bitmap)
{
	this->bitmap = bitmap;
}

void GameImage::draw_onto_bitmap(ALLEGRO_BITMAP * bitmap)
{
	additional_image_layers.push_back(bitmap);
}

void GameImage::refresh_bitmap()
{
	this->bitmap = ImageLoader::get_instance().get_current_image(this);
}

void GameImage::refresh_mask()
{
	if (mask) {
		Mask_Delete(mask);
		mask = NULL;
	}
	ALLEGRO_BITMAP *bitmap = ImageLoader::get_instance().get_current_image(this);
	mask = Mask_New(bitmap);
}

mask_t * GameImage::get_mask()
{
	return mask;
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
		return nullptr;
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
	return this->additional_image_layers;
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
	//TODO: also account for pixel-perfect collision (might do this another place though)
	return x >= this->rect.x && y >= this->rect.y && x < this->get_x() + this->rect.width && y < this->rect.y + this->rect.height;
}

bool GameImage::intersects_area(Rect area)
{
	return rect.intersects_rect(area);
}

/*
void GameImage::set_current_level(Level* level)
{
	current_level = level;
}
*/

bool GameImage::outside_level(std::pair<int, int> level_dimensions)
{
	//if (!current_level) return true;
	return rect.right() < 0 || rect.x > level_dimensions.first || rect.bottom() < 0 || rect.y > level_dimensions.second;
}

Animation * GameImage::load_animation_single_row(std::string filename, int row, std::pair<int, int> frame_dimensions)
{
	
	//TODO
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


/*
inline bool center_comparsion::operator()(const GameImage & gi1, const GameImage & gi2)
{
	return (gi1.get_center().second < gi2.get_center().second);
}
*/
/*
bool operator<(const GameImage & g1, const GameImage & g2)
{
	return (g1.get_center().second < g2.get_center().second);
}

bool operator>(const GameImage & g1, const GameImage & g2)
{
	return (g1.get_center().second > g2.get_center().second);
}
*/