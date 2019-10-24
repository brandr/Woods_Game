#ifndef EXCHANGE_INVENTORY_SCREEN_H
#define EXCHANGE_INVENTORY_SCREEN_H

#include "InventoryScreen.h"

class ExchangeInventoryScreen :
	public InventoryScreen
{
private:
	Inventory * exchange_inventory = NULL;
	const bool is_selecting_exchange_inventory();
protected:
	virtual const bool allow_trash();
public:
	ExchangeInventoryScreen();
	~ExchangeInventoryScreen();
	virtual void reset();
	virtual void draw(ALLEGRO_DISPLAY *display);
	void set_exchange_inventory(Inventory * inv);
	virtual void items_tab_select();
	const bool has_exchange_inventory();
	virtual const bool selecting_internal_inventory();
	virtual Item* selected_item();
	virtual Item* dragging_item();
	virtual const int num_inventory_rows();
	virtual const int num_inventory_cols();
};

#endif