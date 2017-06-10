#include "GameImage.h"
#include "ImageLoader.h"
#include<iostream>

GameImage::GameImage(std::string filename)
{
	//this->image_filename = filename;
}

GameImage::GameImage()
{
	//TEMP: actually set width and height upon loading image
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
	//TODO: allow for mulitple animations (map of string->animation)
	std::string anim_filename = "";
	std::pair<int, int> ss_frame_dimensions;
	std::vector<std::string> anim_keys;
	int size = attributes.size();
	std::map<std::string, std::pair<int, int>> frame_counts;
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
			center_offset = FileManager::string_to_pair(contents[i]);
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
			//...
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
			anim->load_content(anim_filename + "_" + key, frame_count, std::pair<int, int>(0, 0), ss_frame_dimensions);
			ImageLoader::get_instance().load_spritesheet(*anim);
			animations[key] = anim;
		}
	}
	mask = NULL; //TEMP
	if (anim_filename != "") {
		load_mask(anim_filename);
		/*]
		ALLEGRO_BITMAP *mask_image = ImageLoader::get_instance().get_image(anim_filename + "_mask");
		if (!mask_image) {
			ImageLoader::get_instance().load_image(anim_filename + "_mask");
			mask_image = ImageLoader::get_instance().get_image(anim_filename + "_mask");
		}
		if (mask_image) {
			mask = Mask_New(mask_image);
		}
		//TODO: consider loading mask from sprite by default if a mask image is not found.
		mask_image = NULL;
		*/
	}
}

void GameImage::set_content(std::string image_filename, Rect* image_subsection, std::pair<int, int> position)
{
	this->image_filename = image_filename;
	this->image_subsection = image_subsection;
	rect.x = position.first, rect.y = position.second;
	//refresh_mask();
}

void GameImage::load_mask(std::string base_filename)
{
	//std::cout << "loading mask for type " << get_type() << std::endl;
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

void GameImage::unload_content()
{
	if(image_subsection)
		delete image_subsection;
	if(ss_animation)
		delete ss_animation;
	if(mask)
		Mask_Delete(mask);
	for (auto const &it : animations) {
		it.second->unload_content();
		delete it.second;
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
	if (ss_animation) draw_bitmap = ImageLoader::get_instance().get_current_image(this);
	//bitmap = ImageLoader::get_instance().get_current_image(this);
	if (draw_bitmap && rect.x + x_offset < DEFAULT_SCREEN_WIDTH && rect.right() + x_offset > 0 && 
		rect.y + y_offset < DEFAULT_SCREEN_HEIGHT && rect.bottom() + y_offset > 0) {
		al_draw_bitmap(bitmap, rect.x + x_offset, rect.y + y_offset, 0);
		
	}
	

}

void GameImage::update()
{
	if (ss_animation) {	//TEMP: might sometimes only have animation
		Animation* anim = get_animation();
		if (anim) {
			//TODO: figure out how to set animation direction
			//anim->set_row(get_animation_index());
			anim->set_row(get_animation_direction());
			ss_animation->update(*anim);
		}
		//std::string anim_key = get_anim_key();
		//TODO
		
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
	this->center_offset = offset;
}

const std::pair<int, int> GameImage::get_center() const
{
	return std::pair<int, int>(static_cast<int>(rect.x) + center_offset.first, static_cast<int>(rect.y) + center_offset.second);
}

void GameImage::set_bitmap(ALLEGRO_BITMAP * bitmap)
{
	this->bitmap = bitmap;
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

/*
int GameImage::get_animation_index()
{
	auto it = animation_dir_map.find(std::pair<int, int>(anim_state, direction));
	if (it == animation_dir_map.end()) return 0;
	return it->second;
}
*/

mask_t * GameImage::get_mask()
{
	return mask;
}

std::string GameImage::get_anim_state_key()
{
	switch (anim_state) {
	case ANIM_STATE_NEUTRAL:
		return "neutral";
	case ANIM_STATE_WALKING:
		return "walking";
	}
		
	return std::string();
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
	return 0;
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