#include "CutsceneScript.h"



CutsceneScript::CutsceneScript()
{
	setClassName("CutsceneScript");
	Register("cutscene_key", &cutscene_key);
	Register("replayable", &replayable);
	Register("blocks", &blocks);
}


CutsceneScript::~CutsceneScript()
{
}

const std::string CutsceneScript::get_cutscene_key()
{
	return this->cutscene_key.value();
}

const bool CutsceneScript::is_replayable()
{
	return this->replayable.value();
}

std::vector<CutsceneScriptBlock*> CutsceneScript::get_blocks()
{
	std::vector<CutsceneScriptBlock*> all_blocks;
	const int size = this->blocks.size();
	for (int i = 0; i < size; i++) {
		all_blocks.push_back(this->blocks.getItem(i));
	}
	return all_blocks;
}

CutsceneList::CutsceneList()
{
	setClassName("CutsceneList");
	Register("cutscenes", &cutscenes);
}

CutsceneList::~CutsceneList()
{
}

CutsceneScript * CutsceneList::get_cutscene_script(const std::string key)
{
	const int size = this->cutscenes.size();
	for (int i = 0; i < size; i++) {
		CutsceneScript * cs = this->cutscenes.getItem(i);
		if (cs != NULL && key == cs->get_cutscene_key()) {
			return cs;
		}
	}
	return NULL;
}

CutsceneScriptBlock::CutsceneScriptBlock()
{
	setClassName("CutsceneScriptBlock");
	Register("duration", &duration);
	Register("action_key", &action_key);
}

CutsceneScriptBlock::~CutsceneScriptBlock()
{
}

const int CutsceneScriptBlock::get_duration()
{
	return this->duration.value();
}

const std::string CutsceneScriptBlock::get_action_key()
{
	return this->action_key.value();
}
