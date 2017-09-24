#include <vector>
#include <iostream>
#include <map>

struct ControlMapping {
	std::string label, action_key;
public:
	ControlMapping();
	ControlMapping(std::string label, std::string action_key);
};

// a set of controls for the main gameplay
class Controls
{
private:
	std::map<std::string, ControlMapping> controls_map;
public:
	Controls();
	~Controls();
	void load_content(std::vector<std::vector<std::string>> attributes, std::vector<std::vector<std::string>> contents);
};

