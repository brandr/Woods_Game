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
	virtual void items_tab_mouse_cursor_update(Inventory * cursor_inv, const float mouse_x, const float mouse_y, const int x_off, const int y_off, const bool is_exchange);
	virtual void items_tab_left_click(Inventory * click_inv, const float mouse_x, const float mouse_y, const int x_off, const int y_off, const bool is_exchange);
public:
	ExchangeInventoryScreen();
	~ExchangeInventoryScreen();
	virtual void reset();
	virtual void draw(ALLEGRO_DISPLAY *display);
	virtual void mouse_cursor_update();
	virtual void process_mouse_click_left(const int x, const int y);
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