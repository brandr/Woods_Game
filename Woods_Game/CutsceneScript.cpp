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
	Register("block_index", &block_index);
	Register("next_block_index", &next_block_index);
	Register("duration", &duration);
	Register("action_key", &action_key);
	Register("effect_key", &effect_key);
	Register("music_key", &music_key);
	Register("block_tiles", &block_tiles);
	Register("block_triggers", &block_triggers);
	Register("branches", &branches);
	Register("agent_animations", &agent_animations);
	Register("dialog_item", &dialog_item);
}

CutsceneScriptBlock::~CutsceneScriptBlock()
{
}

const int CutsceneScriptBlock::get_block_index()
{
	return this->block_index.value();
}

const int CutsceneScriptBlock::get_next_block_index()
{
	return this->next_block_index.value();
}

const int CutsceneScriptBlock::get_duration()
{
	return this->duration.value();
}

const std::string CutsceneScriptBlock::get_action_key()
{
	return this->action_key.value();
}

const std::string CutsceneScriptBlock::get_effect_key()
{
	return this->effect_key.value();
}

const std::string CutsceneScriptBlock::get_music_key()
{
	return this->music_key.value();
}

std::vector<CutsceneTile> CutsceneScriptBlock::get_cutscene_tiles()
{
	std::vector<CutsceneTile> tiles;
	const int size = this->block_tiles.size();
	for (int i = 0; i < size; i++) {
		CutsceneBlockTile * cbt = this->block_tiles.getItem(i);
		tiles.push_back(CutsceneTile(cbt->agent_key.value(), cbt->tile_x.value(), cbt->tile_y.value(), cbt->direction.value()));
	}
	return tiles;
}

std::vector<TriggerStatus*> CutsceneScriptBlock::get_block_triggers()
{
	std::vector<TriggerStatus *> triggers;
	const int size = this->block_triggers.size();
	for (int i = 0; i < size; i++) {
		triggers.push_back(this->block_triggers.getItem(i));
	}
	return triggers;
}

std::vector<CutsceneBranch> CutsceneScriptBlock::get_cutscene_branches()
{
	std::vector<CutsceneBranch> branches;
	const int size = this->branches.size();
	for (int i = 0; i < size; i++) {
		CutsceneScriptBranch * csb = this->branches.getItem(i);
		branches.push_back(CutsceneBranch(csb->next_block_index.value(), csb->get_qualifiers()));
	}
	return branches;
}

std::vector<CutsceneAgentAnimation> CutsceneScriptBlock::get_agent_animations()
{
	std::vector<CutsceneAgentAnimation> animations;
	const int size = this->agent_animations.size();
	for (int i = 0; i < size; i++) {
		CutsceneScriptAgentAnimation * csaa = this->agent_animations.getItem(i);
		animations.push_back(CutsceneAgentAnimation(csaa->agent_key.value(), csaa->animation_key.value(), csaa->direction.value()));
	}
	return animations;
}

Dialog * CutsceneScriptBlock::get_dialog()
{
	if (this->action_key.value() == ACTION_DIALOG) {
		Dialog * dialog = new Dialog();
		dialog->parse_dialog(&dialog_item, "");
		return dialog;
	}
	return NULL;
}

CutsceneBlockTile::CutsceneBlockTile()
{
	setClassName("CutsceneBlockTile");
	Register("agent_key", &agent_key);
	Register("tile_x", &tile_x);
	Register("tile_y", &tile_y);
	Register("direction", &direction);
};

CutsceneScriptBranch::CutsceneScriptBranch()
{
	setClassName("CutsceneScriptBranch");
	Register("next_block_index", &next_block_index);
	Register("qualifiers", &qualifiers);
}

std::vector<Qualifier*> CutsceneScriptBranch::get_qualifiers()
{
	std::vector<Qualifier*> all_qualifiers;
	const int size = this->qualifiers.size();
	for (int i = 0; i < size; i++) {
		all_qualifiers.push_back(this->qualifiers.getItem(i));
	}
	return all_qualifiers;
}

CutsceneScriptAgentAnimation::CutsceneScriptAgentAnimation()
{
	setClassName("CutsceneScriptAgentAnimation");
	Register("agent_key", &agent_key);
	Register("animation_key", &animation_key);
	Register("direction", &direction);
}
