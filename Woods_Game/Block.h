#include "Entity.h"
class Block :
	public Entity
{
public:
	Block();
	Block(int, int);	//TODO: might delete this with new entity manager system
	~Block();
	virtual int get_type();
};

