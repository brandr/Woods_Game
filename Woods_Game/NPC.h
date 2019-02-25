#ifndef NPC_H
#define NPC_H

#include "Being.h"
#include "Dialog.h"
#include "XMLSerialization.h"

class Player;
class NPC : public Being, public xmls::Serializable
{
private:
	xmls::xString spawn_key;
	xmls::xString default_dialog_text;
public:
	NPC();
	~NPC();
	const std::string get_spawn_key();
	Dialog * choose_dialog(Player * player);
	virtual const bool interact_action(Player * player);
	const std::string get_default_dialog_text();
};

#endif