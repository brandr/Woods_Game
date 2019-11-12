#include "Encyclopedia.h"

EncyclopediaCategory * Encyclopedia::get_category(const std::string category_key)
{
	const int size = this->categories.size();
	for (int i = 0; i < size; i++) {
		EncyclopediaCategory * ec = this->categories.getItem(i);
		if (category_key == ec->get_category_name()) {
			return ec;
		}
	}
	return NULL;
}

EncyclopediaCategory * Encyclopedia::get_category(const int index)
{
	const int size = this->categories.size();
	if (index >= 0 && index < size) {
		return this->categories.getItem(index);
	}
	return NULL;
}

Encyclopedia::Encyclopedia()
{
	setClassName("Encyclopedia");
	Register("categories", &categories);
}


Encyclopedia::~Encyclopedia()
{
}

void Encyclopedia::reset()
{
	this->categories.Clear();
	//TODO
}

void Encyclopedia::load_content_from_attributes()
{
	ImageLoader::get_instance().load_image("encyclopedia/unknown_entry");
	const int cat_size = this->categories.size();
	for (int c = 0; c < cat_size; c++) {
		EncyclopediaCategory * cat = this->categories.getItem(c);
		cat->load_content_from_attributes();
	}
}

void Encyclopedia::update_entry_state(const std::string category_name, const std::string entry_name, const int state)
{
	const int cat_size = this->categories.size();
	for (int c = 0; c < cat_size; c++) {
		EncyclopediaCategory * cat = this->categories.getItem(c);
		if (category_name == cat->get_category_name()) {
			cat->update_entry_state(entry_name, state);
			break;
		}
	}
}

ALLEGRO_BITMAP * Encyclopedia::get_entry_bitmap(const std::string category_key, const int entry_index)
{
	EncyclopediaCategory * ec = this->get_category(category_key);
	if (ec != NULL) {
		return ec->get_entry_bitmap(entry_index);
	}
	return NULL;
}

const int Encyclopedia::num_categories()
{
	return this->categories.size();
}

const int Encyclopedia::num_entries_for_category(const std::string category_key)
{
	EncyclopediaCategory * ec = this->get_category(category_key);
	if (ec != NULL) {
		return ec->num_entries();
	}
	return 0;
}

const std::string Encyclopedia::category_display_name(const std::string category_key)
{
	EncyclopediaCategory * ec = this->get_category(category_key);
	if (ec != NULL) {
		return ec->get_category_display_name();
	}
	return "";
}

const std::string Encyclopedia::category_display_name(const int index)
{
	EncyclopediaCategory * ec = this->get_category(index);
	if (ec != NULL) {
		return ec->get_category_display_name();
	}
	return "";
}

const std::string Encyclopedia::category_name(const int index)
{
	EncyclopediaCategory * ec = this->get_category(index);
	if (ec != NULL) {
		return ec->get_category_name();
	}
	return "";
}

const std::string Encyclopedia::entry_display_name(const std::string category_key, const int index)
{
	EncyclopediaCategory * ec = this->get_category(category_key);
	if (ec != NULL) {
		return ec->get_entry_display_name(index);
	}
	return "";
}

const std::vector<std::string> Encyclopedia::get_entry_description_lines(const std::string category_key, const int index)
{
	EncyclopediaCategory * ec = this->get_category(category_key);
	if (ec != NULL) {
		return ec->get_entry_description_lines(index);
	}
	return std::vector<std::string>();
}

EncyclopediaEntry * EncyclopediaCategory::get_entry(const int index)
{
	const int size = this->entries.size();
	if (index >= 0 && index < size) {
		return this->entries.getItem(index);
	}
	return NULL;
}

EncyclopediaCategory::EncyclopediaCategory()
{
	setClassName("EncyclopediaCategory");
	Register("category_name", &category_name);
	Register("category_display_name", &category_display_name);
	Register("entries", &entries);
}

EncyclopediaCategory::~EncyclopediaCategory()
{
}

void EncyclopediaCategory::load_content_from_attributes()
{
	const int entry_size = this->entries.size();
	for (int e = 0; e < entry_size; e++) {
		EncyclopediaEntry * entry = this->entries.getItem(e);
		entry->load_content_from_attributes();
	}
}

void EncyclopediaCategory::update_entry_state(const std::string entry_name, const int state)
{
	const int entry_size = this->entries.size();
	for (int e = 0; e < entry_size; e++) {
		EncyclopediaEntry * entry = this->entries.getItem(e);
		if (entry_name == entry->get_entry_name()) {
			entry->set_entry_state(state);
			return;
		}
	}
}

const int EncyclopediaCategory::num_entries()
{
	return this->entries.size();
}

ALLEGRO_BITMAP * EncyclopediaCategory::get_entry_bitmap(const int index)
{
	EncyclopediaEntry * ee = this->get_entry(index);
	if (ee != NULL) {
		return ee->get_display_bitmap();
	}
	return NULL;
}

const std::string EncyclopediaCategory::get_category_name()
{
	return this->category_name.value();
}

const std::string EncyclopediaCategory::get_category_display_name()
{
	return this->category_display_name.value();
}

const std::string EncyclopediaCategory::get_entry_display_name(const int index)
{
	EncyclopediaEntry * ee = this->get_entry(index);
	if (ee != NULL) {
		return ee->get_entry_display_name();
	}
	return "";
}

const std::vector<std::string> EncyclopediaCategory::get_entry_description_lines(const int index)
{
	EncyclopediaEntry * ee = this->get_entry(index);
	if (ee != NULL) {
		return ee->get_entry_description_lines();
	}
	return std::vector<std::string>();
}

EncyclopediaEntry::EncyclopediaEntry()
{
	setClassName("EncyclopediaEntry");
	Register("entry_name", &entry_name);
	Register("entry_display_name", &entry_display_name);
	Register("entry_description", &entry_description);
	Register("entry_state", &entry_state);
}

EncyclopediaEntry::~EncyclopediaEntry()
{
}

void EncyclopediaEntry::load_content_from_attributes()
{
	ImageLoader::get_instance().load_image("encyclopedia/silhouettes/" + this->get_entry_name());
	ImageLoader::get_instance().load_image("encyclopedia/entries/" + this->get_entry_name());
}

ALLEGRO_BITMAP * EncyclopediaEntry::get_display_bitmap()
{
	const int state = this->get_entry_state();
	ALLEGRO_BITMAP * bmp = NULL;
	switch (state) {
	case ENTRY_UNDISCOVERED:
		bmp = ImageLoader::get_instance().get_image("encyclopedia/unknown_entry");
		break;
	case ENTRY_SEEN:
		bmp = ImageLoader::get_instance().get_image("encyclopedia/silhouettes/" + this->get_entry_name());
		break;
	case ENTRY_KNOWN:
		bmp = ImageLoader::get_instance().get_image("encyclopedia/entries/" + this->get_entry_name());
		break;
	default:
		bmp = ImageLoader::get_instance().get_image("encyclopedia/unknown_entry");
		break;
	}
	return bmp != NULL ? bmp : ImageLoader::get_instance().get_image("encyclopedia/unknown_entry");
}

const std::string EncyclopediaEntry::get_entry_name()
{
	return this->entry_name.value();
}

const std::string EncyclopediaEntry::get_entry_display_name()
{
	const int state = this->get_entry_state();
	switch (state) {
	case ENTRY_UNDISCOVERED:
		return "???";
	case ENTRY_SEEN:
		return "???";
	case ENTRY_KNOWN:
		return this->entry_display_name.value();
	default:
		return "???";
	}
}

const std::vector<std::string> EncyclopediaEntry::get_entry_description_lines()
{
	FileManager fm;
	return fm.string_to_parts(this->get_entry_description(), "\\n");
}

const std::string EncyclopediaEntry::get_entry_description()
{
	const int state = this->get_entry_state();
	switch (state) {
	case ENTRY_UNDISCOVERED:
		return "???";
	case ENTRY_SEEN:
		return "???";
	case ENTRY_KNOWN:
		return this->entry_description.value();
	default:
		return "???";
	}
}

const int EncyclopediaEntry::get_entry_state()
{
	return this->entry_state.value();
}

void EncyclopediaEntry::set_entry_state(const int state)
{
	this->entry_state = state;
}
