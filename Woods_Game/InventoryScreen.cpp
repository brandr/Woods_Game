#include "InventoryScreen.h"
#include "Level.h"

ALLEGRO_BITMAP * InventoryScreen::inventory_backdrop()
{
	return ImageLoader::get_instance().get_image("ui/inventory_backdrop");
}

ALLEGRO_BITMAP * InventoryScreen::inventory_tab_selected()
{
	return ImageLoader::get_instance().get_image("ui/inventory_tab_selected");
}

ALLEGRO_BITMAP * InventoryScreen::inventory_tab_deselected()
{
	return ImageLoader::get_instance().get_image("ui/inventory_tab_deselected");
}

// items

ALLEGRO_BITMAP * InventoryScreen::item_box_hotbar()
{
	return ImageLoader::get_instance().get_image("ui/item_box_1");
}

ALLEGRO_BITMAP * InventoryScreen::item_box_selected()
{
	return ImageLoader::get_instance().get_image("ui/item_box_1_light");
}

ALLEGRO_BITMAP * InventoryScreen::item_box_inventory()
{
	return ImageLoader::get_instance().get_image("ui/item_box_1_dark");
}

ALLEGRO_BITMAP * InventoryScreen::item_drag_selection()
{
	return ImageLoader::get_instance().get_image("ui/item_selection_1");
}

ALLEGRO_BITMAP * InventoryScreen::map_frame()
{
	return ImageLoader::get_instance().get_image("ui/map_frame");
}

ALLEGRO_BITMAP * InventoryScreen::map_current_location_icon()
{
	return ImageLoader::get_instance().get_image("ui/map_current_location_icon");
}

const std::string InventoryScreen::label_for_tab(const int index)
{
	switch (index) {
	case INVENTORY_TAB_ITEMS:
		return "Items";
	case INVENTORY_TAB_MAP:
		return "Map";
	case INVENTORY_TAB_JOURNAL:
		return "Journal";
	default:
		return "";
	}
}

void InventoryScreen::items_tab_menu_up()
{
	const int rows = inventory->get_inventory_items().size();
	const int x = inventory_selection.first;
	const int y = inventory_selection.second == -1 ? rows - 1 : inventory_selection.second - 1;
	inventory_selection = std::pair<int, int>(x, y);
	if (y == -1) {
		inventory->set_hotbar_index(x);
	}
}

void InventoryScreen::items_tab_menu_down()
{
	const int rows = inventory->get_inventory_items().size();
	const int x = inventory_selection.first;
	const int y = inventory_selection.second == rows - 1 ? -1 : inventory_selection.second + 1;
	inventory_selection = std::pair<int, int>(x, y);
	if (y == -1) {
		inventory->set_hotbar_index(x);
	}
}

void InventoryScreen::items_tab_menu_left()
{
	const int cols = inventory->get_inventory_items()[0].size();
	int x = inventory_selection.first == 0 ? cols - 1 : inventory_selection.first - 1;
	const int y = inventory_selection.second;
	inventory_selection = std::pair<int, int>(x, y);
	if (y == -1) {
		inventory->hotbar_index_left();
	}
}

void InventoryScreen::items_tab_menu_right()
{
	const int cols = inventory->get_inventory_items()[0].size();
	const int x = inventory_selection.first == cols - 1 ? 0 : inventory_selection.first + 1;
	const int y = inventory_selection.second;
	inventory_selection = std::pair<int, int>(x, y);
	if (y == -1) {
		inventory->hotbar_index_right();
	}
}

void InventoryScreen::items_tab_select()
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
	} else {
		if (item == NULL) return;
		dragging_selection = std::pair<int, int>(inventory_selection.first, inventory_selection.second);
	}
}

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
	ImageLoader::get_instance().load_image("ui/inventory_tab_selected");
	ImageLoader::get_instance().load_image("ui/inventory_tab_deselected");
	// items
	ImageLoader::get_instance().load_image("ui/item_box_1");
	ImageLoader::get_instance().load_image("ui/item_box_1_light");
	ImageLoader::get_instance().load_image("ui/item_box_1_dark");
	ImageLoader::get_instance().load_image("ui/item_selection_1");
	// map
	ImageLoader::get_instance().load_image("ui/map_frame");
	ImageLoader::get_instance().load_image("ui/map_current_location_icon");
}

void InventoryScreen::load_fonts()
{
	font_map[FONT_HOTBAR] = al_load_font("resources/fonts/OpenSans-Regular.ttf", 12, NULL);
	font_map[FONT_DIALOG] = al_load_font("resources/fonts/OpenSans-Regular.ttf", 12, NULL);
	font_map[FONT_TABS] = al_load_font("resources/fonts/OpenSans-Regular.ttf", 20, NULL);
}

void InventoryScreen::draw(ALLEGRO_DISPLAY * display)
{
	const int x = (al_get_display_width(display) - al_get_bitmap_width(inventory_backdrop())) / 2;
	const int y = (al_get_display_height(display) - al_get_bitmap_height(inventory_backdrop())) / 2;
	al_draw_bitmap(inventory_backdrop(), x, y, NULL);
	this->draw_tab_selection(display);
	switch (this->tab_index) {
	case INVENTORY_TAB_ITEMS:
		draw_inventory(display);
		draw_hotbar(display);
		break;
	case INVENTORY_TAB_MAP:
		draw_map(display);
		break;
	case INVENTORY_TAB_JOURNAL:
		//TODO
		break;
	default:
		break;
	}
}

void InventoryScreen::draw_tab_selection(ALLEGRO_DISPLAY * display)
{
	const int x_off = (al_get_display_width(display) - al_get_bitmap_width(inventory_backdrop())) / 2;
	const int y_off = (al_get_display_height(display) - al_get_bitmap_height(inventory_backdrop())) / 2;
	const int tab_spacing = 130;
	const int tab_height = 51;
	for (int i = 0; i < NUM_INVENTORY_TABS; i++) {
		ALLEGRO_BITMAP * tab_bitmap = this->tab_index == i ? this->inventory_tab_selected() : this->inventory_tab_deselected();
		al_draw_bitmap(tab_bitmap, x_off + i * tab_spacing, y_off - tab_height, 0);
		const std::string label = this->label_for_tab(i);
		al_draw_text(this->font_map[FONT_TABS], al_map_rgb(0, 0, 0), x_off + i * tab_spacing + 20, y_off - tab_height + 16, 0, label.c_str());
	}
}

void InventoryScreen::draw_inventory(ALLEGRO_DISPLAY * display)
{
	const std::vector<std::vector<Item*>> inventory_items = inventory->get_inventory_items();
	const int width = al_get_display_width(display);
	const int height = al_get_display_height(display);
	const int box_width = al_get_bitmap_width(item_box_hotbar());
	const int box_height = al_get_bitmap_height(item_box_hotbar());
	const int backdrop_height = al_get_bitmap_height(inventory_backdrop());
	const int rows = inventory_items.size();
	const int cols = inventory_items[0].size();
	for (int row = 0; row < rows; row++) {
		const float y = (height - backdrop_height)/2.0  + box_height*row + 32.0;
		for (int col = 0; col < cols; col++) {
			const float x = (width - box_width*cols) / 2.0 + col*box_width;
			if (col == inventory_selection.first && row == inventory_selection.second) {
				al_draw_bitmap(item_box_selected(), x, y, 0);
			} else {
				al_draw_bitmap(item_box_inventory(), x, y, 0);
			}
			Item* item = inventory->get_item(col, row);
			if (item && !(dragging_selection.first == col && dragging_selection.second == row))
				item->draw(display, x, y);
			if (col == dragging_selection.first && row == dragging_selection.second) {
				al_draw_bitmap(item_drag_selection(), x, y, 0);
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
	const int width = al_get_display_width(display);
	const int height = al_get_display_height(display);
	const int box_width = al_get_bitmap_width(item_box_hotbar());
	const int box_height = al_get_bitmap_height(item_box_hotbar());
	const int hotbar_index = inventory->get_hotbar_index();
	const float y = (height + al_get_bitmap_height(inventory_backdrop()))/2 - box_height - 36;
	const int size = HOTBAR_SIZE;
	for (int i = 0; i < size; i++) {
		const float x = (width - box_width*size) / 2 + i*box_width;
		if (!selecting_internal_inventory() && i == hotbar_index) {
			al_draw_bitmap(item_box_selected(), x, y, 0);
		}
		else {
			al_draw_bitmap(item_box_hotbar(), x, y, 0);
		}
		if (i == dragging_selection.first && dragging_selection.second < 0) {
			al_draw_bitmap(item_drag_selection(), x, y, 0);
		}
		Item* item = inventory->get_hotbar_item(i);
		if (item && !(dragging_selection.first == i && dragging_selection.second < 0)) 
			item->draw(display, x, y);
	}
	if (!selecting_internal_inventory() && dragging_item()) {
		const float drag_x = (width - box_width*size) / 2 + inventory_selection.first*box_width;
		dragging_item()->draw(display, drag_x , y);
	}
}

void InventoryScreen::draw_map(ALLEGRO_DISPLAY * display)
{
	const int width = al_get_display_width(display);
	const int height = al_get_display_height(display);

	ALLEGRO_BITMAP * map_frame_bitmap = this->map_frame();
	const int map_frame_width = al_get_bitmap_width(map_frame_bitmap), map_frame_height = al_get_bitmap_height(map_frame_bitmap);

	const float x = (width - map_frame_width) / 2.0;
	const float y = (height - map_frame_height) / 2.0;
	const int pad = 5;
	
	const std::string filename = this->world_key + "/maps/" + this->dungeon_key;
	ALLEGRO_BITMAP * map_bitmap = ImageLoader::get_instance().get_image(filename, "", "resources/load/saves/");
	ALLEGRO_BITMAP * sub_map_bitmap = al_create_sub_bitmap(map_bitmap, 0, 0, map_frame_width - pad * 2, map_frame_height - pad * 2);
	al_draw_bitmap(map_frame_bitmap, x, y, 0);
	al_draw_bitmap(sub_map_bitmap, x + pad, y + pad, 0);
	const int current_loc_off_x = this->current_location_x / LEVEL_MAP_SCALE;
	const int current_loc_off_y = this->current_location_y / LEVEL_MAP_SCALE;

	//TODO: need to get correct location if player is inside a building (probably happens before here)
	//			do we want the map to show the building? (probably never necessary but might look cool)
	if (this->map_current_location_blink_index < MAP_CURRENT_LOCATION_BLINK_TIME / 2) {
		if (current_loc_off_x > 0 && current_loc_off_y > 0) {
			al_draw_bitmap(this->map_current_location_icon(), x + pad + current_loc_off_x - 10, y + pad + current_loc_off_y - 5, 0);
		}
	}
	//TODO: grid lines?
	//TODO: how to track which grid areas have been explored? (need to serialize, possibly in world state)
}

void InventoryScreen::update()
{
	GameScreen::update();
	if (this->tab_index == INVENTORY_TAB_MAP) {
		this->map_current_location_blink_index = (this->map_current_location_blink_index + 1) % MAP_CURRENT_LOCATION_BLINK_TIME;
	}
}

void InventoryScreen::reset()
{
	dragging_selection = std::pair<int, int>(-1, -1);
}

void InventoryScreen::menu_up()
{
	//TODO: other select options
	switch (this->tab_index) {
	case INVENTORY_TAB_ITEMS:
		this->items_tab_menu_up();
		break;
	default:
		break;
	}
}

void InventoryScreen::menu_down()
{
	//TODO: other select options
	switch (this->tab_index) {
	case INVENTORY_TAB_ITEMS:
		this->items_tab_menu_down();
		break;
	default:
		break;
	}
}

void InventoryScreen::menu_left()
{
	//TODO: other select options
	switch (this->tab_index) {
	case INVENTORY_TAB_ITEMS:
		this->items_tab_menu_left();
		break;
	default:
		break;
	}
}

void InventoryScreen::menu_right()
{
	//TODO: other select options
	switch (this->tab_index) {
	case INVENTORY_TAB_ITEMS:
		this->items_tab_menu_right();
		break;
	default:
		break;
	}
}

void InventoryScreen::tab_left()
{
	this->tab_index = (this->tab_index - 1) % NUM_INVENTORY_TABS;
}

void InventoryScreen::tab_right()
{
	this->tab_index = (this->tab_index + 1) % NUM_INVENTORY_TABS;
}

void InventoryScreen::select()
{
	//TODO: other select options
	switch (this->tab_index) {
	case INVENTORY_TAB_ITEMS:
		this->items_tab_select();
		break;
	default:
		break;
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

void InventoryScreen::set_world_key(const std::string value)
{
	this->world_key = value;
}

void InventoryScreen::set_dungeon_key(const std::string value)
{
	this->dungeon_key = value;
}

void InventoryScreen::set_current_location(const int x, const int y)
{
	this->current_location_x = x;
	this->current_location_y = y;
}
