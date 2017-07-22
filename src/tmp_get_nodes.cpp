#include "tmp_get_nodes.h"

#include <string>

std::list<xmlNode*> get_all_nodes_named(xmlNode *node, std::string name)
{
	std::list<xmlNode*> segments;
	xmlNode *cur_node = NULL;

	for (cur_node = node; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {
			if(xmlStrcmp(cur_node->name, xmlCharStrdup(name.c_str())) == 0) {
				segments.push_back(cur_node);
			} else {
				segments.splice(segments.end(), get_all_nodes_named(cur_node->children, name));
			}
		}
	}

	return segments;
}

std::list<xmlNode*> get_segments(xmlNode *node)
{
	return get_all_nodes_named(node, "trkseg");
}

std::list<xmlNode*> get_trackpoints(xmlNode *node)
{
	return get_all_nodes_named(node, "trkpt");
}