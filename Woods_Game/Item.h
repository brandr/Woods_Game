#include "Being.h"

enum ITEM_KEYS{ITEM_SHEARS, ITEM_MALLET};
enum ITEM_ATTRIBUTES{ITEM_ATTR_POWER};

class Item
{
private:
	ALLEGRO_BITMAP *icon;
	int item_key;
	std::string filename;
	std::map<int, int> item_attributes;
	
public:
	Item();
	~Item();
	Item(Item* copy);
	void load_content(std::string filename, int item_key);
	void unload_content();
	void draw(ALLEGRO_DISPLAY *display, float x, float y);
	const int get_item_key();
	const std::string get_filename();
	void set_item_attributes(std::map<int, int> attributes);
	void set_item_attribute(int attr_key, int attr_val);
	const std::map<int, int> get_item_attributes();
	const int get_item_attribute(int attr_key);
};

