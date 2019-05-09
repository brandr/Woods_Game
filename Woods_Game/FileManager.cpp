#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

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
		std::cout << "deserialized file: " + filename + "\n";
	} else {
		std::cout << "FAILED to deserialize file: " + filename + "\n";
	}
}

void FileManager::replace_xml_content(std::string const & filename,
	std::string const & element_name,
	std::string const & key,
	std::string const & value,
	std::string const & replaceXML)
{
	tinyxml2::XMLDocument doc;
	std::string full_filename = filename + ".xml";
	doc.LoadFile(full_filename.c_str());
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
					e->DeleteChildren();
					for (tinyxml2::XMLNode *replaceChild = replaceDocument.RootElement()->FirstChild();
						replaceChild != NULL;
						replaceChild = replaceChild->NextSibling())
					{
						copy_xml_node(e, replaceChild);
					}
					break;
				}
			}
		}
	}
	doc.SaveFile(full_filename.c_str());
}

void FileManager::save_xml_content(std::string const & filename, 
	std::string const & element_name,
	std::map<std::string, std::string> attributes)
{
	tinyxml2::XMLDocument doc;
	const std::string full_filename = filename + ".xml";
	doc.LoadFile(full_filename.c_str());
	tinyxml2::XMLElement * root = doc.RootElement();
	tinyxml2::XMLElement *save_element = doc.NewElement(element_name.c_str());
	for (auto const &it : attributes) {
		save_element->SetAttribute(it.first.c_str(), it.second.c_str());
	}
	tinyxml2::XMLNode *last_child = root->LastChild();
	if (last_child != NULL) {
		root->InsertAfterChild(last_child, save_element);
	}
	else {
		root->InsertFirstChild(save_element);
	}
	doc.SaveFile(full_filename.c_str());
}

void FileManager::create_xml_file(std::string const & filename)
{
	tinyxml2::XMLDocument doc;
	const std::string full_filename = filename + ".xml";
	doc.NewElement("root");
	tinyxml2::XMLNode *root_element = doc.NewElement("root");
	doc.InsertFirstChild(root_element);
	doc.SaveFile(full_filename.c_str());
}

std::vector<std::string> FileManager::all_xml_keys(std::string const & filename, 
	std::string const & element_name, 
	std::string const & type,
	std::string const & key)
{
	std::vector<std::string> all_keys;
	tinyxml2::XMLDocument doc;
	std::string full_filname = filename + ".xml";
	doc.LoadFile(full_filname.c_str());
	tinyxml2::XMLNode * root = doc.RootElement();
	std::string serializable_str = "";
	tinyxml2::XMLPrinter printer;
	if (root != NULL) {
		for (tinyxml2::XMLElement *e = root->FirstChildElement(element_name.c_str()); 
			e != NULL; e = e->NextSiblingElement(element_name.c_str()))
		{
			if (e->Attribute("Type") != NULL) {
				std::string attribute = e->Attribute("Type");
				if (attribute == type) {
					if (e->Attribute(key.c_str()) != NULL) {
						std::string attribute = e->Attribute(key.c_str());
						all_keys.push_back(attribute);
					}
				}
			}
		}
	}
	return all_keys;
}

const std::map<std::string, std::string> FileManager::all_top_named_attributes(std::string const & xml)
{
	std::map<std::string, std::string> xml_map;
	if (xml.size() > 0) {
		tinyxml2::XMLPrinter printer;
		tinyxml2::XMLDocument doc;
		doc.Parse(xml.c_str());
		tinyxml2::XMLNode *root = doc.FirstChild();
			for (tinyxml2::XMLNode *e = root->FirstChild();
				e != NULL; e = e->NextSibling()) {
				tinyxml2::XMLElement * el = e->ToElement();
				if (el->Attribute("Name") != NULL) {
					const std::string name = el->Attribute("Name");
					const std::string el_value = el->Value();
					//e->Accept(&printer);
					//const std::string serializable_str = printer.CStr();
					if (el_value == "Member") {
						const std::string value = el->GetText();
						xml_map[name] = value;
					} else if (el_value == "Collection"){
						//TODO
					} else {
						//TODO
					}
				}
			}

	}
	return xml_map;
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

void FileManager::replace_content(const char * filename, 
	std::vector<std::vector<std::string>>& attributes, 
	std::vector<std::vector<std::string>>& contents, 
	std::string id)
{
	std::string line, new_line;
	std::ifstream openfile(filename);
	std::stringstream write_str;
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

const std::vector<std::string> FileManager::parse_tags(std::string str, const std::string token)
{
	size_t open_pos = 0;
	size_t close_pos = 0;
	const std::string open_tag = "[" + token + "]";
	const std::string close_tag = "[/" + token + "]";
	std::vector<std::string> parts;
	while ((open_pos = str.find(open_tag)) != std::string::npos) {
		str.erase(0, open_pos + open_tag.length());
		close_pos = str.find(close_tag);
		if (close_pos == std::string::npos) {
			break;
		}
		parts.push_back(str.substr(0, close_pos));
		str.erase(0, close_pos + close_tag.length());
		
	}
	//last iteration
	//parts.push_back(str);
	return parts;
}
