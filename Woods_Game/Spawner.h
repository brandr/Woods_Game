#ifndef SPAWNER_H
#define SPAWNER_H

#include "Entity.h"
#include "XMLSerialization.h"
/*
struct SpawnerData : public xmls::Serializable {
	xmls::xString spawner_key;
	xmls::xInt spawner_index;
	SpawnerData();
};
*/
class Spawner : public Entity, public xmls::Serializable
{
private:
	xmls::xString spawner_id;
public:
	Spawner();
	~Spawner();
	virtual void set_content(std::string image_filename, Rect* image_subsection, std::pair<int, int> position);
	const std::string get_spawner_id();
};

#endif