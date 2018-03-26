#ifndef BLOCK_H
#define BLOCK_H

#include "Entity.h"  // for Entity
#include "utility"   // for pair
#include "xstring"   // for string
#include "XMLSerialization.h"

struct Rect;

//TODO: what needs to be serialized for blocks?
class Block :
	public Entity, public xmls::Serializable
{
public:
	Block();
	Block(int, int);	//TODO: might delete this with new entity manager system
	~Block();
	bool is_empty();
	virtual void set_content(std::string image_filename, Rect* image_subsection, std::pair<int, int> position);
	virtual int get_type();
	virtual Rect *get_bitmap_subsection();
};

#endif