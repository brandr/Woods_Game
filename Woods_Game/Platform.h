#include "Entity.h"
class Platform :
	public Entity
{
public:
	Platform();
	Platform(int, int);	//TODO: might delete this with new entity manager system
	~Platform();
	virtual int get_type();
};

