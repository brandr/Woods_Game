#include "FileManager.h"

FileManager::FileManager()
{
	identifier_found = false;
}


FileManager::~FileManager()
{
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
	//std::cout << "size: " << attributes.size() << std::endl;
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
