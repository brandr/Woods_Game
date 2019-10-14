#ifndef CUTSCENE_SCRIPT_H
#define CUTSCENE_SCRIPT_H

#include "Cutscene.h"
#include "Dialog.h"
#include "DialogTree.h"
#include "Qualifier.h"
#include "TriggerStatus.h"
#include "XMLSerialization.h"

struct CutsceneScriptBranch : public xmls::Serializable {
	xmls::xInt next_block_index;
	xmls::Collection<Qualifier> qualifiers;
	CutsceneScriptBranch();
	std::vector<Qualifier *> get_qualifiers();
};

struct CutsceneBlockTile : public xmls::Serializable {
	xmls::xString agent_key;
	xmls::xInt tile_x;
	xmls::xInt tile_y;
	xmls::xInt direction; // see animation direction in gameImage
	CutsceneBlockTile();
};

struct CutsceneScriptAgentAnimation : public xmls::Serializable {
	xmls::xString agent_key;
	xmls::xString animation_key;
	xmls::xInt direction; // see animation direction in gameImage
	CutsceneScriptAgentAnimation();
};

struct CutsceneScriptBlock : public xmls::Serializable {
private:
	xmls::xInt block_index;
	xmls::xInt next_block_index;
	xmls::xInt duration;
	xmls::xString action_key;
	xmls::xString effect_key;
	xmls::xString music_key;
	xmls::Collection<CutsceneBlockTile> block_tiles;
	xmls::Collection<TriggerStatus> block_triggers;
	xmls::Collection<CutsceneScriptBranch> branches;
	xmls::Collection<CutsceneScriptAgentAnimation> agent_animations;
	DialogItem dialog_item;
public:
	CutsceneScriptBlock();
	~CutsceneScriptBlock();
	const int get_block_index();
	const int get_next_block_index();
	const int get_duration();
	const std::string get_action_key();
	const std::string get_effect_key();
	const std::string get_music_key();
	std::vector<CutsceneTile> get_cutscene_tiles();
	std::vector<TriggerStatus *> get_block_triggers();
	std::vector<CutsceneBranch> get_cutscene_branches();
	std::vector<CutsceneAgentAnimation> get_agent_animations();
	Dialog * get_dialog();
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