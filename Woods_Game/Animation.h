#include<string>
#include<vector>
#include<allegro5/allegro_primitives.h>
#include "FileManager.h"

struct Rect {
	int x, y, width, height;
public:
	Rect();
	Rect(int, int, int, int);
	bool intersects_rect(Rect);
	int right();
	int bottom();	
};

class Animation
{
public:
	Animation();
	~Animation();
	void load_content(std::string, std::pair<int,int>, std::pair<int, int>, std::pair<int, int>);
	void unload_content();
	void draw(ALLEGRO_DISPLAY*, int, int);
	virtual void update();
	bool is_active();
	void set_filename(std::string);
	std::string get_filename();
	Rect get_current_rect();
	std::pair<int, int> &get_frame_count();
	std::pair<int, int> &get_current_frame();
	std::pair<int, int> &get_frame_dimensions();
	std::pair<int, int> get_image_dimensions();
protected:
	//TODO: necessary data to to get the image via imageloader
	float alpha;
	bool active;
	std::string filename;
	std::pair<int, int> frame_count;
	std::pair<int, int> current_frame;
	std::pair<int, int> frame_dimensions;
};

