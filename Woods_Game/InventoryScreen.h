#ifndef INVENTORYSCREEN_H
#define INVENTORYSCREEN_H

#define NUM_INVENTORY_TABS 3
#define MAP_CURRENT_LOCATION_BLINK_TIME 40
#define QUEST_INVENTORY_COLS 5
#define QUEST_INVENTORY_ROWS 4

#include "Inventory.h"
#include "GameScreen.h"
#include "ImageLoader.h"
#include "LocationMarker.h"
#include "QuestData.h"

#include "allegro5/bitmap.h"   // for ALLEGRO_BITMAP
#include "allegro5/display.h"  // for ALLEGRO_DISPLAY
#include <set>
#include "utility"             // for pair

enum INVENTORY_SCREEN_TABS {
	INVENTORY_TAB_ITEMS,
	INVENTORY_TAB_MAP,
	INVENTORY_TAB_JOURNAL
};

enum TAB_MODES {
	TAB_MODE_DEFAULT,
	TAB_MODE_DISPLAY_DESCRIPTION
};

class InventoryScreen :
	public GameScreen
{
protected:

	int tab_index = INVENTORY_TAB_ITEMS;
	int tab_mode = TAB_MODE_DEFAULT;
	
	//UI components

	ALLEGRO_BITMAP* inventory_backdrop();
	ALLEGRO_BITMAP* inventory_backdrop_large();
	ALLEGRO_BITMAP* inventory_tab_selected();
	ALLEGRO_BITMAP* inventory_tab_deselected();

	// items tab
	ALLEGRO_BITMAP* item_box_hotbar();
	ALLEGRO_BITMAP* item_box_selected();
	ALLEGRO_BITMAP* item_box_inventory();
	ALLEGRO_BITMAP* item_box_trash();
	ALLEGRO_BITMAP* item_box_trash_dark();
	ALLEGRO_BITMAP* item_drag_selection();
	ALLEGRO_BITMAP* item_label_backdrop();
	ALLEGRO_BITMAP* item_description_backdrop(); // also used for quest items

	// map tab
	ALLEGRO_BITMAP* map_frame();
	ALLEGRO_BITMAP* map_current_location_icon();
	ALLEGRO_BITMAP* map_location_icon();
	ALLEGRO_BITMAP* map_selection_icon();
	ALLEGRO_BITMAP* map_location_label_frame();
	ALLEGRO_BITMAP* map_fog();

	// journal tab
	ALLEGRO_BITMAP * quest_pane_backdrop();
	ALLEGRO_BITMAP * quest_label_backdrop();
	ALLEGRO_BITMAP * quest_label_backdrop_completed();
	ALLEGRO_BITMAP * quest_label_backdrop_failed();
	ALLEGRO_BITMAP * quest_label_selection();
	ALLEGRO_BITMAP * quest_scroll_up_arrow();
	ALLEGRO_BITMAP * quest_scroll_down_arrow();

	const std::string label_for_tab(const int index);
	void reset_tab_mode();

	// items
	Inventory *inventory;

	std::pair<int, int> inventory_selection = std::pair<int, int>(-1, -1);
	std::pair<int, int> dragging_selection = std::pair<int, int>(-1, -1);
	std::pair<int, int> quest_selection = std::pair<int, int>(0, 0);

	virtual const int num_inventory_rows();
	virtual const int num_inventory_cols();

	virtual const bool allow_trash();
	virtual const bool is_selecting_trash();

	void items_tab_menu_up();
	void items_tab_menu_down();
	void items_tab_menu_left();
	void items_tab_menu_right();
	virtual void items_tab_select();
	void items_tab_secondary_select();

	// map

	int map_current_location_blink_index = 0;

	int map_selected_location_index = 0;

	std::string world_key;
	std::string dungeon_key;
	int current_location_x = -1, current_location_y = -1;
	std::vector <std::pair<std::string, std::pair<int, int>>> locations_for_display;
	int default_level_width, default_level_height;
	std::set<std::pair<int, int>> explored_map;

	void map_tab_menu_up();
	void map_tab_menu_down();
	void map_tab_menu_left();
	void map_tab_menu_right();

	// journal

	QuestData * quest_data;
	std::vector<Quest *> quests_for_display();
	std::vector<Quest *> active_quests;
	std::vector<Quest *> failed_quests;
	std::vector<Quest *> completed_quests;
	int quest_scroll_offset = 0;

	const int num_quest_rows();
	void adjust_quest_scroll();

	void journal_tab_menu_up();
	void journal_tab_menu_down();
	void journal_tab_menu_left();
	void journal_tab_menu_right();
	void journal_tab_secondary_select();
	

public:
	InventoryScreen();
	~InventoryScreen();
	virtual void load_content();
	virtual void load_fonts();
	virtual void draw(ALLEGRO_DISPLAY *display);
	void draw_tab_selection(ALLEGRO_DISPLAY *display);

	//item
	void draw_inventory(ALLEGRO_DISPLAY *display, Inventory * draw_inventory, const int x, const int y, 
		const int select_off_x, const int select_off_y,
		const int drag_x, const int drag_y);
	void draw_hotbar(ALLEGRO_DISPLAY *display, Inventory * draw_inventory, const int x, const int y);
	void draw_item_label(ALLEGRO_DISPLAY *display);
	void draw_item_description(ALLEGRO_DISPLAY *display);
	void draw_money(ALLEGRO_DISPLAY *display);
	void draw_trash(ALLEGRO_DISPLAY *display);

	// map
	void draw_map(ALLEGRO_DISPLAY *display);

	// journal
	void draw_journal(ALLEGRO_DISPLAY *display);
	void draw_quest_description(ALLEGRO_DISPLAY *display);

	virtual void update();
	virtual void reset();
	virtual void menu_up();
	virtual void menu_down();
	virtual void menu_left();
	virtual void menu_right();
	virtual void tab_left();
	virtual void tab_right();
	virtual void select();
	virtual void secondary_select();
	void set_inventory(Inventory *inventory);
	const bool has_inventory();
	void set_quest_data(QuestData *data);
	void set_active_quests(std::vector<Quest *> quests);
	void set_failed_quests(std::vector<Quest *> quests);
	void set_completed_quests(std::vector<Quest *> quests);
	virtual const bool selecting_internal_inventory();
	virtual Item* selected_item();
	virtual Item* dragging_item();
	QuestItem * selected_quest_item();
	Quest * selected_quest();
	void set_world_key(const std::string value);
	void set_dungeon_key(const std::string value);
	void set_current_location(const int x, const int y);
	void set_locations_for_display(const std::vector<std::pair<std::string, std::pair<int, int>>> locations);
	void set_default_level_dimensions(const int default_width, const int default_height);
	void set_explored_map(const std::set<std::pair<int, int>> value);
};

#endif