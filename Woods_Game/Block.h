#ifndef BLOCK_H
#define BLOCK_H

#include "Entity.h"  // for Entity
#include "utility"   // for pair
#include "xstring"   // for string

struct Rect;

class Block :
	public Entity
{
public:
	Block();
	Block(int, int);	//TODO: might delete this with new entity manager system
	~Block();
	virtual void set_content(std::string image_filename, Rect* image_subsection, std::pair<int, int> position);
	virtual int get_type();
};

#endif