#include "MenuManager.h"
class ControlsMenuManager :
	public MenuManager
{
public:
	ControlsMenuManager();
	~ControlsMenuManager();
	virtual void load_content(std::string id);
};

