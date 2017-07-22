#ifndef TMP_GET_TIME_H
#define TMP_GET_TIME_H

#include <libxml/tree.h>
#include <ctime>

std::time_t get_time(xmlDocPtr doc, xmlNode* point);

#endif /* TMP_GET_TIME_H */