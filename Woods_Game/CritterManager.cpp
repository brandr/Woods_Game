#include "CritterManager.h"

CritterTemplate * CritterManager::get_critter_template(const std::string critter_key)
{
	std::vector<CritterTemplate *> all_templates = this->critter_library.get_critter_templates();
	for (CritterTemplate * ct : all_templates) {
		if (critter_key == ct->get_critter_key()) {
			return ct;
		}
	}
	return NULL;
}

CritterManager & CritterManager::get_instance()
{
	static CritterManager manager;
	return manager;
}

CritterManager::~CritterManager()
{
}

void CritterManager::load_critter_templates()
{
	FileManager filemanager;
	const std::string filename = "resources/load/critters";
	filemanager.load_xml_content(&(this->critter_library), filename, "SerializableClass", "CritterLibraryKey", "default_critter_library");
}

Critter * CritterManager::create_critter(const std::string critter_key, const int index)
{
	Critter * critter = NULL;
	std::vector<CritterTemplate *> all_templates = this->critter_library.get_critter_templates();
	for (CritterTemplate * ct : all_templates) {
		if (critter_key == ct->get_critter_key()) {
			critter = new Critter(ct, index);
			break;
		}
	}
	return critter;
}

const std::string CritterManager::get_encyclopedia_category_name(const std::string critter_key)
{
	CritterTemplate * ct = this->get_critter_template(critter_key);
	return ct != NULL ? ct->get_encyclopedia_category_name() : "";
}

const std::string CritterManager::get_encyclopedia_entry_name(const std::string critter_key)
{
	CritterTemplate * ct = this->get_critter_template(critter_key);
	return ct != NULL ? ct->get_encyclopedia_entry_name() : "";
}

Dialog * CritterManager::get_critter_catch_dialog(const std::string critter_key)
{
	std::vector<CritterTemplate *> all_templates = this->critter_library.get_critter_templates();
	for (CritterTemplate * ct : all_templates) {
		if (critter_key == ct->get_critter_key()) {
			return ct->get_catch_dialog();
		}
	}
	return NULL;
}

CritterLibrary::CritterLibrary()
{
	setClassName("CritterLibrary");
	Register("critter_templates", &critter_templates);
}

std::vector<CritterTemplate*> CritterLibrary::get_critter_templates()
{
	std::vector<CritterTemplate*> cts;
	const int size = this->critter_templates.size();
	for (int i = 0; i < size; i++) {
		cts.push_back(this->critter_templates.getItem(i));
	}
	return cts;
}
