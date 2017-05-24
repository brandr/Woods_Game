#include<allegro5/allegro.h>
#include<allegro5/allegro_image.h>
#include<unordered_map>
#include<map>
#include<memory>
#include "Player.h"

class ImageLoader
{
private:
	ImageLoader() {}//;
	std::map<std::pair<std::string, std::string>, ALLEGRO_BITMAP*> image_map;
	//std::map<std::pair<std::string, std::string>, std::shared_ptr<ALLEGRO_BITMAP>> image_map;
	std::string rect_to_string(Rect r);
public:
	ImageLoader(ImageLoader const&) = delete;
	void operator=(ImageLoader const&) = delete;
	static ImageLoader& get_instance();
	static std::string full_filename(std::string);
	~ImageLoader();
	void load_content();
	void unload_content();
	void load_image(std::string filename);
	void load_image(std::string filename, Rect subsection);
	void load_spritesheet(Animation);
	ALLEGRO_BITMAP* get_current_image(GameImage*);
	ALLEGRO_BITMAP* get_image(std::string filename);
	ALLEGRO_BITMAP* get_image(std::string filename, Rect subsection);
};

