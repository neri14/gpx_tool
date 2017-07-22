#include "tmp_get_time.h"

#include <tmp_consts.h>

#include <sstream>
#include <iomanip>
#include <iostream>

std::time_t get_time(xmlDocPtr doc, xmlNode* point)
{
	xmlNode * t_node = point->children;
	while (t_node && xmlStrcmp(t_node->name, xmlCharStrdup("time")) != 0) {
		t_node = t_node->next;
	}

	if (t_node) {
		std::string time_str((char*)xmlNodeListGetString(doc, t_node->xmlChildrenNode, 1));
		// std::cout << time_str << std::endl;

		std::tm tm;
		std::istringstream ss(time_str);
		ss >> std::get_time(&tm, constant::TIME_FORMAT);//2017-03-08T15:04:34Z
		return timegm(&tm);
	} else {
		std::cerr << "no time for point" << std::endl;
		exit(1);
	}
}