#ifndef CRITTER_MANAGER_H
#define CRITTER_MANAGER_H

#include "Critter.h"
#include "Dialog.h"

struct CritterLibrary : public xmls::Serializable {
	xmls::Collection<CritterTemplate> critter_templates;
	CritterLibrary();
	std::vector<CritterTemplate *> get_critter_templates();
};

class CritterManager
{
private:
	CritterManager() {}
	CritterLibrary critter_library;
	CritterTemplate * get_critter_template(const std::string critter_key);
public:
	CritterManager(CritterManager const&) = delete;
	void operator=(CritterManager const&) = delete;
	static CritterManager& get_instance();
	~CritterManager();
	void load_critter_templates();
	Critter * create_critter(const std::string critter_key, const int index);
	const std::string get_encyclopedia_category_name(const std::string critter_key);
	const std::string get_encyclopedia_entry_name(const std::string critter_key);
	Dialog * get_critter_catch_dialog(const std::string critter_key);
};

#endif