#ifndef NPC_H
#define NPC_H

#include "Being.h"
#include "Dialog.h"
#include "XMLSerialization.h"

class Player;
class NPC : public Being, public xmls::Serializable
{
private:
	xmls::xString start_level_key; // level where the NPC starts the game
	xmls::xString default_spawn_level_key;
	xmls::xString start_spawn_key;
	xmls::xString default_spawn_key;
	xmls::xString default_dialog_text;
public:
	NPC();
	~NPC();
	const std::string get_start_level_key();
	const std::string get_current_spawn_level_key();
	const std::string get_start_spawn_key();
	const std::string get_current_spawn_key();
	Dialog * choose_dialog(Player * player);
	virtual const bool interact_action(Player * player);
	const std::string get_default_dialog_text();
};

#endif