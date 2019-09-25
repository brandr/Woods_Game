#ifndef CUTSCENE_SCRIPT_H
#define CUTSCENE_SCRIPT_H

#include "XMLSerialization.h"

struct CutsceneScriptBlock : public xmls::Serializable {
private:
	xmls::xInt duration;
	xmls::xString action_key;
public:
	CutsceneScriptBlock();
	~CutsceneScriptBlock();
	const int get_duration();
	const std::string get_action_key();
};

class CutsceneScript : public xmls::Serializable
{
private:
	xmls::xString cutscene_key;
	xmls::xBool replayable;
	xmls::Collection<CutsceneScriptBlock> blocks;
public:
	CutsceneScript();
	~CutsceneScript();
	const std::string get_cutscene_key();
	const bool is_replayable();
	std::vector<CutsceneScriptBlock * > get_blocks();
};

struct CutsceneList : public xmls::Serializable {
private:
	xmls::Collection<CutsceneScript> cutscenes;
public:
	CutsceneList();
	~CutsceneList();
	CutsceneScript * get_cutscene_script(const std::string key);
};

#endif