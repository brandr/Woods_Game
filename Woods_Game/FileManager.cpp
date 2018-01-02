#include "FileManager.h"

using namespace xmls;

tinyxml2::XMLElement * FileManager::get_element_by_name(tinyxml2::XMLDocument &doc, std::string const & elemt_value)
{
	tinyxml2::XMLElement * elem = doc.RootElement(); //Tree root
	while (elem) {
		if (!std::string(elem -> Value()).compare(elemt_value)) return (elem);
		if (elem -> FirstChildElement()) {
			elem = elem -> FirstChildElement();
		}
		else if (elem -> NextSiblingElement()) {
			elem = elem -> NextSiblingElement();
		}
		else {
			while (!elem  -> Parent() -> NextSiblingElement()) {
				if (elem -> Parent() -> ToElement() == doc.RootElement()) {
					return NULL;
				}
				elem = elem -> Parent() -> NextSiblingElement();
			}
		}
	}
	return (NULL);
}

FileManager::FileManager()
{
	identifier_found = false;
}


FileManager::~FileManager()
{
}

void FileManager::load_xml_content(Serializable *serializable, std::string const & filename, 
	std::string const & element_name, 
	std::string const & key, 
	std::string const & value)
{	
	tinyxml2::XMLDocument doc;
	std::string full_filname = filename + ".xml";
	doc.LoadFile(full_filname.c_str());
	tinyxml2::XMLNode * root = doc.RootElement();
	std::string serializable_str = "";
	tinyxml2::XMLPrinter printer;
	if (root != NULL) {
		for (tinyxml2::XMLElement *e = root->FirstChildElement(element_name.c_str()); e != NULL; e = e->NextSiblingElement(element_name.c_str()))
		{
			if (e->Attribute(key.c_str()) != NULL) {
				std::string attribute = e->Attribute(key.c_str());
				if (attribute == value) {
					e->Accept(&printer);
					serializable_str = printer.CStr();
					break;
				}
			}
		}
	}
	if ( Serializable::fromXML(serializable_str, serializable)) {
		std::cout << "deserialized file\n";
	} else {
		std::cout << "FAILED to deserialize file\n";
	}
}

void FileManager::replace_xml_content(std::string const & filename,
	std::string const & element_name,
	std::string const & key,
	std::string const & value,
	std::string const & replaceXML)
{
	tinyxml2::XMLDocument doc;
	std::string full_filname = filename + ".xml";
	doc.LoadFile(full_filname.c_str());
	tinyxml2::XMLElement * root = doc.RootElement();
	std::string serializable_str = "";
	tinyxml2::XMLPrinter printer;
	if (root != NULL) {
		for (tinyxml2::XMLElement *e = root->FirstChildElement(element_name.c_str()); 
			e != NULL; e = e->NextSiblingElement(element_name.c_str()))
		{
			if (e->Attribute(key.c_str()) != NULL) {
				std::string attribute = e->Attribute(key.c_str());
				if (attribute == value) {
					tinyxml2::XMLDocument replaceDocument;
					replaceDocument.Parse(replaceXML.c_str());
					//tinyxml2::XMLElement *replaceElement = doc.NewElement(""); //doc.NewUnknown(replaceXML.c_str());
					//replaceElement->SetValue(replaceXML.c_str());
					e->DeleteChildren();
					for (tinyxml2::XMLNode *replaceChild = replaceDocument.RootElement()->FirstChild();
						replaceChild != NULL;
						replaceChild = replaceChild->NextSibling())
					{
						copy_xml_node(e, replaceChild);
						//tinyxml2::XMLNode *copyNode = replaceChild->ShallowClone(doc.GetDocument());
						//e->InsertEndChild(copyNode);
					}

					//replaceElement->ParseDeep((char *)replaceXML.c_str(), NULL);
					//tinyxml2::XMLDocument replaceDocument;
					//replaceDocument.Parse(replaceXML.c_str());
					//replaceDocument.
					
					//root->Insert
					//root->InsertAfterChild(e, replaceElement);
					//root->DeleteChild(e);
					break;
				}
			}
		}
	}
	doc.SaveFile(full_filname.c_str());
}

void FileManager::copy_xml_node(tinyxml2::XMLNode * p_dest_parent, const tinyxml2::XMLNode * p_src)
{
	// Protect from evil
	if (p_dest_parent == NULL || p_src == NULL)
	{
		return;
	}

	// Get the document context where new memory will be allocated from
	tinyxml2::XMLDocument *p_doc = p_dest_parent->GetDocument();

	// Make the copy
	tinyxml2::XMLNode *p_copy = p_src->ShallowClone(p_doc);
	if (p_copy == NULL)
	{
		// Error handling required (e.g. throw)
		return;
	}

	// Add this child
	p_dest_parent->InsertEndChild(p_copy);

	// Add the grandkids
	for (const tinyxml2::XMLNode *p_node = p_src->FirstChild(); p_node != NULL; p_node = p_node->NextSibling())
	{
		copy_xml_node(p_copy, p_node);
	}
}

void FileManager::load_content(const char * filename,
	std::vector<std::vector<std::string>> &attributes,
	std::vector<std::vector<std::string>> &contents)
{
	std::string line, new_line;
	std::ifstream openfile(filename);
	if (openfile.is_open()) {
		while (!openfile.eof()) {
			std::stringstream str;
			std::getline(openfile, line);
			if (line.find("Load=") != std::string::npos) {
				type = ATTRIBUTES;
				line = line.erase(0, line.find("=") + 1);
				temp_attributes.clear();
			}
			else {
				type = CONTENT;
				temp_contents.clear();
			}
			str << line;
			while (std::getline(str, new_line, ']')) {

				char remove[] = "[]";
				new_line.erase(std::remove(new_line.begin(), new_line.end(), '['), new_line.end());
				std::string erase = " \t\n\r";
				new_line.erase(new_line.find_last_not_of(erase) + 1);
				if (type == ATTRIBUTES)
					temp_attributes.push_back(new_line);
				else
					temp_contents.push_back(new_line);
			}
			if (type == CONTENT && temp_contents.size() > 0) {
				attributes.push_back(temp_attributes);
				contents.push_back(temp_contents);
			}
		}
	}
}

void FileManager::load_content(const char * filename,
	std::vector<std::vector<std::string>> &attributes,
	std::vector<std::vector<std::string>> &contents,
	std::string id)
{
	std::string line, new_line;
	std::ifstream openfile(filename);
	if (openfile.is_open()) {
		while (!openfile.eof()) {
			std::stringstream str;
			std::getline(openfile, line);
			if (line.find("EndLoad=") != std::string::npos &&
				line.find(id) != std::string::npos) {
				identifier_found = false;
				break;
			}
			else if (line.find("Load=") != std::string::npos &&
				line.find(id) != std::string::npos){
				identifier_found = true;
				continue;
			}
			if (identifier_found) {
				if (line.find("Load=") != std::string::npos) {
					type = ATTRIBUTES;
					line = line.erase(0, line.find("=") + 1);
					temp_attributes.clear();
				}
				else {
					type = CONTENT;
					temp_contents.clear();
				}
				str << line;
				while (std::getline(str, new_line, ']')) {
					char remove[] = "[]";
					new_line.erase(std::remove(new_line.begin(), new_line.end(), '['), new_line.end());
					std::string erase = " \t\n\r";
					new_line.erase(new_line.find_last_not_of(erase) + 1);
					if (type == ATTRIBUTES)
						temp_attributes.push_back(new_line);
					else
						temp_contents.push_back(new_line);
				}
				if (type == CONTENT && temp_contents.size() > 0) {
					attributes.push_back(temp_attributes);
					contents.push_back(temp_contents);
				}
			}
		}
	}
}

//TODO: make this  write to file
void FileManager::replace_content(const char * filename, 
	std::vector<std::vector<std::string>>& attributes, 
	std::vector<std::vector<std::string>>& contents, 
	std::string id)
{
	std::string line, new_line;//, write_line;
	std::ifstream openfile(filename);
	std::stringstream write_str;
	//int attr_line_index = 0;
	if (openfile.is_open()) {
		while (!openfile.eof()) {
			std::stringstream str;
			std::getline(openfile, line);
			if (line.find("EndLoad=") != std::string::npos &&
				line.find(id) != std::string::npos) {
				identifier_found = false;
				break;
			}
			if (line.find("Load=") != std::string::npos &&
				line.find(id) != std::string::npos) {
				identifier_found = true;
				continue;
			}
			if (identifier_found) {
				const int attr_size = attributes.size();
				for (int attr_set_index = 0; attr_set_index < attr_size; attr_set_index++) {
					write_str << "Load=";
					const std::vector<std::string> attr_line = attributes[attr_set_index];
					const int attr_size = attr_line.size();
					for (int attr_index = 0; attr_index < attr_size; attr_index++) {
						write_str << "[" + attr_line[attr_index] + "]";
					}
					write_str << "\n";
					const std::vector<std::string> content_set = contents[attr_set_index];
					const int content_set_size = content_set.size();
					for (int content_set_index = 0; content_set_index < content_set_size; content_set_index++) {
						std::string cont = contents[attr_set_index][content_set_index];
						write_str << "[" + cont + "]";
					}
				}
				
				/*
				if (line.find("Load=") != std::string::npos) {
					type = ATTRIBUTES;
					line = line.erase(0, line.find("=") + 1);
					temp_attributes.clear();
				}
				else {
					type = CONTENT;
					temp_contents.clear();
				}
				str << line;
				while (std::getline(str, new_line, ']')) {

					//char remove[] = "[]";
					new_line.erase(std::remove(new_line.begin(), new_line.end(), '['), new_line.end());
					std::string erase = " \t\n\r";
					new_line.erase(new_line.find_last_not_of(erase) + 1);
					if (type == ATTRIBUTES) {
						std::vector<std::string> attr_line = attributes[attr_line_index];
						const int attr_size = attr_line.size();
						for (int i = 0; i < attr_size; i++) {
							if (attr_line[i] == new_line) {
								//TODO
								break;
							}
						}
						//temp_attributes.push_back(new_line);
					} else {
						//temp_contents.push_back(new_line);
					}
				}
				if (type == CONTENT && temp_contents.size() > 0) {
					//attributes.push_back(temp_attributes);
					//contents.push_back(temp_contents);
				}
				*/
			}
		}
	}
}

std::pair<int, int> FileManager::string_to_pair(std::string tile_string)
{
	std::pair<int, int> pair;
	pair.first = atoi(tile_string.substr(0, tile_string.find(',')).c_str());
	pair.second = atoi(tile_string.substr(tile_string.find(',') + 1).c_str());
	return pair;
}

std::vector<std::string> FileManager::string_to_parts(std::string str, std::string delimiter)
{
	size_t pos = 0;
	std::vector<std::string> parts;
	while ((pos = str.find(delimiter)) != std::string::npos) {
		parts.push_back(str.substr(0,pos));
		str.erase(0, pos + delimiter.length());
	}
	//last iteration
	parts.push_back(str);
	return parts;
}

std::pair<std::string, std::string> FileManager::string_to_pair(std::string str, std::string delimiter)
{
	return std::pair<std::string, std::string>(str.substr(0, str.find(delimiter)), str.substr(str.find(delimiter) + 1));
}
