#ifndef PATHNODE_H
#define PATHNODE_H

#include "Entity.h"
#include "XMLSerialization.h"

struct NeighborNode : public xmls::Serializable {
	xmls::xString node_id;
	NeighborNode();
};

struct NextLevelNode : public xmls::Serializable {
	xmls::xString level_id;
	xmls::xString node_id;
	xmls::xBool animate_walking;
	xmls::xInt x_dir;
	xmls::xInt y_dir;
	NextLevelNode();
};

class PathNode : public Entity, public xmls::Serializable
{
private:
	xmls::xString node_id;
	xmls::Collection<NeighborNode> neighbor_nodes;
	xmls::Collection<NextLevelNode> next_level_nodes;
public:
	PathNode();
	~PathNode();
	virtual void set_content(std::string image_filename, Rect* image_subsection, std::pair<int, int> position);
	const std::string get_node_id();
	virtual Rect *get_bitmap_subsection();
	std::vector<NeighborNode *> get_neighbor_nodes();
	std::vector<NextLevelNode *> get_next_level_nodes();
	const bool should_animate_walking();
	const std::pair<int, int> get_arrival_offset();
};

#endif
