#include "Entity.h"

struct EntityComponentData {
	std::string name = "";
	std::vector<std::string> attributes;
	EntityComponentData();
	EntityComponentData(std::string name, std::vector<std::string> attributes);
};

class EntityGroup:
	public Entity
{
private:
	std::vector<Entity*> entities;
public:
	EntityGroup();
	//EntityGroup(std::pair<int, int> root_offset, std::vector<EntityComponentData> component_data);
	~EntityGroup();
	virtual int get_type();
	void set_entities(std::vector<Entity*> entities);
	virtual void draw(ALLEGRO_DISPLAY* display, int x_offset, int y_offset);
};

