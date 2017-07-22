#ifndef TMP_GET_NODES_H
#define TMP_GET_NODES_H

#include <list>
#include <libxml/tree.h>

std::list<xmlNode*> get_segments(xmlNode *node);
std::list<xmlNode*> get_trackpoints(xmlNode *node);

#endif /* TMP_GET_NODES_H */