#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>

enum ATTRIBUTES { IMAGE, POSITION };

class FileManager
{
private:
	enum LOAD_TYPE{ATTRIBUTES, CONTENT};
	int type;
	bool identifier_found;
	std::vector<std::string> temp_attributes, temp_contents;
public:
	FileManager();
	~FileManager();
	void load_content(const char* filename, std::vector<std::vector<std::string>> &attributes, std::vector<std::vector<std::string>> &contents);
	void load_content(const char* filename, std::vector<std::vector<std::string>> &attributes, std::vector<std::vector<std::string>> &contents, std::string id);
	static std::pair<int, int> string_to_pair(std::string tile_string);
	static std::vector<std::string> string_to_parts(std::string str, std::string delimiter);
	static std::pair<std::string, std::string> string_to_pair(std::string str, std::string delimiter);
	//unload content?
};

