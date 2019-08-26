#include "PathNode.h"

PathNode::PathNode()
{
	setClassName("PathNode");
	this->Register("EntityStartingPosX", &entity_starting_pos_x);
	this->Register("EntityStartingPosY", &entity_starting_pos_y);
	this->Register("EntityDataIndex", &entity_data_index);
	this->Register("EntitySheetCol", &entity_sheet_col);
	this->Register("EntitySheetRow", &entity_sheet_row);
	this->Register("NodeID", &node_id);
	this->Register("NeighborNodes", &neighbor_nodes);
	this->Register("NextLevelNodes", &next_level_nodes);
	this->Register("EntityAttributes", &entity_attributes);
	this->solid = false;
	this->visible = false;
}

PathNode::~PathNode()
{
}

void PathNode::set_content(std::string image_filename, Rect * image_subsection, std::pair<int, int> position)
{
	this->entity_starting_pos_x = position.first;
	this->entity_starting_pos_y = position.second;
	GameImage::set_content(image_filename, image_subsection, position);
}

const std::string PathNode::get_node_id()
{
	return this->node_id.value();
}

Rect * PathNode::get_bitmap_subsection()
{
	return new Rect(this->entity_sheet_col.value()*TILE_SIZE, this->entity_sheet_row.value()*TILE_SIZE, TILE_SIZE, TILE_SIZE);
}

std::vector<NeighborNode*> PathNode::get_neighbor_nodes()
{
	std::vector<NeighborNode*> neighbors;
	const int size = this->neighbor_nodes.size();
	for (int i = 0; i < size; i++) {
		neighbors.push_back(this->neighbor_nodes.getItem(i));
	}
	return neighbors;
}

std::vector<NextLevelNode*> PathNode::get_next_level_nodes()
{
	std::vector<NextLevelNode*> nodes;
	const int size = this->next_level_nodes.size();
	for (int i = 0; i < size; i++) {
		nodes.push_back(this->next_level_nodes.getItem(i));
	}
	return nodes;
}

const bool PathNode::should_animate_walking()
{
	const int size = this->next_level_nodes.size();
	for (int i = 0; i < size; i++) {
		NextLevelNode * next_node = this->next_level_nodes.getItem(i);
		if (next_node->animate_walking.value()) {
			return true;
		}
	}
	return false;
}

const std::pair<int, int> PathNode::get_arrival_offset()
{
	const int size = this->next_level_nodes.size();
	for (int i = 0; i < size; i++) {
		NextLevelNode * next_node = this->next_level_nodes.getItem(i);
		if (next_node->animate_walking.value()) {
			return std::pair<int, int>(next_node->x_dir.value() * TILE_SIZE, next_node->y_dir.value() * TILE_SIZE);
		}
	}
	return std::pair<int, int>(0, 0);
}

NeighborNode::NeighborNode()
{
	setClassName("NeighborNode");
	this->Register("NeighborNodeID", &node_id);
}

NextLevelNode::NextLevelNode()
{
	setClassName("NextLevelNode");
	this->Register("LevelID", &level_id);
	this->Register("NodeID", &node_id);
	this->Register("AnimateWalking", &animate_walking);
	this->Register("XDir", &x_dir);
	this->Register("YDir", &y_dir);
}
