#ifndef ENCYCLOPEDIA_H
#define ENCYCLOPEDIA_H

#include "FileManager.h"
#include "ImageLoader.h"

#include "allegro5/bitmap.h"
#include "XMLSerialization.h"

enum ENCYCLOPEDIA_ENTRY_STATES {
	ENTRY_UNDISCOVERED = 0,
	ENTRY_SEEN = 1,
	ENTRY_KNOWN = 2
};

struct EncyclopediaEntry : public xmls::Serializable
{
private:
	xmls::xString entry_name;
	xmls::xString entry_display_name;
	xmls::xString entry_description;
	xmls::xInt entry_state;
public:
	EncyclopediaEntry();
	~EncyclopediaEntry();
	void load_content_from_attributes();
	ALLEGRO_BITMAP * get_display_bitmap();
	const std::string get_entry_name();
	const std::string get_entry_display_name();
	const std::vector<std::string> get_entry_description_lines();
	const std::string get_entry_description();
	const int get_entry_state();
	void set_entry_state(const int state);
};

struct EncyclopediaCategory : public xmls::Serializable
{
private:
	xmls::xString category_name;
	xmls::xString category_display_name;
	xmls::Collection<EncyclopediaEntry> entries;
	EncyclopediaEntry * get_entry(const int index);
	EncyclopediaEntry * get_entry(const std::string name);
public:
	EncyclopediaCategory();
	~EncyclopediaCategory();
	void load_content_from_attributes();
	void update_entry_state(const std::string entry_name, const int state);
	const int num_entries();
	ALLEGRO_BITMAP * get_entry_bitmap(const int index);
	const std::string get_category_name();
	const std::string get_category_display_name();
	const std::string get_entry_display_name(const int index);
	const std::vector<std::string> get_entry_description_lines(const int index);
	const int get_entry_state(const std::string entry_name);
};

class Encyclopedia : public xmls::Serializable
{
private:
	xmls::Collection<EncyclopediaCategory> categories;
	EncyclopediaCategory * get_category(const std::string category_key);
	EncyclopediaCategory * get_category(const int index);
public:
	Encyclopedia();
	~Encyclopedia();
	void reset();
	void load_content_from_attributes();
	void update_entry_state(const std::string category_name, const std::string entry_name, const int state);
	const int get_entry_state(const std::string category_name, const std::string entry_name);
	ALLEGRO_BITMAP * get_entry_bitmap(const std::string category_name, const int index);
	const int num_categories();
	const int num_entries_for_category(const std::string category_key);
	const std::string category_display_name(const std::string category_key);
	const std::string category_display_name(const int index);
	const std::string category_name(const int index);
	const std::string entry_display_name(const std::string category_key, const int index);
	const std::vector<std::string> get_entry_description_lines(const std::string category_key, const int index);
};

#endif