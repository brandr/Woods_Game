#ifndef ITEM_PICKUP_H
#define ITEM_PICKUP_H

#define PICKUP_WIDTH 32
#define PICKUP_HEIGHT 32

#include "Item.h"
#include "Being.h"
#include "Entity.h"

class Level;
class World;

class ItemPickup : public Being
{
private:
	Item * item = NULL;
protected:
public:
	ItemPickup();
	ItemPickup(const int item_key, const int x_pos, const int y_pos);
	~ItemPickup();
	virtual int get_type();
	Item * get_pickup_item();
};

#endif