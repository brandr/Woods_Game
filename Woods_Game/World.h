#ifndef WORLD_H
#define WORLD_H
#include "Dungeon.h"
#include "DungeonData.h"
#include "XMLSerialization.h"
#include <memory>
class ImageLoader;

class World : public xmls::Serializable
{
private:
	std::vector <std::shared_ptr<Dungeon>> dungeons;
	xmls::xString current_dungeon_key;
	xmls::xString player_key;
	xmls::xInt current_level_grid_x;
	xmls::xInt current_level_grid_y;
	xmls::Collection<DungeonData> dungeon_data;
public:	
	Dungeon* current_dungeon;
	World();
	~World();
	void load_dungeons();
	void load_player();
	void unload_content();
	void load_images(ImageLoader&);
	void add_dungeon(Dungeon*);
	Dungeon* get_current_dungeon();
	Level* get_current_level();
	Dungeon* get_dungeon(std::string dungeon_name);
	std::string get_player_key();
};

#endif