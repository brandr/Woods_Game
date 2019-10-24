#include "ExchangeInventoryScreen.h"

const bool ExchangeInventoryScreen::is_selecting_exchange_inventory()
{
	const int exchange_inv_rows = INVENTORY_ROWS;
	return this->inventory_selection.second >= 0 && this->inventory_selection.second < exchange_inv_rows;
}

const bool ExchangeInventoryScreen::allow_trash()
{
	return false;
}

ExchangeInventoryScreen::ExchangeInventoryScreen()
{
	InventoryScreen();
}


ExchangeInventoryScreen::~ExchangeInventoryScreen()
{
}

void ExchangeInventoryScreen::reset()
{
	this->dragging_selection = std::pair<int, int>(-1, -1);
	this->inventory = NULL;
	this->exchange_inventory = NULL;
}

void ExchangeInventoryScreen::draw(ALLEGRO_DISPLAY * display)
{
	const float screen_width = al_get_display_width(display), screen_height = al_get_display_height(display);
	ALLEGRO_BITMAP * backdrop_bitmap = this->inventory_backdrop_large();
	const float back_width = al_get_bitmap_width(backdrop_bitmap), back_height = al_get_bitmap_height(backdrop_bitmap);
	const float back_x = (screen_width - back_width) / 2.0f, back_y = (screen_height - back_height) / 2.0f;
	const float main_inv_off_x = 0, main_inv_off_y = screen_height / 3 - 36;
	const float exch_inv_height = INVENTORY_ROWS * al_get_bitmap_height(item_box_inventory());
	const float exch_off_x = 0, exch_off_y = main_inv_off_y - exch_inv_height - 36;
	al_draw_bitmap(backdrop_bitmap, back_x, back_y, NULL);
	//TODO: take which inventory we're looking at into account when showing selection square, dragged item, etc.
	//			could have an arg that says whether we should draw these at all, and/or pass in the selection pair as an arg 
	//          (adjust it based on which inventory we're in)
	
	const int exchange_rows = INVENTORY_ROWS;
	const bool is_in_exchange = this->is_selecting_exchange_inventory();
	const bool is_dragging_exchange = this->dragging_selection.second >= 0 && this->dragging_selection.second < exchange_rows;

	const int exchange_inv_select_y = is_in_exchange ? this->inventory_selection.second : -2;
	const int main_inv_select_y = 
		this->inventory_selection.second >= 0 ?
		is_in_exchange ? -2 : this->inventory_selection.second - exchange_rows :
		this->inventory_selection.second;

	const int exchange_drag_y = is_dragging_exchange ? this->dragging_selection.second : -2;
	const int main_inv_drag_y = is_dragging_exchange ? -2 : this->dragging_selection.second - exchange_rows;
	
	// draw the main inventory
	InventoryScreen::draw_inventory(display, this->inventory, main_inv_off_x, main_inv_off_y, 
		this->inventory_selection.first, main_inv_select_y, this->dragging_selection.first, main_inv_drag_y);
	InventoryScreen::draw_hotbar(display, this->inventory, main_inv_off_x, main_inv_off_y);
	// draw the exchange inventory
	InventoryScreen::draw_inventory(display, this->exchange_inventory, exch_off_x, exch_off_y, 
		this->inventory_selection.first, exchange_inv_select_y, this->dragging_selection.first, exchange_drag_y);
}

void ExchangeInventoryScreen::set_exchange_inventory(Inventory * inv)
{
	this->exchange_inventory = inv;
}

void ExchangeInventoryScreen::items_tab_select()
{
	int select_row = -1;
	const int exchange_rows = INVENTORY_ROWS;
	if (this->is_selecting_exchange_inventory()) {
		select_row = this->inventory_selection.second;
	} else if (this->selecting_internal_inventory()) {
		select_row = this->inventory_selection.second - exchange_rows;
	}
	const bool is_dragging_exch_inv = this->dragging_selection.second >= 0 && this->dragging_selection.second < exchange_rows;
	Inventory * source_inventory = is_dragging_exch_inv ? this->exchange_inventory : this->inventory;
	Inventory * dest_inventory = this->is_selecting_exchange_inventory() ? this->exchange_inventory : this->inventory;
	const std::pair<int, int> selection(inventory_selection.first, select_row);
	Item* item = selected_item();
	if (dragging_item() != NULL) {
		if (item == NULL) {
			dest_inventory->set_item(dragging_item(), selection.first, selection.second);
			source_inventory->set_item(NULL, dragging_selection.first, dragging_selection.second);
			dragging_selection = std::pair<int, int>(-1, -1);
		} else {
			const int drag_row = dragging_selection.second < 0 || is_dragging_exch_inv ? dragging_selection.second : dragging_selection.second - exchange_rows;
			dest_inventory->swap_items(source_inventory, selection.first, selection.second, dragging_selection.first, drag_row);
			dragging_selection = std::pair<int, int>(-1, -1);
		}
	}
	else {
		if (item == NULL || item->is_empty()) return;
		// allow it to go above the number of rows so we can drag between inventories
		dragging_selection = std::pair<int, int>(this->inventory_selection.first, this->inventory_selection.second);
	}
}

const bool ExchangeInventoryScreen::has_exchange_inventory()
{
	return this->exchange_inventory != NULL;
}

const bool ExchangeInventoryScreen::selecting_internal_inventory()
{
	if (!this->has_inventory()) {
		return false;
	}
	if (this->is_selecting_exchange_inventory()) {
		return true;
	}
	const int inv_select_x = inventory_selection.first, inv_select_y = inventory_selection.second;
	if (inv_select_x < 0 || inv_select_y < 0) {
		return false;
	}
	return true;
}

Item * ExchangeInventoryScreen::selected_item()
{
	if (is_selecting_exchange_inventory()) {
		return this->exchange_inventory->get_item(inventory_selection.first, inventory_selection.second);
	} else if (selecting_internal_inventory()) {
		const int exchange_rows = INVENTORY_ROWS;
		return inventory->get_item(inventory_selection.first, inventory_selection.second - exchange_rows);
	}
	return inventory->get_selected_hotbar_item();
}

Item * ExchangeInventoryScreen::dragging_item()
{
	int drag_x = dragging_selection.first, drag_y = dragging_selection.second;
	if (drag_y < 0) {
		if (drag_x < 0) return NULL;
		return inventory->get_hotbar_item(drag_x);
	}
	if (drag_x < 0) return NULL;
	const int exchange_rows = INVENTORY_ROWS;
	Inventory * drag_inventory = drag_y < exchange_rows ? exchange_inventory : inventory;
	if (drag_y >= exchange_rows) {
		drag_y = drag_y - exchange_rows;
	}
	return drag_inventory->get_item(drag_x, drag_y);
}

const int ExchangeInventoryScreen::num_inventory_rows()
{
	const int main_inv_rows = INVENTORY_ROWS;
	const int exchange_inv_rows = INVENTORY_ROWS;
	return main_inv_rows + exchange_inv_rows;
}

const int ExchangeInventoryScreen::num_inventory_cols()
{
	return INVENTORY_COLS;
}

//TODO: override some or all of the code in input methods so we can drag between inventories