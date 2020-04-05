#include "MusicData.h"

#include "GlobalTime.h"
#include "Level.h"
#include "World.h"

std::vector<MusicNode*> MusicData::valid_music_nodes(World * world, Level * level, GlobalTime * time)
{
	std::vector<MusicNode*> valid_nodes;
	const int size = this->music_nodes.size();
	for (int i = 0; i < size; i++) {
		MusicNode * node = this->music_nodes.getItem(i);
		if (node->is_valid(world, level, time)) {
			valid_nodes.push_back(node);
		}
	}
	return valid_nodes;
}

MusicData::MusicData()
{
	setClassName("MusicData");
	Register("music_update_interval", &music_update_interval);
	Register("music_nodes", &music_nodes);
}


MusicData::~MusicData()
{
}

const int MusicData::get_music_update_interval()
{
	return this->music_update_interval.value();
}

MusicNode * MusicData::select_music_node(World * world, Level * level, GlobalTime * time)
{
	std::vector<MusicNode *> valid_nodes = this->valid_music_nodes(world, level, time);
	if (!valid_nodes.empty()) {
		if (valid_nodes.size() == 1) {
			return valid_nodes[0];
		}
		// higher weight means more chance to select music
		int total_weight = 0;
		for (MusicNode * node : valid_nodes) {
			total_weight += node->get_weight();
		}
		int running_weight = 0;
		std::srand(std::time(NULL));
		const int roll = std::rand() % total_weight;
		for (MusicNode * node : valid_nodes) {
			running_weight += node->get_weight();
			if (roll < running_weight) {
				return node;
			}
		}
		return valid_nodes[0];
	}
	return NULL;
}

MusicNode::MusicNode()
{
	setClassName("MusicNode");
	Register("music_filename", &music_filename);
	Register("weight", &weight);
	Register("qualifiers", &qualifiers);
}

const std::string MusicNode::get_music_filename()
{
	return this->music_filename.value();
}

const bool MusicNode::is_valid(World * world, Level * level, GlobalTime * time)
{
	const int size = this->qualifiers.size();
	for (int i = 0; i < size; i++) {
		Qualifier * q = this->qualifiers.getItem(i);
		q->set_other_time(time);
		if (!q->evaluate(world, level)) {
			return false;
		}
	}
	return true;
}

const int MusicNode::get_weight()
{
	return this->weight.value();
}
