#include "InventoryScreen.h"

InventoryScreen::InventoryScreen()
{
}


InventoryScreen::~InventoryScreen()
{
}

void InventoryScreen::load_content()
{
	// load images
	ImageLoader::get_instance().load_image("ui/inventory_backdrop");
	ImageLoader::get_instance().load_image("ui/item_box_1");
	ImageLoader::get_instance().load_image("ui/item_box_1_light");
	ImageLoader::get_instance().load_image("ui/item_box_1_dark");
	ImageLoader::get_instance().load_image("ui/item_selection_1");
	inventory_backdrop = ImageLoader::get_instance().get_image("ui/inventory_backdrop");
	item_box_hotbar = ImageLoader::get_instance().get_image("ui/item_box_1");
	item_box_selected = ImageLoader::get_instance().get_image("ui/item_box_1_light");
	item_box_inventory = ImageLoader::get_instance().get_image("ui/item_box_1_dark");
	item_drag_selection = ImageLoader::get_instance().get_image("ui/item_selection_1");
	
	// load font
	font_map[FONT_HOTBAR] = al_load_font("resources/fonts/OpenSans-Regular.ttf", 12, NULL);
}

void InventoryScreen::draw(ALLEGRO_DISPLAY * display)
{
	const int x = (al_get_display_width(display) - al_get_bitmap_width(inventory_backdrop))/2;
	const int y = (al_get_display_height(display) - al_get_bitmap_height(inventory_backdrop)) / 2;
	al_draw_bitmap(inventory_backdrop, x, y, NULL);
	draw_inventory(display);
	draw_hotbar(display);
	
}

void InventoryScreen::draw_inventory(ALLEGRO_DISPLAY * display)
{
	const std::vector<std::vector<Item*>> inventory_items = inventory->get_inventory_items();
	const int width = al_get_display_width(display);
	const int height = al_get_display_height(display);
	const int box_width = al_get_bitmap_width(item_box_hotbar);
	const int box_height = al_get_bitmap_height(item_box_hotbar);
	const int backdrop_height = al_get_bitmap_height(inventory_backdrop);
	const int rows = inventory_items.size();
	const int cols = inventory_items[0].size();
	for (int row = 0; row < rows; row++) {
		const float y = (height - backdrop_height)/2.0  + box_height*row + 32.0;
		for (int col = 0; col < cols; col++) {
			const float x = (width - box_width*cols) / 2.0 + col*box_width;
			if (col == inventory_selection.first && row == inventory_selection.second) {
				al_draw_bitmap(item_box_selected, x, y, 0);
			}
			else {
				al_draw_bitmap(item_box_inventory, x, y, 0);
			}
			Item* item = inventory->get_item(col, row);
			if (item && !(dragging_selection.first == col && dragging_selection.second == row))
				item->draw(display, x, y);
			if (col == dragging_selection.first && row == dragging_selection.second) {
				al_draw_bitmap(item_drag_selection, x, y, 0);
			}
		}
	}
	if (selecting_internal_inventory() && dragging_item()) {
		const float drag_x = (width - box_width*cols) / 2 + inventory_selection.first*box_width;
		const float drag_y = (height - backdrop_height) / 2 + box_height*inventory_selection.second + 32;
		dragging_item()->draw(display, drag_x, drag_y);
	}
}

void InventoryScreen::draw_hotbar(ALLEGRO_DISPLAY * display)
{
	std::vector<Item*> hotbar = inventory->get_hotbar();
	const int width = al_get_display_width(display);
	const int height = al_get_display_height(display);
	const int box_width = al_get_bitmap_width(item_box_hotbar);
	const int box_height = al_get_bitmap_height(item_box_hotbar);
	const int hotbar_index = inventory->get_hotbar_index();
	const float y = (height + al_get_bitmap_height(inventory_backdrop))/2 - box_height - 36;
	const int size = hotbar.size();
	for (int i = 0; i < size; i++) {
		const float x = (width - box_width*size) / 2 + i*box_width;
		const int num = i < 9 ? i + 1 : 0;
		if (!selecting_internal_inventory() && i == hotbar_index) {
			al_draw_bitmap(item_box_selected, x, y, 0);
		}
		else {
			al_draw_bitmap(item_box_hotbar, x, y, 0);
		}
		if (i == dragging_selection.first && dragging_selection.second < 0) {
			al_draw_bitmap(item_drag_selection, x, y, 0);
		}
		//const std::vector<int> rgb = MenuItem::string_to_rgb(FONT_COLOR_HOTBAR);
		//al_draw_text(font_map[FONT_HOTBAR], al_map_rgb(rgb[0], rgb[1], rgb[2]), x + 8.0f, y + 5.0f, 0, std::to_string(num).c_str());
		Item* item = hotbar[i];
		if (item && !(dragging_selection.first == i && dragging_selection.second < 0)) 
			item->draw(display, x, y);
	}
	if (!selecting_internal_inventory() && dragging_item()) {
		const float drag_x = (width - box_width*size) / 2 + inventory_selection.first*box_width;
		dragging_item()->draw(display, drag_x , y);
	}
}

void InventoryScreen::reset()
{
	dragging_selection = std::pair<int, int>(-1, -1);
}

void InventoryScreen::menu_up()
{
	const int rows = inventory->get_inventory_items().size();
	const int x = inventory_selection.first;
	const int y = inventory_selection.second == -1 ? rows - 1 : inventory_selection.second - 1;
	inventory_selection = std::pair<int, int>(x, y);
	if (y == -1) {
		inventory->set_hotbar_index(x);
	}
}

void InventoryScreen::menu_down()
{
	const int rows = inventory->get_inventory_items().size();
	const int x = inventory_selection.first;
	const int y = inventory_selection.second == rows - 1 ? -1 : inventory_selection.second + 1;
	inventory_selection = std::pair<int, int>(x, y);
	if (y == -1) {
		inventory->set_hotbar_index(x);
	}
}

void InventoryScreen::menu_left()
{
	const int cols = inventory->get_inventory_items()[0].size();
	int x = inventory_selection.first == 0 ? cols - 1 : inventory_selection.first - 1;
	const int y = inventory_selection.second;	
	inventory_selection = std::pair<int, int>(x, y);
	if(y == -1){
		inventory->hotbar_index_left();
	}
}

void InventoryScreen::menu_right()
{
	const int cols = inventory->get_inventory_items()[0].size();
	const int x = inventory_selection.first == cols - 1 ? 0 : inventory_selection.first + 1;
	const int y = inventory_selection.second;
	inventory_selection = std::pair<int, int>(x, y);
	if (y == -1) {
		inventory->hotbar_index_right();
	}
}

void InventoryScreen::select()
{
	Item* item = selected_item();
	if (dragging_item() != NULL) {
		if (item == NULL) {
			inventory->set_item(dragging_item(), inventory_selection.first, inventory_selection.second);
			inventory->set_item(NULL, dragging_selection.first, dragging_selection.second);
			dragging_selection = std::pair<int, int>(-1, -1);
		}
		else {
			inventory->swap_items(inventory_selection.first, inventory_selection.second, dragging_selection.first, dragging_selection.second);
			dragging_selection = std::pair<int, int>(-1, -1);
		}
	}
	else {
		if (item == NULL) return;
		dragging_selection = std::pair<int, int>(inventory_selection.first, inventory_selection.second);
	}
}

void InventoryScreen::set_inventory(Inventory * inventory)
{
	this->inventory = inventory;
	inventory_selection = std::pair<int, int>(inventory->get_hotbar_index(), -1);
}

bool InventoryScreen::selecting_internal_inventory()
{
	const int inv_rows = inventory->get_inventory_items().size();
	const int inv_cols = inventory->get_inventory_items()[0].size();
	const int inv_select_x = inventory_selection.first, inv_select_y = inventory_selection.second;
	return inv_select_x >= 0 && inv_select_y >= 0 && inv_select_x < inv_cols && inv_select_y < inv_rows;
}

Item * InventoryScreen::selected_item()
{
	if (selecting_internal_inventory()) {
		return inventory->get_item(inventory_selection.first, inventory_selection.second);
	}
	return inventory->get_selected_hotbar_item();
}

Item * InventoryScreen::dragging_item()
{
	const int drag_x = dragging_selection.first, drag_y = dragging_selection.second;
	if (drag_y < 0) {
		if (drag_x < 0) return NULL;
		return inventory->get_hotbar_item(drag_x);
	}
	if (drag_x < 0) return NULL;
	return inventory->get_item(drag_x, drag_y);
}
