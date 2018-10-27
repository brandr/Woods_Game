#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
//#include "utility"  // for pair
#include "xstring"  // for string
#include "XMLSerialization.h"

enum ATTRIBUTES { IMAGE, POSITION };

class FileManager
{
private:
	enum LOAD_TYPE{ATTRIBUTES, CONTENT};
	int type;
	bool identifier_found;
	std::vector<std::string> temp_attributes, temp_contents;
	tinyxml2::XMLElement *get_element_by_name(tinyxml2::XMLDocument &doc, std::string const & element_value);
public:
	FileManager();
	~FileManager();
	void load_xml_content(xmls::Serializable *serializable, 
		std::string const & filename,
		std::string const & element_name,
		std::string const & key,
		std::string const & value);
	void replace_xml_content(std::string const & filename,
		std::string const & element_name,
		std::string const & key,
		std::string const & value,
		std::string const & replaceXML);
	void save_xml_content(std::string const & filename,
		std::string const & element_name,
		std::map<std::string, std::string> attributes);
	void create_xml_file(std::string const & filename);
	std::vector<std::string> all_xml_keys(std::string const & filename,
		std::string const & element_name,
		std::string const & type,
		std::string const & key);
	void copy_xml_node(tinyxml2::XMLNode *p_dest_parent, const tinyxml2::XMLNode *p_src);
	void load_content(const char* filename, std::vector<std::vector<std::string>> &attributes, std::vector<std::vector<std::string>> &contents);
	void load_content(const char* filename, std::vector<std::vector<std::string>> &attributes, std::vector<std::vector<std::string>> &contents, std::string id);
	void replace_content(const char* filename, std::vector<std::vector<std::string>> &attributes, std::vector<std::vector<std::string>> &contents, std::string id);
	static std::pair<int, int> string_to_pair(std::string tile_string);
	static std::vector<std::string> string_to_parts(std::string str, std::string delimiter);
	static std::pair<std::string, std::string> string_to_pair(std::string str, std::string delimiter);
	//unload content?
};
#endif