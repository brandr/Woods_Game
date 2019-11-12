#include "InventoryScreen.h"
#include "Level.h"

ALLEGRO_BITMAP * InventoryScreen::inventory_backdrop()
{
	return ImageLoader::get_instance().get_image("ui/inventory_backdrop");
}

ALLEGRO_BITMAP * InventoryScreen::inventory_backdrop_large()
{
	return ImageLoader::get_instance().get_image("ui/inventory_backdrop_large");
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

ALLEGRO_BITMAP * InventoryScreen::item_box_trash()
{
	return ImageLoader::get_instance().get_image("ui/item_box_1_trash");
}

ALLEGRO_BITMAP * InventoryScreen::item_box_trash_dark()
{
	return ImageLoader::get_instance().get_image("ui/item_box_1_trash_dark");
}

ALLEGRO_BITMAP * InventoryScreen::item_drag_selection()
{
	return ImageLoader::get_instance().get_image("ui/item_selection_1");
}

ALLEGRO_BITMAP * InventoryScreen::item_label_backdrop()
{
	return ImageLoader::get_instance().get_image("ui/item_label_backdrop");
}

ALLEGRO_BITMAP * InventoryScreen::item_description_backdrop()
{
	return ImageLoader::get_instance().get_image("ui/item_description_backdrop");
}

// map

ALLEGRO_BITMAP * InventoryScreen::map_frame()
{
	return ImageLoader::get_instance().get_image("ui/map_frame");
}

ALLEGRO_BITMAP * InventoryScreen::map_current_location_icon()
{
	return ImageLoader::get_instance().get_image("ui/map_current_location_icon");
}

ALLEGRO_BITMAP * InventoryScreen::map_location_icon()
{
	return ImageLoader::get_instance().get_image("ui/map_location_icon");
}

ALLEGRO_BITMAP * InventoryScreen::map_selection_icon()
{
	return ImageLoader::get_instance().get_image("ui/map_selection_icon");
}

ALLEGRO_BITMAP * InventoryScreen::map_location_label_frame()
{
	return ImageLoader::get_instance().get_image("ui/map_location_label_frame");
}

ALLEGRO_BITMAP * InventoryScreen::map_fog()
{
	return ImageLoader::get_instance().get_image("ui/map_fog");
}

ALLEGRO_BITMAP * InventoryScreen::quest_pane_backdrop()
{
	return ImageLoader::get_instance().get_image("ui/quest_pane_backdrop");
}

ALLEGRO_BITMAP * InventoryScreen::quest_label_backdrop()
{
	return ImageLoader::get_instance().get_image("ui/quest_label_backdrop");
}

ALLEGRO_BITMAP * InventoryScreen::quest_label_backdrop_completed()
{
	return ImageLoader::get_instance().get_image("ui/quest_label_backdrop_completed");
}

ALLEGRO_BITMAP * InventoryScreen::quest_label_backdrop_failed()
{
	return ImageLoader::get_instance().get_image("ui/quest_label_backdrop_failed");
}

ALLEGRO_BITMAP * InventoryScreen::quest_label_selection()
{
	return ImageLoader::get_instance().get_image("ui/quest_label_selection");
}

ALLEGRO_BITMAP * InventoryScreen::quest_scroll_up_arrow()
{
	return ImageLoader::get_instance().get_image("ui/quest_scroll_up_arrow");
}

ALLEGRO_BITMAP * InventoryScreen::quest_scroll_down_arrow()
{
	return ImageLoader::get_instance().get_image("ui/quest_scroll_down_arrow");
}

ALLEGRO_BITMAP * InventoryScreen::encyclopedia_pane_backdrop()
{
	return ImageLoader::get_instance().get_image("ui/encyclopedia_pane_backdrop");
}

ALLEGRO_BITMAP * InventoryScreen::encyclopedia_label_backdrop()
{
	return ImageLoader::get_instance().get_image("ui/encyclopedia_label_backdrop");
}

ALLEGRO_BITMAP * InventoryScreen::encyclopedia_label_selection()
{
	return ImageLoader::get_instance().get_image("ui/encyclopedia_label_selection");
}

ALLEGRO_BITMAP * InventoryScreen::encyclopedia_entry_image_frame()
{
	return ImageLoader::get_instance().get_image("ui/encyclopedia_entry_image_frame");
}

// tabs

const std::string InventoryScreen::label_for_tab(const int index)
{
	switch (index) {
	case INVENTORY_TAB_ITEMS:
		return "Items";
	case INVENTORY_TAB_MAP:
		return "Map";
	case INVENTORY_TAB_JOURNAL:
		return "Journal";
	case INVENTORY_TAB_ENCYCLOPEDIA:
		return "Encyclopedia";
	default:
		return "";
	}
}

void InventoryScreen::reset_tab_mode()
{
	this->tab_mode = TAB_MODE_DEFAULT;
}

const int InventoryScreen::num_inventory_rows()
{
	return INVENTORY_ROWS;
}

const int InventoryScreen::num_inventory_cols()
{
	return INVENTORY_COLS;
}

const bool InventoryScreen::allow_trash()
{
	return true;
}

const bool InventoryScreen::is_selecting_trash()
{
	return this->allow_trash() && inventory_selection.second == -2 && inventory_selection.first == 0;
}

void InventoryScreen::items_tab_menu_up()
{
	const int rows = this->num_inventory_rows(); 
	const int x = inventory_selection.first;
	const int min_y = this->allow_trash() && x == 0 ? -2 : -1;
	int y = 0;
	if (inventory_selection.second == -2) {
		y = -1;
	} else if (inventory_selection.second == -1) {
		y = rows - 1;
	} else if (inventory_selection.second == 0) {
		y = this->allow_trash() && x == 0 ? -2 : -1;
	} else {
		y = inventory_selection.second - 1;
	}
	inventory_selection = std::pair<int, int>(x, y);
	if (y == -1) {
		inventory->set_hotbar_index(x);
	}
}

void InventoryScreen::items_tab_menu_down()
{
	
	const int rows = this->num_inventory_rows();
	const int x = inventory_selection.first;
	int y = 0;
	if (inventory_selection.second == rows - 1) {
		y = -1;
	} else if (inventory_selection.second == -2) {
		y = 0;
	} else if (inventory_selection.second == -1) {
		y = this->allow_trash() && x == 0 ? -2 : 0;
	} else {
		y = inventory_selection.second + 1;
	}
	inventory_selection = std::pair<int, int>(x, y);
	if (y == -1) {
		inventory->set_hotbar_index(x);
	}
}

void InventoryScreen::items_tab_menu_left()
{
	if (this->is_selecting_trash()) {
		return;
	}
	const int cols = this->num_inventory_cols();
	const int y = inventory_selection.second;
	const int x = inventory_selection.first == 0 ? cols - 1 : inventory_selection.first - 1;
	inventory_selection = std::pair<int, int>(x, y);
	if (y == -1) {
		inventory->hotbar_index_left();
	}
}

void InventoryScreen::items_tab_menu_right()
{
	if (this->is_selecting_trash()) {
		return;
	}
	const int cols = this->num_inventory_cols();
	const int y = inventory_selection.second;
	const int x = inventory_selection.first == cols - 1 ? 0 : inventory_selection.first + 1;
	inventory_selection = std::pair<int, int>(x, y);
	if (y == -1) {
		inventory->hotbar_index_right();
	}
}

void InventoryScreen::items_tab_select()
{
	if (this->is_selecting_trash()) {
		if (dragging_item() != NULL && !dragging_item()->is_empty() && dragging_item()->get_may_be_discarded()) {
			this->inventory->remove_item_with_key(dragging_item()->get_item_key());
		}
		return;
	}
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
		if (item == NULL || item->is_empty()) return;
		dragging_selection = std::pair<int, int>(inventory_selection.first, inventory_selection.second);
	}
}

void InventoryScreen::items_tab_secondary_select()
{
	if (this->tab_mode == TAB_MODE_DISPLAY_DESCRIPTION) {
		this->tab_mode = TAB_MODE_DEFAULT;
		return;
	}
	if (this->dragging_item() != NULL) {
		return;
	}
	Item * item = this->selected_item();
	if (item == NULL || item->is_empty()) {
		return;
	}
	this->tab_mode = TAB_MODE_DISPLAY_DESCRIPTION;
}

void InventoryScreen::map_tab_menu_up()
{
	if (this->locations_for_display.size() > 0) {
		this->map_selected_location_index = (this->map_selected_location_index - 1) % this->locations_for_display.size();
	}
}

void InventoryScreen::map_tab_menu_down()
{
	if (this->locations_for_display.size() > 0) {
		this->map_selected_location_index = (this->map_selected_location_index + 1) % this->locations_for_display.size();
	}
}

void InventoryScreen::map_tab_menu_left()
{
	if (this->locations_for_display.size() > 0) {
		this->map_selected_location_index = (this->map_selected_location_index - 1) % this->locations_for_display.size();
	}
}

void InventoryScreen::map_tab_menu_right()
{
	if (this->locations_for_display.size() > 0) {
		this->map_selected_location_index = (this->map_selected_location_index + 1) % this->locations_for_display.size();
	}
}

std::vector<Quest*> InventoryScreen::quests_for_display()
{
	std::vector<Quest*> quests;
	quests.insert(quests.end(), this->active_quests.begin(), this->active_quests.end());
	quests.insert(quests.end(), this->failed_quests.begin(), this->failed_quests.end());
	quests.insert(quests.end(), this->completed_quests.begin(), this->completed_quests.end());
	return quests;
}

const int InventoryScreen::num_quest_rows()
{
	return this->quests_for_display().size();
}

void InventoryScreen::adjust_quest_scroll()
{
	if (this->num_quest_rows() == 0 || this->quest_selection.first < QUEST_INVENTORY_COLS) {
		return;
	}
	const int quest_y = this->quest_selection.second;
	if (quest_y < this->quest_scroll_offset) {
		this->quest_scroll_offset = quest_y;
	} else if (quest_y - 5 >= this->quest_scroll_offset) {
		this->quest_scroll_offset = quest_y - 4;
	}
}

void InventoryScreen::journal_tab_menu_up()
{
	const int journal_entry_rows = this->num_quest_rows();
	const int x = quest_selection.first;
	const bool is_quests = x == QUEST_INVENTORY_COLS;
	const int num_rows = !is_quests ? QUEST_INVENTORY_ROWS : journal_entry_rows;
	const int y = quest_selection.second == 0 ? num_rows - 1 : quest_selection.second - 1;
	quest_selection = std::pair<int, int>(x, y);
	this->adjust_quest_scroll();
}

void InventoryScreen::journal_tab_menu_down()
{
	const int journal_entry_rows = this->num_quest_rows();
	const int x = quest_selection.first;
	const bool is_quests = x == QUEST_INVENTORY_COLS;
	const int num_rows = !is_quests ? QUEST_INVENTORY_ROWS : journal_entry_rows;
	const int y = quest_selection.second == num_rows - 1 ? 0 : quest_selection.second + 1;
	quest_selection = std::pair<int, int>(x, y);
	this->adjust_quest_scroll();
}

void InventoryScreen::journal_tab_menu_left()
{
	const int journal_entry_rows = this->num_quest_rows();
	const int max_cols = journal_entry_rows > 0 ? QUEST_INVENTORY_COLS : QUEST_INVENTORY_COLS - 1;
	int x = quest_selection.first == 0 ? max_cols : quest_selection.first - 1;
	int y = 0;
	if (journal_entry_rows > 0 && x == QUEST_INVENTORY_COLS) { // we went from quests to journal entries
		y = std::min(journal_entry_rows - 1, quest_selection.second);
	} else if (journal_entry_rows > 0 && x == QUEST_INVENTORY_COLS - 1) { // we went from journal entries to quests
		y = std::min(QUEST_INVENTORY_ROWS - 1, quest_selection.second);
	} else {
		y = quest_selection.second;
	}
	quest_selection = std::pair<int, int>(x, y);
}

void InventoryScreen::journal_tab_menu_right()
{
	const int journal_entry_rows = this->num_quest_rows();
	const int max_cols = journal_entry_rows > 0 ? QUEST_INVENTORY_COLS : QUEST_INVENTORY_COLS - 1;
	int x = quest_selection.first == max_cols ? 0 : quest_selection.first + 1;
	int y = 0;
	if (journal_entry_rows > 0 && x == QUEST_INVENTORY_COLS) { // we went from quests to journal entries
		y = std::min(journal_entry_rows - 1, quest_selection.second);
	}
	else if (journal_entry_rows > 0 && x == QUEST_INVENTORY_COLS - 1) { // we went from journal entries to quests
		y = std::min(QUEST_INVENTORY_ROWS - 1, quest_selection.second);
	} else {
		y = quest_selection.second;
	}
	quest_selection = std::pair<int, int>(x, y);
}

void InventoryScreen::journal_tab_secondary_select()
{
	if (this->tab_mode == TAB_MODE_DISPLAY_DESCRIPTION) {
		this->tab_mode = TAB_MODE_DEFAULT;
		return;
	}
	QuestItem * qi = this->selected_quest_item();
	if (qi != NULL && !qi->is_empty() && qi->get_is_obtained()) {
		this->tab_mode = TAB_MODE_DISPLAY_DESCRIPTION;
	}
	Quest * q = this->selected_quest();
	if (q != NULL) {
		this->tab_mode = TAB_MODE_DISPLAY_DESCRIPTION;
	}
}

const int InventoryScreen::encyclopedia_rows_for_display()
{
	if (this->selected_encyclopedia_category.empty()) {
		return this->encyclopedia->num_categories();
	} else {
		return this->encyclopedia->num_entries_for_category(this->selected_encyclopedia_category);
	}
	return 0;
}

const std::string InventoryScreen::encyclopedia_label_text(const int index)
{
	if (this->selected_encyclopedia_category.empty()) {
		return this->encyclopedia->category_display_name(index);
	} else {
		return this->encyclopedia->entry_display_name(this->selected_encyclopedia_category, index);
	}
	return "";
}

void InventoryScreen::adjust_encyclopedia_scroll()
{
	if (this->encyclopedia_selection < this->encyclopedia_scroll_offset) {
		this->encyclopedia_scroll_offset = this->encyclopedia_selection;
	} else if (this->encyclopedia_selection - 5 >= this->encyclopedia_scroll_offset) {
		this->encyclopedia_scroll_offset = this->encyclopedia_selection - 4;
	}
}

const bool InventoryScreen::should_show_encyclopedia_entry()
{
	if (this->tab_index == INVENTORY_TAB_ENCYCLOPEDIA && !this->selected_encyclopedia_category.empty()) {
		return !this->encyclopedia->entry_display_name(this->selected_encyclopedia_category, this->encyclopedia_selection).empty();
	}
	return false;
}

void InventoryScreen::encyclopedia_tab_menu_up()
{
	const int num_rows = this->encyclopedia_rows_for_display();
	encyclopedia_selection = encyclopedia_selection == 0 ? num_rows - 1 : encyclopedia_selection - 1;
	this->adjust_encyclopedia_scroll();
}

void InventoryScreen::encyclopedia_tab_menu_down()
{
	const int num_rows = this->encyclopedia_rows_for_display();
	encyclopedia_selection = encyclopedia_selection < num_rows - 1 ? encyclopedia_selection + 1 : 0;
	this->adjust_encyclopedia_scroll();
}

void InventoryScreen::encyclopedia_tab_menu_left()
{
	this->selected_encyclopedia_category = "";
	this->encyclopedia_selection = std::min(this->encyclopedia_rows_for_display() - 1, this->encyclopedia_selection);
	this->adjust_encyclopedia_scroll();
}

void InventoryScreen::encyclopedia_tab_menu_right()
{
	if (this->selected_encyclopedia_category.empty()) {
		this->selected_encyclopedia_category = this->encyclopedia->category_name(this->encyclopedia_selection);
	}
	this->encyclopedia_selection = std::min(this->encyclopedia_rows_for_display() - 1, this->encyclopedia_selection);
	this->adjust_encyclopedia_scroll();
}

void InventoryScreen::encyclopedia_tab_select()
{
	if (this->selected_encyclopedia_category.empty()) {
		this->encyclopedia_tab_menu_right();
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
	ImageLoader::get_instance().load_image("ui/inventory_backdrop_large");
	ImageLoader::get_instance().load_image("ui/inventory_tab_selected");
	ImageLoader::get_instance().load_image("ui/inventory_tab_deselected");
	// items
	ImageLoader::get_instance().load_image("ui/item_box_1");
	ImageLoader::get_instance().load_image("ui/item_box_1_light");
	ImageLoader::get_instance().load_image("ui/item_box_1_dark");
	ImageLoader::get_instance().load_image("ui/item_box_1_trash");
	ImageLoader::get_instance().load_image("ui/item_box_1_trash_dark");
	ImageLoader::get_instance().load_image("ui/item_selection_1");
	ImageLoader::get_instance().load_image("ui/item_label_backdrop");
	ImageLoader::get_instance().load_image("ui/item_description_backdrop");	
	// map
	ImageLoader::get_instance().load_image("ui/map_frame");
	ImageLoader::get_instance().load_image("ui/map_current_location_icon");
	ImageLoader::get_instance().load_image("ui/map_location_icon");
	ImageLoader::get_instance().load_image("ui/map_selection_icon");
	ImageLoader::get_instance().load_image("ui/map_location_label_frame");
	ImageLoader::get_instance().load_image("ui/map_fog");
	// journal
	ImageLoader::get_instance().load_image("ui/quest_pane_backdrop");
	ImageLoader::get_instance().load_image("ui/quest_label_backdrop");
	ImageLoader::get_instance().load_image("ui/quest_label_backdrop_completed");
	ImageLoader::get_instance().load_image("ui/quest_label_backdrop_failed");
	ImageLoader::get_instance().load_image("ui/quest_label_selection");
	ImageLoader::get_instance().load_image("ui/quest_scroll_up_arrow");
	ImageLoader::get_instance().load_image("ui/quest_scroll_down_arrow");
	// encyclopedia
	ImageLoader::get_instance().load_image("ui/encyclopedia_pane_backdrop");
	ImageLoader::get_instance().load_image("ui/encyclopedia_label_backdrop");
	ImageLoader::get_instance().load_image("ui/encyclopedia_label_selection");
	ImageLoader::get_instance().load_image("ui/encyclopedia_entry_image_frame");
	
}

void InventoryScreen::load_fonts()
{
	font_map[FONT_HOTBAR] = al_load_font("resources/fonts/OpenSans-Regular.ttf", 12, NULL);
	font_map[FONT_DIALOG] = al_load_font("resources/fonts/OpenSans-Regular.ttf", 12, NULL);
	font_map[FONT_TABS] = al_load_font("resources/fonts/OpenSans-Regular.ttf", 16, NULL);
	font_map[FONT_LOCATION_LABEL] = al_load_font("resources/fonts/OpenSans-Regular.ttf", 20, NULL);
	font_map[FONT_ITEM_LABEL] = al_load_font("resources/fonts/OpenSans-Regular.ttf", 24, NULL);
	font_map[FONT_ITEM_DESCRIPTION] = al_load_font("resources/fonts/OpenSans-Regular.ttf", 18, NULL);
	font_map[FONT_MONEY] = al_load_font("resources/fonts/OpenSans-Regular.ttf", 28, NULL);
	font_map[FONT_QUEST_LABEL] = al_load_font("resources/fonts/OpenSans-Regular.ttf", 24, NULL);
	font_map[FONT_ENCYCLOPEDIA_ENTRY_NAME] = al_load_font("resources/fonts/OpenSans-Regular.ttf", 24, NULL);
	font_map[FONT_ENCYCLOPEDIA_ENTRY_DESCRIPTION] = al_load_font("resources/fonts/OpenSans-Regular.ttf", 16, NULL);
}

void InventoryScreen::draw(ALLEGRO_DISPLAY * display)
{
	const int x = (al_get_display_width(display) - al_get_bitmap_width(inventory_backdrop())) / 2;
	const int y = (al_get_display_height(display) - al_get_bitmap_height(inventory_backdrop())) / 2;
	al_draw_bitmap(inventory_backdrop(), x, y, NULL);
	this->draw_tab_selection(display);
	switch (this->tab_index) {
	case INVENTORY_TAB_ITEMS:
		draw_inventory(display, this->inventory, 0, 0, this->inventory_selection.first, this->inventory_selection.second,
			this->dragging_selection.first, this->dragging_selection.second);
		draw_hotbar(display, this->inventory, 0, 0);
		draw_item_label(display);
		draw_money(display);
		if (this->tab_mode == TAB_MODE_DISPLAY_DESCRIPTION) {
			draw_item_description(display);
		}
		if (this->allow_trash()) {
			draw_trash(display);
		}
		break;
	case INVENTORY_TAB_MAP:
		draw_map(display);
		break;
	case INVENTORY_TAB_JOURNAL:
		draw_journal(display);
		if (this->tab_mode == TAB_MODE_DISPLAY_DESCRIPTION) {
			draw_quest_description(display);
		}
		break;
	case INVENTORY_TAB_ENCYCLOPEDIA:
		draw_encyclopedia(display);
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

void InventoryScreen::draw_inventory(ALLEGRO_DISPLAY * display, Inventory * draw_inv, const int x_off, const int y_off, 
	const int select_x, const int select_y,
	const int drag_x, const int drag_y)
{
	const std::vector<std::vector<Item*>> inventory_items = draw_inv->get_inventory_items();
	const int width = al_get_display_width(display);
	const int height = al_get_display_height(display);
	const int box_width = al_get_bitmap_width(item_box_hotbar());
	const int box_height = al_get_bitmap_height(item_box_hotbar());
	const int backdrop_height = al_get_bitmap_height(inventory_backdrop());
	const int rows = inventory_items.size();
	const int cols = inventory_items[0].size();
	const std::pair<int, int> selection(select_x, select_y);
	for (int row = 0; row < rows; row++) {
		const float y = (height - backdrop_height)/2.0  + box_height*row + 32.0;
		for (int col = 0; col < cols; col++) {
			const float x = (width - box_width*cols) / 2.0 + col*box_width;
			if (col == selection.first && row == selection.second) {
				al_draw_bitmap(item_box_selected(), x_off + x, y_off + y, 0);
			} else {
				al_draw_bitmap(item_box_inventory(), x_off + x, y_off + y, 0);
			}
			Item* item = draw_inv->get_item(col, row);
			if (item && !(drag_x == col && drag_y == row))
				item->draw(display, x_off + x, y_off + y);
			if (col == drag_x && row == drag_y) {
				al_draw_bitmap(item_drag_selection(), x_off + x, y_off + y, 0);
			}
		}
	}
	if (selecting_internal_inventory() && dragging_item()) {
		const float drag_x = (width - box_width*cols) / 2 + select_x*box_width;
		const float drag_y = (height - backdrop_height) / 2 + box_height*select_y + 32;
		dragging_item()->draw(display, x_off + drag_x, y_off + drag_y);
	}
}

void InventoryScreen::draw_hotbar(ALLEGRO_DISPLAY * display, Inventory * draw_inv, const int x_off, const int y_off)
{
	const int width = al_get_display_width(display);
	const int height = al_get_display_height(display);
	const int box_width = al_get_bitmap_width(item_box_hotbar());
	const int box_height = al_get_bitmap_height(item_box_hotbar());
	const int hotbar_index = draw_inv->get_hotbar_index();
	const float y = (height + al_get_bitmap_height(inventory_backdrop()))/2 - box_height - 100;
	const int size = HOTBAR_SIZE;
	for (int i = 0; i < size; i++) {
		const float x = (width - box_width*size) / 2 + i*box_width;
		if (this->inventory_selection.second == -1 && i == hotbar_index) {
			al_draw_bitmap(item_box_selected(), x_off + x, y_off + y, 0);
		} else {
			al_draw_bitmap(item_box_hotbar(), x_off + x, y_off + y, 0);
		}
		if (!this->is_selecting_trash() && i == dragging_selection.first && dragging_selection.second < 0) {
			al_draw_bitmap(item_drag_selection(), x_off + x, y_off + y, 0);
		}
		Item* item = draw_inv->get_hotbar_item(i);
		if (item && !(dragging_selection.first == i && dragging_selection.second < 0)) 
			item->draw(display, x_off + x, y_off + y);
	}
	if (this->inventory_selection.second == -1 && dragging_item()) {
		const float drag_x = (width - box_width*size) / 2 + inventory_selection.first*box_width;
		dragging_item()->draw(display, x_off + drag_x , y_off + y);
	}
}

void InventoryScreen::draw_item_label(ALLEGRO_DISPLAY * display)
{
	const int width = al_get_display_width(display);
	const int height = al_get_display_height(display);
	const int box_width = al_get_bitmap_width(item_label_backdrop());
	const int box_height = al_get_bitmap_height(item_label_backdrop());
	const float x = (width - box_width) / 2;
	const float y = (height + al_get_bitmap_height(inventory_backdrop())) / 2 - box_height - 26;
	al_draw_bitmap(item_label_backdrop(), x, y, 0);
	Item * item = this->selected_item();
	if (item != NULL) {
		ALLEGRO_FONT * font = this->font_map[FONT_ITEM_LABEL];
		const float text_width = al_get_text_width(font , item->get_item_display_name().c_str());
		al_draw_text(font, al_map_rgb(0, 0, 0), x + (box_width - text_width) / 2.0f, y + 12, 0, item->get_item_display_name().c_str());
	}
}

void InventoryScreen::draw_item_description(ALLEGRO_DISPLAY * display)
{
	const int x = (al_get_display_width(display) - al_get_bitmap_width(item_description_backdrop())) / 2;
	const int y = (al_get_display_height(display) - al_get_bitmap_height(item_description_backdrop())) / 2;
	al_draw_bitmap(item_description_backdrop(), x, y, 0);
	Item * item = this->selected_item();
	if (item == NULL || item->is_empty()) {
		return;
	}
	const std::string description = item->get_item_description();
	FileManager fm;
	ALLEGRO_FONT * font = this->font_map[FONT_ITEM_DESCRIPTION];
	std::vector<std::string> description_lines = fm.string_to_parts(description, "\\n");
	const int size = description_lines.size();
	for (int i = 0; i < size; i++) {
		const std::string line = description_lines[i];
		if (line.empty()) {
			continue;
		}
		al_draw_text(font, al_map_rgb(0, 0, 0), x + 40, y + 28 + i * al_get_font_line_height(font), 0, line.c_str());
	}
}

void InventoryScreen::draw_money(ALLEGRO_DISPLAY * display)
{
	const int width = al_get_display_width(display);
	const int height = al_get_display_height(display);
	const int back_w = al_get_bitmap_width(inventory_backdrop());
	const int back_h = al_get_bitmap_height(inventory_backdrop());
	const int x_off = (width - back_w) / 2;
	const int y_off = (height - back_h) / 2;
	const std::string money_text = this->inventory->money_display_text();
	const float text_width = al_get_text_width(this->font_map[FONT_MONEY], money_text.c_str());
	const float text_height = al_get_font_line_height(this->font_map[FONT_MONEY]);
	al_draw_text(this->font_map[FONT_MONEY], al_map_rgb(0, 0, 0), 
		x_off + back_w - text_width - 20, y_off + back_h - text_height - 20, 0, money_text.c_str());
}

void InventoryScreen::draw_trash(ALLEGRO_DISPLAY * display)
{
	const int width = al_get_display_width(display);
	const int height = al_get_display_height(display);
	const int back_width = al_get_bitmap_width(inventory_backdrop());
	const int back_height = al_get_bitmap_height(inventory_backdrop());
	const int box_width = al_get_bitmap_width(item_box_hotbar());
	const int box_height = al_get_bitmap_height(item_box_hotbar());
	const int size = HOTBAR_SIZE;

	const float x = (width - box_width * size) / 2;
	const float y = (height + al_get_bitmap_height(inventory_backdrop())) / 2 - box_height - 28;
	ALLEGRO_BITMAP * trash_box = this->is_selecting_trash() ? item_box_trash() : item_box_trash_dark();
	al_draw_bitmap(trash_box, x, y, 0);

	if (this->is_selecting_trash() && dragging_item()) {
		dragging_item()->draw(display, x, y);
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
	
	al_draw_bitmap(sub_map_bitmap, x + pad, y + pad, 0);

	ALLEGRO_BITMAP * full_fog_bitmap = this->map_fog();
	const int full_fog_w = al_get_bitmap_width(full_fog_bitmap), full_fog_h = al_get_bitmap_height(full_fog_bitmap);
	const int grid_square_w = this->default_level_width / LEVEL_MAP_SCALE, grid_square_h = this->default_level_height / LEVEL_MAP_SCALE;
	const int grid_rows = map_frame_height / grid_square_h, grid_cols = map_frame_width / grid_square_w;
	for (int gy = 0; gy < grid_rows; gy++) {
		for (int gx = 0; gx < grid_cols; gx++) {
			if (this->explored_map.find(std::pair<int, int>(gx, gy)) == this->explored_map.end()) {
				al_draw_scaled_bitmap(full_fog_bitmap, 0, 0, full_fog_w, full_fog_h, x + pad + gx * grid_square_w, y + pad + gy * grid_square_h, grid_square_w + 3, grid_square_h + 3, 0);
			}
		}
	}

	al_draw_bitmap(map_frame_bitmap, x, y, 0);

	const int current_loc_off_x = this->current_location_x / LEVEL_MAP_SCALE;
	const int current_loc_off_y = this->current_location_y / LEVEL_MAP_SCALE;

	ALLEGRO_BITMAP * location_icon = this->map_location_icon();
	ALLEGRO_BITMAP * selection_icon = this->map_selection_icon();

	const int display_loc_count = this->locations_for_display.size();
	for (int i = 0; i < display_loc_count; i++) {
		const std::pair<std::string, std::pair<int, int>> loc = this->locations_for_display[i];
		const std::pair<int, int> loc_pos = loc.second;
		const int loc_off_x = loc_pos.first / LEVEL_MAP_SCALE;
		const int loc_off_y = loc_pos.second / LEVEL_MAP_SCALE;
		const int loc_grid_x = loc_off_x / grid_square_w, loc_grid_y = loc_off_y / grid_square_h;
		if (this->explored_map.find(std::pair<int, int>(loc_grid_x, loc_grid_y)) != this->explored_map.end()) {
			al_draw_bitmap(location_icon, x + pad + loc_off_x - 10, y + pad + loc_off_y - 5, 0);
			if (i == this->map_selected_location_index) {
				al_draw_bitmap(selection_icon, x + pad + loc_off_x - 10, y + pad + loc_off_y - 5, 0);
			}
		}
	}

	if (this->map_current_location_blink_index < MAP_CURRENT_LOCATION_BLINK_TIME / 2) {
		if (current_loc_off_x > 0 && current_loc_off_y > 0) {
			al_draw_bitmap(this->map_current_location_icon(), x + pad + current_loc_off_x - 10, y + pad + current_loc_off_y - 5, 0);
		}
	}

	ALLEGRO_BITMAP * location_frame = this->map_location_label_frame();

	const std::string location_text 
		= this->map_selected_location_index >= 0 
			&& this->map_selected_location_index  < this->locations_for_display.size()
			? this->locations_for_display[this->map_selected_location_index].first : "???";

	const int location_frame_width = al_get_bitmap_width(location_frame), location_frame_height = al_get_bitmap_height(location_frame);
	const int l_frame_x = x + (map_frame_width - location_frame_width) / 2;
	const int l_frame_y = y + map_frame_height + 8;
	al_draw_bitmap(location_frame, l_frame_x, l_frame_y, 0);
	al_draw_text(font_map[FONT_LOCATION_LABEL], al_map_rgb(0, 0, 0), l_frame_x + 8, l_frame_y + 4, 0, location_text.c_str());
}

void InventoryScreen::draw_journal(ALLEGRO_DISPLAY * display)
{
	const int width = al_get_display_width(display);
	const int height = al_get_display_height(display);
	const int back_w = al_get_bitmap_width(inventory_backdrop());
	const int back_h = al_get_bitmap_height(inventory_backdrop());
	const int x_off = (width - back_w) / 2;
	const int y_off = (height - back_h) / 2;
	const int quest_items_off_x = 16, quest_items_off_y = 16;
	const int x_pad = 4, y_pad = 4;

	ALLEGRO_BITMAP * item_label = item_label_backdrop();
	const int label_width = (al_get_bitmap_width(item_label));
	const int label_x = x_off + (back_w / 4) - label_width/2; // in the middle of the left half
	const int label_y = y_off + back_h - al_get_bitmap_height(item_label) - 16;
	al_draw_bitmap(item_label_backdrop(), label_x, label_y, 0);

	std::vector<QuestItem *> quest_items = this->quest_data->get_quest_items();
	for (QuestItem * qi : quest_items) {
		if (qi->get_is_obtained()) {
			const int qi_x = qi->get_inventory_pos_x(), qi_y = qi->get_inventory_pos_y();
			const int x = x_off + quest_items_off_x + qi_x * (TILE_SIZE + x_pad);
			const int y = y_off + quest_items_off_y + qi_y * (TILE_SIZE + y_pad);
			qi->draw(display, x, y);
			if (qi_x == quest_selection.first && qi_y == quest_selection.second) {
				// draw item name
				ALLEGRO_FONT * font = this->font_map[FONT_ITEM_LABEL];
				const float text_width = al_get_text_width(font, qi->get_item_display_name().c_str());
				al_draw_text(font, al_map_rgb(0, 0, 0), label_x + (label_width - text_width) / 2.0f, label_y + 12, 0, qi->get_item_display_name().c_str());
			}
		}
	}

	// draw quests

	const int quest_pane_x = x_off + back_w / 2 + 24, quest_pane_y = y_off + 24;

	al_draw_bitmap(this->quest_pane_backdrop(), quest_pane_x , quest_pane_y , 0);

	const int qx = quest_pane_x + 16;
	const int quest_entry_height = 63; 
	const int quest_count = this->quests_for_display().size();
	const int draw_quest_rows = std::min(quest_count, 5);
	std::vector<Quest *> quests = this->quests_for_display();
	for (int i = 0; i < draw_quest_rows; i++) {
		Quest * q = quests[i + this->quest_scroll_offset];
		const int qy = quest_pane_y + 16 + (i * quest_entry_height);
		ALLEGRO_BITMAP * label_backdrop = NULL;
		if (q->is_completed()) {
			label_backdrop = this->quest_label_backdrop_completed();
		}
		else if (q->is_failed()) {
			label_backdrop = this->quest_label_backdrop_failed();
		}
		else {
			label_backdrop = this->quest_label_backdrop();
		}
		al_draw_bitmap(label_backdrop, qx, qy, 0);
		al_draw_text(this->font_map[FONT_QUEST_LABEL], al_map_rgb(0, 0, 0), qx + 16, qy + 10, 0, q->get_quest_name().c_str());
	}

	const bool is_quest_item = this->quest_selection.first < QUEST_INVENTORY_COLS;

	const int select_x = is_quest_item ? 
		x_off + quest_items_off_x + this->quest_selection.first * (TILE_SIZE + x_pad)
		: quest_pane_x + 16;
	const int select_y = is_quest_item ? 
		y_off + quest_items_off_y + this->quest_selection.second * (TILE_SIZE + y_pad)
		: quest_pane_y + 16 + (this->quest_selection.second - this->quest_scroll_offset) * quest_entry_height;
	
	ALLEGRO_BITMAP * select_bitmap = is_quest_item ? this->item_drag_selection() : this->quest_label_selection();
	al_draw_bitmap(select_bitmap, select_x, select_y, 0);

	const bool is_hiding_quests_up = this->quest_scroll_offset > 0;
	const bool is_hiding_quests_down = this->quest_scroll_offset + 5 < this->quests_for_display().size();
	if (is_hiding_quests_up) {
		al_draw_bitmap(quest_scroll_up_arrow(), quest_pane_x + 16 + al_get_bitmap_width(quest_label_backdrop()) + 6, quest_pane_y + 16, 0);
	}
	if (is_hiding_quests_down) {
		al_draw_bitmap(quest_scroll_down_arrow(), quest_pane_x + 16 + al_get_bitmap_width(quest_label_backdrop()) + 6, 
			quest_pane_y + al_get_bitmap_height(this->quest_pane_backdrop()) - al_get_bitmap_height(quest_scroll_down_arrow()) - 16, 0);
	}
}

void InventoryScreen::draw_quest_description(ALLEGRO_DISPLAY * display)
{
	FileManager fm;
	ALLEGRO_FONT * font = this->font_map[FONT_ITEM_DESCRIPTION];
	const int x = (al_get_display_width(display) - al_get_bitmap_width(item_description_backdrop())) / 2;
	const int y = (al_get_display_height(display) - al_get_bitmap_height(item_description_backdrop())) / 2;
	al_draw_bitmap(item_description_backdrop(), x, y, 0);
	Quest * quest = this->selected_quest();
	if (quest != NULL) {
		const std::string description = quest->get_quest_summary();
		std::vector<std::string> description_lines = fm.string_to_parts(description, "\\n");
		const int size = description_lines.size();
		for (int i = 0; i < size; i++) {
			const std::string line = description_lines[i];
			if (line.empty()) {
				continue;
			}
			al_draw_text(font, al_map_rgb(0, 0, 0), x + 40, y + 28 + i * al_get_font_line_height(font), 0, line.c_str());
		}
	}
	QuestItem * item = this->selected_quest_item();
	if (item != NULL && !item->is_empty() && item->get_is_obtained()) {
		const std::string description = item->get_item_description();
		std::vector<std::string> description_lines = fm.string_to_parts(description, "\\n");
		const int size = description_lines.size();
		for (int i = 0; i < size; i++) {
			const std::string line = description_lines[i];
			if (line.empty()) {
				continue;
			}
			al_draw_text(font, al_map_rgb(0, 0, 0), x + 40, y + 28 + i * al_get_font_line_height(font), 0, line.c_str());
		}
	}
}

void InventoryScreen::draw_encyclopedia(ALLEGRO_DISPLAY * display)
{
	const int width = al_get_display_width(display);
	const int height = al_get_display_height(display);
	const int back_w = al_get_bitmap_width(inventory_backdrop());
	const int back_h = al_get_bitmap_height(inventory_backdrop());
	const int x_off = (width - back_w) / 2;
	const int y_off = (height - back_h) / 2;

	ALLEGRO_BITMAP * item_label = item_label_backdrop();
	const int label_width = (al_get_bitmap_width(item_label));
	const int label_x = x_off + (back_w / 4) - label_width / 2; // in the middle of the left half
	const int label_y = y_off + back_h - al_get_bitmap_height(item_label) - 16;

	// draw selection

	const int encyclopedia_pane_x = x_off + back_w / 2 + 24, encyclopedia_pane_y = y_off + 24;

	al_draw_bitmap(this->encyclopedia_pane_backdrop(), encyclopedia_pane_x, encyclopedia_pane_y, 0);

	const int ex = encyclopedia_pane_x + 16;
	const int encyclopedia_entry_height = 63;
	const int encyclopedia_row_count = this->encyclopedia_rows_for_display();
	const int draw_rows = std::min(encyclopedia_row_count, 5);
	for (int i = 0; i < draw_rows; i++) {
		const int ey = encyclopedia_pane_y + 16 + (i * encyclopedia_entry_height);
		ALLEGRO_BITMAP * label_backdrop = this->encyclopedia_label_backdrop();
		al_draw_bitmap(label_backdrop, ex, ey, 0);
		const std::string label_text = this->encyclopedia_label_text(i);
		al_draw_text(this->font_map[FONT_QUEST_LABEL], al_map_rgb(0, 0, 0), ex + 16, ey + 10, 0, label_text.c_str());
	}

	const int select_x = encyclopedia_pane_x + 16;
	const int select_y = encyclopedia_pane_y + 16 
		+ (this->encyclopedia_selection - this->encyclopedia_scroll_offset) * encyclopedia_entry_height;

	ALLEGRO_BITMAP * select_bitmap = this->encyclopedia_label_selection();
	al_draw_bitmap(select_bitmap, select_x, select_y, 0);

	const bool is_hiding_up = this->encyclopedia_scroll_offset > 0;
	const bool is_hiding_down = this->encyclopedia_scroll_offset + 5 < encyclopedia_row_count;
	if (is_hiding_up) {
		al_draw_bitmap(quest_scroll_up_arrow(), encyclopedia_pane_x + 16 + al_get_bitmap_width(encyclopedia_label_backdrop()) + 6, encyclopedia_pane_y + 16, 0);
	}
	if (is_hiding_down) {
		al_draw_bitmap(quest_scroll_down_arrow(), encyclopedia_pane_x + 16 + al_get_bitmap_width(encyclopedia_label_backdrop()) + 6,
			encyclopedia_pane_y + al_get_bitmap_height(this->encyclopedia_pane_backdrop()) - al_get_bitmap_height(quest_scroll_down_arrow()) - 16, 0);
	}
	if (this->should_show_encyclopedia_entry()) {
		this->draw_encyclopedia_entry(display);
	}
}

void InventoryScreen::draw_encyclopedia_entry(ALLEGRO_DISPLAY * display)
{
	const int width = al_get_display_width(display);
	const int height = al_get_display_height(display);
	const int back_w = al_get_bitmap_width(inventory_backdrop());
	const int back_h = al_get_bitmap_height(inventory_backdrop());
	const int x_off = (width - back_w) / 2;
	const int y_off = (height - back_h) / 2;
	const float adj_x_off = x_off + 24, adj_y_off = y_off + 24;

	// entry image
	al_draw_bitmap(this->encyclopedia_entry_image_frame(), adj_x_off, adj_y_off , 0);
	ALLEGRO_BITMAP * entry_image = this->encyclopedia->get_entry_bitmap(this->selected_encyclopedia_category, this->encyclopedia_selection);
	al_draw_bitmap(entry_image, adj_x_off + 4, adj_y_off + 4, 0);

	// entry name
	const float entry_name_x = adj_x_off + al_get_bitmap_width(this->encyclopedia_entry_image_frame()) + 12, entry_name_y = adj_y_off + 4;
	const std::string entry_name = this->encyclopedia->entry_display_name(this->selected_encyclopedia_category, this->encyclopedia_selection);
	al_draw_text(this->font_map[FONT_ENCYCLOPEDIA_ENTRY_NAME], al_map_rgb(0, 0, 0), entry_name_x, entry_name_y, 0, entry_name.c_str());
	

	// entry description
	const float desc_line_height = 22; //temp
	const float entry_desc_x = adj_x_off + 4, entry_desc_y = adj_y_off + al_get_bitmap_height(this->encyclopedia_entry_image_frame()) + 12;
	const std::vector<std::string> entry_desc_lines = this->encyclopedia->get_entry_description_lines(this->selected_encyclopedia_category, this->encyclopedia_selection);
	const int line_count = entry_desc_lines.size();
	for (int i = 0; i < line_count; i++) {
		al_draw_text(this->font_map[FONT_ENCYCLOPEDIA_ENTRY_DESCRIPTION], al_map_rgb(0, 0, 0), entry_desc_x, entry_desc_y + (i * desc_line_height), 0, entry_desc_lines[i].c_str());
	}
	//TODO: latin name? num caught?
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
	this->dragging_selection = std::pair<int, int>(-1, -1);
	this->reset_tab_mode();
	this->quest_selection = std::pair<int, int>(0, 0); //easy way to keep from having weird bugs when there are no quests left
	this->quest_scroll_offset = 0;
}

void InventoryScreen::menu_up()
{
	switch (this->tab_index) {
	case INVENTORY_TAB_ITEMS:
		if (this->tab_mode == TAB_MODE_DEFAULT) {
			this->items_tab_menu_up();
		}
		break;
	case INVENTORY_TAB_MAP:
		this->map_tab_menu_up();
		break;
	case INVENTORY_TAB_JOURNAL:
		if (this->tab_mode == TAB_MODE_DEFAULT) {
			this->journal_tab_menu_up();
		}
		break;
	case INVENTORY_TAB_ENCYCLOPEDIA:
		this->encyclopedia_tab_menu_up();
		break;
	default:
		break;
	}
}

void InventoryScreen::menu_down()
{
	switch (this->tab_index) {
	case INVENTORY_TAB_ITEMS:
		if (this->tab_mode == TAB_MODE_DEFAULT) {
			this->items_tab_menu_down();
		}
		break;
	case INVENTORY_TAB_MAP:
		this->map_tab_menu_down();
		break;
	case INVENTORY_TAB_JOURNAL:
		if (this->tab_mode == TAB_MODE_DEFAULT) {
			this->journal_tab_menu_down();
		}
		break;
	case INVENTORY_TAB_ENCYCLOPEDIA:
		this->encyclopedia_tab_menu_down();
		break;
	default:
		break;
	}
}

void InventoryScreen::menu_left()
{
	switch (this->tab_index) {
	case INVENTORY_TAB_ITEMS:
		if (this->tab_mode == TAB_MODE_DEFAULT) {
			this->items_tab_menu_left();
		}
		break;
	case INVENTORY_TAB_MAP:
		this->map_tab_menu_left();
		break;
	case INVENTORY_TAB_JOURNAL:
		if (this->tab_mode == TAB_MODE_DEFAULT) {
			this->journal_tab_menu_left();
		}
		break;
	case INVENTORY_TAB_ENCYCLOPEDIA:
		this->encyclopedia_tab_menu_left();
		break;
	default:
		break;
	}
}

void InventoryScreen::menu_right()
{
	switch (this->tab_index) {
	case INVENTORY_TAB_ITEMS:
		if (this->tab_mode == TAB_MODE_DEFAULT) {
			this->items_tab_menu_right();
		}
		break;
	case INVENTORY_TAB_MAP:
		this->map_tab_menu_right();
		break;
	case INVENTORY_TAB_JOURNAL:
		if (this->tab_mode == TAB_MODE_DEFAULT) {
			this->journal_tab_menu_right();
		}
		break;
	case INVENTORY_TAB_ENCYCLOPEDIA:
		this->encyclopedia_tab_menu_right();
		break;
	default:
		break;
	}
}

void InventoryScreen::tab_left()
{
	if (this->tab_index == 0) {
		this-> tab_index = NUM_INVENTORY_TABS - 1;
	} else {
		this->tab_index = this->tab_index - 1;
	}
	this->reset_tab_mode();
}

void InventoryScreen::tab_right()
{
	this->tab_index = (this->tab_index + 1) % NUM_INVENTORY_TABS;
	this->reset_tab_mode();
}

void InventoryScreen::select()
{
	//TODO: other select options
	switch (this->tab_index) {
	case INVENTORY_TAB_ITEMS:
		switch (this->tab_mode) {
		case TAB_MODE_DEFAULT:
			this->items_tab_select();
			break;
		case TAB_MODE_DISPLAY_DESCRIPTION:
			this->tab_mode = TAB_MODE_DEFAULT;
			break;
		default:
			break;
		}
		break;
	case INVENTORY_TAB_MAP:
		//TODO: display location descriptions?
		break;
	case INVENTORY_TAB_ENCYCLOPEDIA:
		this->encyclopedia_tab_select();
		break;
	default:
		break;
	}
}

void InventoryScreen::secondary_select()
{
	//TODO: other select options
	switch (this->tab_index) {
	case INVENTORY_TAB_ITEMS:
		this->items_tab_secondary_select();
		break;
	case INVENTORY_TAB_MAP:
		//TODO: location description?
		break;
	case INVENTORY_TAB_JOURNAL:
		this->journal_tab_secondary_select();
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

const bool InventoryScreen::has_inventory()
{
	return this->inventory != NULL;
}

void InventoryScreen::set_quest_data(QuestData * data)
{
	this->quest_data = data;
}

void InventoryScreen::set_active_quests(std::vector<Quest*> quests)
{
	this->active_quests = quests;
}

void InventoryScreen::set_failed_quests(std::vector<Quest*> quests)
{
	this->failed_quests = quests;
}

void InventoryScreen::set_completed_quests(std::vector<Quest*> quests)
{
	this->completed_quests = quests;
}

void InventoryScreen::set_encyclopedia(Encyclopedia * e)
{
	this->encyclopedia = e;
}

const bool InventoryScreen::selecting_internal_inventory()
{
	if (!this->has_inventory()) {
		return false;
	}
	const int inv_rows = this->num_inventory_rows();
	const int inv_cols = this->num_inventory_cols();
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

QuestItem * InventoryScreen::selected_quest_item()
{
	return this->quest_data->get_quest_item(this->quest_selection.first, this->quest_selection.second);
}

Quest * InventoryScreen::selected_quest()
{
	if (this->quest_selection.first < QUEST_INVENTORY_COLS) {
		return NULL;
	}
	return this->quest_selection.second >= 0 && this->quest_selection.second < this->quests_for_display().size() ?
	this->quests_for_display()[this->quest_selection.second] : NULL;
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

void InventoryScreen::set_locations_for_display(const std::vector<std::pair<std::string, std::pair<int, int>>> locations)
{
	this->locations_for_display = locations;
}

void InventoryScreen::set_default_level_dimensions(const int default_width, const int default_height)
{
	this->default_level_width = default_width;
	this->default_level_height = default_height;
}

void InventoryScreen::set_explored_map(const std::set<std::pair<int, int>> value)
{
	this->explored_map = value;
}
